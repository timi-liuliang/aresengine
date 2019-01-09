#pragma once

#include <QtGui/QDockWidget>
#include "ui_FxObjectViewer.h"
#include <Engine/FxSystem/AresFxObject.h>
#include <string>

using namespace std;
using namespace Ares;

namespace QT_UI
{
	//----------------------------------------
	// 模型编辑器 主窗口 2010-12-30
	//----------------------------------------
	class FxObjectViewer : public QDockWidget, public Ui_FxObjectViewer
	{
		Q_OBJECT

	public:
		// 构造函数
		FxObjectViewer( QWidget* parent = 0);

	public:
		// 右键菜单
		virtual void contextMenuEvent( QContextMenuEvent* event);

		// 更新列表显示
		void UpdateObjectListDisplay();

		// 组织效果对象显示名称
		QString GetFxObjectDisplayName( FxObject* object);

	private slots:
		// 添加3D面片
		void Add3dPatch();

		// 添加粒子系统
		void AddPartcileSystem();

		// 选中特效对象
		void OnSelectedFxObject();

		// 删除选中Object
		void DeleteSelectedObject();

	private:
		QMenu*          m_mouseMenu;    // 鼠标右键菜单
		vector<QIcon>	m_icons;		// 图标
	};
}
