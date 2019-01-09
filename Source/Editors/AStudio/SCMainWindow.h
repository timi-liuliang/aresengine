#ifndef SCENE_MAINWINDOW_H
#define SCENE_MAINWINDOW_H

#include "LightMap.h"
#include "LogPanel.h"
#include <QtGui/QMainWindow>
#include "ui_SCMainWindow.h"
#include <Core/AresSingleton.hpp>
#include "SceneInspector.h"
#include "PropertyEditor.h"
#include "AI_UI.h"
#include "WorldMap.h"
#include "ContentBrowser.h"
#include "VideoSetting.h"
#include "WaterEdit.h"
#include "ImageProcess.h"
#include <QMainWindowEx.h>

namespace QT_UI
{
	//----------------------------------------
	// 模型编辑器 主窗口 2010-12-30
	//----------------------------------------
	class SCMainWindow : public QObject, public Ui_SCMainWindow
	{
		Q_OBJECT

	public:
		SCMainWindow( QMainWindowEx* parent = 0);
		~SCMainWindow();

		// 开始渲染
		void BeginRender();

		// 设置styleSheet
		void SetStyleSheet( const char* fileName);

		// 场景状态改变,控制界面显示
		void OnSceneStateChanged();

		// 激活
		void Active();

		// 失效
		void Deactive();

	public:
		// 获取属性编辑窗口
		CPropertyEditor* GetPropertyEditor() { return m_pPropertyEditor; }

		// 地图资源查看器
		SceneInspector* GetMapView() { return m_inspector; }

		// 获取工具箱
		//ToolKit* GetToolKit() { return m_toolKit; }

	private:
		// 更新最近打开文件列表显示
		void UpdateRecentFilesDisplay( const list<string>& rencentlyFiles);

	private slots:
		// 网络填充模式
		void SwitchToWireFrameFillMode();

		// 正常填充模式
		void SwitchToNormalFillMode();

		// 更换到默认皮肤
		void SwitchToDefaultSkin();

		// 更换到经典皮肤
		void SwitchToClassicSkin();

		// 选择用户自定义皮肤
		void SwitchToCustomSkin();

	private slots:
		// 新建地图
		void  NewMap();

		// 打开数据
		void  OpenMap();

		// 打开文件
		void OpenFile( const char* filePath);

		// 打开最近文件
		void OpenRecentFile();

		// 导入文件到当前场景
		void ImportFileToCurScene();

		// 显示大地图
		void ShowWorldMap();

		// 显示关闭纹理编辑工具
		void HideShowTextureTool();

		// 打开效果编辑器
		void HideShowShaderEdit();

		// 打开Lightmass
		void HideShowLightmass();

		// 显示关闭内容浏览器
		void HideShowContentBrowser();

		// 显示隐藏光照图烘焙管理窗口
		void HideShowLightmapBake();

		// 显示隐藏导航网格生成
		void HideShowNavMeshBuild();

		// 显示隐藏设置窗口
		void HideShowSettings();

		// 编辑实体
		void  EntityMove();
		void  EntityScale();
		void  EntityRotate();
		void  EntitySelect();

		// 路径查找
		void AIPathFind();

		// 设置移动类型
		void SetMoveType();

		// 合并实体
		void CombinationBatch();

	private:
		CAIUI*            m_pAI;				// AI编辑
		CWorldMap*		  m_pWorldMap;			// 世界地图显示
		ContentBrowser*   m_contentBrowser;		// 内容浏览器
		CWaterEdit*		  m_pWaterEdit;			// 水编辑
		//ToolKit*		  m_toolKit;		// 地形编辑
		CVideoSetting*    m_pVideoSetting;		// 显示设置
		CImageProcess*    m_pImageProcess;		// 图象处理UI
		CPropertyEditor*  m_pPropertyEditor;	// 属性编辑
		Lightmap*		  m_lightMap;			// 光照图烘焙
		SceneInspector*   m_inspector;			// 地图查看编辑树型结构
		QMainWindowEx*	  m_mainWindow;			// 主窗口
		map<void*, bool>  m_visibles;			// 可见性
	};

	extern	SCMainWindow*				g_mainWindowSC;
	#define SCUI_MainWindow				g_mainWindowSC
	#define SCUI_Property				SCUI_MainWindow->GetPropertyEditor()
	#define SCUI_SceneInspector			SCUI_MainWindow->GetMapView()
	#define SCUI_SceneInspectorModel	SCUI_SceneInspector->GetSceneInsepctorModel()
	//#define SCUI_ToolKit				SCUI_MainWindow->GetToolKit()
}

#endif