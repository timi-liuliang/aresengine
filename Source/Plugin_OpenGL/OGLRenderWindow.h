#pragma once

#include <Core/Graphics/AresRenderWindow.h>
#include "OGLTypedefs.h"
#include "OGLFrameBuffer.h"

namespace Ares
{
	//-----------------------------------
	// OGLESRenderWindow 2013-02-05 帝林
	//-----------------------------------
	class OGLRenderWindow : public RenderWindow
	{
	public:
		OGLRenderWindow( const RenderSettings& settings);

		// 获取类型
		virtual const char* GetType() { return "OGLRenderWindow"; }

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
		string						m_title;			// 标题
		ElementFormat				m_colorFormat;		// color format
		ElementFormat				m_depthStencilFormat;	// depth stencil format
		UINT						m_syncIntervel;		// 同步间隔
		OGLFrameBufferPtr			m_frameBuffer;		// Default FrameBuffer

		UINT32						m_colorBits;		// 颜色位数

#ifdef ARES_PLATFORM_WINDOWS
		HWND						m_hwnd;				// 窗口句柄
		HGLRC						m_hRC;
		HDC							m_hDC;				// what?
#endif
	};
}