#include <QtGui/QtGui>
#include "MapBuilder.h"
#include "SCMainWindow.h"
#include "MainWindow.h"
#include "SCLogic.h"
#include <string>
#include <boost/filesystem.hpp>

using namespace std;
using namespace Ares;
using namespace boost::filesystem;

namespace QT_UI
{
	// 构造函数
	MapBuilder::MapBuilder( QWidget* parent/* = 0*/)
		: QDialog( parent)
	{
		setupUi( this);

		connect( m_okNewMap,	  SIGNAL(clicked()), this, SLOT( NewMap()));
		connect( m_locationOpen,  SIGNAL(clicked()), this, SLOT( SelectMapLocation()));
	}

	// 新建区域
	void MapBuilder::AddTerrain()
	{/*
		string zoneName = m_pZoneNameEdit->text().toStdString();
		int width  = m_pZoneWidthEdit->text().toInt();
		int height = m_pZoneHeightEdit->text().toInt();

		Rect3 rect;
		rect.z0 = -125.f; rect.z1 = 500.f;
		rect.x0 = 0.f,    rect.x1 = width;
		rect.y0 = 0.f,    rect.y1 = height;

		int terrainShift = m_terrainShift->value();

		//g_pLogic->NewScene( zoneName.c_str(), rect, 7, ARES_TERRAIN_DEFAULT, rect, terrainShift);

		setVisible( false);*/
	}

	// 新建地图
	void MapBuilder::NewMap()
	{
		string mapLocation = m_mapLocation->text().toStdString();
		string mapName     = m_mapName->text().toStdString();

		if( mapLocation.empty())
			m_error->setText( "地图位置不能为空");

		if( mapName.empty())
			m_error->setText( "地图名不能为空");
		else
		{
			m_error->setText("");
				
			string tLocation = mapLocation + "/" + mapName + "/";
			mapName += ".map";

			// 目录已存在,提醒并清空
			path locationPath( tLocation);
			if( exists( locationPath))
			{
				try
				{
					if( QMessageBox::Yes ==QMessageBox::warning( this, "Warning", "目录已存在,确定将清空目录下所有资源", QMessageBox::Yes | QMessageBox::Cancel))
						remove_all( locationPath);
					else
						return;
				}
				catch( filesystem::filesystem_error&)
				{
					if( QMessageBox::Yes ==QMessageBox::warning( this, "Warning", "清空目录下资源失败,请选择其它目录创建场景", QMessageBox::Yes))
						return;
				}
			}

			// 创建路径
			create_directory( locationPath);

			// 光照图路径
			path lightmapPath( string( tLocation) + string("lightmap/"));
			create_directory( lightmapPath);

			// 界面更新
			UI_MainWindow->setWindowTitle( mapName.c_str() + QString("-Ares Studio(32位,Dx11)"));

			// 逻辑处理
			SCLogic_Map->CreateScene( mapName.c_str(), tLocation.c_str());

			setVisible( false);
		}
	}

	// 新建默认地图
	void MapBuilder::NewMapUnSaved()
	{
		string mapLocation = "data/maps";
		string mapName     = "UnSaved";

		string tLocation = mapLocation + "/" + mapName + "/";
		mapName += ".map";

		// 目录已存在,提醒并清空
		path locationPath( tLocation);
		if( exists( locationPath))
		{
			try 
			{ 
				remove_all( locationPath); 
			}
			catch( filesystem::filesystem_error&) 
			{}
		}

		try
		{
			// 创建路径
			create_directory( locationPath);

			// 光照图路径
			path lightmapPath( string( tLocation) + string("lightmap/"));
			create_directory( lightmapPath);
		}
		catch( filesystem::filesystem_error&) 
		{

		}

		// 界面更新
		UI_MainWindow->setWindowTitle( mapName.c_str() + QString("-Ares Studio(32位,Dx11)"));

		// 逻辑处理
		SCLogic_Map->CreateScene( mapName.c_str(), tLocation.c_str());
	}

	// 选择地图存放位置
	void MapBuilder::SelectMapLocation()
	{
		QString qFileDir = QFileDialog::getExistingDirectory( this, tr("choose a directory"), "data\\map\\", QFileDialog::ShowDirsOnly);

		QDir dir( QDir::currentPath());
		qFileDir = dir.relativeFilePath( qFileDir);

		if( !qFileDir.isEmpty())
		{
			m_mapLocation->setText( qFileDir);
		}
	}
}