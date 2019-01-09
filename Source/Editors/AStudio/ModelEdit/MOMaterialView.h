#pragma once

#include <QtGui/QDockWidget>
#include "ui_MOMaterialView.h"

namespace QT_UI
{
	//----------------------------------------
	// 材质编辑	2011-05-16   帝林
	//----------------------------------------
	class MateriaViewer : public QDockWidget, public Ui_MaterialView
	{
		Q_OBJECT

	public:
		// 构造函数
		MateriaViewer(  QWidget* parent = 0);

		// 打开材质文件
		void RefreshDisplay();

	private slots:
		// 材质选中改变
		void OnSelectedMaterialChanged( int idx);

		// 编辑材质
		void OnEditMaterial();

	private:
	};
}