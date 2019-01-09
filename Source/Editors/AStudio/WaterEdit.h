#pragma once

#include <QtGui/QDockWidget>
#include "ui_WaterEdit.h"

namespace QT_UI
{
	//----------------------------------------
	// 新建场景    2011-01-11
	//----------------------------------------
	class CWaterEdit : public QDockWidget, public Ui_WaterEdit
	{
		Q_OBJECT

	public:
		// 构造函数
		CWaterEdit( QWidget* parent = 0);

	private slots:
		// 放置水
		void PutWater(bool checked);

	public:
	};
}