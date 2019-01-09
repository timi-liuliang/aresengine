#include "D3D11RenderMethod.h"
#include "D3D11RenderDevice.h"
#include "D3D11RenderState.h"
#include "D3D11Texture.h"
#include <fstream>
#include <D3Dcompiler.h>

#ifdef ARES_EDITOR_MODE
	#pragma comment( lib, "d3dcompiler.lib")
#endif

namespace Ares
{
	// 构造函数
	D3D11RenderMethod::D3D11RenderMethod()
		: RenderMethod()
	{
		m_hasDiscard	   = true;
		m_hasTessellation  = false;
		m_isShaderValidate.assign( true);
	}

	// 析构函数
	D3D11RenderMethod::~D3D11RenderMethod()
	{

	}

	// 加载 *.hlsl
	bool D3D11RenderMethod::Load( const char* fileName, const ShaderMacros& macros)
	{
		RenderMethod::Load( fileName, macros);

		string	 code;
		ifstream ifs( fileName);
		if( ifs.is_open())
		{
			ostringstream oss; oss << ifs.rdbuf();
			code = oss.str();

			m_isEnabled = LoadFromMemory( code.c_str(), code.size());

			return m_isEnabled;
		}

		return false;
	}

	// 从内存加载
	bool D3D11RenderMethod::LoadFromMemory( const void* code, size_t length)
	{
		// 编译顶点着色器
		ID3DBlobPtr vsByteCode = CompileToBytecode( code, length, ST_VertexShader);
		if( vsByteCode)
		{
			AttachByteCode( vsByteCode, ST_VertexShader);
			ParseParameters( vsByteCode, ST_VertexShader);
		}

		// 编译HullShader
		ID3DBlobPtr hsByteCode = CompileToBytecode( code, length, ST_HullShader);
		if( hsByteCode)
		{
			AttachByteCode( hsByteCode, ST_HullShader);
			ParseParameters( hsByteCode, ST_HullShader);
		}

		// 编译DomainShader
		ID3DBlobPtr dsByteCode = CompileToBytecode( code, length, ST_DomainShader);
		if( dsByteCode)
		{
			AttachByteCode( dsByteCode, ST_DomainShader);
			ParseParameters( dsByteCode, ST_DomainShader);
		}

		// 编译像素着色器
		ID3DBlobPtr psByteCode = CompileToBytecode( code, length, ST_PixelShader);
		if( psByteCode)
		{
			AttachByteCode( psByteCode, ST_PixelShader);
			ParseParameters( psByteCode, ST_PixelShader);
		}

		// 解析默认状态
		ParseStates();

		return true;
	}

	// 保存ByteCode
	void D3D11RenderMethod::SaveByteCode( const char* fileName, ShaderType type)
	{
		if( !m_shaderCodes[type])
			return;

		FILE* fileHandle = fopen( fileName, "wt");
		if( fileHandle)
		{
			fwrite( static_cast<const char*>(m_shaderCodes[type]->GetBufferPointer()), m_shaderCodes[type]->GetBufferSize(), 1, fileHandle);

			fflush( fileHandle);
			fclose( fileHandle);
		}
	}

	// 绑定
	void D3D11RenderMethod::Bind()
	{
		D3D11RenderDevice* device = D3D11RenderDevice::GetSingletonPtr();
		//const ID3D11DeviceContextPtr& d3dImmCtx = device->GetD3DDeviceImmContext();

		device->VSSetShader( m_vertexShader);
		device->GSSetShader( m_geometryShader);
		device->PSSetShader( m_pixelShader);
		device->CSSetShader( m_computeShader);
		device->HSSetShader( m_hullShader);
		device->DSSetShader( m_domainShader);
	}

