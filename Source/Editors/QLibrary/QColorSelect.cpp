#include "QColorSelect.h"
#include <Core/AresMath.h>
#include <QtGui/QColorDialog>
#include <EditorKit/DataTypeConversion.h>

namespace QT_UI
{
	QColorSelect::QColorSelect( QWidget* parent)
		: QWidget( parent)
	{
		// 布局控件
		m_horizonLayout = new_ QHBoxLayout( this);
		m_horizonLayout->setContentsMargins(0, 0, 0, 0);
		m_horizonLayout->setObjectName( QString::fromUtf8("horizontalLayout"));

		// 颜色按钮
		m_colorButton = new_ QPushButton( this);
		m_colorButton->setObjectName( QString::fromUtf8("colorButton"));
		m_colorButton->setContentsMargins(0, 0, 0, 0);
		m_horizonLayout->addWidget( m_colorButton);

		// 消息
		connect( m_colorButton, SIGNAL(clicked()), this, SLOT(OnSelectColor()));
	}

	// 选择路径
	void QColorSelect::OnSelectColor()
	{
		QColor color = QColorDialog::getColor( QColor( (int)(m_color.r*255.f), (int)(m_color.g*255.f), (int)(m_color.b*255.f)), this);

		SetColor( ColorRGB( color.red()/255.f, color.green()/255.f, color.blue()/255.f));
	}

	// 设置路径
	void QColorSelect::SetColor( string color) 
	{ 
		m_color = Ares::DTC::StringToColorRGB( color);

		UpdateDisplay();
	}

	// 设置颜色
	void QColorSelect::SetColor( const ColorRGB& color)
	{ 
		m_color=color;

		UpdateDisplay();
	}

	// 获取路径
	string QColorSelect::GetColor() 
	{ 
		return Ares::DTC::ColorRGBToString( m_color);
	}

	// 更新显示
	void QColorSelect::UpdateDisplay()
	{
		char result[512];

		sprintf_s( result, 512, "background-color: rgb(%d,%d,%d);", (int)(m_color.r*255.f), (int)(m_color.g*255.f), (int)(m_color.b*255.f));

		m_colorButton->setStyleSheet( result);
	}

	// MVC渲染
	void QColorSelect::ItemDelegatePaint( QPainter *painter, const QRect& rect, const string& val)
	{
		ColorRGB color = DTC::StringToColorRGB( val);

		QRect tRect = rect; tRect.setWidth( rect.width()-1); tRect.setHeight( rect.height()-1);
		painter->setBrush( QColor( (int)(color.r*255.f), (int)(color.g*255.f), (int)(color.b*255.f)));
		painter->drawRect( tRect);
		painter->setPen( Qt::black);
		painter->drawRect( tRect);
	}
}