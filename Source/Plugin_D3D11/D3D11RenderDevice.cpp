#include "D3D11Texture.h"
#include "D3D11CgMethod.h"
#include "D3D11RenderMethod.h"
#include "D3D11RenderLayout.h"
#include "D3D11RenderState.h"
#include "D3D11RenderDevice.h"
#include "D3D11RenderView.h"

namespace Ares
{
	template<> D3D11RenderDevice* Singleton<D3D11RenderDevice>::m_singleton = NULL;

	// 构造函数
	D3D11RenderDevice::D3D11RenderDevice()
		: RenderDevice( D3D11)
		, m_numBuffers( 0)
		, m_preLayout( NULL)
		, m_preMaterial( NULL)
	{
		// 创建DXGIFactory
		IDXGIFactory1* pDXGIFactory1 = NULL;
		CreateDXGIFactory1( __uuidof(IDXGIFactory1), (void**)&pDXGIFactory1);
		m_giFactory = MakeCOMPtr( pDXGIFactory1);

		m_adapterList.Enumerate( m_giFactory.get());
	}

	// 析构函数
	D3D11RenderDevice::~D3D11RenderDevice()
	{

	}

	// 获取名字
	const char* D3D11RenderDevice::GetName() const
	{
		return "D3D11";
	}

	// 开始帧
	void D3D11RenderDevice::BeginFrame()
	{
		ResetRenderStates();
	}

	// 结束帧
	void D3D11RenderDevice::EndFrame()
	{

	}

	// 创建渲染窗口
	RenderWindow*  D3D11RenderDevice::CreateRenderWindow( const RenderSettings& settings)	
	{
		RenderDevice::CreateRenderWindow( settings);

		D3D11RenderWindow* renderWindow = new_ D3D11RenderWindow( m_giFactory, GetActiveAdapter(), settings);

		switch( m_d3dFeatureLevel)
		{
		case D3D_FEATURE_LEVEL_11_0:
			{

			}
			break;

		case D3D_FEATURE_LEVEL_10_1:
			{

			}
			break;

		case D3D_FEATURE_LEVEL_10_0:
			{

			}
			break;

		default:
			{

			}
			break;
		}

		// 重置渲染状态
		ResetRenderStates();

		// 绑定FrameBuffer
		BindFrameBuffer( renderWindow->GetFrameBuffer());

		m_renderWindows.push_back( renderWindow);

		return renderWindow;
	}

	// 获取ActiveAdapter
	D3D11Adapter* D3D11RenderDevice::GetActiveAdapter() const
	{
		return m_adapterList.GetAdapter( m_adapterList.GetCurrentAdapterIdx());
	}

	// 重置渲染状态
	void D3D11RenderDevice::ResetRenderStates()
	{
		m_vertexShaderCache.reset();
		m_pixelShaderCache.reset();
		m_geometryShaderCache.reset();
		m_compueShaderCache.reset();
		m_hullShaderCache.reset();
		m_domainShderCache.reset();

		m_rasterizerStateCache.reset();
		m_depthStencilStateCache.reset();
		m_blendStateCache.reset();

		m_stencilRefCache = 0;

		m_topologyTypeCache = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
	}

	// 创建1D纹理
	TexturePtr D3D11RenderDevice::CreateTexture1D( UINT widht, UINT numMipMaps, UINT arraySize, ElementFormat format, UINT sampleCount, UINT sampleQuality, UINT accessHine, const ElementInitData* initData)
	{
		return TexturePtr();
	}

	// 创建2D纹理
	TexturePtr D3D11RenderDevice::CreateTexture2D(UINT width, UINT height, UINT numMipMaps, UINT arraySize, ElementFormat format, UINT sampleCount, UINT sampleQuality, UINT accessHint, ElementInitData* const initData)
	{
		if( IsTextureFormatSupport( format))
			return TexturePtr( new_ D3D11Texture2D( width, height, numMipMaps, arraySize, format, sampleCount, sampleQuality, accessHint, initData));

		return TexturePtr();
	}

