#include "OGLFrameBuffer.h"
#include "OGLPlatform.h"
#include "OGLRenderDevice.h"

namespace Ares
{
	// 构造函数
	OGLFrameBuffer::OGLFrameBuffer( RenderDevice* device)
		: FrameBuffer( device)
	{
		m_left = 0;
		m_top  = 0;
		m_fbo  = 0;
	}

	// 析构函数
	OGLFrameBuffer::~OGLFrameBuffer()
	{

	}

	// Clear
	void OGLFrameBuffer::Clear( const ColorRGB& color, float depth, UINT stencil, UINT flags)
	{
		GLuint oldFbo = PDRenderDevice.GetCurFrameBufferObject();
		PDRenderDevice.BindFrameBufferObject( m_fbo);

		//const DepthStencilStateDesc& ds_desc = re.CurDSSObj()->GetDesc();
		//const BlendStateDesc&		 blend_desc = re.CurBSObj()->GetDesc();

		if( flags & CBM_Color)
		{
			/*if (glloader_GL_EXT_draw_buffers2())
			{
				for (int i = 0; i < 8; ++ i)
				{
					if ( blend_desc.color_write_mask[i] != MASK_All)
					{
						glColorMaskIndexedEXT(i, true, true, true, true);
					}
				}
			}
			else
			{
				if ( blend_desc.color_write_mask[0] != MASK_All)
				{
					glColorMask(true, true, true, true);
				}
			}*/
		}
		if( flags & CBM_Depth)
		{
			/*if (!ds_desc.depth_write_mask)
			{
				glDepthMask(GL_TRUE);
			}*/
		}
		if (flags & CBM_Stencil)
		{
			/*if (!ds_desc.front_stencil_write_mask)
			{
				glStencilMaskSeparate(GL_FRONT, GL_TRUE);
			}
			if (!ds_desc.back_stencil_write_mask)
			{
				glStencilMaskSeparate(GL_BACK, GL_TRUE);
			}*/
		}

		if (glloader_GL_VERSION_3_0())
		{
			if (flags & CBM_Color)
			{
				if( m_fbo)
				{
					for (size_t i = 0; i < m_clrViews.size(); ++ i)
					{
						if (m_clrViews[i])
							glClearBufferfv( GL_COLOR, static_cast<GLint>(i), (float*)(&color));
					}
				}
				else
				{
					glClearBufferfv( GL_COLOR, 0, (float*)(&color));
				}
			}

			if ((flags & CBM_Depth) && (flags & CBM_Stencil))
			{
				glClearBufferfi(GL_DEPTH_STENCIL, 0, depth, stencil);
			}
			else
			{
				if (flags & CBM_Depth)
				{
					glClearBufferfv(GL_DEPTH, 0, &depth);
				}
				else
				{
					if (flags & CBM_Stencil)
					{
						GLint s = stencil;
						glClearBufferiv(GL_STENCIL, 0, &s);
					}
				}
			}
		}
		else
		{
			//GLbitfield ogl_flags = 0;
			//if (flags & CBM_Color)
			//{
			//	ogl_flags |= GL_COLOR_BUFFER_BIT;
			//	re.ClearColor(clr.r(), clr.g(), clr.b(), clr.a());
			//}
			//if (flags & CBM_Depth)
			//{
			//	ogl_flags |= GL_DEPTH_BUFFER_BIT;
			//	re.ClearDepth(depth);
			//}
			//if (flags & CBM_Stencil)
			//{
			//	ogl_flags |= GL_STENCIL_BUFFER_BIT;
			//	re.ClearStencil(stencil);
			//}

			//glClear(ogl_flags);
		}

		if (flags & CBM_Color)
		{
			/*if (glloader_GL_EXT_draw_buffers2())
			{
				for (int i = 0; i < 8; ++ i)
				{
					if (blend_desc.color_write_mask[i] != CMASK_All)
					{
						glColorMaskIndexedEXT(i, (blend_desc.color_write_mask[i] & CMASK_Red) != 0,
							(blend_desc.color_write_mask[i] & CMASK_Green) != 0,
							(blend_desc.color_write_mask[i] & CMASK_Blue) != 0,
							(blend_desc.color_write_mask[i] & CMASK_Alpha) != 0);
					}
				}
			}
			else
			{
				if (blend_desc.color_write_mask[0] != CMASK_All)
					glColorMask((blend_desc.color_write_mask[0] & CMASK_Red) != 0, (blend_desc.color_write_mask[0] & CMASK_Green) != 0, (blend_desc.color_write_mask[0] & CMASK_Blue) != 0, (blend_desc.color_write_mask[0] & CMASK_Alpha) != 0);
			}*/
		}
		if( flags & CBM_Depth)
		{
			//if ( !ds_desc.depth_write_mask)
			//	glDepthMask(GL_FALSE);
		}
		if( flags & CBM_Stencil)
		{
			//if (!ds_desc.front_stencil_write_mask)
			//	glStencilMaskSeparate(GL_FRONT, GL_FALSE);
			//if (!ds_desc.back_stencil_write_mask)
			//	glStencilMaskSeparate( GL_BACK, GL_FALSE);
		}

		PDRenderDevice.BindFrameBufferObject( oldFbo);
	}

	// 绑定
	void OGLFrameBuffer::Bind()
	{
		PDRenderDevice.BindFrameBufferObject( m_fbo);

		A_ASSERT( GL_FRAMEBUFFER_COMPLETE_EXT == glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT));
		if( m_fbo)
		{
			PDRenderDevice.EnableFrameBufferSRGB( true);

			std::vector<GLenum> targets( m_clrViews.size());
			for( size_t i=0; i<m_clrViews.size(); i++)
				targets[i] = static_cast<GLenum>( GL_COLOR_ATTACHMENT0_EXT + i);

			glDrawBuffers( static_cast<GLsizei>(targets.size()), targets.data());
		}
		else
		{
			PDRenderDevice.EnableFrameBufferSRGB( false);

			GLenum targets[] = { GL_BACK_LEFT };
			glDrawBuffers( 1, &targets[0]);
		}
	}
}