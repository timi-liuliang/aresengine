#pragma once

#include <QtGui/QtGui>
#include <QtGui/QMainWindow>
#include "ui_LightMap.h"
#include "GpuLightMapBake.h"

using namespace Ares;

namespace QT_UI
{
	//---------------------------------------
	// 光照图烘焙管理界面 20102-6-20 帝林
	//---------------------------------------
	class Lightmap : public QMainWindow, public Ui_Lightmap
	{
		Q_OBJECT

	public:
		// 构造函数
		Lightmap( QWidget* parent = 0);

		// 析构函数
		~Lightmap();

	public slots:
		// 烘焙主函数
		void Bake();

		// 烘焙
		void BakeSelected();

		// 完全体显示

		// 简体显示
	};
}