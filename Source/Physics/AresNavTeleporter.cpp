#include <Core/3rdParty/RapidXml/rapidxml_utils.hpp>
#include <boost/property_tree/detail/rapidxml.hpp>
#include "Physics/AresAStar.h"
#include "Physics/AresNavTeleporter.h"
#include <Core/AresSerialization.h>

#define NAV_VERSION			20120202

using namespace rapidxml;

namespace Ares
{
	// 构造函数
	CNavTeleporter::CNavTeleporter()
		: IFileIO(),
		  m_header( NULL)
	{
		m_aStar   = new_ CAStar;
		m_aStar->SetNavMap( this);
	}

	// 析构函数
	CNavTeleporter::~CNavTeleporter()
	{
		SAFE_DELETE( m_aStar);
	}

	// 查找路径 
	size_t CNavTeleporter::FindPath( PathResult& oPath, UINT startRegionID, const Vector3& startPos, UINT endRegionID, const Vector3& endPos)
	{
		oPath.clear();

		// 查找区域内的传送门
		UINT startTeleporterID;
		UINT endTeleporterID;
		static vector<PolyRef> tPath;
		const SRegion* pStartRegion = GetRegion( startRegionID);
		const SRegion* pEndRegion   = GetRegion( endRegionID);

		// 区域不存在
		if( !pStartRegion || !pEndRegion)
			return 0;

		for ( UINT i=0; i<pStartRegion->m_numLeaveTeleporters; i++)
		{
			for ( UINT j=0; j<pEndRegion->m_numEnterTeleporters; j++)
			{
				startTeleporterID = pStartRegion->m_teleporterIDs[i];
				endTeleporterID   = pEndRegion->m_teleporterIDs[j+pEndRegion->m_numLeaveTeleporters];

				size_t tNumTeleporters = m_aStar->FindPath( startTeleporterID, endTeleporterID, startPos, endPos, tPath);
				if( tNumTeleporters)
				{
					// 返回传送点
					for ( size_t i=0; i<tPath.size(); i++)
					{
						const STeleporter* tTeleporter = GetTeleporter( tPath[i]);

						oPath.push_back( tTeleporter);
					}

					return oPath.size();
				}
			}
		}		

		return 0;
	}

	// 到达下一个区域所使用的传送门
	const CNavTeleporter::STeleporter* CNavTeleporter::GetTeleporter(  UINT id)
	{
		for ( UINT i=0; i<m_header->m_numTeleporters; i++)
		{
			const STeleporter* pTeleporter = &m_header->m_teleporters[i];
			if( pTeleporter->m_ID == id)
				return pTeleporter;
		}

		return NULL;
	}

	// 获取可通行周围接点
	void CNavTeleporter::GetPassAbleNeighbor( int* childID, int& numChild,int fatherID)
	{
		numChild = 0;

		const STeleporter* pTeleporter1 = GetTeleporter( fatherID);
		if( !pTeleporter1)
			return;

		// 1.根据父传送门ID获取传送门区域
		UINT tFartherRegionID = pTeleporter1->m_regionID;

		// 2.根据父区域ID获取相通的传送门
		const SRegion* pRegion = GetRegion( tFartherRegionID);
		if( !pRegion)
			return;

		for ( UINT i=0; i<pRegion->m_numLeaveTeleporters; i++)
		{
			const STeleporter* pTeleporter2 = GetTeleporter( pRegion->m_teleporterIDs[i]);
			if( pTeleporter2->m_ID != fatherID)
			{
				childID[numChild] = pTeleporter2->m_ID;
				numChild++;
			}
		}
	}

	// 根据ID计算两多边形间h
	float CNavTeleporter::CalcH(  const Vector3& vStart, const Vector3& vEnd, int idFrom, int idTo/*=-1*/)
	{
		// 无合适的方法进行计算,返回固定值
		return 1e2f;
	}

	// 根据ID计算两多边形间g
	float CNavTeleporter::CalcG( const Vector3& vStart, int idFrom, int idTo, int idFromFrom/*=-1*/)
	{
		Vector3 p0, p1;

		if( idFromFrom == -1)
		{
			p0 = vStart;
		}
		else
		{
			const STeleporter* pTeleporter = GetTeleporter( idFrom);
			if( !pTeleporter)
				return FLT_BIG;
			else
				p0 = pTeleporter->m_aimPos;
		}

		{
			const STeleporter* pTeleporter = GetTeleporter( idTo);
			if( !pTeleporter)
				return FLT_BIG;
			else
				p1 = pTeleporter->m_pos;
		}

		return ( p1-p0).Length();
	}

