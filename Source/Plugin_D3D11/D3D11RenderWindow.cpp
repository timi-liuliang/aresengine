#include "D3D11Mapping.h"
#include "D3D11RenderView.h"
#include "D3D11RenderWindow.h"
#include "D3D11RenderDevice.h"
#include "D3D11Texture.h"
#include <boost/tuple/tuple.hpp>
#include <Core/AresPtr.h>
#include <Core/AresException.h>

namespace Ares
{
	// constructor
	D3D11RenderWindow::D3D11RenderWindow( IDXGIFactory1Ptr giFactory, D3D11Adapter* adapter, const RenderSettings& settings)
		: m_ready( false)
		, m_closed( false)
		, m_adapter( adapter)
		, m_giFactory( giFactory)
	{
		// default frame buffer
		ResetFrameBuffer();

		m_hwnd			= settings.m_hwnd;
		m_title			= settings.m_title;
		m_width			= settings.m_width;
		m_height		= settings.m_height;
		m_isFullScreeen = settings.m_fullScreen;
		m_syncIntervel	= settings.m_syncInterval;

		ElementFormat tFormat = settings.m_colorFormat;

		// 记录左上角坐标
		if( IsFullScreen())
		{
			m_left = 0;
			m_top  = 0;
		}
		else
		{
			m_top = settings.m_top;
			m_left= settings.m_left;
		}

		m_backBufferFormat = D3D11Mapping::MappingFormat( tFormat);

		// 设置swapchain
		std::memset( &m_swapChainDesc, 0, sizeof(m_swapChainDesc));
		m_swapChainDesc.BufferCount = 1;
		m_swapChainDesc.BufferUsage       = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		m_swapChainDesc.BufferDesc.Width  = GetWidth();
		m_swapChainDesc.BufferDesc.Height = GetHeight();
		m_swapChainDesc.BufferDesc.Format = m_backBufferFormat;
		m_swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		m_swapChainDesc.BufferDesc.Scaling= DXGI_MODE_SCALING_UNSPECIFIED;
		m_swapChainDesc.BufferDesc.RefreshRate.Numerator   = 60;
		m_swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

		m_swapChainDesc.OutputWindow = m_hwnd;
		m_swapChainDesc.SampleDesc.Count   = std::min<UINT>(static_cast<UINT>(D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT), settings.m_sampleCount);
		m_swapChainDesc.SampleDesc.Quality = settings.m_sampleQuality;
		m_swapChainDesc.Windowed  = !IsFullScreen();
		m_swapChainDesc.SwapEffect= DXGI_SWAP_EFFECT_DISCARD;
		m_swapChainDesc.Flags	  = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		D3D11RenderDevice* pRenderDevice = D3D11RenderDevice::GetSingletonPtr();
		if( pRenderDevice->GetD3DDevice())
		{
			// 新建swapChain
			m_d3dDevice = pRenderDevice->GetD3DDevice();

			IDXGISwapChain* pSwapChain = NULL;
			m_giFactory->CreateSwapChain( m_d3dDevice.get(), &m_swapChainDesc, &pSwapChain);

			m_swapChain = MakeCOMPtr( pSwapChain);
		}
		else
		{
			// create device and swapchain
			UINT tCreateDeviceFlags = 0;
			bool usePerfHUD         = ( m_adapter->GetDescription() == L"NVIDIA PerfHUD");

			vector<boost::tuple<string, D3D_DRIVER_TYPE>> deviceTypeBehaviors;
			if( usePerfHUD)
			{
				deviceTypeBehaviors.push_back( boost::make_tuple( "PerfHUD", D3D_DRIVER_TYPE_REFERENCE));
			}
			else
			{
				deviceTypeBehaviors.push_back( boost::make_tuple( "HW",   D3D_DRIVER_TYPE_HARDWARE));
				deviceTypeBehaviors.push_back( boost::make_tuple( "WARP", D3D_DRIVER_TYPE_WARP));
				deviceTypeBehaviors.push_back( boost::make_tuple( "SW",   D3D_DRIVER_TYPE_SOFTWARE));
				deviceTypeBehaviors.push_back( boost::make_tuple( "REF",  D3D_DRIVER_TYPE_REFERENCE));
			}

			// feature levles
			const D3D_FEATURE_LEVEL featureLevels[] = 
			{
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0,
				D3D_FEATURE_LEVEL_9_3,
			};
			size_t numFeatureLevels = sizeof(featureLevels) / sizeof(D3D_FEATURE_LEVEL);

			for ( size_t i=0; i<deviceTypeBehaviors.size(); i++)
			{
				IDXGISwapChain*		 swapChain   = NULL;
				ID3D11Device*		 d3d11Device = NULL;
				ID3D11DeviceContext* d3d11ImmCtx = NULL;
				IDXGIAdapter*		 d3dAdapter  = m_adapter->GetAdapter();
				D3D_DRIVER_TYPE      driveType   = deviceTypeBehaviors[i].get<1>();

				// If you set the pAdapter parameter to a non-NULL value, 
				// you must also set the DriverType parameter to the 
				// D3D_DRIVER_TYPE_UNKNOWN value <<Windows DirectX Graphics>>
				if ((D3D_DRIVER_TYPE_HARDWARE == driveType) || usePerfHUD)
				{
					d3dAdapter = m_adapter->GetAdapter();
					driveType  = D3D_DRIVER_TYPE_UNKNOWN;
				}

				D3D_FEATURE_LEVEL outFeatureLevel;
				HRESULT hr = D3D11CreateDeviceAndSwapChain( d3dAdapter, driveType, NULL, tCreateDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &m_swapChainDesc, &swapChain, &d3d11Device, &outFeatureLevel, &d3d11ImmCtx);
				if( SUCCEEDED( hr))
				{
					m_swapChain		  = MakeCOMPtr(swapChain);
					m_d3dDevice		  = MakeCOMPtr(d3d11Device);
					m_immediateContex = MakeCOMPtr(d3d11ImmCtx);

					pRenderDevice->SetDevice( m_d3dDevice, m_immediateContex, outFeatureLevel);
				}
			}

			m_mainWnd = true;
		}

		// 设定深度模板缓冲格式
		bool isDepthBuffered = IsDepthFormat( settings.m_depthStencilFormat);
		UINT depthBits		 = NumDepthBits( settings.m_depthStencilFormat);
		UINT stencilBits	 = NumStencilBits( settings.m_depthStencilFormat);
		if( isDepthBuffered)
		{
			BOOST_ASSERT( 32==depthBits || 24==depthBits || 16==depthBits || 0==depthBits);
			BOOST_ASSERT( 8==stencilBits||  0==stencilBits);

			UINT tFormatSupport;
			if( 32==depthBits)
			{
				BOOST_ASSERT( 0==stencilBits);

				// Try 32 bit zbuffer
				m_d3dDevice->CheckFormatSupport( DXGI_FORMAT_D32_FLOAT, &tFormatSupport);
				if( tFormatSupport & D3D11_FORMAT_SUPPORT_DEPTH_STENCIL)
				{
					m_depthStencilFormat = DXGI_FORMAT_D32_FLOAT;
					stencilBits = 0;
				}
				else
				{
					depthBits = 24;
				}
			}
			if( 24==depthBits)
			{
				m_d3dDevice->CheckFormatSupport( DXGI_FORMAT_D24_UNORM_S8_UINT, &tFormatSupport);
				if( tFormatSupport & D3D11_FORMAT_SUPPORT_DEPTH_STENCIL)
				{
					m_depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
					stencilBits = 8;
				}
				else
				{
					depthBits = 16;
				}
			}
			if( 16==depthBits)
			{
				m_d3dDevice->CheckFormatSupport( DXGI_FORMAT_D16_UNORM, &tFormatSupport);
				if( tFormatSupport & D3D11_FORMAT_SUPPORT_DEPTH_STENCIL)
				{
					m_depthStencilFormat = DXGI_FORMAT_D16_UNORM;
					stencilBits	= 0;
				}
				else
				{
					depthBits = 0;
				}
			}
		}

		m_giFactory->MakeWindowAssociation( m_hwnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);
		m_swapChain->SetFullscreenState( IsFullScreen(), NULL);

		UpdateSurfacePtrs();

		m_ready  = true;
		m_frameBuffer->SetActive( true);
	}

