#pragma once

#include <Engine/RenderSystem/AresVisualShape.h>

#define YND_Engine

namespace Ares
{
	//--------------------------------------------
	// 碰撞编辑逻辑 2012-9-12 帝林
	//--------------------------------------------
	class CollisionBuildLogic
	{
	public:
		CollisionBuildLogic();

		// 添加shape
		void AddShape( VisualShapeType type);

		// 获取当前所有形状
		const vector<VisualShape*>& GetShapes() { return m_visualShapes; }

		// 设置当前选中形状
		void SetSelected( size_t idx);

		// 获取当前选中形状
		VisualShape* GetSelected();

		// 删除当前选中
		void DeleteSelected();

		// 网络填充模式
		void SwitchToWireFrameFillMode();

		// 正常填充模式
		void SwitchToNormalFillMode();

		// 导出物理碰撞模型
		void SavePhysicsShapes( const std::string& filePath);

		// 加载物理碰撞模型
		void LoadPhysicsShapes( const std::string& filePath);

		// 导出物理碰撞模型
		void YNDSavePhysicsShapes( const std::string& filePath);

		// 加载物理碰撞模型
		void YNDLoadPhysicsShapes( const std::string& filePath);

	private:
		// 抽取物理外形
		Shape* ExtractShape( VisualShape* vs);

		// 还原VisualShape
		void RestoreVisualShape( Shape* shape);

	private:
		size_t					m_selectedIdx;
		vector<VisualShape*>	m_visualShapes;
		//VisualShape*			m_visualShape;		// 只有一个外形
	};
}