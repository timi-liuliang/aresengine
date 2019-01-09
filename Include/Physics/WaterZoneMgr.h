/********************************************************************
	created:	2011/06/15
	created:	15:6:2011   10:11
	filename: 	f:\work\MoveManager\WaterZoneMgr.h
	file path:	f:\work\MoveManager
	file base:	WaterZoneMgr
	file ext:	h
	author:		王后建
	
	purpose:	当前世界地图的水区域管理类，用来判断处理游泳和其他
				控制器的切换
*********************************************************************/
#pragma once

#include <Core/AresMath.h>
#include <Physics/Rect3.h>

namespace Ares
{
	/******************************************************************************
	 SWaterInfoFlag
	 一个点的信息
	******************************************************************************/
	struct SWaterInfoFlag
	{
		float m_fFlag;
		SWaterInfoFlag(void)
		{
			m_fFlag = FLT_MAX;
		}
	};

	class WaterZoneMgr : public Shape
	{
	public:
		WaterZoneMgr(void);
		~WaterZoneMgr(void);

		bool LoadWaterFlags(const char* pszFileName);
		bool PtInWater(int nX, int nZ, float& fDeep) const;

		// 判断是否在水中
		virtual bool IsPointIn( const Vector3& point) const;

		// 构建AABB包围盒
		virtual void BuildAABB( Rect3& box, const Transform& trans) const;

		// 计算运动物体惯性
		virtual Vector3 CalculateLocalInertia( float mass) { A_ASSERT(false); return Vector3::Zero; }

	private:
		int				m_nXGrids;	//地图x方向上的格子数量(点的数量要加1)
		int				m_nZGrids;  //地图z方向上的格子数量(点的数量要加1)
		int				m_nGridSize;//格子边长
		SWaterInfoFlag* m_pWaterInfoFlag;	//地图上所有的点的水信息
	};
}

