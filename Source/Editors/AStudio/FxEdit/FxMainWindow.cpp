#include <QtGui/QtGui>
#include "FxMainWindow.h"

using namespace Ares;

namespace QT_UI
{
	FxMainWindow*	g_mainWindowFx = NULL;			// 主窗口

	// 构造函数
	FxMainWindow::FxMainWindow( QMainWindowEx* parent/* = 0*/)
		: m_mainWindow( parent)
	{
		g_mainWindowFx = this;

		// 逻辑启动
		new_ FxLogic();

		setupUi( m_mainWindow);

		// 消息连接
		connect( m_pActionAbout, SIGNAL(triggered()), this, SLOT(AresAbout()));
		connect( m_actionNew,	 SIGNAL(triggered()), this, SLOT(New()));
		connect( m_actionOpen,	 SIGNAL(triggered()), this, SLOT(Open()));
		connect( m_actionSave,	 SIGNAL(triggered()), this, SLOT(Save()));
		connect( m_gridVisible,  SIGNAL(triggered()), this, SLOT(SwitchGridVisible()));
		connect( m_bgVisible,	 SIGNAL(triggered()), this, SLOT(SwitchBackgroundVisible()));

		connect( m_generalCamera,SIGNAL(triggered()), this, SLOT(SwitchToGeneralCamera()));
		connect( m_skyCamera,    SIGNAL(triggered()), this, SLOT(SwitchToSkyCamera()));

		// 对象查看器
		m_objectViewer = new_ FxObjectViewer( m_mainWindow);
		m_mainWindow->addDockWidget( Qt::RightDockWidgetArea, m_objectViewer);

		// 属性编辑器
		m_propertyEdit = new_ FxPropertyEdit( m_mainWindow);
		m_mainWindow->addDockWidget( Qt::RightDockWidgetArea, m_propertyEdit);

		connect( m_actionTrans,	   SIGNAL(triggered()), this, SLOT(SetTransTypeMove()));

		m_objectViewer->setVisible( false);
		m_propertyEdit->setVisible( false);
	}

	// 激活
	void FxMainWindow::Active()
	{
		m_mainWindow->setMenuBar( m_menuBar);

		m_toolBar->setVisible( true);
		m_objectViewer->setVisible( true);
		m_propertyEdit->setVisible( true);
	}

	// 失效
	void FxMainWindow::Deactive()
	{
		m_toolBar->setVisible( false);
		m_objectViewer->setVisible( false);
		m_propertyEdit->setVisible( false);
	}

	// 开始渲染
	void FxMainWindow::BeginRender() 
	{ 
	}

	// 新建效果
	void FxMainWindow::New()
	{
		if( FxLogic_FxEdit->GetFxEntity())
		{
		}

		FxLogic_FxEdit->New();
	}

	// 打开效果文件
	void FxMainWindow::Open()
	{
		QString qFileName = QFileDialog::getOpenFileName( m_mainWindow, tr("Open Model"), "", tr("*.afx"));
		if( !qFileName.isEmpty())
		{
			string fileName = qFileName.toLocal8Bit();

			FxLogic_FxEdit->OpenAfx( fileName.c_str());

			m_objectViewer->UpdateObjectListDisplay();
		}
	}

	// 保存效果文件
	void FxMainWindow::Save()
	{
		string fileName = FxLogic_FxEdit->GetAfxPath();
		if( fileName.empty())
		{
			QString qFileName = QFileDialog::getSaveFileName( m_mainWindow, tr("Save Afx"), "", tr("*.afx"));
			if( !qFileName.isEmpty())
				fileName = qFileName.toLocal8Bit();
		}
		
		FxLogic_FxEdit->SaveAfx( fileName.c_str());
	}

	// 显示隐藏格子
	void FxMainWindow::SwitchGridVisible()
	{
		//Logic_Auxiliary->SetGridVisible( m_gridVisible->isChecked());
	}

	// 显示隐藏背景
	void FxMainWindow::SwitchBackgroundVisible()
	{
		//Logic_Auxiliary->SetBackgroundVisible( m_bgVisible->isChecked());
	}

	// 设置转换类型为平移
	void FxMainWindow::SetTransTypeMove()
	{
		//Logic_Transform->SetTranformType( CAres3DAxis::EM_EDIT_TRANSLATE);
	}

	// 转换到常规摄像机
	void FxMainWindow::SwitchToGeneralCamera()
	{
		//Logic_Camera->SetActive( CT_Normal);
	}

	// 转换为天空摄像机
	void FxMainWindow::SwitchToSkyCamera()
	{
		//Logic_Camera->SetActive( CT_Sky);
	}

	// about
	void  FxMainWindow::AresAbout()
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