	// 刷新常量Buffer数据
	void D3D11RenderMethod::RefreshBufferData( const RenderMaterial& mat, ShaderType type)
	{
		ID3D11DeviceContextPtr& d3dImmCtx = PDRenderDevice.GetD3DDeviceImmContext();

		vector<ID3D11BufferPtr>& buffers = m_constantBuffers[type];
		if( !buffers.empty())
		{
			for ( size_t c=0; c<buffers.size(); c++)
			{
				vector<HLSL::VariableDesc>& varDesc = m_shaderDescs[type].m_cbDesc[c].m_varDesc;
				for( size_t v=0; v<varDesc.size(); v++)
				{
					const ShaderParamPtr& pParam = mat.GetShaderParam( varDesc[v].m_name);
					if( pParam /*&& *pParam != *m_curParamValues[i]*/)
					{	
						UINT copyByte = min( varDesc[v].m_size, (UINT)pParam->m_bytes);
						std::memcpy( &m_shaderDescs[type].m_cbDesc[c].m_memorys[varDesc[v].m_startOffset], pParam->m_data, copyByte);
						//if( pParam->m_bytes != varDesc[v].m_size)
						//	PDLogSystem.Warning( "D3D11:ShaderParam Bytes not equal Variable descreption!");
					}
				}

				d3dImmCtx->UpdateSubresource( buffers[c].get(), 0, NULL, m_shaderDescs[type].m_cbDesc[c].m_memorys.data(), 0, 0);
			}
		}

		// 采样数据
		vector<ID3D11SamplerStatePtr>& samplers = m_samplers[type];
		if( !samplers.empty())
		{
			for( size_t i=0; i<m_shaderDescs[type].m_sampDesc.size(); i++)
			{
				const ShaderParamPtr& pParam = mat.GetShaderParam( m_shaderDescs[type].m_sampDesc[i].m_name);
				if( pParam)
				{
					D3D11SamplerState*  samplerState = dynamic_cast<D3D11SamplerState*>( (*pParam->m_samplerState).get());
					if( samplerState)
						samplers[i] = samplerState->GetD3D11SamperState();
				}
			}
		}

		// 纹理类资源
		vector<ID3D11ShaderResourceViewPtr>& srvs = m_srvs[type];
		if( !srvs.empty())
		{
			for( size_t i=0; i<m_shaderDescs[type].m_srvDesc.size(); i++)
			{
				const ShaderParamPtr& pParam = mat.GetShaderParam( m_shaderDescs[type].m_srvDesc[i].m_name);
				if( pParam && pParam->IsValid())
				{
					D3D11Texture2D*	tex2D = dynamic_cast<D3D11Texture2D*>( (*pParam->m_texture).get());
					if( tex2D)
						srvs[i] = tex2D->RetriveD3DShaderResourceView();
				}
			}
		}
	}

	// 应用参数
	void D3D11RenderMethod::SetParams(const RenderMaterial& mat)
	{
		// 循环所有Shader类型
		for ( size_t type=0; type<ST_NumShaderTypes; type++)
		{
			// 更新数据
			RefreshBufferData( mat, static_cast<ShaderType>(type));

			// 设置常变量
			PDRenderDevice.SetConstantBuffers( m_constantBuffers[type], static_cast<ShaderType>(type));

			// 采样状态
			PDRenderDevice.SetSamplers( m_samplers[type], static_cast<ShaderType>(type));

			// 纹理类资源
			PDRenderDevice.SetShaderResources( m_srvs[type], static_cast<ShaderType>(type));
		}
	}

	// 解绑
	void D3D11RenderMethod::Unbind()
	{

	}

	// 编译
	ID3DBlobPtr D3D11RenderMethod::CompileToBytecode( const void* pSrcData, size_t srcDataSize, ShaderType type)
	{
		std::string shaderProfile;
		std::string shaderEntryPoint;
		switch( type)
		{
		case ST_VertexShader:
			{
				shaderEntryPoint = "VS";
				shaderProfile    = "vs_5_0";
			}
			break;

		case ST_HullShader:
			{
				shaderEntryPoint = "HS";
				shaderProfile	 = "hs_5_0";
			}
			break;

		case ST_DomainShader:
			{
				shaderEntryPoint = "DS";
				shaderProfile	 = "ds_5_0";
			}
			break;

		case ST_PixelShader:
			{
				shaderEntryPoint = "PS";
				shaderProfile	 = "ps_5_0";
			}
			break;
		}

		// 映射 D3D_SHADER_MACRO
		vector<D3D_SHADER_MACRO> macros;
		for( size_t i=0; i<m_macros.size(); i++)
		{
			D3D_SHADER_MACRO macro_d3d = { m_macros[i].m_name.c_str(), m_macros[i].m_definition.c_str() };
			macros.push_back( macro_d3d);
		}

		D3D_SHADER_MACRO macroEnd = { NULL, NULL };
		macros.push_back( macroEnd);

		UINT shaderFlags = 0;
#if ARES_DEBUG
		shaderFlags |= D3DCOMPILE_DEBUG;
		shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
		//shaderFlags |= D3DCOMPILE_FORCE_VS_SOFTWARE_NO_OPT;
		//shaderFlags |= D3DCOMPILE_FORCE_PS_SOFTWARE_NO_OPT;
#endif

		ID3DBlob* byteCode=NULL;
		ID3DBlob* errMsg=NULL;
		D3DCompile( pSrcData, srcDataSize, NULL, macros.data(), NULL, shaderEntryPoint.c_str(), shaderProfile.c_str(), shaderFlags, 0, &byteCode, &errMsg);
		if( errMsg)
		{
			string errsg = static_cast<char*>( errMsg->GetBufferPointer());
			PDLogSystem.Error( "CompileToBytecode Error:\n%s", static_cast<char*>( errMsg->GetBufferPointer()));
		}
		else
			PDLogSystem.Info( " CompileToBytecode succed;");

		SAFE_RELEASE( errMsg);

		return MakeCOMPtr( byteCode);
	}

