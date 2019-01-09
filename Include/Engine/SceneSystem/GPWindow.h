#pragma once

#include "GPIndex.h"
#include "GPWorld.h"

namespace Ares
{
	//-----------------------------------------
	// GPWindow 2011-08-18  帝林
	// Description:
	//	   This class is the navigator of 
	// the GP, It uses the GPIndex to navigate
	// through the GPWorld, this class also 
	// tells to the gpworld which GPTiles must
	// be loaded and which of them must be
	// unloaded
	//------------------------------------------
	class CGPWindow
	{
		typedef map<CGPIndex, CGPTile*> GPTileMap;
	public:
		// 构造函数
		CGPWindow();

		// 析构函数
		virtual ~CGPWindow();

	public:
		// 初始化
		virtual void Init( const CGPIndex& index);

		// 设置GPWorld
		void SetWorld( CGPWorld* pWorld) { m_world = pWorld; }

		// 更新位置
		virtual void UpdatePosition( float x, float y, bool checkMovement=true);

		// 更新半径
		virtual void UpdateRadius( float radius);

	public:
		// 获取GPWorld
		CGPWorld* GetWorld() { return m_world; }

		// 获取X
		float GetX() { return m_x; }

		// 获取Y
		float GetY() { return m_y; }

		// 获取半径
		float GetRadius() { return m_radius; }

	protected:
		// 更新tilesmap
		void UpdateTilesMap();

	protected:
		CGPWorld*   m_world;		// GP世界
		GPTileMap   m_tilesMap;		// 窗口中tile

		float		m_x, m_y;		// 中心点
		float		m_radius;		// 半径

		CGPIndex	m_centerIdx;	// 中心块
	};
}