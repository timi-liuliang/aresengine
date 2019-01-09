#include "OGLRenderWindow.h"
#include "OGLRenderDevice.h"
#include "OGLRenderView.h"

namespace Ares
{
	// 构造函数
	OGLRenderWindow::OGLRenderWindow( const RenderSettings& settings)
	{
		// Default frame buffer
		ResetFrameBuffer();

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
		m_hwnd = settings.m_hwnd;
		m_hDC  = GetDC( m_hwnd);

		UINT32 style;
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

			style = WS_POPUP;
		}
		else
		{
			m_top = settings.m_top;
			m_left= settings.m_left;

			style = WS_OVERLAPPEDWINDOW;
		}

		//RECT rc = { 0, 0, static_cast<LONG>(m_width), static_cast<LONG>(m_height) };
		//::AdjustWindowRect( &rc, style, false);

		//::SetWindowLongPtrW( m_hwnd, GWL_STYLE, style);
		//::SetWindowPos( m_hwnd, NULL, settings.m_left, settings.m_top, rc.right-rc.left, rc.bottom-rc.top, SWP_SHOWWINDOW | SWP_NOZORDER);

		// there is no guarantee that the contents of the stack that become
		// the pfd are zeroed, therefore _make sure to clear these bits
		PIXELFORMATDESCRIPTOR pfd;
		std::memset( &pfd, 0, sizeof(pfd));
		pfd.nSize			= sizeof( pfd);
		pfd.nVersion		= 1;
		pfd.dwFlags			= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType		= PFD_TYPE_RGBA;
		pfd.cColorBits		= static_cast<BYTE>( m_colorBits);
		pfd.cDepthBits		= static_cast<BYTE>( depthBits);
		pfd.cStencilBits	= static_cast<BYTE>( stencilBits);
		pfd.iLayerType		= PFD_MAIN_PLANE;

		int pixelFormat		= ::ChoosePixelFormat( m_hDC, &pfd);

		A_ASSERT( pixelFormat);

		::SetPixelFormat( m_hDC, pixelFormat, &pfd);

		m_hRC = wglCreateContext( m_hDC);
		::wglMakeCurrent( m_hDC, m_hRC);

		UINT32 sampleCount = settings.m_sampleCount;
		if( sampleCount>1)
		{
			UINT  numFormats;
			float floatAttrs[] = { 0, 0};
			BOOL  valid;
			do 
			{
				int intAttrs[] = 
				{
					WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
					WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
					WGL_ACCELERATION_ARB,	WGL_FULL_ACCELERATION_ARB,
					WGL_COLOR_BITS_ARB,		static_cast<int>(m_colorBits),
					WGL_DEPTH_BITS_ARB,		static_cast<int>(depthBits),
					WGL_STENCIL_BITS_ARB,	static_cast<int>(stencilBits),
					WGL_DOUBLE_BUFFER_ARB,	GL_TRUE,
					WGL_SAMPLE_BUFFERS_ARB,	GL_TRUE,
					WGL_SAMPLES_ARB,		static_cast<int>(sampleCount),
					0,						0
				};

				valid = wglChoosePixelFormatARB( m_hDC, intAttrs, floatAttrs, 1, &pixelFormat, &numFormats);
				if( !valid || (numFormats<1))
					--sampleCount;
			} while ( (sampleCount>1) && (!valid || (numFormats<1)));

			if( valid && (sampleCount>1))
			{
				::wglMakeCurrent( m_hDC, NULL);
				::wglDeleteContext( m_hRC);
				::ReleaseDC( m_hwnd, m_hDC);

				A_ASSERT( FALSE);
			}
		}

		if( !glloader_GL_VERSION_4_0() && !glloader_GL_VERSION_3_0() && glloader_WGL_ARB_create_context())
		{
			int flags = 0;

#ifdef ARES_DEBUG
			flags |= WGL_CONTEXT_DEBUG_BIT_ARB;
#endif

			int versions[8][2] =
			{
				{ 4, 3 },
				{ 4, 2 },
				{ 4, 1 },
				{ 4, 0 },
				{ 3, 3 },
				{ 3, 2 },
				{ 3, 1 },
				{ 3, 0 },
			};

			int attribs[] = { WGL_CONTEXT_MAJOR_VERSION_ARB, 4, WGL_CONTEXT_MINOR_VERSION_ARB, 3, WGL_CONTEXT_FLAGS_ARB, flags, WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB, 0 };
			for( int i=0; i<8; i++)
			{
				attribs[1] = versions[i][0];
				attribs[3] = versions[i][1];
				HGLRC newhRC = wglCreateContextAttribsARB( m_hDC, NULL, attribs);
				if( newhRC)
				{
					::wglMakeCurrent( m_hDC, NULL);
					::wglDeleteContext( m_hRC);
					m_hRC = newhRC;

					::wglMakeCurrent( m_hDC, m_hRC);

					// reinit glloader
					glloader_init();

					break;
				}
			}
		}
#endif

		if (!glloader_GL_VERSION_2_0() || !glloader_GL_EXT_framebuffer_object())
		{
			A_ASSERT( FALSE);
		}

		if (!glloader_GL_VERSION_3_2() && (glloader_GL_VERSION_3_1() && !glloader_GL_ARB_compatibility()))
		{
			A_ASSERT( FALSE);
		}

		if (glloader_GL_ARB_color_buffer_float())
		{
			glClampColorARB( GL_CLAMP_VERTEX_COLOR_ARB, GL_FALSE);
			glClampColorARB( GL_CLAMP_FRAGMENT_COLOR_ARB, GL_FALSE);
			glClampColorARB( GL_CLAMP_READ_COLOR_ARB, GL_FALSE);
		}

#if defined ARES_PLATFORM_WINDOWS
		if (glloader_WGL_EXT_swap_control())
			wglSwapIntervalEXT(settings.m_syncInterval);
#endif


		glPixelStorei( GL_PACK_ALIGNMENT, 1);
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1);

		UpdateSurfacePtrs();

		m_frameBuffer->SetActive( true);
	}

	// 更新SurfacePtrs
	void OGLRenderWindow::UpdateSurfacePtrs()
	{
		// Attach backbuffer and depthstencil
		m_frameBuffer->AttachRenderView( FrameBuffer::ATT_Color0,      OGLScreenRenderTargetRenderViewPtr( new_ OGLScreenRenderTargetRenderView( GetWidth(), GetHeight(), m_colorFormat)));
		m_frameBuffer->AttachRenderView( FrameBuffer::ATT_DepthStencil,OGLScreenDepthStencilRenderViewPtr( new_ OGLScreenDepthStencilRenderView( GetWidth(), GetHeight(), m_depthStencilFormat)));

		// Bind
		PDRenderDevice.BindFrameBuffer( m_frameBuffer);
	}

	// 更改窗口大小
	void OGLRenderWindow::Resize( size_t width, size_t height)
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
	void OGLRenderWindow::SetFullScreen( bool fullScreen)
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
#endif
		}
	}

	// 重置
	void OGLRenderWindow::ResetFrameBuffer()
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
		m_frameBuffer =dynamic_pointer_cast<OGLFrameBuffer>( PDRenderDevice.CreateFrameBuffer());
		m_frameBuffer->SetViewport( tViewPort);
	}

	// Swap Buffers
	void OGLRenderWindow::SwapBuffers()
	{
#ifdef ARES_PLATFORM_WINDOWS
		::SwapBuffers( m_hDC);
#elif defined ARES_PLATFORM_LINUX
		glXSwapBuffers( m_xDisplay, m_xWindow);
#endif
	}
}