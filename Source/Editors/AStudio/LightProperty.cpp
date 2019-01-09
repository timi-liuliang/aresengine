#include "LightProperty.h"
#include <QtGui/QColorDialog>
#include <Core/AresMath.h>

using namespace Ares;

namespace QT_UI
{
	// 构造函数
	MQLightProperty::MQLightProperty( QWidget* parent)
		: m_light( NULL)
	{
		setupUi( this);

		connect( m_color,			SIGNAL( clicked()), this, SLOT( SelectColor()));
		connect( m_colorIntensity,	SIGNAL( valueChanged(double)), this, SLOT( UpdateLightData()));
	}

	// 选择光源颜色
	void MQLightProperty::SelectColor()
	{
		if( m_light)
		{
			ColorRGB colorL = m_light->GetColor();
			QColor   color  = QColorDialog::getColor( QColor( (int)(colorL.r*255.f), (int)(colorL.g*255.f), (int)(colorL.b*255.f), 255), this);

			char result[512];
			sprintf_s( result, 512, "background-color: rgb(%d,%d,%d);", (int)(color.red()), (int)(color.green()), (int)(color.blue()));
			m_color->setStyleSheet( result);
			m_tColor = float3( color.redF(), color.greenF(), color.blueF());


			UpdateLightData();
		}
	}

	// 设置关联光源
	void MQLightProperty::SetCurrLight( Light* light)
	{
		m_light = light;

		switch( light->GetTypeDetail())
		{
			// 隐藏显示一些控件
		default:
			break;
		}

		UpdateUIDisplay();
	}

	// 更新界面显示
	void MQLightProperty::UpdateUIDisplay()
	{
		if( m_light)
		{
			ColorRGB color = m_light->GetColor();

			char result[512];
			sprintf_s( result, 512, "background-color: rgb(%d,%d,%d);", (int)(color.r * 255.f), (int)(color.g * 255.f), (int)(color.b * 255.f));
			m_color->setStyleSheet( result);

			m_colorIntensity->setValue( color.a);
		}
	}

	// 更新光源数据
	void MQLightProperty::UpdateLightData()
	{
		float colorIntensity = m_colorIntensity->value();

		m_light->SetColor( ColorRGB( m_tColor.x, m_tColor.y, m_tColor.z, colorIntensity));
	}
}