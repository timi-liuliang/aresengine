#pragma once

#include <QtGui/QDockWidget>
#include "ui_ParametersEditor.h"

namespace Ares
{
	//----------------------------------------
	// 模型编辑器 主窗口 2010-12-30
	//----------------------------------------
	class UIParametersEditor : public QDockWidget, public Ui_ParametersEditor
	{
		Q_OBJECT

	public:
		// 构造函数
		UIParametersEditor( QWidget* parent = 0);

		// 打开材质
		void OpenMaterial( const char* materialPath, size_t index);

	private slots:
		// 材质属性改变
		void MaterialPropertyChanged();

	private:
	};
}