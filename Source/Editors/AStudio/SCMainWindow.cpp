#include <QtGui/QtGui>
#include "SCMainWindow.h"
#include "AI_UI.h"
#include "ContentBrowser.h"
#include "MapBuilder.h"
#include "WaterEdit.h"
#include "WorldMap.h"
#include "ImageProcess.h"
#include "PropertyEditor.h"
#include "VideoSetting.h"
#include "SCLogic.h"
#include "ModelEdit/MOMainWindow.h"
#include "ShaderEdit/SHMainWindow.h"
#include "TextureTool/TextureTool.h"
#include "lightmass/LMMainWindow.h"
#include <string>

using namespace std;
using namespace Ares;

namespace QT_UI
{
	SCMainWindow*	g_mainWindowSC = NULL;		

	// 构造函数
	SCMainWindow::SCMainWindow( QMainWindowEx* parent/* = 0*/)
		: QObject( parent)
		, m_contentBrowser( NULL)
		, m_lightMap( NULL)
		, m_pWorldMap( NULL)
		, m_mainWindow( parent)
	{
		g_mainWindowSC = this;

		setupUi( m_mainWindow);

		// 消息连接
		connect( m_pActionAbout, SIGNAL(triggered()), m_mainWindow, SLOT(AresAbout()));
		connect( m_actionNewMap, SIGNAL(triggered()), this, SLOT(NewMap()));

		connect( m_pActionSelect,   SIGNAL( triggered()), this, SLOT( EntitySelect()));
		connect( m_pActionMove,     SIGNAL( triggered()), this, SLOT( EntityMove()));
		connect( m_pActionScale,    SIGNAL( triggered()), this, SLOT( EntityScale()));
		connect( m_pActionRotation, SIGNAL( triggered()), this, SLOT( EntityRotate()));
		connect( m_actionPathFind, SIGNAL( triggered()),  this, SLOT( AIPathFind()));

		connect( m_actionSwitch_to_Game, SIGNAL(triggered()), this, SLOT(SetMoveType()));
		connect( m_actionCombination, SIGNAL(triggered()), this, SLOT(CombinationBatch()));

		connect( m_skinDefault,SIGNAL( triggered()), this, SLOT( SwitchToDefaultSkin()));
		connect( m_skinClassic,SIGNAL( triggered()), this, SLOT( SwitchToClassicSkin()));
		connect( m_skinCustom, SIGNAL( triggered()), this, SLOT( SwitchToCustomSkin()));

		connect( m_actionWorldMap, SIGNAL( triggered()), this, SLOT( ShowWorldMap()));

		// 资源管理器
		m_inspector = new_ SceneInspector( m_mainWindow);
		m_mainWindow->addDockWidget( Qt::RightDockWidgetArea, m_inspector);

		// 世界地图(默认不可见)
		m_pWorldMap = new_ CWorldMap( m_mainWindow);
		//this->addDockWidget( Qt::AllDockWidgetAreas, m_pWorldMap);	

		// 地形编辑
		//m_toolKit = new ToolKit( m_mainWindow);
		//m_mainWindow->addDockWidget( Qt::RightDockWidgetArea, m_toolKit);

		// 视频设置
		m_pVideoSetting = new_ CVideoSetting( m_mainWindow);

		// AI
		m_pAI = new_ CAIUI( m_mainWindow);

		// 水编辑
		m_pWaterEdit = new CWaterEdit( m_mainWindow);
		m_mainWindow->addDockWidget( Qt::RightDockWidgetArea, m_pWaterEdit);

		// 图象处理
		m_pImageProcess = new CImageProcess( m_mainWindow);
		m_mainWindow->addDockWidget( Qt::RightDockWidgetArea, m_pImageProcess);	

		// 属性编辑
		m_pPropertyEditor = new CPropertyEditor( m_mainWindow);
		m_mainWindow->addDockWidget( Qt::RightDockWidgetArea, m_pPropertyEditor);

		//this->tabifyDockWidget( m_pPropertyEditor, m_pWaterEdit);
		//this->tabifyDockWidget( m_pVideoSetting, m_pTerrainEdit);
		//this->tabifyDockWidget( m_pPropertyEditor, m_pTerrainEdit);
		//this->tabifyDockWidget( m_pAI, m_mapView);

		connect( m_pActionSave,  SIGNAL(triggered()), m_pWorldMap, SLOT(SaveMap()));
		connect( m_pActionOpen,  SIGNAL(triggered()), this, SLOT(OpenMap()));
		//connect( m_pShowArea, SIGNAL(fpsChanged(float)), m_pPropertyEditor->GetFrame("Setting"), SLOT(SetFps(float)));
		//connect( m_actionWorldMap, SIGNAL( triggered(bool)), m_pWorldMap, SLOT(setVisible(bool)));
		//connect( m_pWorldMap, SIGNAL( EditAreaChanged(IArea*)), m_pTerrainEdit, SLOT(SetCurrArea( IArea*)));

		connect( m_actionTextureTool,   SIGNAL(triggered()), this, SLOT(HideShowTextureTool()));
		connect( m_actionShaderEdit,	SIGNAL(triggered()), this, SLOT(HideShowShaderEdit()));
		connect( m_actionLightmass,		SIGNAL(triggered()), this, SLOT(HideShowLightmass()));
		connect( m_actionContentBrowser,SIGNAL(triggered()), this, SLOT(HideShowContentBrowser()));
		connect( m_actionBuildLightmap, SIGNAL(triggered()), this, SLOT(HideShowLightmapBake()));
		connect( m_actionNavigationMesh,SIGNAL(triggered()), this, SLOT(HideShowNavMeshBuild()));
		connect( m_actionPropertys,		SIGNAL(triggered()), this, SLOT(HideShowSettings()));

		// 填充方式
		connect( m_actionWireFrame,		SIGNAL(triggered()), this, SLOT(SwitchToWireFrameFillMode()));
		connect( m_actionNormal,		SIGNAL(triggered()), this, SLOT(SwitchToNormalFillMode()));

		// 导入
		connect( m_actionImportCurScene, SIGNAL(triggered()), this, SLOT( ImportFileToCurScene()));

		// 导出
		connect( m_actionExportNavigationMesh, SIGNAL(triggered()), m_pAI, SLOT(SaveNavMesh()));

		// 默认隐藏
		m_pImageProcess->setVisible( false);
		m_pWaterEdit->setVisible( false);
		//m_toolKit->setVisible( false);
		m_pWorldMap->setVisible( false);
		m_pVideoSetting->setVisible( false);

		m_visibles[m_pPropertyEditor] = true;
		m_visibles[m_inspector]		  = true;
	}

