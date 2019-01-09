#pragma once

#include <QtGui/QWidget>
#include "ui_LightProperty.h"
#include <Engine/RenderSystem/AresLight.h>

using namespace Ares;

namespace QT_UI
{
	//-------------------------------------
	// 光源属性编辑 2013-8-30
	//-------------------------------------
	class MQLightProperty : public QWidget, public Ui_LightProperty
	{
		Q_OBJECT

	public:
		MQLightProperty( QWidget* parent=0);

		// 设置关联光源
		void SetCurrLight( Light* light);

		// 更新界面显示
		void UpdateUIDisplay();

	private slots:
		// 选择光源颜色
		void SelectColor();

		// 更新光源数据
		void UpdateLightData();

	private:
		Light*		m_light;			// 光源
		float3		m_tColor;			// 临时颜色值
	};
}