	// 创建FrameBuffer
	FrameBufferPtr D3D11RenderDevice::CreateFrameBuffer()
	{
		return D3D11FrameBufferPtr( new_ D3D11FrameBuffer( this));
	}

	// 设置光栅化状态
	void D3D11RenderDevice::RSSetState( const ID3D11RasterizerStatePtr& ras)
	{
		if( m_rasterizerStateCache != ras)
		{
			m_d3d11ImmContex->RSSetState( ras.get());
			m_rasterizerStateCache = ras;
		}
	}

	// 设置深度模板缓冲状态
	void D3D11RenderDevice::OMSetDepthStencilState( const ID3D11DepthStencilStatePtr& ds, UINT stencilRef)
	{
		if( m_depthStencilStateCache != ds || stencilRef != m_stencilRefCache)
		{
			m_d3d11ImmContex->OMSetDepthStencilState( ds.get(), stencilRef);
			m_depthStencilStateCache = ds;
			m_stencilRefCache        = stencilRef;
		}
	}

	// 设置混合状态
	void D3D11RenderDevice::OMSetBlendState( const ID3D11BlendStatePtr& bs, const ColorRGB blendFactor, UINT sampleMask)
	{
		if( m_blendStateCache != bs || m_blendFactorCache != blendFactor || m_sampleMaskCache != sampleMask)
		{
			m_d3d11ImmContex->OMSetBlendState( bs.get(), &blendFactor.r, sampleMask);

			m_blendStateCache = bs;
			m_blendFactorCache= blendFactor;
			m_sampleMaskCache = sampleMask;
		}
	}

	// 分离SRV
	void D3D11RenderDevice::DetachSRV( void* rtvSrc, UINT rtFirstSubres, UINT rtNumsubRes)
	{
		/*for ( UINT st=0; st<ShaderProgram::ST_NU)
		{
		}*/
	}

	// 设置视口
	void D3D11RenderDevice::RSSetViewports( UINT numViewPorts, const D3D11_VIEWPORT* viewPorts)
	{
		if( numViewPorts > 1)
		{
			m_d3d11ImmContex->RSSetViewports( numViewPorts, viewPorts);
		}
		else
		{
			if( true)
			{
				m_d3d11ImmContex->RSSetViewports( numViewPorts, viewPorts);
			}
		}
	}

	// 设置D3D11Device
	void D3D11RenderDevice::SetDevice( ID3D11DevicePtr& device, ID3D11DeviceContextPtr& immCtx, D3D_FEATURE_LEVEL featureLevel)
	{
		m_d3d11Device    = device;
		m_d3d11ImmContex = immCtx;
		m_d3dFeatureLevel= featureLevel;

		FillRenderDeviceCaps();

		// 效果文件初始化
		// D3D11CgMethod::InitCg( m_d3d11Device);
	}

