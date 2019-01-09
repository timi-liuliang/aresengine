#include <QtGui/QtGui>
#include <QFile>
#include "LMMainWindow.h"
#include <QtDeclarative/QDeclarativeContext>
#include <pbrt/core/api.h>
#include <pbrt/core/parser.h>
#include <pbrt/core/parallel.h>
#include <algorithm>
#include <ostream>
#include <Core/AresChar.h>
#include "QLoadTga.h"

namespace Ares
{
	#define  TAB_INDEX_IMG  0
	#define  TAB_INDEX_PBRT 1

	// 构造函数
	LMMainWindow::LMMainWindow( QWidget* parent/* = 0*/)
		: QMainWindow( parent)
	{
		// 1.成员变量默认值
		m_saveDirectory = "gallery\\";

		// 2.设置UI
		setupUi( this);
		m_progressBar->setVisible(false);

		// 3.联接Qt信号槽
		connect( m_actionOpen, SIGNAL( triggered()), this, SLOT( Open()));
		connect( m_actionSave, SIGNAL( triggered()), this, SLOT( Save()));
		connect( m_actionBuild, SIGNAL( triggered()), this, SLOT( LookScene()));
		connect( m_actionReBuild, SIGNAL( triggered()), this, SLOT( RenderScene()));
		connect( m_pbrtTextEdit,  SIGNAL( textChanged()), this, SLOT( PbrtChanged()));
		connect( m_actionSutra,   SIGNAL( triggered()), this, SLOT( SetSkin()));

		// 4.联接boost信号槽
		//ConfigMgr::GetSingleton().Signal_OnRencentFilesChanged.connect( boost::bind<void>(&MainWindow::UpdateRecentFilesDisplay, this, _1));
		//ConfigMgr::GetSingleton().Signal_OnRestoreState.connect( boost::bind<void>(&MainWindow::RestoreState, this, _1, _2, _3));
		//Pbrt::Signal_Error.connect( boost::bind<void>( &MainWindow::AppendOutput, this, _1));
		//Pbrt::Signal_Process.connect( boost::bind<void>( &MainWindow::SetProcess, this, _1));

		m_graphicsItem.setFlag( QGraphicsItem::ItemIsMovable, true);
		m_graphicsScene.addItem( &m_graphicsItem);
		m_graphicsView->setScene( &m_graphicsScene);

		// 语法高亮
		m_pbrtHighlight = new QPbrtSyntaxHighLighter( m_pbrtTextEdit->document());

		//m_fileSystemModel.setRootPath("D:\\Youdao");
		//m_galleryView->setModel( &m_fileSystemModel);

		// 5.加载显示最近打开文件列表
		//ConfigMgr::GetSingleton().Load();

		// 开启任务队列(放到初始放的最后执行)
		//m_pbrtTaskQueue.Start( 10);
		//m_pbrtTaskQueue.Signal_OnPbrtFinised.connect( boost::bind<void>( &MainWindow::ShowResultImage, this, _1));

		// 设置界面皮肤
		SetSkin();
	}

	// 析构函数
	LMMainWindow::~LMMainWindow()
	{
		//m_pbrtTaskQueue.Finish();
		//m_pbrtTaskQueue.End();

		// 保存程序状态
		//ConfigMgr::GetSingleton().SetProgramState( m_currPbrt.c_str(), m_currImg.c_str(), m_mainWidget->currentIndex());
		//ConfigMgr::GetSingleton().Save();
	}

	// 恢复上次关闭时状态
	void LMMainWindow::RestoreState( const char* pbrtFile, const char* img, int tabIdx)
	{
		// 编辑显示
		ShowPbrt( pbrtFile);

		ShowResultImage( img);

		m_mainWidget->setCurrentIndex( tabIdx);
	}

	// 打开文件
	void LMMainWindow::Open()
	{
		QString qFileName = QFileDialog::getOpenFileName( this, tr("Open pbrt"), "", tr("(*.pbrt)"));
		if( !qFileName.isEmpty())
		{
			string fileName = qFileName.toStdString();

			Open( fileName.c_str());
		}
	}

	// 打开文件
	void LMMainWindow::Open( const char* filePath)
	{
		if( filePath)
		{
			//ConfigMgr::GetSingleton().AddToRecentlyFile( filePath); 

			// 编辑显示
			ShowPbrt( filePath);
		}
	}

	// 保存文件
	void LMMainWindow::Save()
	{
		FILE* fileHandle = fopen( m_currPbrt.c_str(), "wt");
		if( fileHandle)
		{
			string plainText = m_pbrtTextEdit->toPlainText().toStdString();
			fwrite( plainText.c_str(), plainText.size(), 1, fileHandle);

			fflush(fileHandle);
			fclose(fileHandle);
		}

		// 更改标题
		m_mainWidget->setTabText( 1, m_currPbrtFileExt.c_str());
	}

	// 打开最近文件
	void LMMainWindow::OpenRecentFile()
	{
		QAction *action = qobject_cast<QAction *>(sender());
		if (action)
		{
			string fileName= action->text().toStdString();
			if( fileName.size())
			{
				//ConfigMgr::GetSingleton().AddToRecentlyFile( fileName.c_str()); 

				// 编辑显示
				ShowPbrt( fileName.c_str());
			}
		}
	}

