#pragma once

#include "Core/AresPlatform.h"
#include "AresFrameBuffer.h"

namespace Ares
{
	//-----------------------------------------------------------------------
	// IRenderWindow 2011-11-14 帝林
	// 主要参考 Ogre
	//		Instances are created and communicated with by the 
	// render system. although client programs can get refrence
	// to it from the render system, if required for resizing or
	// moving.			
	//											auther: Steven Streeting
	//-----------------------------------------------------------------------
	class RenderWindow
	{
	public:
		// 获取类型
		virtual const char* GetType()=0;

		// 获取宽
		UINT	GetWidth() const { return m_width; }

		// 获取高
		UINT	GetHeight() const { return m_height; }

		// 是否全屏
		bool  IsFullScreen() { return m_isFullScreeen; }

		// 更改窗口大小
		virtual void Resize( size_t width, size_t height)=0;

		// 设置是否全屏
		virtual void SetFullScreen( bool fullScreen)=0;

		// 获取FrameBuffer
		virtual FrameBufferPtr GetFrameBuffer()=0;

		// 获取背景缓冲纹理
		virtual TexturePtr GetBackbufferTextureCopy()=0;

		// Present the information rendered to the back buffer 
		// to the front buffer(the screen), Refresh screen.
		virtual void SwapBuffers() {}

	protected:
		UINT			m_left;			
		UINT			m_top;				// 左上位置
		UINT			m_width;			// 宽
		UINT			m_height;			// 高
		bool			m_isFullScreeen;	// 是否全屏
	};
}