	// 填充渲染设备能力
	void D3D11RenderDevice::FillRenderDeviceCaps()
	{
		BOOST_ASSERT( m_d3d11Device);

		if( m_d3dFeatureLevel <= D3D_FEATURE_LEVEL_9_3)
		{
			m_deviceCaps.m_maxVertexTextureUnits = 0;
			m_deviceCaps.m_maxTextureHeight		 = m_deviceCaps.m_maxTextureWidth = (D3D_FEATURE_LEVEL_9_3==m_d3dFeatureLevel) ? 4096 : 2048;
			m_deviceCaps.m_maxTextureDepth       = 256;
			m_deviceCaps.m_maxTextureCubeSize    = (D3D_FEATURE_LEVEL_9_3==m_d3dFeatureLevel) ? 4096 : 512;
			m_deviceCaps.m_maxTextureArrayLength = 1;
			m_deviceCaps.m_maxSimulataneousRts	 = (D3D_FEATURE_LEVEL_9_3==m_d3dFeatureLevel) ? 4: 1;
			m_deviceCaps.m_maxVertices			 = (D3D_FEATURE_LEVEL_9_1==m_d3dFeatureLevel) ? 65535 : 1048575;
			m_deviceCaps.m_maxIndices			 = (D3D_FEATURE_LEVEL_9_1==m_d3dFeatureLevel) ? 65535 : 1048575;
			m_deviceCaps.m_alphaToCoverageSupport= false;
			m_deviceCaps.m_mrtIndependentBitDepthsSupport = false;
		}
		else
		{
			m_deviceCaps.m_maxVertexTextureUnits = 16;
			m_deviceCaps.m_maxTextureHeight		 = m_deviceCaps.m_maxTextureWidth = 8192;
			m_deviceCaps.m_maxTextureDepth		 = 2048;
			m_deviceCaps.m_maxTextureCubeSize	 = 8192;
			m_deviceCaps.m_maxTextureArrayLength = 512;
			m_deviceCaps.m_maxSimulataneousRts	 = 8;
			m_deviceCaps.m_maxVertices			 = 8388607;
			m_deviceCaps.m_maxIndices			 = 16777215;
			m_deviceCaps.m_alphaToCoverageSupport= true;
			m_deviceCaps.m_mrtIndependentBitDepthsSupport = true;
		}

		m_deviceCaps.m_maxVertexStreams		= 16;
		m_deviceCaps.m_maxTextureAnisotropy = (D3D_FEATURE_LEVEL_9_1==m_d3dFeatureLevel) ? 2 : 16;
		m_deviceCaps.m_hwInstancingSupport  = (m_d3dFeatureLevel>=D3D_FEATURE_LEVEL_9_3);
		m_deviceCaps.m_instanceIdSupport	= (m_d3dFeatureLevel>=D3D_FEATURE_LEVEL_10_0);
		{
			D3D11_FEATURE_DATA_THREADING mtFeature;
			m_d3d11Device->CheckFeatureSupport(D3D11_FEATURE_THREADING, &mtFeature, sizeof(mtFeature));

			m_deviceCaps.m_multithreadRenderingSupport	= mtFeature.DriverCommandLists      ? true : false;
			m_deviceCaps.m_multithreadResCreatingSupport= mtFeature.DriverConcurrentCreates ? true : false;
		}

		switch( m_d3dFeatureLevel)
		{
		case D3D_FEATURE_LEVEL_11_0:
			{
				m_deviceCaps.m_maxShaderModel		   = 5;
				m_deviceCaps.m_streamOutputSupport	   = true;
				m_deviceCaps.m_maxPixelTextureUnits	   = 32;
				m_deviceCaps.m_maxGeometryTextureUnits = 32;
				m_deviceCaps.m_primitiveRestartSupport = true;
				m_deviceCaps.m_gsSupport			   = true;
				m_deviceCaps.m_csSupport			   = true;
				m_deviceCaps.m_dsSupport			   = true;
				m_deviceCaps.m_tessllationMethod	   = TM_Hardware;
			}
			break;
		case D3D_FEATURE_LEVEL_10_1:
		case D3D_FEATURE_LEVEL_10_0:
			{
				m_deviceCaps.m_maxShaderModel		   = 4;
				m_deviceCaps.m_streamOutputSupport	   = true;
				m_deviceCaps.m_maxPixelTextureUnits	   = 32;
				m_deviceCaps.m_maxGeometryTextureUnits =32;
				m_deviceCaps.m_primitiveRestartSupport =true;
				m_deviceCaps.m_gsSupport			   = true;
				m_deviceCaps.m_hsSupport			   = false;
				m_deviceCaps.m_dsSupport			   = false;
				m_deviceCaps.m_tessllationMethod	   = TM_Instanced;

				D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS cs4Feature;
				m_d3d11Device->CheckFeatureSupport( D3D11_FEATURE_D3D10_X_HARDWARE_OPTIONS, &cs4Feature, sizeof(cs4Feature));
				m_deviceCaps.m_csSupport = cs4Feature.ComputeShaders_Plus_RawAndStructuredBuffers_Via_Shader_4_x ? true : false;
			}
			break;
		default:
			{
				m_deviceCaps.m_maxShaderModel			= 2;
				m_deviceCaps.m_streamOutputSupport		= false;
				m_deviceCaps.m_maxPixelTextureUnits		= 16;
				m_deviceCaps.m_maxGeometryTextureUnits = 0;
				m_deviceCaps.m_primitiveRestartSupport	= false;
				m_deviceCaps.m_gsSupport				= false;
				m_deviceCaps.m_csSupport				= false;
				m_deviceCaps.m_hsSupport				= false;
				m_deviceCaps.m_dsSupport				= false;
				m_deviceCaps.m_tessllationMethod		= TM_No;
			}
			break;
		}

		// 格式对应
		std::pair<ElementFormat, DXGI_FORMAT> formats[] =
		{
			std::make_pair(EF_R8, DXGI_FORMAT_R8_UNORM),
			std::make_pair(EF_SIGNED_R8, DXGI_FORMAT_R8_SNORM),
			std::make_pair(EF_GR8, DXGI_FORMAT_R8G8_UNORM),
			std::make_pair(EF_SIGNED_GR8, DXGI_FORMAT_R8G8_SNORM),
			std::make_pair(EF_ARGB8, DXGI_FORMAT_B8G8R8A8_UNORM),
			std::make_pair(EF_ABGR8, DXGI_FORMAT_R8G8B8A8_UNORM),
			std::make_pair(EF_SIGNED_ABGR8, DXGI_FORMAT_R8G8B8A8_SNORM),
			std::make_pair(EF_A2BGR10, DXGI_FORMAT_R10G10B10A2_UNORM),
			std::make_pair(EF_R8UI, DXGI_FORMAT_R8_UINT),
			std::make_pair(EF_R8I, DXGI_FORMAT_R8_SINT),
			std::make_pair(EF_GR8UI, DXGI_FORMAT_R8G8_UINT),
			std::make_pair(EF_GR8I, DXGI_FORMAT_R8G8_SINT),
			std::make_pair(EF_ABGR8UI, DXGI_FORMAT_R8G8B8A8_UINT),
			std::make_pair(EF_ABGR8I, DXGI_FORMAT_R8G8B8A8_SINT),
			std::make_pair(EF_A2BGR10UI, DXGI_FORMAT_R10G10B10A2_UINT),
			std::make_pair(EF_R16, DXGI_FORMAT_R16_UNORM),
			std::make_pair(EF_SIGNED_R16, DXGI_FORMAT_R16_SNORM),
			std::make_pair(EF_GR16, DXGI_FORMAT_R16G16_UNORM),
			std::make_pair(EF_SIGNED_GR16, DXGI_FORMAT_R16G16_SNORM),
			std::make_pair(EF_ABGR16, DXGI_FORMAT_R16G16B16A16_UNORM),
			std::make_pair(EF_SIGNED_ABGR16, DXGI_FORMAT_R16G16B16A16_SNORM),
			std::make_pair(EF_R16UI, DXGI_FORMAT_R16_UINT),
			std::make_pair(EF_R16I, DXGI_FORMAT_R16_SINT),
			std::make_pair(EF_GR16UI, DXGI_FORMAT_R16G16_UINT),
			std::make_pair(EF_GR16I, DXGI_FORMAT_R16G16_SINT),
			std::make_pair(EF_ABGR16UI, DXGI_FORMAT_R16G16B16A16_UINT),
			std::make_pair(EF_ABGR16I, DXGI_FORMAT_R16G16B16A16_SINT),
			std::make_pair(EF_R32UI, DXGI_FORMAT_R32_UINT),
			std::make_pair(EF_R32I, DXGI_FORMAT_R32_SINT),
			std::make_pair(EF_GR32UI, DXGI_FORMAT_R32G32_UINT),
			std::make_pair(EF_GR32I, DXGI_FORMAT_R32G32_SINT),
			std::make_pair(EF_BGR32UI, DXGI_FORMAT_R32G32B32_UINT),
			std::make_pair(EF_BGR32I, DXGI_FORMAT_R32G32B32_SINT),
			std::make_pair(EF_ABGR32UI, DXGI_FORMAT_R32G32B32A32_UINT),
			std::make_pair(EF_ABGR32I, DXGI_FORMAT_R32G32B32A32_SINT),
			std::make_pair(EF_R16F, DXGI_FORMAT_R16_FLOAT),
			std::make_pair(EF_GR16F, DXGI_FORMAT_R16G16_FLOAT),
			std::make_pair(EF_B10G11R11F, DXGI_FORMAT_R11G11B10_FLOAT),
			std::make_pair(EF_ABGR16F, DXGI_FORMAT_R16G16B16A16_FLOAT),
			std::make_pair(EF_R32F, DXGI_FORMAT_R32_FLOAT),
			std::make_pair(EF_GR32F, DXGI_FORMAT_R32G32_FLOAT),
			std::make_pair(EF_BGR32F, DXGI_FORMAT_R32G32B32_FLOAT),
			std::make_pair(EF_ABGR32F, DXGI_FORMAT_R32G32B32A32_FLOAT),
			std::make_pair(EF_BC1, DXGI_FORMAT_BC1_UNORM),
			std::make_pair(EF_BC2, DXGI_FORMAT_BC2_UNORM),
			std::make_pair(EF_BC3, DXGI_FORMAT_BC3_UNORM),
			std::make_pair(EF_BC4, DXGI_FORMAT_BC4_UNORM),
			std::make_pair(EF_SIGNED_BC4, DXGI_FORMAT_BC4_SNORM),
			std::make_pair(EF_BC5, DXGI_FORMAT_BC5_UNORM),
			std::make_pair(EF_SIGNED_BC5, DXGI_FORMAT_BC5_SNORM),
			std::make_pair(EF_BC6, DXGI_FORMAT_BC6H_UF16),
			std::make_pair(EF_SIGNED_BC6, DXGI_FORMAT_BC6H_SF16),
			std::make_pair(EF_BC7, DXGI_FORMAT_BC7_UNORM),
			std::make_pair(EF_D16, DXGI_FORMAT_D16_UNORM),
			std::make_pair(EF_D24S8, DXGI_FORMAT_D24_UNORM_S8_UINT),
			std::make_pair(EF_D32F, DXGI_FORMAT_D32_FLOAT),
			std::make_pair(EF_ARGB8_SRGB, DXGI_FORMAT_B8G8R8A8_UNORM_SRGB),
			std::make_pair(EF_ABGR8_SRGB, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB),
			std::make_pair(EF_BC1_SRGB, DXGI_FORMAT_BC1_UNORM_SRGB),
			std::make_pair(EF_BC2_SRGB, DXGI_FORMAT_BC2_UNORM_SRGB),
			std::make_pair(EF_BC3_SRGB, DXGI_FORMAT_BC3_UNORM_SRGB),
			std::make_pair(EF_BC7_SRGB, DXGI_FORMAT_BC7_UNORM_SRGB)
		};

		// 检测支持的格式
		size_t tNumFormats = sizeof(formats)/sizeof(formats[0]);
		for ( size_t i=0; i< tNumFormats; i++)
		{
			UINT formatSupport;
			m_d3d11Device->CheckFormatSupport( formats[i].second, &formatSupport);
			if( formatSupport!=0)
			{
				m_vertexFormats.insert( formats[i].first);
				m_textureFormats.insert( formats[i].first);

				UINT count   = 1;
				UINT quality = 1;
				while( count < D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT)
				{
					if( SUCCEEDED( m_d3d11Device->CheckMultisampleQualityLevels( formats[i].second, count, &quality)))
					{
						if( quality>0)
						{
							m_renderTargetFormats[formats[i].first].push_back( make_pair( count, quality));
							count <<= 1;
						}
						else
							break;
					}
					else
						break;
				}
			}
		}

		// 绑定IsFormatSupport函数
		m_deviceCaps.IsVertexFormatSupport		 = boost::bind<bool>(&D3D11RenderDevice::IsVertexFormatSupport,  this, _1);
		m_deviceCaps.IsTextureFormatSupport		 = boost::bind<bool>(&D3D11RenderDevice::IsTextureFormatSupport, this, _1);
		m_deviceCaps.IsRenderTargetFormatSupport = boost::bind<bool>(&D3D11RenderDevice::IsRenderTargetFormatSupport, this, _1, _2, _3);
	}

