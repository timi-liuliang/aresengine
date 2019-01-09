#pragma once

#include <QtGui/QMainWindow>
#include "QMainWindowEx.h"
#include "ui_UIMainWindow.h"
#include "UIEdit.h"

namespace QT_UI
{
	//----------------------------------------
	// 模型编辑器 主窗口 2010-12-30
	//----------------------------------------
	class UIMainWindow : public QObject, public Ui_UIMainWindow
	{
		Q_OBJECT

	public:
		// 构造函数
		UIMainWindow( QMainWindowEx* parent = 0);

		// 开始渲染
		void BeginRender();

		// 激活
		void Active();

		// 失效
		void Deactive();

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

		// about
		void  AresAbout();

	private:
		UiEdit*				m_uiEditLogic;		// 界面编辑逻辑
		QMainWindowEx*		m_mainWindow;		// 主窗口
	};
}