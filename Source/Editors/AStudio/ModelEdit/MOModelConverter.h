#pragma once

#include <QtGui/QWidget>
#include "ui_MOModelConverter.h"

namespace QT_UI
{
	//----------------------------------------
	// 模型编辑器 主窗口 2010-12-30
	//----------------------------------------
	class MOModelConverter : public QDialog, public Ui_MOModelConverter
	{
		Q_OBJECT

	public:
		// 构造函数
		MOModelConverter( QWidget* parent = 0);

	private slots:
		// 选择YND资源路径
		void SelectYNDLocation();

		// 选择Ares存储路径
		void SelectAresLocation();

		// 转换
		void ConvertexFromYNDToAres();

	private:
	};
}