	// 激活
	void SCMainWindow::Active()
	{
		m_toolBar->setVisible( true);

		// 默认隐藏
		m_pImageProcess->setVisible( false);
		m_pPropertyEditor->setVisible( m_visibles[m_pPropertyEditor]);
		m_pWaterEdit->setVisible( false);
		//m_toolKit->setVisible( false);
		m_pWorldMap->setVisible( false);
		m_inspector->setVisible( m_visibles[m_inspector]);
		m_pVideoSetting->setVisible( false);

		m_mainWindow->setMenuBar( m_menuBar);
	}

	// 失效
	void SCMainWindow::Deactive()
	{
		m_toolBar->setVisible( false);

		// 默认隐藏
		m_pImageProcess->setVisible( false);
		m_pPropertyEditor->setVisible( false);
		m_pWaterEdit->setVisible( false);
		//m_toolKit->setVisible( false);
		m_pWorldMap->setVisible( false);
		m_inspector->setVisible( false);
		m_pVideoSetting->setVisible( false);
	}

	// 开始渲染
	void SCMainWindow::BeginRender() 
	{ 
		// 加载配置
		Logic_Cfg->Signal_OnRencentFilesChanged.connect( boost::bind<void>(&SCMainWindow::UpdateRecentFilesDisplay, this, _1));
		Logic_Cfg->Signal_CameraSettingsChanged.connect( boost::bind<void>(&CVideoSetting::UpdateCameraSettingsDisplay, m_pVideoSetting));
		Logic_Cfg->Load();

		// 场景信息
		SCLogic_Map->Signal_OnSceneChanged.connect( boost::bind<void>(&SCMainWindow::OnSceneStateChanged, this));
		SCLogic_Map->Restore();

		// 更改标题
		switch( ARenderDevice->GetType())
		{
		case D3D11:		m_mainWindow->setWindowTitle( tr("Ares Studio(32位, Dx11)"));		break;
		case OPENGL:	m_mainWindow->setWindowTitle( tr("Ares Studio(32位, OpenGL)"));		break;
		case OPENGLES:	m_mainWindow->setWindowTitle( tr("Ares Studio(32位, OpenGLES)"));	break;
		default:		m_mainWindow->setWindowTitle( tr("Ares Studio(32位, UnKnown)"));		break;	
		}
	}

