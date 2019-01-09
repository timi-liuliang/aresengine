#include <QApplication>
#include <QtGui/QSplashScreen>
#include "MainWindow.h"
#include "SCMainWindow.h"
#include "ModelEdit/MOMainWindow.h"
#include "CMDLine.h"
#include <Windows.h>
#include <QTextCodec>
#include <Physics/ConvexHull2D.h>
#include <boost/regex.hpp>

#ifdef ARES_PLATFORM_WINDOWS
//#include <vld.h>
#endif

using namespace Ares;
using namespace QT_UI;

extern "C"
{
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

//-----------------------------------
// 程序入口( 标准main函数)
//-----------------------------------
int main( int argc, char* argv[])
{
	QApplication app( argc, argv);

	// 设置编码方式
	QTextCodec *codec = QTextCodec::codecForName("GB2312");  

	QTextCodec::setCodecForLocale(codec);  
	QTextCodec::setCodecForCStrings(codec);  
	QTextCodec::setCodecForTr(codec);  

	// QSplashScreen
	QSplashScreen* pSplash = new QSplashScreen;
	pSplash->setPixmap( QPixmap("Icon/Logo.png"));
	pSplash->show();

	//pSplash->showMessage( QObject::tr("战神开发工具包(32-位, DX11) \n Copyright @ 2009-2012 帝林"),  Qt::AlignLeft | Qt::AlignBottom, Qt::white);

	if( !CMDLine::Parser( argc, argv))
	{
		// 主窗口初始化
		MainWindow* mainWindow = new_ MainWindow();
		mainWindow->showMaximized();
		mainWindow->BeginRender();

		pSplash->finish( mainWindow);
		SAFE_DELETE( pSplash);

		mainWindow->SetEditorType( "Scene");

		app.exec();

		SAFE_DELETE( mainWindow);
	}
	else
	{
		QWidget* widget = new QWidget;
		pSplash->finish( widget);

		SAFE_DELETE( pSplash);
		SAFE_DELETE( widget);
	}
}