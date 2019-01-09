#pragma once

#include <boost/function.hpp>
#include <Core/AresCore.h>
#include <Physics/QuadTree.hpp>
#include <Physics/Shapes.h>
#include "Physics/WaterZoneMgr.h"
#include "AresRecast.h"
	
using namespace std;

namespace Ares
{
	// NavMapFilter 2013-5-21 帝林
	struct NavMeshFilter
	{	
		enum Constant
		{
			Max_AreaType = 32,
		};

	public:
		NavMeshFilter();

		// 设置消耗
		void SetCost( BYTE areaType, float cost);

		// 获取消耗
		float GetCost( BYTE areaType);

		// 获取标记
		UINT GetFlags() const { return m_includeFlags; }

		// 设置区域是否可通行
		void SetAreaPathable( BYTE areaType, bool value);

		// 判断区域是否可通行
		bool IsAreaPathable( BYTE areaType) const;

	private:
		float			m_areaCost[Max_AreaType];	// cost per area type, (Used by default implementation)
		UINT			m_includeFlags;				// flags for path that can be visited
	};

	typedef UINT PolyRef;

	#define ARES_STATIC_VERTS_PER_POLYGON 6
	//--------------------------------------------
	// 静态导航网格, 2011-02-18 帝林
	// 主要参考Recast by Mikko Mononen
	// http://code.google.com/p/recastnavigation/
	//--------------------------------------------
	class NavMeshDetour;
	class CStaticNavMesh
	{
	public:
		// 路径点
		struct SPathNode
		{
			Vector3     m_vPos;	// 当前点
		};
		typedef vector<SPathNode> PathResult; 

		// 多边形通过方式
		enum EPolyFlag
		{
			EM_WALK = 0,		// 行走
			EM_CLIMB,			// 爬
		};

		// 顶点标记
		enum VertexFlag
		{
			VF_APEX = 1,		// 端点
		};

		// structure holding the navigation polygon data
		struct SStaticPoly
		{
			UINT			m_indices [ARES_STATIC_VERTS_PER_POLYGON];		// Indices to vertices of the poly( 顶点索引)
			PolyRef			m_neighbor[ARES_STATIC_VERTS_PER_POLYGON];		// Refs to neighbors of the poly( 领接多边形)
			unsigned char	m_iNumVerts;									// 使用的顶点数
			EPolyFlag	    m_flag;											// Flags (not used)

			// 构造函数
			SStaticPoly()
			{
				m_iNumVerts = 0;
				m_flag      = EM_WALK;
			}
		};

		struct SStaticPolyDetail
		{
			unsigned int   m_iVertBase;		// Offset to detail vertex array
			unsigned int   m_iTriBase;		// Offset to triangle array
			unsigned short m_iNumVerts;		// Number of vertices in the detail mesh
			unsigned short m_iNumTris;		// Number of triangles
		};

		// 导航数据
		struct SStaticNavMeshHeader
		{
			unsigned int		m_iVersion;				// 版本
			unsigned int		m_iNumPolys;			// 多边形数
			unsigned int		m_iNumVerts;			// 顶点数
			UINT				m_iNumApexVerts;		// 边缘顶点数	
			int					m_iNVP;					// 每个多边形的最大顶点数
			float				m_fCellSize;			// cell size
			Vector3				m_vMin;				
			Vector3				m_vMax;					// 包围盒

			int					m_iDNumVerts;			// 辅助加载
			int					m_iDNumTris;			// Detail num triangles

			UINT*				m_pPolyRoadIDs;			// 路编号
			unsigned char*		m_areaIDs;				// 区域类型ID
			SStaticPoly*		m_pPolys;				// 多边形集
			Vector3*			m_pVerts;				// 顶点数据
			BYTE*				m_pVertFlags;			// 点标记
			SStaticPolyDetail*	m_pDetailPolys;			// 三角化
			Vector3*			m_pDetailVerts;			// 顶点

			unsigned char*				m_pDetailTris;		// 三角形数据
			quad_tree_member<PolyRef>*	m_pQuadTreeMember;	// 四叉树成员
		};	

	public:
		CStaticNavMesh();
		~CStaticNavMesh();

		// 加载
		bool Load( const char* fileName);

		// 查找路径 
		size_t FindPath( vector<SPathNode>&	pathResult, NavMeshFilter& filter, const Vector3& startPos, const Vector3& endPos,Vector3 startExtendBox, Vector3 endExtendBox, float inwardOffset=0.1f, float walkRagne=-1.f);

		// 查找点到通路最近的点
		bool CloestPointToRold( int roldIDs, const Vector3& center, Vector3& closetPoint) { return false; }