	// 析构函数
	SCMainWindow::~SCMainWindow()
	{
		Logic_Cfg->Save();
	}

	// 平移
	void  SCMainWindow::EntityMove()
	{
		SCLogic_Main->SetHandleType( EM_HT_EDITOENTITY);
		SCLogic_EntityEdit->SetEditType( CEntityEditLogic::EM_MOVE);

		m_pActionScale->setChecked( false);
		m_pActionSelect->setChecked( false);
		m_pActionRotation->setChecked( false);
	}

	// 缩放
	void  SCMainWindow::EntityScale()
	{
		SCLogic_Main->SetHandleType( EM_HT_EDITOENTITY);
		SCLogic_EntityEdit->SetEditType( CEntityEditLogic::EM_SCALE);

		m_pActionSelect->setChecked( false);
		m_pActionRotation->setChecked( false);
		m_pActionMove->setChecked( false);
	}

	// 旋转
	void  SCMainWindow::EntityRotate()
	{
		SCLogic_Main->SetHandleType( EM_HT_EDITOENTITY);
		SCLogic_EntityEdit->SetEditType( CEntityEditLogic::EM_ROTATE);

		m_pActionScale->setChecked( false);
		m_pActionSelect->setChecked( false);
		m_pActionMove->setChecked( false);
	}

	// 选择
	void  SCMainWindow::EntitySelect()
	{
		SCLogic_Main->SetHandleType( EM_HT_EDITOENTITY);
		SCLogic_EntityEdit->SetEditType( CEntityEditLogic::EM_SELECT);

		m_pActionScale->setChecked( false);
		m_pActionRotation->setChecked( false);
		m_pActionMove->setChecked( false);
	}

	// 路径查找
	void SCMainWindow::AIPathFind()
	{
		SCLogic_Main->SetHandleType( EM_HT_AI);
	}

	// 设置子状态
	static void SetActionsEnabled( QList<QAction*>& actions, bool enabled)
	{
		for( int i=0; i<actions.size(); i++)
			actions[i]->setEnabled( enabled);
	}


	// 场景状态改变,控制界面显示
	void SCMainWindow::OnSceneStateChanged()
	{
		if( SCLogic_Scene && SCLogic_Scene->GetLocation().size())
		{
			// Enable all menubar, toolbar actions
			SetActionsEnabled( m_menuBar->actions(), true);
			SetActionsEnabled( m_toolBar->actions(), true);
			SetActionsEnabled( m_menuF->actions(), true);
		}
		else
		{
			// Disenable all menubar actions
			QAction*		 menuFAction = m_menuF->menuAction();
			QList<QAction*>	 menuActions = m_menuBar->actions(); menuActions.removeAll( menuFAction);

			SetActionsEnabled( menuActions, false);
			SetActionsEnabled( m_menuF->actions(), false);
			SetActionsEnabled( m_toolBar->actions(), false);

			m_actionNewMap->setEnabled( true);
			m_pActionOpen->setEnabled( true);

			MapBuilder::NewMapUnSaved();
		}
	}

	// 新建地图
	void  SCMainWindow::NewMap()
	{
		static MapBuilder* pForm = NULL;
		if( !pForm)
			pForm = new MapBuilder( m_mainWindow);

		pForm->setVisible( true);
	}

	// 打开数据
	void  SCMainWindow::OpenMap()
	{
		m_pWorldMap->OpenMap();

		//m_pTerrainEdit->UpdateTerrainTexture();
	}

	// 显示大地图
	void SCMainWindow::ShowWorldMap()
	{
		m_pWorldMap->setVisible( !m_pWorldMap->isVisible());
	}

	// 显示关闭纹理编辑工具
	void SCMainWindow::HideShowTextureTool()
	{
		static TextureTool* textureTool = NULL;
		if( !textureTool)
			textureTool = new_ TextureTool;		

		textureTool->show();
	}

	// 打开效果编辑器
	void SCMainWindow::HideShowShaderEdit()
	{
		static SHMainWindow* shMainWindow = NULL;
		if( !shMainWindow)
			shMainWindow = new_ SHMainWindow;		
		
		shMainWindow->show();
	}

	// 打开Lightmass
	void SCMainWindow::HideShowLightmass()
	{
		static LMMainWindow* lmMainWindow = NULL;
		if( !lmMainWindow)
			lmMainWindow = new_ LMMainWindow( m_mainWindow);		

		lmMainWindow->show();
	}

