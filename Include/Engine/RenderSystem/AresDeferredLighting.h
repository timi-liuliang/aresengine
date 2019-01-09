#pragma once

#include "AresFrameFilter.h"
#include "AresGaussianBlur.h"
#include "Engine/RenderSystem/AresLight.h"
#include <Engine/RenderSystem/AresDirectionalLight.h>

namespace Ares
{
	//-----------------------------------
	// 延迟渲染光照阶段 2013-1-7 帝林
	//-----------------------------------
	class DeferredLighting : public FrameFilter
	{
		// 顶点格式
		struct VertexFormat
		{
			float3 m_position;			// 顶点位置
			float2 m_info;				// 光源信息
			float3 m_color;				// float3光源颜色 float 光源类型
			float3 m_direction;			// 方向光朝向,点光源位置
		};

	public:
		DeferredLighting(  Scene& scene);
		~DeferredLighting();

		// 执行滤镜
		virtual void Execute();

		// 设置输入
		void SetInput( const TexturePtr& mrt1, const TexturePtr& zDistance, vector<Light*> lights, const LightPtr& dominantLight);

	private:
		// 根据光源数据组织渲染Buffer
		void OgranizeRenderLayoutByLights();

		// 生成Rect3面数据
		void GenerateRect3Surface( vector<Vector3>& surface, const Rect3 rect);

	private:
		BlurTexOffseter			m_blurTexOffseter;		// 高斯模糊
		ShaderParamPtr			m_mrt1Texture;			// 纹理1
		ShaderParamPtr			m_depthTexture;			// 深度
		ShaderParamPtr			m_samplerState;			// 采样状态
		ShaderParamPtr			m_pointSamplerState;	// 点采样状态

		vector<Light*>			m_lights;				// 光源
		AmbientLight*			m_ambientLight;			// 环境光
		DirectionalLightPtr		m_dominantLight;		// 主光源
		VertexElementType		m_elementType;			// 顶点格式
		RenderLayoutPtr			m_lightsRenderLayout;	// 光源渲染流
		vector<VertexFormat>	m_vertexsCache;			// 顶点数据缓存

		Renderable				m_ambRenderable;		// 环境光
	};
	typedef s_ptr<DeferredLighting> DeferredLightingPtr;
}