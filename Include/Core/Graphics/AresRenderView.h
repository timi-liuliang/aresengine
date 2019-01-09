#pragma once

#include <boost/shared_ptr.hpp>
#include "AresElementFormat.h"
#include "AresFrameBuffer.h"

namespace Ares
{
	//--------------------------------------
	// RenderView 2012-3-16  帝林
	//--------------------------------------
	class RenderView
	{
	public:
		RenderView() {}
		virtual ~RenderView(){}

		// 获取宽度
		UINT GetWidth() { return m_width; }

		// 获取高度 
		UINT GetHeight() { return m_height; }

		// 获取纹理(以纹理创建的View可获取)
		virtual TexturePtr GetTexture() { return TexturePtr(); }

		// 绑定FramberBuffer
		virtual void OnBind( FrameBuffer& frameBuffer, UINT attach);

		// 解绑FrameBuffer
		virtual void OnUnbind( FrameBuffer& frameBuffer, UINT attach);

	public:
		// 当附加FrameBuffer
		virtual void OnAttachedFrameBuffer( FrameBuffer& frameBuffer, FrameBuffer::Attachment att);

		// 分离Framebuffer
		virtual void OnDetachedFrameBuffer( FrameBuffer& frameBuffer, FrameBuffer::Attachment att);

	protected:
		UINT			m_width;	// 宽
		UINT			m_height;	// 高
		ElementFormat	m_format;	// 格式
	};

	typedef boost::shared_ptr<RenderView> RenderViewPtr;
}