	// 附加汇编
	void D3D11RenderMethod::AttachByteCode( const ID3DBlobPtr& byteCode, ShaderType type)
	{
		// 构建Shader
		switch( type)
		{
		case ST_VertexShader:
			{
				ID3D11VertexShader* vertexShader=NULL;
				PDRenderDevice.GetD3DDevice()->CreateVertexShader( byteCode->GetBufferPointer(), byteCode->GetBufferSize(), NULL, &vertexShader);
				m_vertexShader = MakeCOMPtr( vertexShader);
			}
			break;

		case ST_HullShader:
			{
				ID3D11HullShader* hullShader=NULL;
				PDRenderDevice.GetD3DDevice()->CreateHullShader( byteCode->GetBufferPointer(), byteCode->GetBufferSize(), NULL, &hullShader);
				m_hullShader = MakeCOMPtr( hullShader);
			}
			break;

		case ST_DomainShader:
			{
				ID3D11DomainShader* domainShader=NULL;
				PDRenderDevice.GetD3DDevice()->CreateDomainShader( byteCode->GetBufferPointer(), byteCode->GetBufferSize(), NULL, &domainShader);
				m_domainShader = MakeCOMPtr( domainShader);
			}
			break;

		case ST_PixelShader:
			{
				ID3D11PixelShader* pixelShader=NULL;
				PDRenderDevice.GetD3DDevice()->CreatePixelShader( byteCode->GetBufferPointer(), byteCode->GetBufferSize(), NULL, &pixelShader);
				m_pixelShader = MakeCOMPtr( pixelShader);	
			}
			break;
		}

		// 记录汇编代码
		m_shaderCodes[type] = byteCode;
	}

