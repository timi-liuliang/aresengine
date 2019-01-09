#ifndef IMAGE_PROCESS_H
#define IMAGE_PROCESS_H

#include <QtGui/QDockWidget>
#include "ui_ImageProcess.h"

namespace QT_UI
{
	//----------------------------------------
	// 模型编辑器 主窗口 2010-12-30
	//----------------------------------------
	class CImageProcess : public QDockWidget, public Ui_ImageProcess
	{
		Q_OBJECT

	public:
		// 构造函数
		CImageProcess( QWidget* parent = 0);

	private slots:
		// 设置Filter提醒 
		void FilterLostFocus();

		// 双击
		void OnClicked( QListWidgetItem* pItem);
	};
}

#endif