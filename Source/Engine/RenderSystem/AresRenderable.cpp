#include "Engine/RenderSystem/AresRenderable.h"
#include "Engine/AresRoot.h"
#include <Engine/SceneSystem/AresScene.h>
#include <Engine/RenderSystem/AresRenderElementMgr.h>

namespace Ares
{
	// 构造函数
	Renderable::Renderable()
	{
		m_material = MakeSharePtr( new_ SuperMaterial);
	}

	// 析构函数
	Renderable::~Renderable()
	{}

	// 渲染
	void Renderable::Render()
	{
		if( IsReady())
		{
			ARenderDevice->Render( m_layout.get(), m_material.get());
		}
	}

	// 是否完全初始化(渲染条件满足)
	bool Renderable::IsReady()
	{ 
		return m_layout && m_material;
	}

	// 获取渲染方法
	RenderMethodPtr Renderable::GetRenderMethod()
	{
		return m_material->GetMethod();
	}

	// 设置材质
	void Renderable::SetMaterial( const RenderMaterialPtr& mat) 
	{ 
		m_material = mat; 
	}

	// 提交到渲染队列
	void Renderable::SubmitRenderElements( RenderElementMgr& renderElementMgr)
	{
		A_ASSERT( m_material->GetRenderQueue());
	
		renderElementMgr.AddRenderable( m_layout, m_material.get());
	}
}