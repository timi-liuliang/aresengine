#pragma once

#include <QtGui/QPainter>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>
#include <Core/AresMath.h>

using namespace std;
using namespace Ares;

namespace QT_UI
{
	//-------------------------------------------
	// ColorSelect 颜色选择按钮 2013-3-4 帝林
	//-------------------------------------------
	class QColorSelect : public QWidget
	{
		Q_OBJECT
	public:
		QColorSelect( QWidget* parent = 0);

		// 设置颜色
		void SetColor( string color);

		// 设置颜色
		void SetColor( const ColorRGB& color);

		// 获取颜色
		string GetColor();

		// 获取颜色
		const ColorRGB& GetColor() const { return m_color; }

		// MVC渲染
		static void ItemDelegatePaint( QPainter *painter, const QRect& rect, const string& val);

	private:
		// 更新显示
		void UpdateDisplay();

	private slots:
		// 选择路径
		void OnSelectColor();

	private:
		ColorRGB		m_color;
		QPushButton*	m_colorButton;
		QHBoxLayout*	m_horizonLayout;
	};
}