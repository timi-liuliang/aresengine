#include <QtGui/QtGui>
#include "MOMainWindow.h"
//#include "OutPut.h"
//#include "ModelList.h"
//#include "VideoSetting.h"
//#include "Equipment.h"
#include "MOLogic.h"
#include <Engine/SceneSystem/AresEntity.h>
#include <boost/filesystem.hpp>
#include <Engine/AresRoot.h>

using namespace Ares;
using namespace boost::filesystem;

namespace QT_UI
{
	MOUILayout*	g_mainWindowME = NULL;			// 主窗口

	//COutPut* CMainWindow::m_outPut = NULL;

	// 构造函数
	MOUILayout::MOUILayout( QMainWindowEx* parent/* = 0*/)
		: QObject( parent)
		, m_filesystemWatcher( NULL)
		, m_mainWindow( parent)
		, m_modelConverter( NULL)
	{
		setupUi( m_mainWindow);

		// 消息连接
		connect( m_actionOpen,           SIGNAL(triggered()), this, SLOT(Open()));
		connect( m_actionSave,			 SIGNAL(triggered()), this, SLOT(Save()));
		connect( m_pActionAbout,         SIGNAL(triggered()), m_mainWindow, SLOT(AresAbout()));
		connect( m_actionEnglish,        SIGNAL(triggered()), this, SLOT(SwitchLanguage()));
		connect( m_actionChinese,        SIGNAL(triggered()), this, SLOT(SwitchLanguage()));
		//connect( m_pActionFillPoint,     SIGNAL(triggered()), this, SLOT(UpdateFillMode()));

		// 动画播放控制
		m_animationPlay = new CAnimationEdit( m_mainWindow);
		m_mainWindow->addDockWidget( static_cast<Qt::DockWidgetArea>(2), m_animationPlay);

		// 材质查看
		m_materialView = new MateriaViewer( m_mainWindow);
		m_mainWindow->addDockWidget( Qt::RightDockWidgetArea, m_materialView);

		// 光照图UV生成
		m_uvBuild = new UVBuild( m_mainWindow);
		m_mainWindow->addDockWidget( Qt::RightDockWidgetArea, m_uvBuild);

		// 碰撞数据
		m_collisionBuild = new CollisionBuild( m_mainWindow);
		m_mainWindow->addDockWidget( Qt::RightDockWidgetArea, m_collisionBuild);

		// 旋转
		//this->tabifyDockWidget( m_equipment, m_animationPlay);

		m_animationPlay->setVisible( false);
		//m_equipment->setVisible( false);
		//m_pModelList->setVisible( false);


		//// 视频设置
		////m_pVideoSetting = new CVideoSetting( this);
		////this->addDockWidget(  static_cast<Qt::DockWidgetArea>(2), m_pVideoSetting);	

		// 动画面版
		//connect( m_pModelList, SIGNAL( OnSelectChanged(const char*)), this,            SLOT(OpenEntity(const char*)));
		//connect( m_pModelList, SIGNAL( OnSelectChanged(const char*)), m_animationPlay, SLOT(Reset()));
		//m_pModelList->setVisible( false);

		FileCorelation();

		connect( m_actionGenerateLightmapUV, SIGNAL(triggered()), this, SLOT(HideShowUVBuild()));
		connect( m_actionCollision,			 SIGNAL(triggered()), this, SLOT(HideShowCollisionBuild()));

		// 移动旋转信号
		connect( m_actionTrans,	   SIGNAL(triggered()), this, SLOT(SetTransTypeMove()));
		connect( m_actionRotation, SIGNAL(triggered()), this, SLOT(SetTransTypeRotation()));

		// 导出碰撞模型
		connect( m_actionExportCollisionShapes, SIGNAL(triggered()), m_collisionBuild, SLOT( ExportCollisionShapes()));
		connect( m_actionImportCollisionShapes, SIGNAL(triggered()), m_collisionBuild, SLOT( ImportCollisionShapes()));

		// 模型转换面板
		connect( m_actionYNDToAres, SIGNAL(triggered()), this, SLOT(ShowModelConverter()));
	}

	// 激活
	void MOUILayout::Active()
	{
		m_toolBar->setVisible( true);
		m_animationPlay->setVisible( m_visibleState[1]);
		m_materialView->setVisible( m_visibleState[2]);
		m_uvBuild->setVisible( true);
		m_collisionBuild->setVisible( true);
		m_animationPlay->setVisible( m_visibleState[5]);

		m_mainWindow->setMenuBar( m_menuBar);
	}

	// 失效
	void MOUILayout::Deactive()
	{
		m_visibleState[1] = m_animationPlay->isVisible();
		m_visibleState[2] = m_materialView->isVisible();
		m_visibleState[3] = m_uvBuild->isVisible();
		m_visibleState[4] = m_collisionBuild->isVisible();
		m_visibleState[5] = m_animationPlay->isVisible();

		m_toolBar->setVisible( false);
		m_animationPlay->setVisible( false);
		m_materialView->setVisible( false);
		m_uvBuild->setVisible( false);
		m_collisionBuild->setVisible( false);
		m_animationPlay->setVisible( false);
	}

	// 开始渲染
	void MOUILayout::BeginRender()
	{
		// 加载配置
		//MOLogic_Cfg->Signal_ResDirectorysChanged.connect( boost::bind<void>(&ModelEdit::UpdateResDirectorys, MOLogic_ModelEdit, _1));
		//MOLogic_Cfg->Load();

		// 链接日志面板
		//ALogSystem.Signal_Warning.connect( boost::bind<void>(&LogPanel::Warning, m_logPanel, _1));
	}