	// 是否顶点格式支持
	bool D3D11RenderDevice::IsVertexFormatSupport( ElementFormat eleformat)
	{
		return m_vertexFormats.find( eleformat) != m_vertexFormats.end();
	}

	// 是否支持纹理格式
	bool D3D11RenderDevice::IsTextureFormatSupport( ElementFormat eleformat)
	{
		return m_textureFormats.find( eleformat) != m_textureFormats.end();
	}

	// 是否支持RenderTarget格式
	bool D3D11RenderDevice::IsRenderTargetFormatSupport( ElementFormat eleformat, UINT sampleCount, UINT sampleQuality)
	{
		RenderTargetFormat::iterator iter = m_renderTargetFormats.find( eleformat);
		if( iter != m_renderTargetFormats.end())
		{
			for( size_t i=0; i<iter->second.size(); i++)
			{
				pair<UINT, UINT> tValue = iter->second[i];
				if( sampleCount==tValue.first && sampleQuality==tValue.second)
					return true;
			}
		}

		return false;
	}

	// 创建RenderLayout
	RenderLayoutPtr D3D11RenderDevice::CreateRenderLayout()
	{
		return RenderLayoutPtr( new_ D3D11RenderLayout);
	}

	// 创建索引缓冲区
	GraphicBufferPtr D3D11RenderDevice::CreateIndexBuffer( BufferUsage usage, UINT accessHint, const ElementInitData* initData, ElementFormat format/*=EF_UnKnown*/)
	{
		return GraphicBufferPtr( new_ D3D11GraphicsBuffer( usage, accessHint, D3D11_BIND_INDEX_BUFFER, initData, format));
	}

