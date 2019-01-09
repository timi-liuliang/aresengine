#pragma once

#include <Core/Graphics/AresFrameBuffer.h>
#include <Engine/RenderSystem/AresGaussianBlur.h>
#include <Engine/RenderSystem/AresDeferredLighting.h>
#include <Engine/RenderSystem/AresDeferredShading.h>

namespace Ares
{
	//------------------------------------
	// 延迟渲染管理器 2013-1-6 帝林
	//------------------------------------
	class DeferredRenderingMgr
	{
	public:
		DeferredRenderingMgr( size_t width, size_t height, Scene& scene);
		~DeferredRenderingMgr();

		// 进入延迟渲染阶段
		void BeginDeferredPhase( const ColorRGB& gbufferbackcolor);

		// 结束延迟渲染阶段
		void EndDeferredPhase( const FrameBufferPtr& output, const vector<Light*>& lights, const LightPtr& dominantLight);

		// 更改窗口大小
		void Resize( size_t width, size_t height);

		// 获取GBuffer
		FrameBufferPtr& GetOpaqueGBuffer() { return m_opaqueGBuffer; }

		// 获取深度纹理
		ShaderParamPtr GetSPABOpaqueZDistance() { return m_spabOpaqueGBufferZDistance; }

	private:
		// 执行光照阶段
		void ExecLightingPhase( const vector<Light*>& lights, const LightPtr& dominantLight);

		// 执行着色阶段
		void ExecShadingPhase();

	private:
		size_t				m_gBufferWidth;
		size_t				m_gbufferheight;
		FrameBufferPtr		m_opaqueGBuffer;
		TexturePtr			m_opaqueGBufferRT0Diffuse;		// RT0 漫反射
		TexturePtr			m_opaqueGBufferRT1Normal;		// RT1 法线
		TexturePtr			m_opaqueGBufferZDistance;		// RT2 位置

		ShaderParamPtr		m_spabOpaqueGBufferZDistance;	// 深度贴图

		FrameBufferPtr		m_lightingBuffer;				// Lighting Buffer
		TexturePtr			m_lightingTexture;				// Lighting Texture

		DeferredLightingPtr m_deferredLighting;				// 光照阶段滤镜
		GaussianBlurPtr		m_gaussianBlur;					// 高斯模糊(软阴影)
		DeferredShadingPtr	m_deferredShading;				// 最终着色阶段

		Scene&				m_scene;					// 所属场景系统
	};
	typedef s_ptr<DeferredRenderingMgr> DeferredRenderingMgrPtr;
}