#include <QApplication>
#include <Windows.h>
#include <QTextCodec>
#include "MainWindow.h"

using namespace Ares;

//-----------------------------------
// 程序入口( 标准main函数)
//-----------------------------------
int main( int argc, char* argv[])
{
	float* a = new float;

	QApplication app( argc, argv);

	// 设置编码方式
	//QTextCodec *codec = QTextCodec::codecForName("gb2312");  

	//QTextCodec::setCodecForLocale(codec);  
	//QTextCodec::setCodecForCStrings(codec);  
	//QTextCodec::setCodecForTr(codec);  

	MainWindow mainWindow;

	mainWindow.show();

	app.exec();
}