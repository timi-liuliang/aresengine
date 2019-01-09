#pragma once

#include "AresFrameFilter.h"

namespace Ares
{
	//------------------------------------
	// 空间扭曲 2012-8-30 帝林
	//------------------------------------
	class SpaceDistorted : public FrameFilter
	{
	public:
		// 构造函数
		SpaceDistorted( Scene& scene);

		// 设置源纹理
		bool SetSrcTexture( TexturePtr& src);

	public:
		// 直接设置DistortedTexture
		void SetDistortedTexture( const TexturePtr& texture) { m_distortedTexture = texture; }

		// 获取DistortedTexture
		TexturePtr GetDistortedTexture() { return m_distortedTexture; }

		// 绑定DistortedFrameBuffer
		FrameBufferPtr GetDistortedFrameBuffer() { return m_distortedFrameBuffer; }

	private:
		TexturePtr		m_distortedTexture;			// 标记扭曲位置
		FrameBufferPtr	m_distortedFrameBuffer;		// 用于更新扭曲纹理
	};
}