	// 打开
	void MOUILayout::Open()
	{
		// 重置文件系统监视器
		SAFE_DELETE( m_filesystemWatcher);
		m_filesystemWatcher = new_ QFileSystemWatcher( this);
		connect( m_filesystemWatcher, SIGNAL( fileChanged( const QString&)), this, SLOT( OnFileChanged( const QString&)));

		QString qFileName = QFileDialog::getOpenFileName( m_mainWindow, tr("Open Model"), "", tr("*.model *.GMe"));
		if( !qFileName.isEmpty())
		{
			string fileName = qFileName.toLocal8Bit();

			OpenEntity( fileName.c_str());
		}
	}

	// 打开实体文件
	void MOUILayout::OpenEntity( const char* fileName)
	{
		MOLogic_ModelEdit->SetResEntity( fileName);

		// 加载碰撞外型
		path cPath( fileName); 
		
		if( MOLogic_ModelEdit->GetModelType() == MT_YND)
			cPath.replace_extension(".cshapes");
		else
		{
			cPath.replace_extension(".cs");

			// 更新材质显示
			m_materialView->RefreshDisplay();
		}

		m_collisionBuild->ImportCollisionShapes( cPath.string());

		m_animationPlay->Reset();

		// 更改窗口标题
		char tDrive[256];
		char tPath[1024];
		char tName[512];
		char tExt[256];
		_splitpath( fileName, tDrive, tPath, tName, tExt);

		string tTitle = string(tName) + tExt + "    -Path:"+ tDrive + tPath + "    -Ares Model Editor";
		m_mainWindow->setWindowTitle( tTitle.c_str());
	}

	// 保存所有
	void MOUILayout::Save()
	{
		string resPath = MOLogic_ModelEdit->GetResPath();
		if( !resPath.empty())
		{
			//保存碰撞外型
			path cPath( resPath);

			if( MOLogic_ModelEdit->GetModelType() == MT_YND)
				cPath.replace_extension(".cshapes");
			else
				cPath.replace_extension(".cs");

			m_collisionBuild->ExportCollisionShapes( cPath.string());
		}
	}

	// 文件关联
	void MOUILayout::FileCorelation()
	{/*
		string currPath = QDir::currentPath().toStdString();
		replace( currPath.begin(), currPath.end(), '/', '\\');

		// 计算图标路径,执行程序路径
		string iconPath = currPath + "\\corelation\\file_ares_m1.ico";

#ifdef _DEBUG
		string exePath     = currPath + "\\ModelEditor_d.exe";
		string exePathfile = currPath + "\\ModelEditor_d.exe %1";
#else
		string exePath     = currPath + "\\ModelEditor.exe";
		string exePathfile = currPath + "\\ModelEditor.exe %1";
#endif // _DEBUG

		// 写注册表
		QSettings reg("HKEY_CLASSES_ROOT\\.m1", QSettings::NativeFormat);
		reg.setValue(".", "m1file");

		QSettings regIcon("HKEY_CLASSES_ROOT\\m1file\\DefaultIcon", QSettings::NativeFormat);
		regIcon.setValue(".", iconPath.c_str());

		QSettings regOpen("HKEY_CLASSES_ROOT\\m1file\\shell\\edit\\command", QSettings::NativeFormat);
		regOpen.setValue(".", exePathfile.c_str());

		// 注册系统位置
		//QSettings regPath( QSettings::NativeFormat, QSettings::SystemScope, "Ares", "ModelEditor");
		QSettings regPath("HKEY_CURRENT_USER\\Software\\Ares\\ModelEditor", QSettings::NativeFormat);
		regPath.setValue("ExePath", exePath.c_str());*/
	}

	// 构建唯一UV
	void MOUILayout::HideShowUVBuild()
	{
		//m_uvBuild->setVisible( !m_uvBuild->isVisible());

		MOLogic_RenderWindow->SetFullScreen( true);
	}

	// 显示关闭CollisionBuild();
	void MOUILayout::HideShowCollisionBuild()
	{
		m_collisionBuild->setVisible( !m_collisionBuild->isVisible());
	}

	// 显示模型转换面板
	void MOUILayout::ShowModelConverter()
	{
		if( !m_modelConverter)
			m_modelConverter = new_ MOModelConverter( m_mainWindow);

		m_modelConverter->setVisible( true);
	}

	// 转换语言
	void MOUILayout::SwitchLanguage()
	{		
		m_translator.load("translator//modeleditor_zh.qm");	

		this->retranslateUi( m_mainWindow);							// 主界面
		//m_pShowArea->retranslateUi( m_pShowArea);			// 显示区域
		//m_pModelList->retranslateUi(m_pModelList);			// 模型列表管理
		//m_pVideoSetting->retranslateUi(m_pVideoSetting);	// 显示设置
		//m_materialEdit->retranslateUi(m_materialEdit);		// 材质编辑
		m_animationPlay->retranslateUi(m_animationPlay);	// 动画播放控制
	}

	// 设置转换类型为平移
	void MOUILayout::SetTransTypeMove()
	{
		MOLogic_Transform->SetTranformType( CAres3DAxis::EM_EDIT_TRANSLATE);
	}

	// 设置转换类型为旋转
	void MOUILayout::SetTransTypeRotation()
	{
		MOLogic_Transform->SetTranformType( CAres3DAxis::EM_EDIT_ROTATE);
	}

	// 文件发生变化
	void MOUILayout::OnFileChanged( const QString& file)
	{
		MOLogic_ModelEdit->Reload();
	}
}