	// 生成目标文件名
	void LMMainWindow::GenerateImageFileName( string& oImageFile, const char* sceneFullPath)
	{
		char sceneFileName[512];
		_splitpath( sceneFullPath, NULL, NULL, sceneFileName, NULL);

		oImageFile = m_saveDirectory + sceneFileName + ".tga";
	}

	//// 更新最近打开文件列表显示
	//void LMMainWindow::UpdateRecentFilesDisplay( const ConfigMgr::FileList& rencentlyFiles)
	//{
	//	m_actionRecentFiles->clear();

	//	for ( ConfigMgr::FileList::const_iterator it=rencentlyFiles.begin(); it!=rencentlyFiles.end(); it++)
	//	{
	//		QAction* tAction = new QAction( m_actionRecentFiles);
	//		if( tAction)
	//		{
	//			tAction->setText(  it->c_str());
	//			connect( tAction, SIGNAL( triggered()), this, SLOT( OpenRecentFile()));

	//			m_actionRecentFiles->insertAction( NULL, tAction);
	//		}
	//	}	
	//}

	// 查看场景结果
	void LMMainWindow::LookScene()
	{
		string sceneTga;
		GenerateImageFileName( sceneTga, m_currPbrt.c_str());

		// 判断文件是否存在
		if( QFile::exists(sceneTga.c_str()))
		{
			ShowResultImage( sceneTga.c_str());
		}
		else
		{
			RenderScene();
		}
	}

	// 渲染场景
	void LMMainWindow::RenderScene()
	{
		if( m_currPbrt.empty())
			return;

		if( m_pbrtTaskQueue.IsTaskExist( m_currPbrt.c_str()))
			return;

		m_outPut->clear();

		// 测试pbrt
		Options options;
		options.nCores		= 3;
		options.openWindow	= true;
		options.quickRender = false;
		options.quiet		= false;
		options.verbose		= true;

		// 设定输出文件位置
		GenerateImageFileName( options.imageFile, m_currPbrt.c_str());

#if 0
		// 初始化
		pbrtInit( options);

		// 分析场景文件
		ParseFile( m_currPbrt.c_str());

		// 清除数据
		pbrtCleanup();
#else
		// 添加到任务队列
		m_pbrtTaskQueue.AddTask( m_currPbrt.c_str(), options);
#endif
		m_progressBar->setValue( 0);
		m_progressBar->setVisible( true);
	}

	// 显示生成的图片
	void LMMainWindow::ShowResultImage( const char* resultImage)
	{
		if( strlen( resultImage))
		{
			m_progressBar->setVisible( false);
			m_mainWidget->setCurrentIndex( 0);

			m_graphicsItem.setPixmap( QPixmap( LoadTga(resultImage)));

			m_currImg = resultImage;
		}
	}

	// 显示pbrt文件内容
	void LMMainWindow::ShowPbrt( const char* fileName)
	{
		if( !strlen( fileName))
			return;

		// 打开文件
		QFile file( fileName);
		if( file.open( QFile::ReadOnly | QFile::Text))
		{
			m_currPbrt = fileName;

			m_pbrtTextEdit->setTabStopWidth( 24);
			m_pbrtTextEdit->setPlainText( file.readAll());
		}

		// 更改标题
		char tFileName[512];
		char tFileExt[512];
		_splitpath( m_currPbrt.c_str(), NULL, NULL, tFileName, tFileExt);

		m_currPbrtFileExt = string( tFileName) + string( tFileExt);
		m_mainWidget->setTabText( 1, m_currPbrtFileExt.c_str());
		m_mainWidget->setCurrentIndex( TAB_INDEX_PBRT);
	}

	// Pbrt文件被修改
	void LMMainWindow::PbrtChanged()
	{
		if( !m_currPbrtFileExt.empty())
		{
			// 更改标题
			string title = m_currPbrtFileExt + string("*");
			m_mainWidget->setTabText( 1, title.c_str());
		}
	}

	// 附加消息
	void LMMainWindow::AppendOutput( const char* msg)
	{
		m_outPut->append( msg);
	}

	// 设置进度
	void LMMainWindow::SetProcess( float process)
	{
		int curValue = m_progressBar->value();
		curValue     = std::max<int>( curValue, static_cast<int>(process*100.f));

		m_progressBar->setValue( curValue);
	}

	// 设置皮肤样式
	void LMMainWindow::SetSkin()
	{
		// 获取皮肤路径
		//QString qssPath;
		//QAction *action = qobject_cast<QAction *>(sender());
		//if( action->text() == QString("sutra"))
		//{
		//	qssPath = ":/Qss/qss/sutra.qss";
		//}

		//// 设置皮肤
		//QFile qssFile(qssPath);
		//qssFile.open( QFile::ReadOnly);
		//QString styleSheet = QLatin1String( qssFile.readAll());

		//qApp->setStyleSheet( styleSheet);
	}

	// drag enter event
	void LMMainWindow::dragEnterEvent( QDragEnterEvent* event)
	{
		if( event->mimeData()->hasFormat("text/uri-list"))
			event->acceptProposedAction();
	}

	// drop event
	void LMMainWindow::dropEvent( QDropEvent* event)
	{
		QList<QUrl> urls = event->mimeData()->urls();
		if( urls.isEmpty())
			return;

		QString fileName = urls.first().toLocalFile();
		if( !fileName.isEmpty())
		{
			Open( fileName.toStdString().c_str());
		}
	}
}