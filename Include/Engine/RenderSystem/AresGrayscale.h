#pragma once

#include "AresFrameFilter.h"

namespace Ares
{
	//------------------------------------
	// EdgePadding 2012-8-8 帝林
	//------------------------------------
	class GrayscaleFilter : public FrameFilter
	{
	public:
		// 构造函数
		GrayscaleFilter( Scene& scene);

		// 设置源纹理
		bool SetSrcTexture( TexturePtr& src);
	};
	typedef s_ptr<GrayscaleFilter> GrayScalePtr;
}