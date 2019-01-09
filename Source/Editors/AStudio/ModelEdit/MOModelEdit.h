#pragma once

#include "YNDMesh/YNDMeshRender.h"
#include <Engine/RenderSystem/AresVisualShape.h>
#include <Engine/RenderSystem/ZWriteMgr.h>
#include <boost/filesystem.hpp>

using namespace boost;
using namespace boost::filesystem;

namespace Ares
{
	// 模型类型
	enum ModelType
	{
		MT_NULL,		// 无编辑模型
		MT_ARES,		// Ares引擎模型
		MT_YND,			// YND引擎模型
		FX_ARES,		// Ares特效
	};

	//--------------------------------------
	// 效果编辑逻辑 2012-8-20 帝林
	//--------------------------------------
	class ModelEdit
	{
	public:
		ModelEdit();

		// 辅助网格
		void CreateWhiteGrid();

		// 创建红色中线
		void CreateRedCross();

		// 设置实体
		void SetResEntity( const char* resPath);

		// 获取模型类型
		ModelType GetModelType();

		// 获取模型包围盒半径
		float GetModelRadius();

		// 获取模型世界包围盒
		Rect3 GetModelWorldBounds();

		// 获取当前资源路径
		const char* GetResPath() { return m_currResPath.c_str(); }

		// 获取当前实体
		ModelPtr&  GetCurrEntity() { return m_currEntity; }

		// 更新
		void FrameMove( float elapsedTime);

		// 重新加载
		void Reload();

	private:
		// Zoom
		void Zoom();

	private:
		VisualShape*			m_whiteGrid;		// 辅助网格
		VisualShape*			m_redCross;			// 中心红十字

		string					m_currResPath;		// 当前模型路径
		ModelPtr				m_currEntity;		// 当前编辑实体

		string					m_modelPath;		// 模型存储路径
	};
}