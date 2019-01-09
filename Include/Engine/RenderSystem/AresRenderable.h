#pragma once

#include <Core/AresCore.h>

namespace Ares
{
	//------------------------------------
	// 渲染组件 2012-3-30 帝林
	//------------------------------------
	class RenderElementMgr;
	class Renderable
	{
	public:
		Renderable();
		~Renderable();

		// 渲染
		void Render();

		// 获取Renderlayout
		RenderLayoutPtr& GetRenderLayout() { return m_layout; }

		// 获取材质
		RenderMaterialPtr&  GetMaterial() { return m_material; }

		// 获取渲染方法
		RenderMethodPtr  GetRenderMethod();

		// 是否完全初始化(渲染条件满足)
		bool IsReady();

		// 设置渲染标记
		void SetReady( bool ready) { m_ready = ready; }

		// 设置Renderlayout
		void SetRenderlayout( const RenderLayoutPtr& renderLayout) { m_layout = renderLayout;}

		// 设置材质
		void SetMaterial( const RenderMaterialPtr& mat);

		// 提交到渲染队列
		void SubmitRenderElements( RenderElementMgr& renderElementMgr);

	protected:
		RenderLayoutPtr		m_layout;			// RenderGeometry
		RenderMaterialPtr	m_material;			// RenderMaterial
		bool				m_ready;			// ready to render
	};
}