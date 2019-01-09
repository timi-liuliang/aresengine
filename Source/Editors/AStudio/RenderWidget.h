#pragma once

#include <string>
#include <QUrl>
#include <QtGui/QtEvents>
#include <QtGui/QWidget>
#include <boost/signals2/signal.hpp>

using namespace std;
using namespace boost::signals2;

namespace QT_UI
{
	//-----------------------------------
	// Dx9MdiArea dx显示窗口
	//-----------------------------------
	class MORenderWidget : public QWidget
	{
		Q_OBJECT

	public:
		MORenderWidget( QWidget* parent = NULL);
		~MORenderWidget();

		// 鼠标移动事件
		virtual void mouseMoveEvent(QMouseEvent *);

	protected:
		// 鼠标进入事件
		void dragEnterEvent( QDragEnterEvent * event);

		// 鼠标释放事件
		void dropEvent( QDropEvent* event);
	};
}