	// 创建顶点缓冲区
	GraphicBufferPtr D3D11RenderDevice::CreateVertexBuffer( BufferUsage usage, UINT accessHint,const ElementInitData* initData, ElementFormat format/*=EF_UnKnown*/)
	{
		return GraphicBufferPtr( new_ D3D11GraphicsBuffer( usage, accessHint, D3D11_BIND_VERTEX_BUFFER, initData, format));
	}

	// 创建效果文件 "*.hlsl"
	RenderMethodPtr D3D11RenderDevice::CreateRenderMethod()
	{
		return RenderMethodPtr( new_ D3D11RenderMethod());
	}

	// 创建纹理采样状态
	SamplerStatePtr D3D11RenderDevice::CreateSamplerState( const SamplerStateDesc& desc)
	{
		SamplerStatePool::iterator it = m_ssPool.find( desc);
		if( it==m_ssPool.end())
		{
			SamplerStatePtr samplerState = SamplerStatePtr( new_ D3D11SamplerState( desc));
			if( samplerState)
				m_ssPool[desc] = samplerState;

			return samplerState;
		}

		return it->second;
	}

	// 创建光栅化状态
	RasterizerStatePtr& D3D11RenderDevice::CreateRasterizerState( const RasterizerStateDesc& desc)
	{
		RasterizerStatePool::iterator it = m_rsPool.find( desc);
		if( it==m_rsPool.end())
		{		
			m_rsPool[desc] = RasterizerStatePtr( new_ D3D11RasterizerState( desc));

			return m_rsPool[desc];
		}

		return it->second;
	}

