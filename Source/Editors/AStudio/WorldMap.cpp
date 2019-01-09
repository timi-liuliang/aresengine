#include "WorldMap.h"
#include <QtGui/QFileDialog>
#include "SCLogic.h"
#include <boost/filesystem.hpp>
#include "SCMainWindow.h"

using namespace Ares;
using namespace boost::filesystem;

namespace QT_UI
{
	// 构造函数
	CWorldMap::CWorldMap( QWidget* parent)
		: QMainWindow( parent)
	{
		setupUi( this);

		m_areaGroupBox->setVisible( false);

		connect( m_areaAdd,     SIGNAL(clicked()), this, SLOT( AddArea()));
		connect( m_areaConfirm, SIGNAL(clicked()), this, SLOT( EditArea()));
		connect( m_areaList,    SIGNAL(activated(QString)), this, SLOT( EditAreaChanged( QString)));
		connect( m_areaName,     SIGNAL(textChanged ( const QString&)), this, SLOT(UpdateEnabled()));

		m_graphicsView->setScene( &m_graphicsScene);

		// 时间事件
		//connect( &m_timer, SIGNAL( timeout()), this, SLOT( UpdateGPWindow()));
		//m_timer.start( 20);

		m_gpWindow = NULL;
	}

	// 析构函数
	CWorldMap::~CWorldMap()
	{
	}

	// 更新GPWindow显示
	void CWorldMap::UpdateGPWindow()
	{
		Vector3 tPos;
		float tRadius = static_cast<float>(m_radius->value());
		if( SCLogic_Move && SCLogic_Move->GetMoveType() == CMoveLogic::EM_FOLLOW)
		{
			// 游戏模式正常窗口
			tPos = SCLogic_Move->GetCenterPos();

			SCLogic_Map->SetNavWindow( tPos.x, tPos.y, tRadius);
		}
		else
		{
			// 编辑模式看整个场景
			Scene* scene = SCLogic_Scene;
			if( scene)
			{
				//Scene::SSize tSize = scene->GetSize();

				// 求半径, 中心点
				//tRadius = (std::max)( tSize.m_height, tSize.m_width) / 2.f;
				//tPos.x   = tSize.m_posX + tSize.m_width / 2.f;
				//tPos.y   = tSize.m_posY + tSize.m_height/ 2.f; 

				//SCLogic_Map->SetNavWindow( tPos.x,  tPos.y, tRadius);
			}
		}

		// 更新界面
		float tX = (tPos.x - tRadius)* 0.25f;
		float tY = (tPos.y - tRadius)* 0.25f;
		float tScaleRadius = tRadius * 0.5f;

		if( !m_gpWindow)
		{
			QPen   pen( QColor( 0, 0, 128, 80));
			QBrush brush( QColor( 0, 128, 128, 80));
			pen.setWidth( 2);

			m_gpWindow = m_graphicsScene.addRect( tX, tY, tScaleRadius, tScaleRadius, pen, brush);
			m_graphicsScene.setFocusItem( m_gpWindow);
		}
		else
		{
			m_gpWindow->setRect( tX, tY, tScaleRadius, tScaleRadius);
		}
	}

	// 新建
	void CWorldMap::NewScene( const char* name, const char* location)
	{
		// 目录已存在,提醒并清空
		path locationPath( location);
		if( exists( locationPath))
		{
			if( QMessageBox::Yes ==QMessageBox::warning( this, "Warning", "目录已存在,确定将清空目录下所有资源", QMessageBox::Yes | QMessageBox::Cancel))
				remove_all( locationPath);
			else
				return;
		}

		// 创建路径
		create_directory( locationPath);

		// 光照图路径
		path lightmapPath( string( location) + string("lightmap/"));
		create_directory( lightmapPath);

		// 界面更新
		setWindowTitle( name);

		// 逻辑处理
		SCLogic_Map->CreateScene( name, location);

		m_graphicsScene.clear();
		m_gpWindow = NULL;
	}

	// 打开地图
	void CWorldMap::OpenMap()
	{
		QString fileName = QFileDialog::getOpenFileName( this, tr("Open Map"), "", tr("(*.map)"));
		if( !fileName.isEmpty())
		{
			string tFileName = fileName.toStdString();

			SCLogic_Map->OpenScene( tFileName.c_str());
			Scene* tMap = SCLogic_Scene;
			if( tMap)
			{
				m_areaList->clear();

				setWindowTitle(tMap->GetName());

				m_areaList->addItem( tMap->GetName());
	
				/*if( tMap->GetAreaNum())
				{
					size_t tAreaIdx = 0;
					EditAreaChanged( tMap->GetArea( tAreaIdx)->GetName());
				}*/

				UpdateMapShow();
			}
		}
	}

	// 更新map显示
	void CWorldMap::UpdateMapShow()
	{
		m_graphicsScene.clear();

		m_gpWindow = NULL;

		Scene* scene = SCLogic_Scene;
		if( scene)
		{
			//const Scene::SSize& size = scene->GetSize();

			//QRect tRect( size.m_posX/4, size.m_posY/4, size.m_width/4, size.m_height/4);
			//QGraphicsRectItem  *rect = m_graphicsScene.addRect( tRect);
			//if( rect)
			//{

			//}
		}
	}

	// 更新添加区域按钮可用性
	void CWorldMap::UpdateEnabled()
	{
		string tAreaName = m_areaName->text().toStdString();
		if( tAreaName.empty() || !SCLogic_Scene)
			m_areaAdd->setEnabled( false);
		else
			m_areaAdd->setEnabled( true);
	}

	// 保存地图
	void CWorldMap::SaveMap()
	{
		// 此处改到另存为
		//QString tName    = m_mapName->text();
		//QString fileName = QFileDialog::getSaveFileName( this, tr("Save Map"), tName, tr("(*.map)"));
		//if( !fileName.isEmpty())
		//{
		//	string tFileName = fileName.toStdString();
			SCLogic_Map->SaveMap( NULL);
		//}
	}

	// 添加区域
	void CWorldMap::AddArea()
	{/*
		string tAreaName = m_areaName->text().toStdString();

		// 逻辑添加
		g_pLogic->m_mapLogic.SetSceneSize( tAreaName.c_str(), m_locationX->value(), m_locationY->value(), m_width->value(), m_height->value());

		// 更改界面
		m_areaList->addItem( tAreaName.c_str());

		// 世界地图显示
		UpdateMapShow();

		// 隐藏区域添加group
		m_areaGroupBox->setVisible( false);*/
	}

	// 编辑区域
	void CWorldMap::EditArea()
	{

	}

	// 设置编辑区域
	void CWorldMap::EditAreaChanged(QString areaName)
	{
		/*g_pLogic->m_mapLogic.SetActiveArea( areaName.toStdString().c_str());

		IArea* pArea = g_pLogic->m_mapLogic.GetActiveArea();
		if( pArea)
		{
			emit EditAreaChanged( pArea);

			return;
		}

		emit EditAreaChanged( NULL);*/
	}
}