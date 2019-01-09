#include <QtGui/QtGui>
#include "UIMainWindow.h"
#include <Engine/AresRoot.h>

namespace QT_UI
{
	// 构造函数
	UIMainWindow::UIMainWindow( QMainWindowEx* parent/* = 0*/)
		: QObject( parent)
		, m_mainWindow( parent)
	{
		m_uiEditLogic = new_ UiEdit;

		setupUi( m_mainWindow);

		// 消息连接
		connect( m_pActionAbout, SIGNAL(triggered()), this, SLOT(AresAbout()));
		connect( m_actionNew,	 SIGNAL(triggered()), this, SLOT(New()));
		connect( m_actionOpen,	 SIGNAL(triggered()), this, SLOT(Open()));
		connect( m_actionSave,	 SIGNAL(triggered()), this, SLOT(Save()));
	}

	// 激活
	void UIMainWindow::Active()
	{
		m_toolBar->setVisible( true);

		m_mainWindow->setMenuBar( m_menubar);
	}

	// 失效
	void UIMainWindow::Deactive()
	{
		m_toolBar->setVisible( false);
	}

	// 开始渲染
	void UIMainWindow::BeginRender() 
	{ 
	}

	// 新建效果
	void UIMainWindow::New()
	{
		m_uiEditLogic->New();
	}

	// 打开效果文件
	void UIMainWindow::Open()
	{
		QString fileName = QFileDialog::getOpenFileName( m_mainWindow, tr("Open gui"), "", tr("Designer UI files(*.gui)"));
		if( !fileName.isEmpty())
		{
			string tFileName = fileName.toStdString();

			m_uiEditLogic->Open( tFileName.c_str());
		}
	}

	// 保存效果文件
	void UIMainWindow::Save()
	{

	}

	// 显示隐藏格子
	void UIMainWindow::SwitchGridVisible()
	{
		//Logic_Auxiliary->SetGridVisible( m_gridVisible->isChecked());
	}

	// 显示隐藏背景
	void UIMainWindow::SwitchBackgroundVisible()
	{
		//Logic_Auxiliary->SetBackgroundVisible( m_bgVisible->isChecked());
	}

	// about
	void  UIMainWindow::AresAbout()
	{	
		QMessageBox::about( m_mainWindow, tr("About Ares"),
			tr("<h2>Ares Effect Editor 1.0</h2>"
			   "<p>Ares Effect Editor is the AresEngine'"
			   "Editor. used for Effect editor.</p>"
			   "<p>Copyright &copy; 2011 Software Inc.</p>"
			   "<p>See <a href=www.51play.com>www.quyougame.com</a>"
			   " for more information</p>"
			   ));
	}
}