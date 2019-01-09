#pragma once

#include "Engine/Foundation/Ares3DEngine_h.h"
#include "Engine/RenderSystem/AresHDREncode.h"
#include <Core/Graphics/AresTexture.h>

namespace Ares
{
	//--------------------------------------
	// 纹理滤镜 cpu 2012-7-18 帝林
	//--------------------------------------
	class ARES_EXPORT CpuTextureFilter
	{
	public:
		// 边缘填充：主要用于解决光照图color bleeding问题
		// source      被编辑纹理
		// ColorRGB	   背景色, 被处理的像素点
		// extendPixel 背景添充扩展的像素量
		// isRGBE	   图片是否为RGBE8编码格式
		static bool EdgePadding( TexturePtr& source, ColorRGB backgroundColor, int extendPixel, bool isRGBE);

		// 高斯模糊
		static bool GaussianBlur( TexturePtr& source, float radius);

	private:
	};
}