	// 剖析参数
	void D3D11RenderMethod::ParseParameters( const ID3DBlobPtr& byteCode, ShaderType type)
	{
		// 获取参数类信息
		ID3D11ShaderReflection* reflection=NULL;
		D3DReflect( byteCode->GetBufferPointer(), byteCode->GetBufferSize(), IID_ID3D11ShaderReflection, reinterpret_cast<void**>(&reflection));
		if( reflection)
		{
			D3D11_SHADER_DESC desc;
			reflection->GetDesc( &desc);
			m_shaderDescs[type].m_cbDesc.resize( desc.ConstantBuffers);
			m_constantBuffers[type].resize( desc.ConstantBuffers);
			for ( UINT c=0; c<desc.ConstantBuffers; ++c)
			{
				D3D11_SHADER_BUFFER_DESC			  cbDesc;
				ID3D11ShaderReflectionConstantBuffer* reflectionCB = reflection->GetConstantBufferByIndex( c);
				reflectionCB->GetDesc( &cbDesc);
				m_shaderDescs[type].m_cbDesc[c].m_size = cbDesc.Size;
				m_shaderDescs[type].m_cbDesc[c].m_memorys.resize(cbDesc.Size);
				for ( UINT v=0; v<cbDesc.Variables; ++v)
				{
					D3D11_SHADER_VARIABLE_DESC		varDesc;
					ID3D11ShaderReflectionVariable* reflectionVar = reflectionCB->GetVariableByIndex( v);
					reflectionVar->GetDesc( &varDesc);
					if( varDesc.uFlags & D3D_SVF_USED)
					{
						D3D11_SHADER_TYPE_DESC typeDesc;
						reflectionVar->GetType()->GetDesc( &typeDesc);

						// 描述记录
						HLSL::VariableDesc variableDesc;
						variableDesc.m_name			= varDesc.Name;
						variableDesc.m_startOffset	= varDesc.StartOffset;
						variableDesc.m_size			= varDesc.Size;
						variableDesc.m_type			= static_cast<BYTE>(typeDesc.Type);
						variableDesc.m_rows			= static_cast<BYTE>(typeDesc.Rows);
						variableDesc.m_columns		= static_cast<BYTE>(typeDesc.Columns);
						variableDesc.m_elements		= static_cast<WORD>(typeDesc.Elements);
						m_shaderDescs[type].m_cbDesc[c].m_varDesc.push_back( variableDesc);

						// 值关联描述
						AddVariable( varDesc.Name, SPT_FloatArray);
					}
				}

				// Constand Buffer
				D3D11_BUFFER_DESC cbuffDesc;
				cbuffDesc.ByteWidth		= ( cbDesc.Size+15)/16*16;
				cbuffDesc.Usage			= D3D11_USAGE_DEFAULT;
				cbuffDesc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
				cbuffDesc.CPUAccessFlags= 0;
				cbuffDesc.MiscFlags		= 0;
				ID3D11Buffer* tCBuffer	= NULL;
				TIF( PDRenderDevice.GetD3DDevice()->CreateBuffer( &cbuffDesc, NULL, &tCBuffer));
				m_constantBuffers[type][c] = MakeCOMPtr( tCBuffer);
			}

			// 非常量数据类型
			for ( UINT i=0; i<desc.BoundResources; i++)
			{
				D3D11_SHADER_INPUT_BIND_DESC siDesc;
				reflection->GetResourceBindingDesc( i, &siDesc);

				// 描述记录
				HLSL::BoundResourceDesc boundResourceDesc;
				boundResourceDesc.m_name		= siDesc.Name;
				boundResourceDesc.m_type		= static_cast<BYTE>( siDesc.Type);
				boundResourceDesc.m_bindpoint	= static_cast<WORD>( siDesc.BindPoint);
				switch( siDesc.Type)
				{
				case D3D_SIT_TEXTURE:
					{
						m_shaderDescs[type].m_srvDesc.push_back( boundResourceDesc);

						AddVariable( siDesc.Name, SPT_Texture);
					}
					break;

				case D3D_SIT_SAMPLER:
					{			
						m_shaderDescs[type].m_sampDesc.push_back( boundResourceDesc);

						AddVariable( siDesc.Name, SPT_SamplerState, MakeShaderParam( PDRenderDevice.CreateSamplerState( SamplerStateDesc())));
					}
					break;
				}
			}

			m_samplers[type].resize( m_shaderDescs[type].m_sampDesc.size());
			m_srvs[type].resize( m_shaderDescs[type].m_srvDesc.size());
		}
	}

	// 剖析状态
	void D3D11RenderMethod::ParseStates()
	{
		RasterizerStateDesc		defaultRSDesc;
		DepthStencilStateDesc	defaultDSDesc;
		BlendStateDesc			defaultBSDesc;

		m_rasterState	    = PDRenderDevice.CreateRasterizerState( defaultRSDesc);
		m_depthStencilState = PDRenderDevice.CreateDepthStencilState( defaultDSDesc);
		m_blendState		= PDRenderDevice.CreateBlendState( defaultBSDesc);
	}

	// 向接口中添加变量
	void D3D11RenderMethod::AddVariable( const char* name, ShaderParamType type, ShaderParamPtr& defaultValue)
	{
		// 值关联描述
		ShaderParamDesc paramDesc;
		paramDesc.m_name = name;
		paramDesc.m_type = type;
		if( std::find( m_paramDescs.begin(), m_paramDescs.end(), paramDesc)==m_paramDescs.end())
		{
			m_paramDescs.push_back( paramDesc);
			m_curParamValues.push_back( defaultValue);
		}
	}
}