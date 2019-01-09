#ifndef NEW_MAP_H
#define NEW_MAP_H

#include <QtGui/QDialog>
#include "ui_NewZone.h"

namespace QT_UI
{
	//----------------------------------------
	// 新建场景    2011-01-11
	//----------------------------------------
	class CNewZone : public QDialog, public Ui_NewZone
	{
		Q_OBJECT

	public:
		enum Type
		{
			EM_NEWMAP_OLD = 1,
			EM_NEWMAP,
		};

		// 构造函数
		CNewZone( QWidget* parent = 0);

		// 设置显示类型
		void SetType( Type type);

	public: signals:
		// 新建地图
		void CreateMap( const char* mapName, const char* mapLocation);

	private slots:
		// 新建区域
		void AddTerrain();

		// 新建地图
		void NewMap();

		// 选择地图存放位置
		void SelectMapLocation();
	};
}

#endif