#include "LogPanel.h"
#include <QtGui/QListWidgetItem>

namespace QT_UI
{
	// 构造函数
	LogPanel::LogPanel( QWidget* parent/* = 0*/)
		: QDialog( parent)
	{
		setupUi( this);
	}

	// 析构函数
	LogPanel::~LogPanel()
	{
	}

	// 警告
	void LogPanel::Warning( const char* msg)
	{
		QListWidgetItem* warningItem = new QListWidgetItem( msg);
		if( warningItem)
		{	
			warningItem->setIcon( QIcon(":/icon/Icon/MapCheck_Warning.png"));
			m_logList->addItem( warningItem);
		}

		// 显示界面
		setVisible( true);
	}

	// 关闭
	void LogPanel::closeEvent(QCloseEvent *event)
	{
		m_logList->clear();

		QDialog::closeEvent( event);
	}
}