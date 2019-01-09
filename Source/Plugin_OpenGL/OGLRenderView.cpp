#include "OGLRenderView.h"

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
	OGLDepthStencilRenderView::OGLDepthStencilRenderView( UINT width, UINT height, ElementFormat format, UINT sampleCount, UINT sampleQuality)
	{

	}
}