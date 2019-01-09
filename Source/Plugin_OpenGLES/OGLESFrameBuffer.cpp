#include "OGLESFrameBuffer.h"
#include "OGLESPlatform.h"
#include "OGLESRenderDevice.h"

namespace Ares
{
	// 构造函数
	OGLESFrameBuffer::OGLESFrameBuffer( RenderDevice* device, bool offScreen)
		: FrameBuffer( device)
	{
		m_left = 0;
		m_top  = 0;
		m_fbo  = 0;
        
        if( offScreen)
        {
			glGenFramebuffers( 1, &m_fbo);
        }
	}

	// 析构函数
	OGLESFrameBuffer::~OGLESFrameBuffer()
	{
		if( m_fbo)
			glDeleteFramebuffers( 1, &m_fbo);
	}

	// Clear
	void OGLESFrameBuffer::Clear( const ColorRGB& color, float depth, UINT stencil, UINT flags)
	{
		GLuint oldFbo = PDRenderDevice.GetCurFrameBufferObject();
		PDRenderDevice.BindFrameBufferObject( m_fbo);

		GLbitfield oglFlags =  0;
		if( flags & CBM_Color)
		{
			oglFlags |= GL_COLOR_BUFFER_BIT;

			OGLESDebug( glClearColor( color.r, color.g, color.b, color.a));
		}

		if( flags & CBM_Depth)
		{
			oglFlags |= GL_DEPTH_BUFFER_BIT;

			OGLESDebug( glClearDepthf( depth));
		}

		if( flags & CBM_Stencil)
		{
			oglFlags |= GL_STENCIL_BUFFER_BIT;

			OGLESDebug( glClearStencil( stencil));
		}

		OGLESDebug( glClear( oglFlags));

		PDRenderDevice.BindFrameBufferObject( oldFbo);
	}

	// 绑定
	void OGLESFrameBuffer::Bind()
	{
		PDRenderDevice.BindFrameBufferObject( m_fbo, true);
        
        OGLESDebug( glViewport( m_viewPort.m_left, m_viewPort.m_top, m_viewPort.m_width, m_viewPort.m_height));

		GLenum status = OGLESDebug( glCheckFramebufferStatus( GL_FRAMEBUFFER));
		if( status!=GL_FRAMEBUFFER_COMPLETE)
		{

		}
	}
}