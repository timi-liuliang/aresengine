#pragma once

#include "AresFrameFilter.h"

namespace Ares
{
	//-----------------------------------------
	// ToneMapping 2013-8-27 帝林
	//-----------------------------------------
	class ToneMapping : public FrameFilter
	{
		// 枚举
		enum FrameBufferType
		{
			FBT_FullSize = 0,
			FBT_DownSampled256x256,
			FBT_DownSampled64x64,
			FBT_DownSampled16x16,
			FBT_DownSampled4x4,
			FBT_LuminancePrevious,
			FBT_LuminanceCurrent,
			FBT_LuminanceTemp,
			FBT_FrameBuffers,
		};

	public:
		ToneMapping( Scene& scene);
		~ToneMapping();

		// 设置源纹理
		bool SetSrcTexture( TexturePtr& src, FrameBufferPtr& oFrameBuffer);

		// 执行滤镜
		void ToneMapping::Execute();

	private:
		// 转换为亮度
		void ConvertToLogLuminance();

		// 求平均亮度
		void AverageLuminance();

		// 调整亮度
		void AdaptiveLuminance();

		// 自动暴光
		void AutoExposure();

		// 交换亮度结果
		void SwapLuminanceFrameBuffer( FrameBufferType type0, FrameBufferType type1);

	private:
		ShaderParamPtr		m_adaptiveSpeed;						// 自动曝光调整速度
		ShaderParamPtr		m_middleGray;							// 灰度值
		ShaderParamPtr		m_multiplierClamp;						// 不确定用处
		ShaderParamPtr		m_lumianceTable;						// 亮度计算
		ShaderParamPtr		m_downSampledOffset[4];					// 采样参数
		ShaderParamPtr		m_spabTextureFullSize;					// 源纹理
		ShaderParamPtr		m_spabTextures[FBT_FrameBuffers];		// 采样纹理

		Renderable			m_luminanceRenderable;					// 原图片转换为亮度
		Renderable			m_renderableDownSampled;				// 求平均亮度
		Renderable			m_renderableExpLuminance;				// 求亮度指数
		Renderable			m_renderableLuminanceAdaptive;			// 亮度调整
		Renderable&			m_renderAutoExposure;					// 自动暴光
		TexturePtr			m_textures[FBT_FrameBuffers];			// 渲染目标
		FrameBufferPtr		m_frameBuffers[FBT_FrameBuffers];		// 渲染缓存

		FrameBufferPtr*		m_output;								// 输出渲染目标
	};
	typedef s_ptr<ToneMapping> ToneMappingPtr;
}