	// 更新SurfacePtrs
	void D3D11RenderWindow::UpdateSurfacePtrs()
	{
		// get backbuffer
		ID3D11Texture2D* backBuffer;
		TIF( m_swapChain->GetBuffer( 0, IID_ID3D11Texture2D, reinterpret_cast<void**>(&backBuffer)));
		m_backbuffer		= MakeCOMPtr( backBuffer);

		// get backbuffer desc
		D3D11_TEXTURE2D_DESC backBufferDesc;
		m_backbuffer->GetDesc( &backBufferDesc);
		m_backBufferTexture = D3D11Texture2DPtr( new_ D3D11Texture2D( m_backbuffer, backBufferDesc.SampleDesc.Count, backBufferDesc.SampleDesc.Quality, EAH_GPURead | EAH_GPUWrite));

		// create a render target view
		ID3D11RenderTargetView* renderTargetView;
		TIF( m_d3dDevice->CreateRenderTargetView( m_backbuffer.get(), NULL, &renderTargetView));
		m_renderTargetView = MakeCOMPtr( renderTargetView);

		// create depth stencil texture
		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width     = GetWidth();
		depthStencilDesc.Height	   = GetHeight();
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format    = m_depthStencilFormat;
		depthStencilDesc.SampleDesc.Count   = backBufferDesc.SampleDesc.Count;
		depthStencilDesc.SampleDesc.Quality = backBufferDesc.SampleDesc.Quality;
		depthStencilDesc.Usage	   = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags=0;
		depthStencilDesc.MiscFlags = 0;

		ID3D11Texture2D* depthStencil;
		TIF( m_d3dDevice->CreateTexture2D( &depthStencilDesc, NULL, &depthStencil));
		m_depthStencil = MakeCOMPtr( depthStencil);

		// create the depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		depthStencilViewDesc.Format = m_depthStencilFormat;
		if( backBufferDesc.SampleDesc.Count > 1)
			depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		else
			depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Flags  = 0;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		ID3D11DepthStencilView* depthStencilView;
		TIF( m_d3dDevice->CreateDepthStencilView( m_depthStencil.get(), &depthStencilViewDesc, &depthStencilView));
		m_depthStencilView = MakeCOMPtr( depthStencilView);

		// Attach backbuffer and depthstencil
		m_frameBuffer->AttachRenderView( FrameBuffer::ATT_Color0,      D3D11RenderTargetRenderViewPtr( new_ D3D11RenderTargetRenderView( m_renderTargetView, GetWidth(), GetHeight(), D3D11Mapping::MappingFormat( m_backBufferFormat))));
		m_frameBuffer->AttachRenderView( FrameBuffer::ATT_DepthStencil,D3D11DepthStencilRenderViewPtr( new_ D3D11DepthStencilRenderView( m_depthStencilView, GetWidth(), GetHeight(), D3D11Mapping::MappingFormat( m_depthStencilFormat))));
	
		// Bind
		PDRenderDevice.BindFrameBuffer( m_frameBuffer);
	}

