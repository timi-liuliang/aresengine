#pragma once

// 注：此头文件可不对外发布

#include "AresRecast.h"
#include "AresStaticNavMesh.h"
#include "Physics/Shapes.h"

namespace Ares
{
	//---------------------------------
	// 操作管理器(按区块处理) 2011-03-24 帝林
	//---------------------------------
	struct SRcChunkyTriMesh;
	class  CStaticMeshTiledBuilder
	{
	public:
		// 参数集
		struct SBuildParam
		{
			Vector3*	  m_pVerts;			// 顶点
			int           m_iVerts;			// 顶点数
			int*		  m_pTris;			// 三角形(顶点索引)
			//WaterZoneMgr* m_waterZone;		// 水区域
			//const float*  m_pNormals;		// 面法线
			int           m_iTris;			// 三角形数量
			VolumeList	  m_convexVolumes;	// 凸包区域
			Vector3		  m_vMin;
			Vector3		  m_vMax;			// 区域包围盒
			float         m_fCellSize;		// 格子大小(世界单位)
			float         m_fCellHeight;	// 格子高度
			float         m_fAgentMaxSlope;	// 最大倾斜度
			float         m_fAgentHeight;	// 可通间高度
			float         m_fAgentClimb;	// 可通过高度
			float		  m_fAgentRadius;	// 半径

			float         m_fRegionMinSize;
			float		  m_fRegionMergeSize;

			float		  m_fEdgeMaxLen;
			float		  m_fEdgeMaxError;
			int  		  m_iVertesPerPoly;
			int			  m_iTileSize;		// width and height of a tile

			float		  m_fDetailSampleDist;
			float		  m_fDetailSampleMaxError;

			// 构造函数
			SBuildParam()
			{
				m_pVerts   = 0;
				m_pTris    = 0;
				//m_waterZone= NULL;
				//m_pNormals = 0;
				m_iVerts   = 0;
				m_iTris    = 0;

				m_fCellSize   = 0.2f;
				m_fCellHeight = 0.2f;

				m_fAgentHeight	 = 2.8f;
				m_fAgentRadius	 = 0.45f;
				m_fAgentClimb	 = 0.6f;
				m_fAgentMaxSlope = 45.f;

				m_fRegionMinSize   = 50.f;
				m_fRegionMergeSize = 20.f;

				m_fEdgeMaxLen   = 24.f;
				m_fEdgeMaxError = 1.3f;
				m_iVertesPerPoly= 6;
				m_iTileSize     = 100;

				m_fDetailSampleDist   	= 6.f;
				m_fDetailSampleMaxError = 1.f;
			}

			// 析构
			~SBuildParam()
			{
				//SAFE_DELETE( m_waterZone);
				SAFE_DELETE_ARRAY( m_pVerts);
				SAFE_DELETE_ARRAY( m_pTris);
			}
		};

		// tile( 片, 区块)
		struct STile
		{
			SRcHeightField*			m_pSolid;				// height field	
			SRcCompactHeightField*	m_pCHF;					// compact height field
			SRcContourSet*          m_pCSet;				// contour set
			RcPolyMesh*				m_pMesh;				// 多边形mesh
			SRcPolyMeshDetail*		m_pDMesh;				// 详细多边形mesh

			// 构造函数
			STile()
			{
				m_pSolid = NULL;
				m_pCHF   = NULL;
				m_pCSet  = NULL;
				m_pMesh  = NULL;
				m_pDMesh = NULL;
			}

			// 析构函数
			~STile()
			{
				Reset();
			}

			// 重置
			void Reset()
			{
				SAFE_DELETE( m_pSolid);
				SAFE_DELETE( m_pCHF);
				SAFE_DELETE( m_pCSet);
				SAFE_DELETE( m_pMesh);
				SAFE_DELETE( m_pDMesh);
			}	
		};

		CStaticMeshTiledBuilder();
		~CStaticMeshTiledBuilder();

	public:
		// 构建
		bool HandleBuild( CStaticNavMesh& navMesh, SBuildParam& params, LogSystem* logSystem=NULL);

		// 构建路编号(加速无路径查询)
		static void ConstructRoadIDs( vector<UINT>& oRoadIds, const CStaticNavMesh& navMesh, const NavMeshFilter& filter);

		// 标记路号
		static void MarkRoadId( vector<UINT>& oRoadIds, const NavMeshFilter& filter, const CStaticNavMesh& navMesh, set<PolyRef>& polyIDs, int roadNum);

	private:
		// 重置
		void  Reset();

		// 初始化Config
		bool InitCfg( const SBuildParam& params);

		// 初始化ChunkMesh
		bool InitChunkMesh( const SBuildParam& params);

		// 根据索引构建Tile
		bool BuildTile( STile& tile, int xIdx, int yIdx, const SBuildParam& params);

		// 批处理
		void TileBatching( STile* tile, vector<STile*>& tiles, vector<RcPolyMesh*>& polyMeshs, vector<SRcPolyMeshDetail*>& polyMeshDetails,  bool isEnd);

		// 构建块Mesh
		bool MergeTilesMesh( RcPolyMesh*& outMesh, SRcPolyMeshDetail*& outMeshD, vector<STile*>& tiles);

		// 构建NavMesh
		bool ConstructStaticNavMesh( CStaticNavMesh& navMesh);

		// 标记顶端点
		void MarkApexVertex( CStaticNavMesh& navMesh);

		// 是否为顶端点
		bool IsApexPoint( CStaticNavMesh& navMesh, PolyRef polyID, UINT pointIdx);

	public:
		SRcConfig				m_rcConfig;				// 配置
		SRcChunkyTriMesh*       m_pChunkMesh;			// chunk Mesh(辅助tile构建)
		RcPolyMesh*			m_pMesh;				// 多边形mesh
		SRcPolyMeshDetail*		m_pDMesh;				// 详细多边形mesh
		LogSystem*				m_logSystem;			// 日志系统
	};
}