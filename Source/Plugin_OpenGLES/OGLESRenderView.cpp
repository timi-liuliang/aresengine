#include "OGLESRenderView.h"
#include "OGLESMapping.h"
#include "OGLESRenderDevice.h"

namespace Ares
{
	// 构造函数
	OGLTexture2DRenderView::OGLTexture2DRenderView( TexturePtr& texture, int arrayIndex, int level)
	{

	}

	// 构造函数
	OGLScreenRenderTargetRenderView::OGLScreenRenderTargetRenderView( UINT width, UINT height, ElementFormat ef)
	{
		m_width  = width;
		m_height = height;
		m_format = ef;
	}

	// 构造函数
	OGLScreenDepthStencilRenderView::OGLScreenDepthStencilRenderView( UINT width, UINT height, ElementFormat ef)
	{

	}

	// 构造函数
	OGLESDepthStencilRenderView::OGLESDepthStencilRenderView( UINT width, UINT height, ElementFormat format, UINT sampleCount, UINT sampleQuality)
		: m_level( -1)
	{
        m_width  = width;
        m_height = height;
        m_format = format;
        
        GLint  internalFormat;
        GLenum glFormat;
        GLenum glType;
        OGLESMapping::MappingFormat( internalFormat, glFormat, glType, m_format);
        
        glGenBuffers( 1, &m_rbo);
        glBindRenderbuffer( GL_RENDERBUFFER, m_rbo);
        glRenderbufferStorage( GL_RENDERBUFFER, internalFormat, m_width, m_height);
    }
    
    // 绑定FramberBuffer
    void OGLESDepthStencilRenderView::OnAttachedFrameBuffer( FrameBuffer& frameBuffer, UINT attach)
    {
		A_ASSERT( FrameBuffer::ATT_DepthStencil == attach);

		m_index = 0;
        
        m_fbo = (dynamic_cast<OGLESFrameBuffer*>(&frameBuffer))->GetFbo();
		if( m_level<0)
		{
			PDRenderDevice.BindFrameBufferObject( m_fbo);

			glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
			if( IsStencilFormat( m_format))
				glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

			PDRenderDevice.BindFrameBufferObject( 0);
		}
		else
		{

		}
    }
}