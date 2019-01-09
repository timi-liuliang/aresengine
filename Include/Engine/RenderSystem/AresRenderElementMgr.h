#pragma once

#include "AresRenderQueue.h"
#include "Engine/RenderSystem/AresLight.h"
#include "Engine/RenderSystem/AresDirectionalLight.h"

namespace Ares
{
	//---------------------------------
	// 渲染队列管理器, 2011-04-20 帝林
	// 职责：
	//		1.状态改变
	//		2.排序
	//---------------------------------
	class RenderElementMgr
	{
	public:
		// 清空函数
		void Reset();

		// 执行队列渲染
		void Render( RenderQueueFlag flag);

		// 添加到渲染队列
		void AddRenderable( RenderLayoutPtr& layout, RenderMaterial* material);

		// 队列是否为空
		bool IsEmpty( RenderQueueFlag flag);

	public:
		// 获取光源
		const vector<Light*>& GetLights() const { return m_lights; }

		// 添加光源
		void AddLight( Light* light) { m_lights.push_back( light); }

		// 获取主光源
		const DirectionalLightPtr& GetDomainLight() const { return m_dominantLight; }

		// 设置主光源
		void SetDomainLight( DirectionalLightPtr& light) { m_dominantLight=light; }

	private:
		vector<Light*>							m_lights;			// 光源
		DirectionalLightPtr						m_dominantLight;	// 主光源
		boost::array<RenderQueue, RQ_Max>		m_rendrQueues;		// 渲染队列
	};
}