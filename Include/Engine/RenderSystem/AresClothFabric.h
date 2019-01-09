#pragma once

#include <Physics/MassSpringSystem.h>
#include <Engine/RenderSystem/AresRenderEntity.h>
#include <Engine/RenderSystem/AresSuperRenderable.h>

namespace Ares
{
	//------------------------------------
	// 布料织物 2013-7-12 帝林
	//------------------------------------
	class ClothFabric : public RenderEntity
	{
	public:
		ClothFabric();
		~ClothFabric();

		// 获取质点弹簧系统
		MassSpringSystem& GetMassSpringSystem() { return m_massSpringSystem; }

	public:
		// 更新
		virtual void  FrameMove( float fTime);

		// 添加到场景消息
		virtual void OnAddToScene( Scene* scene);

		// 提交渲染元素
		virtual void SubmitRenderElements( RenderElementMgr& renderElementMgr);

	private:
		// 更新Buffer
		void UpdateBuffer();

	private:
		MassSpringSystem		m_massSpringSystem;		// 质点弹簧系统
		RenderLayoutPtr			m_renderLayout;			// 渲染几何体
		SuperRenderablePtr		m_renderable;			// 渲染
	};
	typedef s_ptr<ClothFabric> ClothFabricPtr;
}