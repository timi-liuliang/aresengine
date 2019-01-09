#pragma once

#include <QtGui/QDockWidget>
#include "ui_MOUVBuild.h"

namespace QT_UI
{
	//----------------------------------------
	// 对象查看器 主窗口 2012-8-28
	//----------------------------------------
	class UVBuild : public QDockWidget, public Ui_UVBuild
	{
		Q_OBJECT

	public:
		// 构造函数
		UVBuild( QWidget* parent = 0);

		// 警告
		void Warning( const char* msg);

	private slots:
		// 构建唯一UV
		void Build();

		// 还原模型
		void Restore();
	};
}