	// 创建深度模板状态
	DepthStencilStatePtr D3D11RenderDevice::CreateDepthStencilState( const DepthStencilStateDesc& desc)
	{
		return DepthStencilStatePtr( new_ D3D11DepthStencilState( desc));
	}

	// 创建混合状态 
	BlendStatePtr D3D11RenderDevice::CreateBlendState( const BlendStateDesc& desc)
	{
		BlendStatePool::iterator it = m_bsPool.find( desc);
		if( it==m_bsPool.end())
		{
			BlendStatePtr blendState = BlendStatePtr( new_ D3D11BlendState( desc));
			if( blendState)
				m_bsPool[desc] = blendState;

			return blendState;
		}

		return it->second;
	}

	// 渲染Renderable
	void D3D11RenderDevice::Render( RenderLayout* layout, RenderMaterial* material)
	{
		D3D11RenderLayout* preRenderLayout = dynamic_cast<D3D11RenderLayout*>( m_preLayout);
		D3D11RenderLayout* curRenderLayout = dynamic_cast<D3D11RenderLayout*>( layout);
		D3D11RenderMethod* hlslMethod	   = dynamic_cast<D3D11RenderMethod*>(material->GetMethod().get());

		A_ASSERT( hlslMethod);

		if( hlslMethod->IsEnabled())
		{
			// 设置材质(BindProgram, 设置参数状态)
			material->SetActive();

			// 绘制几何体
			curRenderLayout->Render( preRenderLayout, hlslMethod);

			m_preLayout   = layout;
			m_preMaterial = material;
		}
	}

