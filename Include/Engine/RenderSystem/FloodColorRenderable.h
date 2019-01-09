#pragma once

#include <Core/AresCore.h>
#include <Engine/RenderSystem/SuperMaterial.h>

namespace Ares
{
	//------------------------------------
	// FloodLightRenderable 2013-3-6 帝林
	//------------------------------------
	class FloodColorRenderable
	{
	public:
		FloodColorRenderable();
		~FloodColorRenderable();

		// 渲染
		void Render( const Matrix44& worldViewProj);

		// 设置几何布局
		void SetRenderlayout( const RenderLayoutPtr& layout) { m_layout = layout; }

	private:
		RenderLayoutPtr		m_layout;			// 几何布局
		SuperMaterial		m_material;			// 材质
		ShaderParamPtr		m_worldViewProj;	// 参数
		ShaderParamPtr		m_floodColor;		// 泛光颜色
	};
}