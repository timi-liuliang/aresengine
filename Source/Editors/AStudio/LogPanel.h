#pragma once

#include <QtGui/QDialog>
#include "ui_LogPanel.h"

namespace QT_UI
{
	//----------------------------------------
	// 输出 主窗口 2010-12-30
	//----------------------------------------
	class LogPanel : public QDialog, public Ui_LogPanel
	{
		Q_OBJECT

	public:
		// 构造函数
		LogPanel( QWidget* parent = 0);

		// 析构函数
		~LogPanel();

		// 警告
		void Warning( const char* msg);

	protected:
		// 关闭
		void closeEvent(QCloseEvent *event);

	};
}