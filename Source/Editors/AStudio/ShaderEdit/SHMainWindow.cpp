#include <QtGui/QtGui>
#include "SHMainWindow.h"
#include "UIShaderTreeDisplay.h"
#include "TabWidget.h"
#include "ParametersEditor.h"
#include "SHLogic.h"
#include <Engine/AresRoot.h>
#include <boost/filesystem.hpp>

using namespace Ares;

namespace Ares
{
	SHMainWindow*	g_mainWindowSH = NULL;			// 主窗口

	// 构造函数
	SHMainWindow::SHMainWindow( QWidget* parent/* = 0*/)
		: QMainWindow( parent)
	{
		g_mainWindowSH = this;
		m_shLogic	   = new_ SHLogic;

		setupUi( this);

		// 消息连接
		connect( m_actionNew,	 SIGNAL(triggered()), this, SLOT(NewShaderTree()));
		connect( m_pActionAbout, SIGNAL(triggered()), this, SLOT(AresAbout()));

		// 显示区域
		m_pMdiArea = new UIShaderTreeDisplay( this);
		setCentralWidget( m_pMdiArea);

		// 材质表达式
		m_materialExpression = new_ UIMaterialExpression( this);
		this->addDockWidget( Qt::RightDockWidgetArea, m_materialExpression);

		// 属性编辑
		m_parametersEditor = new UIParametersEditor( this);
		this->addDockWidget( Qt::LeftDockWidgetArea, m_parametersEditor);
		m_parametersEditor->setVisible( false);

		//m_pOutPut = new COutPut( this);
		//this->addDockWidget(Qt::BottomDockWidgetArea, m_pOutPut);
		//m_pOutPut->setVisible( false);

		connect( m_pActionOpen,  SIGNAL(triggered()), this, SLOT( Open()));
		connect( m_pActionSave,	SIGNAL(triggered()),  this, SLOT( Save()));

		// 编译
		connect( m_pActionCompile, SIGNAL(triggered()), m_pMdiArea->m_pTabWidget, SLOT( Compile()));
		//connect( m_pMdiArea->m_pTabWidget, SIGNAL(Error(const char*)), m_pOutPut, SLOT(SetText(const char*)));

		FileCorelation();
	}

	// 开始渲染
	void SHMainWindow::BeginRender() 
	{ 
		//m_pMdiArea->BeginRender();

		// 链接日志面板
		//ALogSystem.Signal_Info.connect( boost::bind<void>(&COutPut::SetText, m_pOutPut, _1));
		//ALogSystem.Signal_Warning.connect( boost::bind<void>(&COutPut::SetText, m_pOutPut, _1));
		//ALogSystem.Signal_Error.connect( boost::bind<void>(&COutPut::Error, m_pOutPut, _1));

		OnStateChanged();
	}

	// 打开
	void  SHMainWindow::Open()
	{
		QString qFileName = QFileDialog::getOpenFileName( this, tr("open shader"), "", tr("*.hlsl *.glsl *.ast"));

		QDir dir( QDir::currentPath());
		qFileName = dir.relativeFilePath( qFileName);

		boost::filesystem::path tPath( qFileName.toStdString());
		if( tPath.extension()==".ast")
		{
			SHLogic_ShaderTreeEdit->Load( tPath.string());

			// 更新界面显示
			SHUI_ShaderTreeDisplay->Reset();
			SHUI_ShaderTreeDisplay->UpdateShaderTreeDisplay();
		}
		else 
		{
			Open( qFileName.toStdString().c_str());
		}

		// 状态改变
		OnStateChanged();
	}

	// 保存
	void SHMainWindow::Save()
	{
		if( SHLogic_ShaderTreeEdit->GetShaderTree())
		{
			if( !SHLogic_ShaderTreeEdit->IsHaveSavePath())
			{
				QString qFileName = QFileDialog::getSaveFileName( this, tr("save shader tree"), "", tr("*.ast"));
				SHLogic_ShaderTreeEdit->SetSavePath( qFileName.toStdString());
			}

			SHLogic_ShaderTreeEdit->Save();
		}
	}

	// 打开文件
	void SHMainWindow::Open( const char* fileName)
	{
		m_pMdiArea->m_pTabWidget->Open( fileName);
	}

	// 状态改变,控制界面显示
	void SHMainWindow::OnStateChanged()
	{
		m_materialExpression->setEnabled( SHLogic_ShaderTreeEdit->GetShaderTree() ? true : false);
		m_pMdiArea->setEnabled( SHLogic_ShaderTreeEdit->GetShaderTree() ? true : false);
	}

	// 新建材质树
	void SHMainWindow::NewShaderTree() 
	{ 
		SHLogic_ShaderTreeEdit->New();

		// 更新界面显示
		m_pMdiArea->UpdateShaderTreeDisplay();

		// 状态改变
		OnStateChanged();
	}

	// zoom
	void  SHMainWindow::ZoomEntity()
	{
		//g_pSHLogic->ZoomEntity();
	}

	// 文件关联
	void SHMainWindow::FileCorelation()
	{
		string currPath = QDir::currentPath().toStdString();
		replace( currPath.begin(), currPath.end(), '/', '\\');

		// 计算图标路径,执行程序路径
		string iconPath = currPath + "\\corelation\\hlsl.ico";

#ifdef _DEBUG
		string exePath     = currPath + "\\MaterialEdit_d.exe";
		string exePathfile = currPath + "\\MaterialEdit_d.exe %1";
#else
		string exePath     = currPath + "\\MaterialEdit.exe";
		string exePathfile = currPath + "\\MaterialEdit.exe %1";
#endif // _DEBUG

		// 写注册表
		QSettings reg("HKEY_CLASSES_ROOT\\.hlsl", QSettings::NativeFormat);
		reg.setValue(".", "hlslfile");

		QSettings regIcon("HKEY_CLASSES_ROOT\\hlslfile\\DefaultIcon", QSettings::NativeFormat);
		regIcon.setValue(".", iconPath.c_str());

		QSettings regOpen("HKEY_CLASSES_ROOT\\hlslfile\\shell\\edit\\command", QSettings::NativeFormat);
		regOpen.setValue(".", exePathfile.c_str());

		// 注册系统位置
		QSettings regPath("HKEY_CURRENT_USER\\Software\\Ares\\MaterialEdit", QSettings::NativeFormat);
		regPath.setValue("ExePath", exePath.c_str());
	}

	// about
	void  SHMainWindow::AresAbout()
	{	
		QMessageBox::about( this, tr("About Ares"),
			tr("<h2>Ares Effect Editor 1.0</h2>"
			   "<p>Ares Effect Editor is the AresEngine'"
			   "Editor. used for Effect editor.</p>"
			   "<p>Copyright &copy; 2011 Software Inc.</p>"
			   "<p>See <a href=www.51play.com>www.quyougame.com</a>"
			   " for more information</p>"
			   ));
	}
}