#ifndef MODEL_MAINWINDOW_H
#define MODEL_MAINWINDOW_H

#include "QMainWindowEx.h"
#include "ui_FxMainWindow.h"
#include "FxPropertyEdit.h"
#include "FxObjectViewer.h"
#include "FxLogic.h"

using namespace Ares;

namespace QT_UI
{
	//----------------------------------------
	// 模型编辑器 主窗口 2010-12-30
	//----------------------------------------
	class FxMainWindow : public QObject, public Ui_FxMainWindow
	{
		Q_OBJECT

	public:
		// 构造函数
		FxMainWindow( QMainWindowEx* parent = 0);

		// 激活
		void Active();

		// 失效
		void Deactive();

		// 开始渲染
		void BeginRender();

		// 获取属性编辑界面
		FxPropertyEdit* GetPropertyEditor() { return m_propertyEdit; }

		// 获取对象查看器
		FxObjectViewer* GetObjectViewer() { return m_objectViewer; }

	private slots:
		// 新建效果
		void New();

		// 打开效果文件
		void Open();

		// 保存效果文件
		void Save();

		// 显示隐藏格子
		void SwitchGridVisible();

		// 显示隐藏背景
		void SwitchBackgroundVisible();

		// 转换到常规摄像机
		void SwitchToGeneralCamera();

		// 转换为天空摄像机
		void SwitchToSkyCamera();

		// 设置转换类型为平移
		void SetTransTypeMove();

		// about
		void  AresAbout();

	private:
		FxPropertyEdit*		m_propertyEdit;		// 属性编辑顺
		FxObjectViewer*		m_objectViewer;		// 对象查看器
		QMainWindowEx*		m_mainWindow;		// 主窗口
	};

	extern  FxMainWindow*		g_mainWindowFx;						
	#define FxUI_MainWindow		g_mainWindowFx							// 主窗口
	#define FxUI_PropertyEdit	FxUI_MainWindow->GetPropertyEditor()	// 属性编辑窗口
	#define FxUI_ObjectView		FxUI_MainWindow->GetObjectViewer()
}

#endif