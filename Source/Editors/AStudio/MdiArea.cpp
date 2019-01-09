#include <QtGui/QtGui>
#include "MdiArea.h"
#include "ModelEdit/MOLogic.h"
#include "SCMainWindow.h"
#include "ModelEdit/MOMainWindow.h"
#include "SCLogic.h"
#include "Application.h"

namespace QT_UI
{
	// 构造函数
	MOMdiArea::MOMdiArea( QWidget* parent/* = NULL*/)
		: QMdiArea( parent)
		, m_moRender( false)
		, m_scRender( false)
	{
		m_renderWidgetMO = new MORenderWidget( this);
		m_subWindowMO	 = addSubWindow( m_renderWidgetMO);
		m_subWindowMO->setWindowTitle( tr("Model|Fx"));
		m_subWindowMO->show();

		m_renderWidgetSC = new_ MORenderWidget( this);
		m_subWindowSC	 = addSubWindow( m_renderWidgetSC);
		m_subWindowSC->setWindowTitle( tr("Scene"));
		m_subWindowSC->show();

		// 扩展策略
		setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

		adjustSize();
	}

	// 析构函数
	MOMdiArea::~MOMdiArea()
	{
		//SAFE_DELETE( m_pTimer);		
		//SAFE_DELETE( m_pDx9Widget);
		//SAFE_DELETE( m_pDx9App);	
	}

	// 开始渲染
	void MOMdiArea::BeginRender()
	{
		QRect geometry = this->geometry();

		m_subWindowMO->setGeometry( this->geometry());
		m_subWindowSC->setGeometry( this->geometry());

		// 初始化Dx9App
		G_Application->InitSCLogic( m_renderWidgetSC->winId());
		//G_Application->InitMOLogic( m_renderWidgetMO->winId());

		// 时间事件
		//m_pTimer = new_ QTimer( this);
		//connect( m_pTimer, SIGNAL( timeout()), this, SLOT( Render()));
		//m_pTimer->start( 15);

		SCUI_MainWindow->BeginRender();
		//MOUI_MainWindow->BeginRender();
	}

	// 设置渲染
	void MOMdiArea::SetRenderenable( bool scRender, bool moRender)
	{
		m_scRender = scRender;
		m_moRender = moRender;

		if( m_scRender)
		{
			m_subWindowSC->showMaximized();

			SCLogic_Scene->SetActived( true);
			//EntityEdit_Scene->SetActived( false);
		}
		//if( m_moRender)
		//{
		//	m_subWindowMO->showMaximized();

		//	SCLogic_Scene->SetActived( false);
		//	EntityEdit_Scene->SetActived( true);
		//}
	}

	// 渲染更新
	void  MOMdiArea::Render()
	{
		// 鼠标是否位于窗口中
		ResizeWindow();

		//if( m_moRender)
		//	m_AppMO->Exec( m_subWindowMO->isActiveWindow());
		
		//if( m_scRender)
		//	m_AppSC->Exec( m_subWindowSC->isActiveWindow());
	}

	// 窗口大小改变
	void MOMdiArea::ResizeWindow()
	{
		if( m_scRender)
			SCLogic_Main->ResizeWindow( width(), height());

		if( m_moRender)
			MOLogic_Main->ResizeWindow( width(), height());
	}

	// 重置设备
	void  MOMdiArea::ResetDevice()
	{

	}


	//// 显示右键菜单
	// 场景编辑右键菜单
	//void CTabWidget::ShowMenu( QMouseEvent* mouseEvent)
	//{
	//	if( m_timer.elapsed() > 0.2f)
	//		return;

	//	if( !m_mouseMenu)
	//	{
	//		m_mouseMenu = new QMenu();

	//		// 选择菜单
	//		QMenu* selecMenu = new QMenu("选择");
	//		selecMenu->addAction( m_actionSelectAll);
	//		selecMenu->addAction( m_actionSelectNull);
	//		selecMenu->addAction( m_actionSelectInvert);
	//		m_mouseMenu->addMenu( selecMenu);
	//	}

	//	m_mouseMenu->exec( mouseEvent->globalPos());
	//}
}