	// 创建RenderView
	RenderViewPtr D3D11RenderDevice::Create2DRenderView( TexturePtr texture, int firstArrayIndex, int arraySize, int levle)
	{
		return D3D11RenderTargetRenderViewPtr( new_ D3D11RenderTargetRenderView( texture, firstArrayIndex, arraySize, levle));
	}

	// 创建深度View
	RenderViewPtr D3D11RenderDevice::Create2DDepthStencilRenderView( UINT width, UINT height, ElementFormat format, UINT sampleCount, UINT sampleQuality)
	{
		return D3D11DepthStencilRenderViewPtr( new_ D3D11DepthStencilRenderView( width, height, format, sampleCount, sampleQuality));
	}

	// 创建深度View
	RenderViewPtr D3D11RenderDevice::Create2DDepthStencilRenderView( TexturePtr& texture, int firstArrayIndex, int arraySize, int level)
	{
		return D3D11DepthStencilRenderViewPtr( new_ D3D11DepthStencilRenderView( texture, firstArrayIndex, arraySize, level));
	}

	// 设置顶点着色器
	void D3D11RenderDevice::VSSetShader( const ID3D11VertexShaderPtr& shader)
	{
		if( m_vertexShaderCache != shader)
		{
			m_d3d11ImmContex->VSSetShader( shader.get(), NULL, 0);
			m_vertexShaderCache = shader;
		}
	}

	// 设置像像素着色器
	void D3D11RenderDevice::PSSetShader( const ID3D11PixelShaderPtr& shader)
	{
		if( m_pixelShaderCache != shader)
		{
			m_d3d11ImmContex->PSSetShader( shader.get(), NULL, 0);
			m_pixelShaderCache = shader;
		}
	}

	// 设置几何着色器
	void D3D11RenderDevice::GSSetShader( const ID3D11GeometryShaderPtr& shader)
	{
		if( m_geometryShaderCache != shader)
		{
			m_d3d11ImmContex->GSSetShader( shader.get(), NULL, 0);
			m_geometryShaderCache = shader;
		}
	}

	// 设置计算着色器
	void D3D11RenderDevice::CSSetShader( const ID3D11ComputeShaderPtr& shader)
	{
		if( m_compueShaderCache != shader)
		{
			m_d3d11ImmContex->CSSetShader( shader.get(), NULL, 0);
			m_compueShaderCache = shader;
		}
	}

	// 设置Hull着色器
	void D3D11RenderDevice::HSSetShader( const ID3D11HullShaderPtr& shader)
	{
		if( m_hullShaderCache != shader)
		{
			m_d3d11ImmContex->HSSetShader( shader.get(), NULL, 0);
			m_hullShaderCache = shader;
		}
	}

	// 设置Domain着色器
	void D3D11RenderDevice::DSSetShader( const ID3D11DomainShaderPtr& shader)
	{
		if( m_domainShderCache != shader)
		{
			m_d3d11ImmContex->DSSetShader( shader.get(), NULL, 0);
			m_domainShderCache = shader;
		}
	}

