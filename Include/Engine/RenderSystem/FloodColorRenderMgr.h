#pragma once

#include <Engine/RenderSystem/AresModel.h>
#include <Engine/RenderSystem/FloodColorRenderable.h>

namespace Ares
{
	//--------------------------------
	// 泛光效果 2013-3-6 帝林
	//--------------------------------
	class FloodColorRenderMgr
	{
		typedef multimap<EntityPtr, FloodColorRenderable*> MultimapFR;
	public:
		// 渲染轮廓
		void Write( vector<EntityPtr>& entitys);

	private:
		// 构建FloodLightRenderalbe
		void RefreshRenderableWrite( vector<EntityPtr>& entitys);

		// 刷新模型泛光Renderable
		void RefreshRenderableWrite( ModelPtr& model);

		// 执行泛光渲染
		void ExecFloodLightWrite( vector<EntityPtr>& entitys);

	private:
		MultimapFR		m_renderables;
	};
}