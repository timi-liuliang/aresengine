#include "QMainWindowEx.h"
#include <QtGui/QLayout>
#include <QtGui/QMenuBar>

namespace QT_UI
{
	// 构造函数
	QMainWindowEx::QMainWindowEx( QWidget* parent)
		: QMainWindow( parent)
	{

	}

	// 设置菜单
	void QMainWindowEx::setMenuBar(QMenuBar *menuBar)
	{
		QLayout *topLayout = layout();

		if (topLayout->menuBar() && topLayout->menuBar() != menuBar) {
			// Reparent corner widgets before we delete the old menu bar.
			QMenuBar *oldMenuBar = qobject_cast<QMenuBar *>(topLayout->menuBar());
			if (menuBar) {
				// TopLeftCorner widget.
				QWidget *cornerWidget = oldMenuBar->cornerWidget(Qt::TopLeftCorner);
				if (cornerWidget)
					menuBar->setCornerWidget(cornerWidget, Qt::TopLeftCorner);
				// TopRightCorner widget.
					cornerWidget = oldMenuBar->cornerWidget(Qt::TopRightCorner);
				if (cornerWidget)
					menuBar->setCornerWidget(cornerWidget, Qt::TopRightCorner);

				menuBar->show();
			}
			oldMenuBar->hide();
			//oldMenuBar->deleteLater();
		}
		topLayout->setMenuBar(menuBar);
	}

	void QMainWindowEx::setCentralWidgetEx(QWidget *widget)
	{
		if (this->centralWidget() && this->centralWidget() != widget) {
			//d->layout->centralWidget()->hide();
			//d->layout->centralWidget()->deleteLater();
		}
		setCentralWidget(widget);
	}
}