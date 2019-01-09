#pragma once

#include <QMainWindowEx.h>
#include <QFileSystemWatcher>
#include <QTranslator>
#include "ui_MOMainWindow.h"
#include "MOAnimationEdit.h"
#include "MOUVBuild.h"
#include "MOCollisionBuild.h"
#include "MOMaterialView.h"
#include "MOVideoSetting.h"
#include "MOModelConverter.h"
#include <boost/array.hpp>

namespace QT_UI
{
	//----------------------------------------
	// 模型编辑界面布局 2010-12-30
	//----------------------------------------
	class MOUILayout : public QObject, public Ui_MOMainWindow
	{
		Q_OBJECT

	public:
		// 构造函数
		MOUILayout( QMainWindowEx* parent = 0);

		// 激活
		void Active();

		// 失效
		void Deactive();

		// 开始渲染
		void BeginRender();

		// 文件关联
		void  FileCorelation();

		// 获取translator
		QTranslator& GetTranslator() { return m_translator; }

		// 获取UVBuild窗口
		UVBuild* GetUVBuild() { return m_uvBuild; }

		// 获取文件监视器
		QFileSystemWatcher* GetFileSystemWatcher() { return m_filesystemWatcher; }

	public slots:
		// 打开
		void Open();

		// 保存所有
		void Save();

		// 打开实体文件
		void OpenEntity( const char* fileName);

		// 设置转换类型为平移
		void SetTransTypeMove();

		// 设置转换类型为旋转
		void SetTransTypeRotation();

	private slots:
		// 显示关闭UVBuild
		void HideShowUVBuild();

		// 显示关闭CollisionBuild();
		void HideShowCollisionBuild();

		// 显示关闭模型转换面板
		void ShowModelConverter();

		// 文件发生变化
		void OnFileChanged( const QString& file);

		// 转换语言
		void SwitchLanguage();

	private:
		MOVideoSetting*		m_pVideoSetting;		// 显示设置
		CAnimationEdit*		m_animationPlay;		// 动画播放控制
		UVBuild*			m_uvBuild;				// 光照图构建
		CollisionBuild*		m_collisionBuild;		// 碰撞数据编辑
		MateriaViewer*		m_materialView;			// 材质查看面版
		QTranslator			m_translator;			// 翻译
		QFileSystemWatcher*	m_filesystemWatcher;	// 文件系统监视器
		QMainWindowEx*		m_mainWindow;			// 主窗口
		MOModelConverter*	m_modelConverter;		// 模型转换面板
		boost::array<bool,6>m_visibleState;			// 可见性状态
	};

	extern  MOUILayout*				g_mainWindowME;						
	#define MOUI_MainWindow			g_mainWindowME							// 主窗口
	#define MOUI_UVBuild			MOUI_MainWindow->GetUVBuild()				// UVBuild窗口
	#define MOUI_FileSystemWatcher	MOUI_MainWindow->GetFileSystemWatcher()	// 文件系统监视器
}