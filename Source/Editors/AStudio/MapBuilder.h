#ifndef NEW_MAP_H
#define NEW_MAP_H

#include <QtGui/QDialog>
#include "ui_MapBuilder.h"

namespace QT_UI
{
	//----------------------------------------
	// 新建场景    2011-01-11
	//----------------------------------------
	class MapBuilder : public QDialog, public Ui_MapBuilder
	{
		Q_OBJECT

	public:
		// 构造函数
		MapBuilder( QWidget* parent = 0);

		// 新建默认地图
		static void NewMapUnSaved();

	public: signals:
		// 新建地图
		void CreateMap( const char* mapName, const char* mapLocation);

	private slots:
		// 新建地图
		void NewMap();

		// 新建区域
		void AddTerrain();

		// 选择地图存放位置
		void SelectMapLocation();
	};
}

#endif