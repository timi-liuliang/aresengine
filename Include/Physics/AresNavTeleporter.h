#pragma once

#include <vector>
#include <Core/AresFileIO.hpp>
#include "AresNavMap.h"

using namespace std;

namespace Ares
{
	#define MAX_REGION_TELEPORTERS 30
	//---------------------------------------------------------
	// 区域联通网, 2012-02-02 帝林
	// 实现跨地图寻路
	// 1.传送点为单向传送,暂无法快速预判寻路失败的情况
	// 2.传送门ID唯一
	// 3.同时实现xml与二进制格式
	// 4.以下区域具有跨地图含义(应由区域ID与地图ID组合而成)
	// 5.相同区域不进行传送门寻路(详参 魔兽争霸),因无法高效预判
	//---------------------------------------------------------
	class CAStar;
	class CNavTeleporter : public INavMap, public IFileIO
	{
		friend class CAStar;
	public:
		// 传送点
		struct STeleporter
		{
			UINT		m_ID;			// 传送点ID
			UINT		m_regionID;		// 传送点所属区域ID
			Vector3     m_pos;			// 传送点位置
			UINT		m_aimRegionID;	// 目标区域ID
			Vector3		m_aimPos;		// 目的地位置	
		};

		// 区域数据
		struct SRegion
		{
			UINT	m_ID;									// 区域ID
			UINT	m_numLeaveTeleporters;					// 离开此区域的传送点数量
			UINT	m_numEnterTeleporters;					// 进入此区域的传送点数量
			INT		m_teleporterIDs[MAX_REGION_TELEPORTERS];// 区域内的传送点ID
		};

		// Header definition
		struct SHeader
		{
			UINT				m_version;				// 版本
			UINT				m_numRegions;			// 区域数量
			UINT				m_numTeleporters;		// 传送门数量
			SRegion*			m_regions;				// 区域
			STeleporter*		m_teleporters;			// 传送门
		};	

		typedef vector<const STeleporter*>	PathResult; 

	public:
		CNavTeleporter();
		~CNavTeleporter();

	public:
		// 加载xml格式(不支持连结加载)
		virtual BOOL LoadXml( const char* fileName);

		// 查找路径 
		size_t FindPath( PathResult& oPath, UINT startRegionID, const Vector3& startPos, UINT endRegionID, const Vector3& endPos);

	public:
		// 根据传送门ID获取传送门信息
		const STeleporter* GetTeleporter( UINT id);

		// 根据区域ID获取区域信息
		const SRegion* GetRegion( UINT id);

	protected:
		// 获取可通行周围接点
		virtual void GetPassAbleNeighbor( int* childID, int& numChild,int fatherID);

		// 根据ID计算两多边形间h
		virtual float CalcH( const Vector3& vStart, const Vector3& vEnd, int idFrom, int idTo=-1);

		// 根据ID计算两多边形间g
		virtual float CalcG( const Vector3& vStart, int idFrom, int idTo, int idFromFrom=-1);

	public:
		CAStar*		m_aStar;		// A*算法管理器
		SHeader*	m_header;		// 导航用数据包
	};
}