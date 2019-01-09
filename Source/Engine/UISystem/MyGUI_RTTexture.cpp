#include "Engine/UISystem/MyGUI_RTTexture.h"
#include "Engine/AresRoot.h"

namespace MyGUI
{
	// 构造函数
	MyGUIRTTexture::MyGUIRTTexture( TexturePtr texture)
	{
		A_ASSERT( texture);

		m_frame = ARenderDevice->CreateFrameBuffer();
		m_frame->AttachRenderView( FrameBuffer::ATT_Color0,	ARenderDevice->Create2DRenderView( texture, 0, 1, 0));
	}

	// 构造函数
	MyGUIRTTexture::MyGUIRTTexture( FrameBufferPtr frame)
	{
		m_frame = frame;
	}

	// 析构函数
	MyGUIRTTexture::~MyGUIRTTexture()
	{

	}

	// 设置为渲染目标
	void MyGUIRTTexture::begin()
	{
		m_preFrame = ARenderDevice->GetCurFrameBuffer();

		// 设置DepthMap为当前FrameBuffer
		ARenderDevice->BindFrameBuffer( m_frame);
		m_frame->Clear( ColorRGB::Black, 1.f, 0);
	}

	// 结束
	void MyGUIRTTexture::end()
	{
		ARenderDevice->BindFrameBuffer( m_preFrame);
	}

	// 渲染
	void MyGUIRTTexture::DoRender( Renderable& renderable)
	{
		renderable.Render();
	}
}