	// 显示隐藏内容浏览器
	void SCMainWindow::HideShowContentBrowser()
	{
		// 模型列表
		if( !m_contentBrowser)
			m_contentBrowser = new ContentBrowser( m_mainWindow);

		m_contentBrowser->setVisible( !m_contentBrowser->isVisible());
	}

	// 显示隐藏光照图烘焙管理窗口
	void SCMainWindow::HideShowLightmapBake()
	{
		if( !m_lightMap)
			m_lightMap = new Lightmap( m_mainWindow);

		m_lightMap->setVisible( !m_lightMap->isVisible());
	}

	// 显示隐藏导航网格生成
	void SCMainWindow::HideShowNavMeshBuild()
	{
		if( m_pAI)
			m_pAI->setVisible( !m_pAI->isVisible());
	}

	// 显示隐藏设置
	void SCMainWindow::HideShowSettings()
	{
		if( m_pVideoSetting)
			m_pVideoSetting->setVisible( !m_pVideoSetting->isVisible());
	}

	// 设置移动类型
	void SCMainWindow::SetMoveType()
	{
		if( m_actionSwitch_to_Game->isChecked())
		{
			SCLogic_Move->SetMoveType( CMoveLogic::EM_FOLLOW);
		}
		else
		{
			SCLogic_Move->SetMoveType( CMoveLogic::EM_THIRD);
		}
	}

	// 合并实体
	void SCMainWindow::CombinationBatch()
	{
		SCLogic_Main->CombinationBatch();
	}

	// 网络填充模式
	void SCMainWindow::SwitchToWireFrameFillMode()
	{
		// 设置光栅化状态
		Logic_AI->SwitchToWireFrameFillMode();
	}

	// 正常填充模式
	void SCMainWindow::SwitchToNormalFillMode()
	{
		Logic_AI->SwitchToNormalFillMode();
	}

	// 更换到默认皮肤
	void SCMainWindow::SwitchToDefaultSkin()
	{
		qApp->setStyleSheet("");
	}

	// 更换到经典皮肤
	void SCMainWindow::SwitchToClassicSkin()
	{
		SetStyleSheet("edit_res/skin/classic.qss");
	}

	// 选择用户自定义皮肤
	void SCMainWindow::SwitchToCustomSkin()
	{
		QString qFileName = QFileDialog::getOpenFileName( m_mainWindow, tr("open qss"), "", tr("*.qss"));

		QDir dir( QDir::currentPath());
		qFileName = dir.relativeFilePath( qFileName);

		SetStyleSheet( qFileName.toStdString().c_str());
	}

	// 设置styleSheet
	void SCMainWindow::SetStyleSheet( const char* fileName)
	{
		QFile qss( fileName);
		qss.open(QFile::ReadOnly);
		qApp->setStyleSheet(qss.readAll());
		qss.close();
	}

	// 更新最近打开文件列表显示
	void SCMainWindow::UpdateRecentFilesDisplay( const list<string>& rencentlyFiles)
	{
		m_actionRecentFiles->clear();

		for ( ConfigMgr::FileList::const_iterator it=rencentlyFiles.begin(); it!=rencentlyFiles.end(); it++)
		{
			QAction* tAction = new QAction( m_actionRecentFiles);
			if( tAction)
			{
				tAction->setText(  QObject::tr(it->c_str()));
				connect( tAction, SIGNAL( triggered()), this, SLOT( OpenRecentFile()));

				m_actionRecentFiles->insertAction( NULL, tAction);
			}
		}	
	}

	// 打开最近文件
	void SCMainWindow::OpenRecentFile()
	{
		QAction *action = qobject_cast<QAction *>(sender());
		if (action)
		{
			string fileName= action->text().toStdString();

			OpenFile( fileName.c_str());
		}
	}

	// 导入文件到当前场景
	void SCMainWindow::ImportFileToCurScene()
	{
		QString qFileName = QFileDialog::getOpenFileName( m_mainWindow, tr("导入"), "", tr("*.tri *.nav"));
		QDir dir( QDir::currentPath());
		qFileName = dir.relativeFilePath( qFileName);

		OpenFile( qFileName.toStdString().c_str());
	}

	// 打开文件
	void SCMainWindow::OpenFile( const char* filePath)
	{
		// 根据不同后缀名进行处理
		string ext = FileUtils::GetExt( filePath);
		if( ext==".tri")
			Logic_AI->OpenTri( filePath);
		else if( ext==".nav")
			Logic_AI->OpenNav( filePath);
	}
}