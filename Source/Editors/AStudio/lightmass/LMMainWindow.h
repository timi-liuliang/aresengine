#pragma once

#include <list>
#include <string>
//#include <boost/tuple/tuple.hpp>
#include <QtGui/QMainWindow>
#include <QtGui/QFileSystemModel>
#include <QtGui/QGraphicsPixmapItem>
#include "ui_LMMainWindow.h"
//#include "ConfigMgr.h"
#include "QPbrtSyntaxHighLighter.h"
#include "PbrtTaskQueue.h"

using namespace std;

namespace Ares
{
	//----------------------------------------
	// 模型编辑器 主窗口 2011-12-20
	//----------------------------------------
	class LMMainWindow : public QMainWindow, public Ui_LMMainWindow
	{
		Q_OBJECT

	public:
		// 构造函数
		LMMainWindow( QWidget* parent = 0);

		// 析构函数
		~LMMainWindow();

		// 恢复上次关闭时状态
		void RestoreState( const char* pbrtFile, const char* img, int tabIdx);

	private slots:
		// 打开文件
		void Open();

		// 打开文件
		void Open( const char* filePath);

		// 保存文件
		void Save();

		// 打开最近文件
		void OpenRecentFile();

		// 渲染场景
		void RenderScene();

		// 查看场景结果
		void LookScene();

		// Pbrt文件被修改
		void PbrtChanged();

		// 设置皮肤样式
		void SetSkin();

	private:
		// 更新最近打开文件列表显示
		//void UpdateRecentFilesDisplay( const ConfigMgr::FileList& rencentlyFiles);

		// 生成目标文件名
		void GenerateImageFileName( string& oImageFile, const char* sceneFullPath);

		// 显示生成的图片
		void ShowResultImage( const char* resultImage);

		// 显示pbrt文件内容
		void ShowPbrt( const char* fileName);

		// 附加消息
		void AppendOutput( const char* msg);

		// 设置进度
		void SetProcess( float process);

	protected:
		// drag enter event
		void dragEnterEvent( QDragEnterEvent* event);
		
		// drop event
		void dropEvent( QDropEvent* event);

	private:
		string					m_currPbrt;				// 当前pbrt文件
		string					m_currImg;				// 当前打开图片
		string					m_currPbrtFileExt;		// 当前pbrt文件名
		string					m_saveDirectory;		// 设置保存目录
		QFileSystemModel		m_fileSystemModel;		// 文件系统目录
		QGraphicsScene			m_graphicsScene;		// 渲染场景
		QGraphicsPixmapItem		m_graphicsItem;			// 显示用
		QPbrtSyntaxHighLighter*	m_pbrtHighlight;		// pbrt语法高度(pbrt场景脚本)

		PbrtTaskQueue			m_pbrtTaskQueue;		// pbrt任务队列
	};
}