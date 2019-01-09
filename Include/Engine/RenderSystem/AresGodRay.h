#pragma once

#include "AresFrameFilter.h"
#include "Engine/FxSystem/AresFxEntity.h"

namespace Ares
{
	// 发光源
	struct GodRaySource
	{
		Vector2				m_center;			// 中心点
		FxEntityPtr			m_entity;			// RenderLayout
		FrameBufferPtr		m_output;
		RenderViewPtr		m_depth;			// 深度View
	};

	//------------------------------------
	// GodRay 2013-1-29 帝林
	//------------------------------------
	class GodRay : public FrameFilter
	{
	public:
		// 构造函数
		GodRay( size_t width, size_t height, Scene& scene);

		// 结束延迟渲染阶段
		void Process( GodRaySource& source);

		// 更改窗口大小
		void Resize( size_t width, size_t height);

	private:
		// 设置深度Buffer
		void BindDepthBuffer( RenderViewPtr& depth);

	private:
		size_t				m_gBufferWidth;
		size_t				m_gbufferheight;
		TexturePtr			m_godTexture;		// 发光纹理
		FrameBufferPtr		m_godFrameBuffer;

		ShaderParamPtr		m_spWrapSampler;	// 采样状态
		ShaderParamPtr		m_spGodCenter;		// 中心点坐标
		ShaderParamPtr		m_spGodTexture;		// 发光纹理
	};
	typedef s_ptr<GodRay> GodRayPtr;
}