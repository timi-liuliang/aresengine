#ifndef MODEL_MAINWINDOW_H
#define MODEL_MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_SHMainWindow.h"
#include "UIShaderTreeDisplay.h"
#include "ParametersEditor.h"
#include "UIMaterialExpression.h"
#include "SHLogic.h"

namespace Ares
{
	//----------------------------------------
	// 模型编辑器 主窗口 2010-12-30
	//----------------------------------------
	class SHMainWindow : public QMainWindow, public Ui_SHMainWindow
	{
		Q_OBJECT

	public:
		// 构造函数
		SHMainWindow( QWidget* parent = 0);

		// 开始渲染
		void BeginRender();

		// 文件关联
		void FileCorelation();

		// 打开文件
		void Open( const char* fileName);

		// 状态改变,控制界面显示
		void OnStateChanged();

		// 获取参数编辑器
		UIParametersEditor* GetParametersEditor() { return m_parametersEditor; }

		// 获取材质树编辑框
		UIShaderTreeDisplay* GetShaderTreeDisplay() { return m_pMdiArea; }

	private slots:
		// 打开
		void Open();

		// 保存
		void Save();

		// 新建材质树
		void NewShaderTree();

		// zoom
		void  ZoomEntity();

		// about
		void  AresAbout();

	private:
		SHLogic*				m_shLogic;				// 逻辑
		UIParametersEditor*		m_parametersEditor;		// 属性编辑
		UIShaderTreeDisplay*	m_pMdiArea;				// dx9应用
		UIMaterialExpression*	m_materialExpression;	// 材质表达式
	};

	extern  SHMainWindow*			g_mainWindowSH;	
	#define SHUI_MainWindow			g_mainWindowSH
	#define SHUI_ParametersEditor	SHUI_MainWindow->GetParametersEditor()
	#define SHUI_ShaderTreeDisplay	SHUI_MainWindow->GetShaderTreeDisplay()
}

#endif