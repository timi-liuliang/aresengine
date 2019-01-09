#ifndef VIDEO_SETTING_H
#define VIDEO_SETTING_H

#include <QtGui/QDockWidget>
#include "ui_MOVideoSetting.h"

namespace QT_UI
{
	//----------------------------------------
	// 模型编辑器 主窗口 2010-12-30
	//----------------------------------------
	class MOVideoSetting : public QDockWidget, public Ui_MOVideoSetting
	{
		Q_OBJECT

	public:
		// 构造函数
		MOVideoSetting( QWidget* parent = 0);

	private slots:
		// 更新VideoFillMode
		void  UpdateFillMode();
	};
}

#endif