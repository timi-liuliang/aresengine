#pragma once

#include <QtGui/QMainWindow>

namespace QT_UI
{
	//----------------------------------------
	// QMainWindowEx 2013-7-31 帝林
	//----------------------------------------
	class QMainWindowEx : public QMainWindow
	{
		Q_OBJECT

	public:
		QMainWindowEx( QWidget* parent = 0);

		// 设置菜单
		virtual void setMenuBar(QMenuBar *menubar);

		// 设置中心控件
		void setCentralWidgetEx(QWidget *widget);
	};
}