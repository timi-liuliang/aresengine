#include "Core/Graphics/AresRenderView.h"

namespace Ares
{
	// 绑定FramberBuffer
	void RenderView::OnBind( FrameBuffer& frameBuffer, UINT attach)
	{

	}

	// 解绑FrameBuffer
	void RenderView::OnUnbind( FrameBuffer& frameBuffer, UINT attach)
	{

	}

	// 当附加FrameBuffer
	void RenderView::OnAttachedFrameBuffer( FrameBuffer& frameBuffer, FrameBuffer::Attachment att)
	{
	}

	// 分离Framebuffer
	void RenderView::OnDetachedFrameBuffer( FrameBuffer& frameBuffer, FrameBuffer::Attachment att)
	{
	}
}