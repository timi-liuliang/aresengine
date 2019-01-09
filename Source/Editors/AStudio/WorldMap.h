#pragma once

#include "ui_WorldMap.h"
#include <QtGui/QtGui>
#include <QtGui/QMainWindow>
#include <QtGui/QGraphicsScene>
#include <QGraphicsEllipseItem>
#include "MapLogic.h"
#include <Engine/RenderSystem/AresTerrain.h>

using namespace Ares;

namespace QT_UI
{
	//--------------------------------
	// 世界地图 2011-08-16  帝林
	//--------------------------------
	class CWorldMap : public QMainWindow, public Ui_WorldMap
	{
		Q_OBJECT

	public:
		// 构造函数
		CWorldMap( QWidget* parent = 0);

		// 析构函数
		~CWorldMap();

	public: signals:
		// 地形改变
		//void EditAreaChanged( IArea* pAres);

	public slots:
		// 新建
		void NewScene( const char* name, const char* location);

		// 打开地图
		void OpenMap();

		// 保存地图
		void SaveMap();

		// 添加区域
		void AddArea();

		// 编辑区域
		void EditArea();

		// 设置编辑区域
		void EditAreaChanged( QString areaName);

	private:
		// 更新map显示
		void UpdateMapShow();

	public slots: 
		// 更新GPWindow显示
		void UpdateGPWindow();

		// 更新添加区域按钮可用性
		void UpdateEnabled();

	private:
		//QTimer					m_timer;		// 定时器
		QGraphicsScene			m_graphicsScene;// 地图场景	

		QGraphicsRectItem*		m_gpWindow;		// gp窗口
	};
}