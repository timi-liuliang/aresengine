#pragma once

#include "MyGUI_IRenderTarget.h"
#include <Core/Graphics/AresFrameBuffer.h>

using namespace Ares;

namespace MyGUI
{
	//---------------------------------------
	// 渲染目标纹理 2012-10-16 帝林
	//---------------------------------------
	class MyGUIRTTexture : public IRenderTarget
	{
	public:
		// 构造函数
		MyGUIRTTexture( TexturePtr texture);

		// 构造函数
		MyGUIRTTexture( FrameBufferPtr frame);

		// 析构函数
		~MyGUIRTTexture();

		// 设置为渲染目标
		virtual void begin();

		// 结束
		virtual void end();

		// 渲染
		virtual void DoRender( Renderable& renderable);

		// 返回描述信息
		virtual const RenderTargetInfo& getInfo() { return m_info; }

	private:
		RenderTargetInfo	m_info;
		FrameBufferPtr		m_frame;		// FrameBuffer
		
		FrameBufferPtr		m_preFrame;		// 原FrameBuffer
	};
}