#pragma once

#include <QTimer>
#include <QMainWindowEx.h>
#include <Core/AresSingleton.hpp>
#include "ui_MainWindow.h"
#include "SCMainWindow.h"
#include "FxEdit/FxMainWindow.h"
#include "ModelEdit/MOMainWindow.h"
#include "UIEdit/UIMainWindow.h"
#include "Application.h"
#include "MdiArea.h"

namespace QT_UI
{
	//----------------------------------------
	// AStudio 主窗口 2013-07-25
	//----------------------------------------
	class MainWindow : public QMainWindowEx
	{
		Q_OBJECT

	public:
		MainWindow( QWidget* parent = 0);
		~MainWindow();

	public:
		// 激活
		void Active();

		// 失效
		void Deactive();

		// 开始渲染
		void BeginRender();

		// 日志面板
		LogPanel* GetLogPanel() { return m_logPanel; }

	public slots:
		// 设置编辑器类型
		void SetEditorType( const QString& edType);

		// 循环
		void Loop();

		// 关于
		void  AresAbout();

	private:
		QTimer*				m_timer;			// 计时器
		Application*		m_application;		// 应用程序
		Ui_MainWindow		m_mainMenuToolBar;	// 主窗口编辑MenuBar ToolBar
		SCMainWindow*		m_scUILayout;		// 场景编辑器MenuBar ToolBar
		MOUILayout*			m_moUILayout;		// 模型编辑布局
		FxMainWindow*		m_fxUILayout;		// 特效编辑布局
		UIMainWindow*		m_uiUILayout;		// 界面编辑布局
		LogPanel*			m_logPanel;			// 日志面板警告等输出显示

		MOMdiArea*			m_mdiArea;			// 中心区域

		QComboBox*			m_comBoBox;
	};

	extern  MainWindow*			g_mainWindow;	// 主窗口
	#define UI_MainWindow		g_mainWindow	// 主窗口
	#define UI_LogPanel			UI_MainWindow->GetLogPanel()
}