		// 判断某点是否在寻路范围内
		bool IsPtOnNavZone( const Vector3& pos, const Vector3& extendBox);

		// 设置顶点上方向索引( 0-2)
		void SetUpDirIdx( int idx=2);

		// 重置
		void Reset();

		// 编辑器使用↓
	public:
		// 查找具体路径
		int FindStraightPath( const Vector3& vStart, const Vector3& vEnd, const vector<PolyRef>& path, vector<SPathNode>& straightPath, float inwardOffset=0.1f);

		// Finds the nearest navigation polygon around the center location
		bool  FindNearstPoly( PolyRef& polyRef, const Vector3& center, const Vector3& extends);

		// 查找最近相联的两个Poly
		bool FindNearstTwoPoly( PolyRef& polyStart, PolyRef& polyEnd, const Vector3& startCenter, const Vector3& endCenter, const Vector3& extends, NavMeshFilter& filter);

		// 查询多边形
		int  QueryPolygons( const Vector3& center, const Vector3& extents, vector<PolyRef>& polys);

		// 查找点到多边形最近的点
		bool CloestPointToPoly( int ref, const Vector3& center, Vector3& closetPoint);

		// 求某点在垂直方向上与某多边形的交点
		bool CloestPointToPolyInVerticalDirection( int ref, const Vector3& center, Vector3& closetPoint, float minDist, float maxDist);

	public:
		// 保存
		bool Save( const char* fileName);

		// 根据索引获得多边形
		const SStaticPoly* GetPolyByRef( PolyRef iRef) const;

		// 获取多边形区域ID
		BYTE GetPolyAreaIDByRef( PolyRef ifRef) const;

		// 根据索引获得详细多边形
		const SStaticPolyDetail* GetPolyDetailByRef( PolyRef iRef);

		// Returns pointer to specifed vertex
		const unsigned char* GetDetailTri( int i);

		// 获取顶点
		const Vector3& GetVertex( UINT i) const;

		// 获取顶点
		const Vector3& GetDetailVertex( int i);

		// 标记
		bool IsApexVert( PolyRef idx) const;

	public:
		// 获取可通行周围接点
		virtual void GetPassAbleNeighbor( const NavMeshFilter& filter, int* childID, int& numChild,int fatherID, float walkRange=-1.f, const Vector3& startPos=Vector3::Zero) const;

		// 根据ID计算两多边形间h
		virtual float CalcH( NavMeshFilter& filter, const Vector3& vStart, const Vector3& vEnd, int idFrom, int idTo=-1);

		// 根据ID计算两多边形间g
		virtual float CalcG( NavMeshFilter& filter, const Vector3& vStart, int idFrom, int idTo, int idFromFrom=-1);

	public:
		// 返回两多边形的共用点
		bool GetPortalPoints( int idFrom, int idTo, Vector3& vLeft, Vector3& vRight, float inwardOffset=0.1f) const;

		// 返回两边形边缘线的中心点( returns edge middle point between two polygons)
		bool  GetEdgeMiddlePoint( int idFrom, int idTo, Vector3& vMiddle) const;

		// 获取poly包围盒
		void GetPolyBoundingBox( PolyRef iRef, quad_tree_rect& out);

		// 获取路标号
		const vector<UINT>& GetPolyRoadIdsByNavMeshFilter( const NavMeshFilter& filter);

		// 清除数据
		void Clear();

		// 获取Poly中心点
		//void GetCenterPoint( float* out, PolyRef iRef);

		// 根据顶点获取多边形邻居
		void GetPolyNeighborByPoint( PolyRef polyID, UINT pointIdx, PolyRef& leftPoly, PolyRef& rightPoly);

	public:
		// 向内修正路径
		void AdjustPathPointInward( PathResult& oPath, float inwardOffset);

		// 计算角平分线(钝角方向)
		bool CalcAngularBisector( Vector3& result, const Vector3& edge0, const Vector3& edge1);

		// 计算沿角平分线方向偏移长度
		float CalcAngularBisectorInwardOffset( const Vector3& edgeDir, const Vector3& abDir, float inwardOffset);

	public:
		NavMeshDetour*			m_detour;
		SStaticNavMeshHeader*	m_pHeader;		// 导航用数据包
		BYTE*					m_pData;		// 数据
		unsigned int			m_iDataSize;	// 数据大小

		QuadTree<PolyRef>*		m_quadTree;		// 四叉树
		int						m_upDirIdx;		// 上方向索引

		map<UINT, vector<UINT>> m_polyRoldIDs;	// 路标号
	};
}