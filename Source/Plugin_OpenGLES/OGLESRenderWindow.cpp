#include "OGLESRenderWindow.h"
#include "OGLESRenderDevice.h"
#include "OGLESRenderView.h"
#include "OGLESPlatform.h"

namespace Ares
{
	// 构造函数
	OGLESRenderWindow::OGLESRenderWindow( const RenderSettings& settings)
	{
		m_title				= settings.m_title;
		m_width				= settings.m_width;
		m_height			= settings.m_height;
		m_isFullScreeen		= settings.m_fullScreen;
		m_syncIntervel		= settings.m_syncInterval;
		m_colorFormat		= settings.m_colorFormat;		
		m_depthStencilFormat= settings.m_depthStencilFormat;	
		m_colorBits			= NumFormatBits( settings.m_colorFormat);

		UINT32 depthBits	= NumDepthBits( settings.m_depthStencilFormat);
		UINT32 stencilBits	= NumStencilBits( settings.m_depthStencilFormat);

#ifdef ARES_PLATFORM_WINDOWS
		if( IsFullScreen())
		{
			m_left = 0;
			m_top  = 0;

			DEVMODE devMode;
			devMode.dmSize		= sizeof(devMode);
			devMode.dmBitsPerPel= m_colorBits;
			devMode.dmPelsWidth	= m_width;
			devMode.dmPelsHeight= m_height;
			devMode.dmFields	= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
			ChangeDisplaySettings( &devMode, CDS_FULLSCREEN);
		}
		else
		{
			m_top = settings.m_top;
			m_left= settings.m_left;
		}

		m_display = eglGetDisplay( EGL_DEFAULT_DISPLAY);

		int rSize, gSize, bSize, aSize, dSize, sSize;
		switch( settings.m_colorFormat)
		{
		case EF_ARGB8:
		case EF_ABGR8:
			{
				rSize = 8;
				gSize = 8;
				bSize = 8;
				aSize = 8;
			}
			break;

		case EF_A2BGR10:
			{
				rSize = 10;
				gSize = 10;
				bSize = 10;
				aSize = 2;
			}
			break;

		default:
			A_ASSERT( false);
			break;
		}

		switch( settings.m_depthStencilFormat)
		{
		case EF_D16:
			{
				dSize = 16;
				sSize = 0;
			}
			break;

		case EF_D24S8:
			{
				dSize = 24;
				sSize = 8;
			}
			break;

		case EF_D32F:
			{
				dSize = 32;
				sSize = 0;
			}
			break;

		default:
			{
				dSize = 0;
				sSize = 0;
			}
			break;
		}

		vector<EGLint> visualAttr;
		visualAttr.push_back( EGL_RENDERABLE_TYPE);
		visualAttr.push_back( EGL_OPENGL_ES2_BIT);
		visualAttr.push_back( EGL_RED_SIZE);
		visualAttr.push_back( rSize);
		visualAttr.push_back( EGL_GREEN_SIZE);
		visualAttr.push_back( gSize);
		visualAttr.push_back( EGL_BLUE_SIZE);
		visualAttr.push_back( bSize);
		visualAttr.push_back( EGL_ALPHA_SIZE);
		visualAttr.push_back( aSize);
		if( dSize>0)
		{
			visualAttr.push_back( EGL_DEPTH_SIZE);
			visualAttr.push_back( dSize);
		}
		if( sSize>0)
		{
			visualAttr.push_back( EGL_STENCIL_SIZE);
			visualAttr.push_back( sSize);
		}
		if( settings.m_sampleCount>1)
		{
			visualAttr.push_back( EGL_SAMPLES);
			visualAttr.push_back( settings.m_sampleCount);
		}
		visualAttr.push_back( EGL_NONE);

		// 获取OPENGLES版本号
		EGLint majorVer, minorVer, numCfgs;
		eglInitialize( m_display, &majorVer, &minorVer);
		eglGetConfigs( m_display, NULL, 0, &numCfgs);
		eglChooseConfig( m_display, visualAttr.data(), &m_config, 1, &numCfgs);

		m_hwnd = settings.m_hwnd;
		m_hDC  = GetDC( m_hwnd);

		m_surface = eglCreateWindowSurface( m_display, m_config, m_hwnd, NULL);

		EGLint contexAttr[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
		m_context = eglCreateContext( m_display, m_config, EGL_NO_CONTEXT, contexAttr);

		eglMakeCurrent( m_display, m_surface, m_surface, m_context);

		eglSwapInterval( m_display, 0);

#elif defined ARES_PLATFORM_IOS
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
#endif

		OGLESDebug( glPixelStorei( GL_PACK_ALIGNMENT, 1));
		OGLESDebug( glPixelStorei( GL_UNPACK_ALIGNMENT, 1));

		// 描述信息
		ostringstream oss; oss<< glGetString( GL_VENDOR) << " " << glGetString( GL_RENDERER) << " " << glGetString(GL_VERSION);
		if( settings.m_sampleCount>1)
			oss << "(" << settings.m_sampleCount << "xAA)";
		m_description = oss.str();

		// Default frame buffer
		ResetFrameBuffer();

		UpdateSurfacePtrs();

		m_frameBuffer->SetActive( true);
	}

	// 更新SurfacePtrs
	void OGLESRenderWindow::UpdateSurfacePtrs()
	{
		// Attach backbuffer and depthstencil
		m_frameBuffer->AttachRenderView( FrameBuffer::ATT_Color0,      MakeSharePtr( new_ OGLScreenRenderTargetRenderView( GetWidth(), GetHeight(), m_colorFormat)));
		m_frameBuffer->AttachRenderView( FrameBuffer::ATT_DepthStencil,MakeSharePtr( new_ OGLScreenDepthStencilRenderView( GetWidth(), GetHeight(), m_depthStencilFormat)));

		// Bind
		PDRenderDevice.BindFrameBuffer( m_frameBuffer);
	}

	// 更改窗口大小
	void OGLESRenderWindow::Resize( size_t width, size_t height)
	{
		m_width = width;
		m_height= height;

		// Notify viewports of resize
		// 设置视口
		/*ViewPort tViewPort( 0, 0, GetWidth(), GetHeight());
		m_frameBuffer =dynamic_pointer_cast<D3D11FrameBuffer>( PDRenderDevice.CreateFrameBuffer());
		m_frameBuffer->SetViewport( tViewPort);*/
	}

	// 设置是否全屏
	void OGLESRenderWindow::SetFullScreen( bool fullScreen)
	{
		if( m_isFullScreeen!=fullScreen)
		{
			m_left			= 0;
			m_top			= 0;
			m_isFullScreeen = fullScreen;

#ifdef ARES_PLATFORM_WINDOWS
			UINT style;
			if( m_isFullScreeen)
			{
				DEVMODE devMode;
				devMode.dmSize		= sizeof( devMode);
				devMode.dmBitsPerPel= m_colorBits;
				devMode.dmPelsWidth = m_width;
				devMode.dmPelsHeight= m_height;
				devMode.dmFields	= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
				ChangeDisplaySettings( &devMode, CDS_FULLSCREEN);
				style				= WS_POPUP;
			}
			else
			{
				ChangeDisplaySettings( NULL, 0);
				style				= WS_OVERLAPPEDWINDOW;
			}

			SetWindowLongPtrW( m_hwnd, GWL_STYLE, style);

			//Rect rc = { 0, 0, static_cast<LONG>(m_width), static_cast<LONG>(m_height) };
			//AdjustWindowRect( &rc, style, false);
			//m_width = rc.right - rc.left;
			//m_height= rc.bottom- rc.top;

			ShowWindow( m_hwnd, SW_SHOWNORMAL);
			UpdateWindow( m_hwnd);

#elif defined ARES_PLATFORM_LINUX
			m_isFullScreeen = fullScreen;
			XFlush( m_xDisplay);
#elif defined ARES_PLATFORM_IOS
#endif
		}
	}

	// 重置
	void OGLESRenderWindow::ResetFrameBuffer()
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
		m_frameBuffer =dynamic_pointer_cast<OGLESFrameBuffer>( PDRenderDevice.CreateFrameBuffer());
		m_frameBuffer->SetViewport( tViewPort);
	}

	// Swap Buffers
	void OGLESRenderWindow::SwapBuffers()
	{
		OGLESDebug( eglSwapBuffers( m_display, m_surface));
	}
}