	// 根据区域ID获取区域信息
	const CNavTeleporter::SRegion* CNavTeleporter::GetRegion( UINT id)
	{
		assert( m_header);
		
		for ( UINT i=0; i<m_header->m_numRegions; i++)
		{
			if( id == m_header->m_regions[i].m_ID)
				return &m_header->m_regions[i];
		}

		return NULL;
	}

	// 加载xml格式
	BOOL CNavTeleporter::LoadXml( const char* fileName)
	{
        using namespace rapidxml;
        using namespace boost::property_tree::detail::rapidxml;
        
		// 不支持连续加载
		if( m_header)
			return FALSE;

		// 使用Rapidxml
		if( !fileName)
			return FALSE;

		file<> fdoc( fileName);
		xml_document<> doc;
		doc.parse<0>( fdoc.data());

		// 1.主结点, 区域结点, 传送门结点
		xml_node<>* pRootNode		= doc.first_node();
		xml_node<>* pRegionsNode	= pRootNode->first_node( "regions");
		xml_node<>* pTeleportersNode= pRootNode->first_node( "teleporters");

		// 2.区域数量,传送门数量
		INT tRegionNum     = atoi(pRegionsNode->first_attribute("num")->value());
		INT tTeleporterNum = atoi(pTeleportersNode->first_attribute("num")->value());

		// 1.计算总数据大小
		INT tHeadSize		= sizeof(SHeader);
		INT tRegionSize		= sizeof(SRegion) * tRegionNum;
		INT tTeleporterSize = sizeof(STeleporter) * tTeleporterNum;

		m_dataSize = tHeadSize + tRegionSize + tTeleporterSize;
		m_data = new_ BYTE[m_dataSize];

		// 指针数据游标
		BYTE* refPtr = m_data;

		m_header				= (SHeader*)refPtr;	refPtr += tHeadSize;
		m_header->m_regions		= (SRegion*)refPtr;	refPtr += tRegionSize;
		m_header->m_teleporters = (STeleporter*)refPtr;

		m_header->m_version		   = 1;
		m_header->m_numRegions     = tRegionNum;
		m_header->m_numTeleporters = tTeleporterNum;

		// 2.赋值区域数据
		UINT i=0;
		for( xml_node<>* node=pRegionsNode->first_node("region"); node && i<m_header->m_numRegions; node=node->next_sibling("region"))
		{
			m_header->m_regions[i].m_ID	= atoi(node->first_attribute("id")->value());

			// 拥有的传送门
			vector<INT> tLevelTeleporter;
			vector<INT> tEnterTeleporter;
			Serialization::UnPackString( tLevelTeleporter, node->first_attribute("leave_teleporters")->value(), ';');
			Serialization::UnPackString( tEnterTeleporter, node->first_attribute("enter_teleporters")->value(), ';');

			m_header->m_regions[i].m_numLeaveTeleporters = tLevelTeleporter.size();
			for ( UINT j=0; j<tLevelTeleporter.size(); j++)
			{
				m_header->m_regions[i].m_teleporterIDs[j] = tLevelTeleporter[j];
			}

			m_header->m_regions[i].m_numEnterTeleporters = tEnterTeleporter.size();
			for ( UINT j=0; j<tEnterTeleporter.size(); j++)
			{
				m_header->m_regions[i].m_teleporterIDs[j+tLevelTeleporter.size()] = tEnterTeleporter[j];
			}

			i++;
		}

		// 赋值传送点数据
		i=0;
		for( xml_node<>* node=pTeleportersNode->first_node("teleporter"); node && i<m_header->m_numTeleporters; node=node->next_sibling("teleporter"))
		{
			m_header->m_teleporters[i].m_ID			 = atoi(node->first_attribute("id")->value());
			m_header->m_teleporters[i].m_regionID    = atoi(node->first_attribute("region_id")->value());
			m_header->m_teleporters[i].m_aimRegionID = atoi(node->first_attribute("aim_region_id")->value());

			// 传送点位置,传送点目标位置
			vector<FLOAT> tPosResult;
			Serialization::UnPackString( tPosResult, node->first_attribute("pos")->value(), ',');
			Vector3 tPos( tPosResult[0], tPosResult[1], tPosResult[2]);
			m_header->m_teleporters[i].m_pos = tPos;

			vector<FLOAT> tAimPosResult;
			Serialization::UnPackString( tAimPosResult, node->first_attribute("aim_pos")->value(), ',');
			Vector3 tAimPos( tAimPosResult[0], tAimPosResult[1], tAimPosResult[2]);
			m_header->m_teleporters[i].m_aimPos = tAimPos;

			i++;
		}

		return TRUE;
	}
}