	// 更改窗口大小
	void D3D11RenderWindow::Resize( size_t width, size_t height)
	{
		// no changes! return.
		if( m_width==width && m_height==height)
			return;

		m_width = width;
		m_height= height;

		if( m_immediateContex)
			m_immediateContex->ClearState();

		m_renderTargetView.reset();
		m_depthStencilView.reset();
		m_backbuffer.reset();
		m_depthStencil.reset();
		m_backBufferTexture.reset();
		m_backBufferTextureCopy.reset();

		UINT flags = 0;
		if( IsFullScreen())
			flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		// Reset FrameBuffer
		ResetFrameBuffer();
		 
		TIF( m_swapChain->ResizeBuffers( 1, width, height, m_backBufferFormat, flags));

		UpdateSurfacePtrs();

		m_frameBuffer->SetActive( true);
	}

	// 设置是否全屏
	void D3D11RenderWindow::SetFullScreen( bool fullScreen)
	{
		if( m_isFullScreeen != fullScreen)
		{
			m_isFullScreeen			 = fullScreen;
			m_swapChainDesc.Windowed = !fullScreen;
			UINT style			     = fullScreen ? WS_POPUP : WS_OVERLAPPEDWINDOW;

			HRESULT hr = m_swapChain->SetFullscreenState( m_isFullScreeen, NULL);
			if( m_isFullScreeen)
			{
				DXGI_MODE_DESC desc;
				std::memset( &desc, 0, sizeof(desc));
				desc.Width = m_width;
				desc.Height= m_height;
				desc.Format= m_backBufferFormat;
				desc.RefreshRate.Numerator  = 60;
				desc.RefreshRate.Denominator=  1;

				m_swapChain->ResizeTarget( &desc);
			}
		}
	}

	// Present the information rendered to the back buffer to the front buffer(the screen)
	void D3D11RenderWindow::SwapBuffers()
	{
		A_ASSERT( m_d3dDevice);
			
		TIF( m_swapChain->Present( m_syncIntervel, 0));
	}

	// 重置FrameBuffer
	void D3D11RenderWindow::ResetFrameBuffer()
	{
		if( PDRenderDevice.GetCurFrameBuffer() == m_frameBuffer)
			PDRenderDevice.ResetFrameBuffer();

		if( m_frameBuffer)
		{
			m_frameBuffer->Unbind();
			
			// 重置Buffer时, m_frameBuffer引用计数必须为1,以保证ResizeBuffers成功
			A_ASSERT( m_frameBuffer.unique());
		}

		// 设置视口
		ViewPort tViewPort( 0, 0, GetWidth(), GetHeight());
		m_frameBuffer =dynamic_pointer_cast<D3D11FrameBuffer>( PDRenderDevice.CreateFrameBuffer());
		m_frameBuffer->SetViewport( tViewPort);
	}

	// 获取背景缓冲纹理
	TexturePtr D3D11RenderWindow::GetBackbufferTextureCopy()
	{
		// 创建纹理
		if( !m_backBufferTextureCopy || (m_backBufferTextureCopy->GetWidth(0)!=m_width) || (m_backBufferTextureCopy->GetHeight(0)!=m_height))
		{
			m_backBufferTextureCopy	= dynamic_pointer_cast<D3D11Texture2D>(PDRenderDevice.CreateTexture2D( m_width, m_height, 1, 1, D3D11Mapping::MappingFormat( m_backBufferFormat), m_backBufferTexture->GetSampleCount(), m_backBufferTexture->GetSampleQuality(), EAH_GPURead | EAH_GPUWrite, NULL));
		}

		// 纹理拷贝
		m_backBufferTexture->CopyToTexure( *m_backBufferTextureCopy);

		return m_backBufferTextureCopy;
	}
}