#include "OGLESRenderWindow.h"
#include "OGLESRenderDevice.h"
#include "OGLESRenderView.h"
#include "OGLESPlatform.h"
#include <EAGL.h>
#include <EAGLDrawable.h>
#include <CAEAGLLayer.h>

namespace Ares
{
    EAGLContext*    g_context = NULL;
    
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

        m_top = settings.m_top;
        m_left= settings.m_left;
        
        // 创建context
        g_context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        
        // 设置当前context
        [EAGLContext setCurrentContext:g_context];
        
        // 创建Buffer
        GLuint renderBuffer;
        glGenBuffers( 1, &renderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
        
        // 关联窗口layer
        CAEAGLLayer* eaglLayer = (CAEAGLLayer*)settings.m_eaglLayer;
        [g_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglLayer];

		//OGLESDebug( glPixelStorei( GL_PACK_ALIGNMENT, 1));
		//OGLESDebug( glPixelStorei( GL_UNPACK_ALIGNMENT, 1));

		// 描述信息
		ostringstream oss; oss<< glGetString( GL_VENDOR) << " " << glGetString( GL_RENDERER) << " " << glGetString(GL_VERSION);
		if( settings.m_sampleCount>1)
			oss << "(" << settings.m_sampleCount << "xAA)";
		m_description = oss.str();
        
        // Default frame buffer
		ResetFrameBuffer();

		UpdateSurfacePtrs();
        
        // temp
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderBuffer);

		m_frameBuffer->SetActive( true);
	}

	// 更新SurfacePtrs
	void OGLESRenderWindow::UpdateSurfacePtrs()
	{
		// Attach backbuffer and depthstencil
		m_frameBuffer->AttachRenderView( FrameBuffer::ATT_Color0,       MakeSharePtr( new_ OGLScreenRenderTargetRenderView( GetWidth(), GetHeight(), m_colorFormat)));
		m_frameBuffer->AttachRenderView( FrameBuffer::ATT_DepthStencil, MakeSharePtr( new_ OGLScreenDepthStencilRenderView( GetWidth(), GetHeight(), m_depthStencilFormat)));

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
        [g_context presentRenderbuffer:GL_RENDERBUFFER];
	}
}