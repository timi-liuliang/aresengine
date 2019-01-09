#pragma once

#include "AresFrameFilter.h"
#include "FloodColorRenderMgr.h"
#include "AresGaussianBlur.h"

namespace Ares
{
	//------------------------------------
	// FloodColor 2013-3-6 帝林
	//------------------------------------
	class FloodColor : public FrameFilter
	{
	public:
		FloodColor( Scene& scene);
		~FloodColor();

		// 更改窗口大小
		void Resize( size_t width, size_t height);

		// 执行滤镜
		void Process( vector<EntityPtr>& entitys, RenderViewPtr& iDepth, FrameBufferPtr& output);

	private:
		size_t					m_bufferWidth;			// 宽
		size_t					m_bufferHeight;			// 高
		FloodColorRenderMgr		m_floodRenderMgr;		// 渲染管理器

		TexturePtr				m_frameTexture;			// 纹理
		FrameBufferPtr			m_frameBuffer;			// 帧缓冲

		ShaderParamPtr			m_bufferTexture;		// 被模糊纹理 
		GaussianBlurPtr			m_gaussianBlur;			// 高斯模糊
	};
	typedef s_ptr<FloodColor> FloodColorPtr;
}