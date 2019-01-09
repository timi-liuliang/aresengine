#pragma once

#include <Engine/SceneSystem/AresEntity.h>
#include <Engine/RenderSystem/ZRenderable.h>
#include <Engine/RenderSystem/AresModel.h>
#include <Engine/RenderSystem/AresTerrain.h>

namespace Ares
{
	//-----------------------------------
	// 写深度管理器 2013-2-21 帝林
	//-----------------------------------
	class ZWriteMgr
	{
		typedef map<Entity*, map<size_t, ZRenderable*>> MultimapZR;
	public:
		// 写深度
		void Write( vector<Entity*>& entitys, const Matrix44& viewProj);

	private:
		// 刷新Renderable
		void RefreshRenderableZWrite( vector<Entity*>& entitys);

		// 刷新模型Renderable
		void RefreshRenderableZWrite( Model* model);

		// 更新地形ZWriteRenderable
		void RefreshRenderableZWrite( Terrain* terrain);

		// 执行写深度
		void ExecZWrite( vector<Entity*>& entitys, const Matrix44& viewProj);

	private:
		MultimapZR		m_zRenderables;		// ZRenderables
	};
}