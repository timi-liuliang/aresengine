#pragma once

#include <Core/Graphics/AresRenderWindow.h>
#include "OGLESTypedefs.h"
#include "OGLESPlatform.h"
#include "OGLESFrameBuffer.h"

namespace Ares
{
	//-----------------------------------
	// OGLESRenderWindow 2013-02-05 帝林
	//-----------------------------------
	class OGLESRenderWindow : public RenderWindow
	{
	public:
		OGLESRenderWindow( const RenderSettings& settings);

		// 获取类型
		virtual const char* GetType() { return "OGLESRenderWindow"; }

		// 更改窗口大小
		virtual void Resize( size_t width, size_t height);

		// 设置是否全屏
		virtual void SetFullScreen( bool fullScreen);

		// 获取FrameBuffer
		virtual FrameBufferPtr GetFrameBuffer() { return m_frameBuffer; }

		// 获取背景缓冲纹理
		virtual TexturePtr GetBackbufferTextureCopy() { return TexturePtr(); }

		// Present the information rendered to the back buffer to the front buffer(the screen)
		virtual void SwapBuffers();

	private:
		// 更新SurfacePtrs
		void UpdateSurfacePtrs();

		// 重置
		void ResetFrameBuffer();

	private:
#ifdef ARES_PLATFORM_WINDOWS
		HWND						m_hwnd;				// 窗口句柄
		HDC							m_hDC;				// 

		EGLDisplay					m_display;
		EGLSurface					m_surface;
		EGLConfig					m_config;
		EGLContext					m_context;
#endif
    
		string						m_title;			// 标题
		ElementFormat				m_colorFormat;		// color format
		ElementFormat				m_depthStencilFormat;	// depth stencil format
		UINT						m_syncIntervel;		// 同步间隔
		OGLESFrameBufferPtr			m_frameBuffer;		// Default FrameBuffer

		UINT32						m_colorBits;		// 颜色位数
		string						m_description;		// 描述信息
	};
}