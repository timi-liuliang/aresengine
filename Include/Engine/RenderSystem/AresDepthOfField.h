#pragma once

#include "AresFrameFilter.h"
#include "AresGaussianBlur.h"

namespace Ares
{
	//------------------------------
	// 景深 2013-3-1 帝林
	//------------------------------
	class DepthOfField : public FrameFilter
	{
	public:
		DepthOfField( Scene& scene);
		virtual ~DepthOfField();

		// 更改窗口大小
		void Resize( size_t width, size_t height);

		// 执行滤镜
		void Process( FrameBufferPtr output);

		// 获取FrameBuffer
		FrameBufferPtr& GetFrameBuffer() { return m_frameBuffer; }
	
	private:
		FrameBufferPtr		m_frameBuffer;
		TexturePtr			m_frameTexture;
		TexturePtr			m_frameDepth;
		BlurTexOffseter		m_blurTexOffseter;	// 高斯模糊
		ShaderParamPtr		m_zInv;				// zInv;
		ShaderParamPtr		m_depthOfField;		// 景深
		ShaderParamPtr		m_bufferTexture;	// 执行景深处理图片
		ShaderParamPtr		m_depthTexture;		// 深度图片
	};
	typedef s_ptr<DepthOfField> DepthOfFieldPtr;
}