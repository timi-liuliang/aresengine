#include "RenderWidget.h"
#include "ModelEdit/MOLogic.h"
#include <Core/AresPlatform.h>
#include <boost/algorithm/string.hpp>
//#include "MOMainWindow.h"

using namespace std;

namespace QT_UI
{
	// 构造函数
	MORenderWidget::MORenderWidget( QWidget* parent)
		: QWidget( parent)
	{
		setMouseTracking(true);
		setAcceptDrops( true);
	}

	// 析构函数
	MORenderWidget::~MORenderWidget()
	{

	}

	// 鼠标移动事件
	void MORenderWidget::mouseMoveEvent(QMouseEvent *)
	{
		//Logic_Camera->OnMouseMove();
	}

	// 鼠标进入事件
	void MORenderWidget::dragEnterEvent(QDragEnterEvent * event)
	{
		if( event->mimeData()->hasFormat( "text/uri-list"))
			event->acceptProposedAction();
	}

	// 鼠标释放事件
	void MORenderWidget::dropEvent( QDropEvent* event)
	{
		/*if( !event->mimeData()->hasFormat( "text/uri-list"))
			return;

		vector<string> result;
		string         fileNameBytes = QString( event->mimeData()->data( "text/uri-list")).toLocal8Bit();

		boost::algorithm::split( result, fileNameBytes, is_any_of("\n"));
		foreach_( string path, result)
		{
			// tr("*.m1 *.GMe")
			if( !path.empty())
			{
				string tpath = path.c_str() + 8; tpath.pop_back();
				boost::filesystem::path cPath( tpath); 

				if( boost::filesystem::exists( cPath) && ( (cPath.extension()==".m1" )|| (cPath.extension()==".GMe")))
					MOUI_MainWindow->OpenEntity( tpath.c_str());
			}
		}*/
	}
}