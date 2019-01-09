#ifndef TERRAIN_EDIT_H
#define TERRAIN_EDIT_H

#include <QtGui/QDockWidget>
#include <Engine/RenderSystem/AresTerrain.h>
#include "ui_TerrainEdit.h"
#include "QRadioImage.h"

using namespace Ares;

namespace QT_UI
{
	#define  MAX_TEXTURE   10

	//----------------------------------------
	// 地形编辑Frame    2011-01-11
	//----------------------------------------
	class TerrainEdit : public QWidget, public Ui_TerrainEdit
	{
		Q_OBJECT

	public:
		// 构造函数
		TerrainEdit( QWidget* parent = 0);

		// 设置当前编辑地形
		void SetCurrTerrain( Terrain* pTerrain);

		// 更新地形纹理显示
		void UpdateTerrainTexture();

	private slots:
		// 编辑地形尺寸更改
		void  UpdateTerrainEditSize( int size);

		// 编辑地形类型
		void UpdateTerrainHandleType();

		// 编辑地形纹理
		void UpdateTexture();

		// 更改地形纹理 
		void UpdateTexture( QString str);

		// 导入地形纹理混合度
		void ImportTexBlend( QString str);

		// 更改uvScale
		void UpdateUVScale(double uvScale);

		// 选择导入高度图
		void SelectImportHeightMap();

		// 导入高度图
		void ImportTerrainHeight();

	public:
		QRadioImage*    m_pShowImage;
		QRadioImage*    m_imageRadios[MAX_TEXTURE];
		Terrain*		m_terrain;
	};
}

#endif