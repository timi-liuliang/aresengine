#pragma once

#include "AresFrameFilter.h"
#include "AresGaussianBlur.h"

namespace Ares
{
	//-----------------------------------
	// 延迟渲最终阶段 2013-1-7 帝林
	//-----------------------------------
	class DeferredShading : public FrameFilter
	{
	public:
		DeferredShading( Scene& scene);
		~DeferredShading();

		// 设置输入
		void SetInput(  const TexturePtr& diffuse, const TexturePtr& normal, const TexturePtr& light, const TexturePtr& depth);

	private:
		BlurTexOffseter		m_blurTexOffseter;		// 高斯模糊
		ShaderParamPtr		m_depthTexture;			// 深度纹理
		ShaderParamPtr		m_diffuseTexture;		// 漫反射纹理
		ShaderParamPtr		m_normalTexture;
		ShaderParamPtr		m_lightTexture;			// 光照纹理
	};
	typedef s_ptr<DeferredShading> DeferredShadingPtr;
}