	// 设置常量Buffer
	void D3D11RenderDevice::SetConstantBuffers( const vector<ID3D11BufferPtr>& cbs, ShaderType shaderType)
	{
		static vector<ID3D11Buffer*> buffers;
		buffers.resize( cbs.size());
		for ( size_t i=0; i<cbs.size(); i++)
			buffers[i] = cbs[i].get();

		if( cbs.size())
		{
			switch( shaderType)
			{
			case ST_VertexShader:
					m_d3d11ImmContex->VSSetConstantBuffers( 0, cbs.size(), buffers.data());
				break;			
			case ST_PixelShader:
					m_d3d11ImmContex->PSSetConstantBuffers( 0, cbs.size(), buffers.data());
				break;
			case ST_GeometryShader:
					m_d3d11ImmContex->GSSetConstantBuffers( 0, cbs.size(), buffers.data());
				break;
			case ST_ComputeShader:
					m_d3d11ImmContex->CSSetConstantBuffers( 0, cbs.size(), buffers.data());
				break;
			case ST_HullShader:
					m_d3d11ImmContex->HSSetConstantBuffers( 0, cbs.size(), buffers.data());
				break;
			case ST_DomainShader:
					m_d3d11ImmContex->DSSetConstantBuffers( 0, cbs.size(), buffers.data());
				break;
			}	
		}	
	}

	// 设置资源
	void D3D11RenderDevice::SetShaderResources( const vector<ID3D11ShaderResourceViewPtr>& res, ShaderType shaderType)
	{
		static vector<ID3D11ShaderResourceView*> buffers;
		buffers.resize( res.size());
		for ( size_t i=0; i<res.size(); i++)
			buffers[i] = res[i].get();

		if( buffers.size())
		{
			switch( shaderType)
			{
			case ST_VertexShader:
					m_d3d11ImmContex->VSSetShaderResources( 0, buffers.size(), buffers.data());
				break;			
			case ST_PixelShader:
					m_d3d11ImmContex->PSSetShaderResources( 0, buffers.size(), buffers.data());
				break;
			case ST_GeometryShader:
					m_d3d11ImmContex->GSSetShaderResources( 0, buffers.size(), buffers.data());
				break;
			case ST_ComputeShader:
					m_d3d11ImmContex->CSSetShaderResources( 0, buffers.size(), buffers.data());
				break;
			case ST_HullShader:
					m_d3d11ImmContex->HSSetShaderResources( 0, buffers.size(), buffers.data());
				break;
			case ST_DomainShader:
					m_d3d11ImmContex->DSSetShaderResources( 0, buffers.size(), buffers.data());
				break;
			}	
		}	
	}

	// 设置采样状态
	void D3D11RenderDevice::SetSamplers( const vector<ID3D11SamplerStatePtr>& samplers, ShaderType shaderType)
	{
		static vector<ID3D11SamplerState*> buffers;
		buffers.resize( samplers.size());
		for ( size_t i=0; i<samplers.size(); i++)
			buffers[i] = samplers[i].get();

		if( buffers.size())
		{
			switch( shaderType)
			{
			case ST_VertexShader:
					m_d3d11ImmContex->VSSetSamplers( 0, buffers.size(), buffers.data());
				break;			
			case ST_PixelShader:
					m_d3d11ImmContex->PSSetSamplers( 0, buffers.size(), buffers.data());
				break;
			case ST_GeometryShader:
					m_d3d11ImmContex->GSSetSamplers( 0, buffers.size(), buffers.data());
				break;
			case ST_ComputeShader:
					m_d3d11ImmContex->CSSetSamplers( 0, buffers.size(), buffers.data());
				break;
			case ST_HullShader:
					m_d3d11ImmContex->HSSetSamplers( 0, buffers.size(), buffers.data());
				break;
			case ST_DomainShader:
					m_d3d11ImmContex->DSSetSamplers( 0, buffers.size(), buffers.data());
				break;
			}	
		}	
	}

	// 设置拓扑类型
	void D3D11RenderDevice::SetTopologyType( D3D11_PRIMITIVE_TOPOLOGY topologyType)
	{
		if( m_topologyTypeCache!=topologyType)
		{
			m_d3d11ImmContex->IASetPrimitiveTopology( topologyType);
			m_topologyTypeCache = topologyType;
		}
	}
}
