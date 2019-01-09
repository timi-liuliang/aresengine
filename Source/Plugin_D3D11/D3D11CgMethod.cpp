//#include "D3D11CgMethod.h"
//#include "D3D11Texture.h"
//#include "D3D11RenderState.h"
//#include <Core/Graphics/AresRenderMaterial.h>
//#include <boost/format.hpp>
//#include <string>
//
//using namespace std;
//
//namespace Ares
//{
//	CGcontext D3D11CgMethod::m_cgContext = NULL;		// Cg环境(全局唯一)
//
//	// 初始化
//	bool D3D11CgMethod::InitCg( ID3D11DevicePtr& d3d11Device)
//	{
//		if( !m_cgContext)
//		{
//			m_cgContext = cgCreateContext();
//
//			cgD3D11SetDevice( m_cgContext, d3d11Device.get());
//
//			cgD3D11RegisterStates( m_cgContext );
//			cgD3D11SetManageTextureParameters( m_cgContext, CG_TRUE );
//		}
//
//		return true;
//	}
//
//	// 构造函数
//	D3D11CgMethod::D3D11CgMethod()
//		: RenderMethod()
//	{
//
//	}
//
//	// 析构函数
//	D3D11CgMethod::~D3D11CgMethod()
//	{
//
//	}
//
//	// 加载函数 *.cgfx
//	bool D3D11CgMethod::Load(  const char* fileName)
//	{
//		m_cgEffect = cgCreateEffectFromFile( m_cgContext, fileName, NULL);
//		if( m_cgEffect)
//		{
//			// 解析参数描述
//			ParseParameters();
//
//			// 解析状态
//			ParseStates();
//
//			return true;
//		}
//
//		// 显示cg错误信息
//		string error;
//		CheckForCgError( &error);
//
//		A_ASSERT( false);
//		return false;
//	}
//
//	// 从内存加载
//	bool D3D11CgMethod::LoadFromMemory( void* code, size_t length)
//	{
//		m_cgEffect = cgCreateEffect( m_cgContext, (const char*)code, NULL);
//
//		return false;
//	}
//
//	// 应用参数
//	void D3D11CgMethod::SetParams(const RenderMaterial& mat)
//	{
//		cgSetPassState( m_cgPass);
//
//		for ( size_t i=0; i<m_paramDescs.size(); i++)
//		{
//			const ShaderParamPtr& pParam = mat.GetShaderParam(i);
//			if( pParam /*&& *pParam != *m_curParamValues[i]*/)
//			{
//				*m_curParamValues[i] = *pParam;
//
//				if( pParam->m_type != ShaderParam::SPT_Texture)
//				{
//					cgSetParameterValuefr( m_cgParameters[i], m_curParamValues[i]->m_elems,  m_curParamValues[i]->m_dataFloat);
//				}
//				else
//				{
//					D3D11Texture2D*	    tex2D		 = dynamic_cast<D3D11Texture2D*>( (*m_curParamValues[i]->m_texture).get());
//					D3D11SamplerState*  samplerState = dynamic_cast<D3D11SamplerState*>( (*m_curParamValues[i]->m_samplerState).get());
//					ID3D11SamplerState* d3d11SamplerState = samplerState ? samplerState->GetD3D11SamperState().get() : NULL;
//					if( tex2D)
//					{
//						string m_name	 = cgGetParameterName( m_cgParameters[i]);
//
//						cgD3D11SetTextureParameter( m_cgParameters[i],  tex2D->GetD3DResource().get());
//						cgD3D11SetSamplerStateParameter( m_cgParameters[i], d3d11SamplerState);
//					}
//				}
//			}
//		}
//	}
//
//	// 剖析参数
//	void D3D11CgMethod::ParseParameters()
//	{
//		CGparameter cgParameter = cgGetFirstEffectParameter( m_cgEffect);
//		while( cgParameter)
//		{
//			ShaderParamDesc paramDesc;
//
//			paramDesc.m_name	 = cgGetParameterName( cgParameter);
//
//			m_paramDescs.push_back( paramDesc);
//			m_curParamValues.push_back( ShaderParamPtr( new_ ShaderParam()));
//			m_cgParameters.push_back( cgParameter);
//
//			cgParameter = cgGetNextParameter( cgParameter);
//		}
//	}
//
//	// 剖析状态
//	void D3D11CgMethod::ParseStates()
//	{
//		m_cgTechnique = cgGetFirstTechnique( m_cgEffect);
//
//		if( !m_cgTechnique)
//			CheckForCgError();
//
//		V_ASSERT( cgValidateTechnique( m_cgTechnique));
//
//		m_cgPass = cgGetFirstPass( m_cgTechnique);
//	}
//
//	void D3D11CgMethod::CheckForCgError( string* error)
//	{
//		if( error)
//		{
//			format fmt( "Error:%s; Cg compile output:%s;");
//
//			CGerror cgError;
//			fmt % cgGetLastErrorString(&cgError);
//
//			if( cgError != CG_NO_ERROR ) 
//			{
//				if( cgError == CG_COMPILER_ERROR ) 
//					fmt % cgGetLastListing(m_cgContext);
//				else 
//					fmt % "";
//			}
//
//			*error = fmt.str();
//		}
//	}
//}