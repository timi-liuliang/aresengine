#pragma once

#include <QtGui/QMenu>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QDockWidget>
#include "ui_MOCollisionBuild.h"
#include <string>

using namespace std;

namespace QT_UI
{
	//----------------------------------------
	// 对象查看器 主窗口 2012-8-28
	//----------------------------------------
	class CollisionBuild : public QDockWidget, public Ui_CollisionBuild
	{
		Q_OBJECT

	public:
		// 构造函数
		CollisionBuild( QWidget* parent = 0);

		// 导入CollisionShapes
		void ImportCollisionShapes( const string& path);

		// 导出CollisionShapes
		void ExportCollisionShapes( const string& path);

	protected:
		// 右键菜单
		virtual void contextMenuEvent( QContextMenuEvent* event);

		// 更新列表显示
		void UpdateTableDisplay();

		// 更新当前选择形状到界面
		void UpdateShapeParameterToUI();

	private slots:
		// add collision shape capsule
		void AddCapsule();

		// 删除当前选择
		void DeleteSelectedCapsule();

		// 选择collision shape
		void OnSelectedCollisionShape();

		// 更新胶囊界面数据到形状
		void UpdateUICapsuleParameterToShape();

		// 导出碰撞模型
		void ExportCollisionShapes();

		// 导入碰撞模型
		void ImportCollisionShapes();

		// 网络填充模式
		void SwitchToWireFrameFillMode();

		// 正常填充模式
		void SwitchToNormalFillMode();

	private:
		bool			m_locking;		// 锁定中 
		QMenu*          m_mouseMenu;    // 鼠标右键菜单
	};
}
