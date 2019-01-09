#include "Physics/AresStaticMeshTiledBuilder.h"
#include "Physics/AresStaticNavMesh.h"
#include "Physics/AresChunkyTriMesh.h"
#include <stdlib.h>
#include <Core/AresMath.h>

#define DEFAULT_UP_DIR_IDX 2
#define NAV_VERSION	20130527

namespace Ares
{
	// 更换顶点数据
	static void SwitchVertex( Vector3* data, int vertexNum, int idx1, int idx2)
	{
		if( idx1 == idx2)
			return;

		for( int i=0; i<vertexNum; i++)
		{
			float tmp     = data[i][idx1];
			data[i][idx1] = data[i][idx2];
			data[i][idx2] = tmp;
		}
	}

	// 更换顶点数据
	static void SwitchVertex( float* data, int vertexNum, int idx1, int idx2)
	{
		if( idx1 == idx2)
			return;

		for( int i=0; i<vertexNum; i++)
		{
			float tmp      = data[i*3+idx1];
			data[i*3+idx1] = data[i*3+idx2];
			data[i*3+idx2] = tmp;
		}
	}

	// 构造函数
	CStaticMeshTiledBuilder::CStaticMeshTiledBuilder()
	{
		m_pChunkMesh		= NULL;
		m_pMesh				= NULL;
		m_pDMesh			= NULL;
	}

	// 析构函数
	CStaticMeshTiledBuilder::~CStaticMeshTiledBuilder()
	{
		SAFE_DELETE( m_pChunkMesh);
		SAFE_DELETE( m_pMesh);
		SAFE_DELETE( m_pDMesh);
	}

	// 构建
	bool CStaticMeshTiledBuilder::HandleBuild( CStaticNavMesh& navMesh, CStaticMeshTiledBuilder::SBuildParam& params, LogSystem* logSystem)
	{
		if( !params.m_pVerts || !params.m_pTris)
			return false;

		if( params.m_pVerts)
		{
			SwitchVertex( params.m_pVerts, params.m_iVerts, navMesh.m_upDirIdx, DEFAULT_UP_DIR_IDX);

			// min max box
			SwitchVertex( &params.m_vMin, 1, navMesh.m_upDirIdx, DEFAULT_UP_DIR_IDX);
			SwitchVertex( &params.m_vMax, 1, navMesh.m_upDirIdx, DEFAULT_UP_DIR_IDX);
		}

		m_logSystem = logSystem;

		navMesh.Reset();

		// 重置( 保险)
		Reset();

		// step 1. Initialize build config
		InitCfg( params);

		// Build chunky triangle mesh for local polygon queries
		InitChunkMesh( params);

		// Calculate the number of tiles in the output and initialize tiles
		int tileSetWidth  = (m_rcConfig.m_iWidth + m_rcConfig.m_iTileSize - 1) / m_rcConfig.m_iTileSize;
		int tileSetHeight = (m_rcConfig.m_iHeight + m_rcConfig.m_iTileSize - 1) / m_rcConfig.m_iTileSize;

		vector<STile*> tTiles;
		#pragma omp parallel for
		for ( int y=0; y<tileSetHeight; y++)
		{
			for ( int x=0; x<tileSetWidth; x++)
			{
				STile* tTile = new_ STile;
				if( BuildTile( *tTile, x, y, params))
				{
					#pragma omp critical
					tTiles.push_back( tTile);
				}
			}
		}

		std::cout << tTiles.size() << endl;

		SAFE_DELETE( m_pChunkMesh);

		m_pMesh = new_ RcPolyMesh;
		m_pDMesh = new_ SRcPolyMeshDetail;
		if( m_pMesh && m_pDMesh)
			MergeTilesMesh( m_pMesh, m_pDMesh, tTiles);
	
		SAFE_DELETE_VECTOR( tTiles);

		ConstructStaticNavMesh( navMesh);

		return true;
	}

	// 构建NavMesh
	bool CStaticMeshTiledBuilder::ConstructStaticNavMesh( CStaticNavMesh& navMesh)
	{
		// Find unique detail vertices.
		int nvp = m_pMesh->m_iNVP;
		int uniqueDetailVerts = 0;
		if( m_pMesh)
		{
			for ( int i=0; i<m_pDMesh->m_numMeshs; i++)
			{
				const UINT* p = &m_pMesh->m_polys[i*nvp * 2];
				int ndv = m_pDMesh->m_subMeshs[i].m_numVerts;
				int nv  = 0;
				for ( int j=0; j<nvp; j++)
				{
					if( p[j] == INVALID)
						break;

					nv++;
				}
				ndv -= nv;
				uniqueDetailVerts += ndv;
			}
		}

		// 计算数据大小 Calculate data size
		const int headerSize	 = sizeof( CStaticNavMesh::SStaticNavMeshHeader);
		const int polyRoadIdSize = sizeof(int) * m_pMesh->m_numPolys;
		const int areaIDsSize= sizeof(unsigned char) * m_pMesh->m_numPolys;
		const int polySize		 = sizeof(CStaticNavMesh::SStaticPoly) * m_pMesh->m_numPolys;
		const int vertsSize		 = sizeof(Vector3) * m_pMesh->m_iNumVerts;
		const int vertFlagsSize  = sizeof(BYTE)*m_pMesh->m_iNumVerts;
		const int detailPolySize = sizeof(CStaticNavMesh::SStaticPolyDetail) * m_pMesh->m_numPolys;
		const int detailVertSize = sizeof(Vector3) * uniqueDetailVerts;
		const int detailTrisSize = sizeof(unsigned char) * 4 * m_pDMesh->m_numTris;
		const int quadTreeMemSize= sizeof(quad_tree_member<PolyRef>) * m_pMesh->m_numPolys;

		navMesh.m_iDataSize = headerSize + polyRoadIdSize + areaIDsSize +  polySize + vertsSize + vertFlagsSize + detailPolySize + detailVertSize + detailTrisSize + quadTreeMemSize;
		navMesh.m_pData = new_ unsigned char[navMesh.m_iDataSize];
		if( !navMesh.m_pData)
			return false;

		memset( navMesh.m_pData, 0, navMesh.m_iDataSize);

		// 数据指针游标

		unsigned char* dataIndex = navMesh.m_pData;

		navMesh.m_pHeader					 = (CStaticNavMesh::SStaticNavMeshHeader*)dataIndex;	dataIndex += headerSize;
		//navMesh.m_pHeader->m_pPolyRoadIDs	 = (UINT*)dataIndex;					dataIndex += polyRoadIdSize;
		navMesh.m_pHeader->m_areaIDs		 = (unsigned char*)dataIndex;			dataIndex += areaIDsSize;
		navMesh.m_pHeader->m_pPolys			 = (CStaticNavMesh::SStaticPoly*)dataIndex;				dataIndex += polySize;
		navMesh.m_pHeader->m_pVerts			 = (Vector3*)dataIndex;					dataIndex += vertsSize;
		navMesh.m_pHeader->m_pVertFlags		 = (BYTE*)dataIndex;					dataIndex += vertFlagsSize;
		navMesh.m_pHeader->m_pDetailPolys	 = (CStaticNavMesh::SStaticPolyDetail*)dataIndex;		dataIndex += detailPolySize;
		navMesh.m_pHeader->m_pDetailVerts	 = (Vector3*)dataIndex;					dataIndex += detailVertSize;
		navMesh.m_pHeader->m_pDetailTris	 = (unsigned char*)dataIndex;			dataIndex += detailTrisSize;
		navMesh.m_pHeader->m_pQuadTreeMember = (quad_tree_member<PolyRef>*)dataIndex;dataIndex += quadTreeMemSize;

		// Store header
		navMesh.m_pHeader->m_iVersion  = NAV_VERSION;
		navMesh.m_pHeader->m_iNumPolys = m_pMesh->m_numPolys;
		navMesh.m_pHeader->m_iNumVerts = m_pMesh->m_iNumVerts;
		navMesh.m_pHeader->m_iNVP	   = m_pMesh->m_iNVP;
		navMesh.m_pHeader->m_fCellSize = m_pMesh->m_fCellSize;
		navMesh.m_pHeader->m_vMin	   = m_pMesh->m_vMin;
		navMesh.m_pHeader->m_vMax	   = m_pMesh->m_vMax;

		navMesh.m_pHeader->m_iDNumVerts = uniqueDetailVerts;
		navMesh.m_pHeader->m_iDNumTris	= m_pDMesh->m_numTris;

		// store consumes
		memcpy( navMesh.m_pHeader->m_areaIDs, m_pMesh->m_areaIDs.data(), areaIDsSize);

		// store polygons
		for ( unsigned int i=0; i<navMesh.m_pHeader->m_iNumPolys; i++)
		{
			const UINT* pSrc = &m_pMesh->m_polys[i*navMesh.m_pHeader->m_iNVP * 2];

			CStaticNavMesh::SStaticPoly& tPoly = navMesh.m_pHeader->m_pPolys[i]; 
			tPoly.m_iNumVerts = 0;

			for ( int j=0; j<navMesh.m_pHeader->m_iNVP; j++)
			{
				if( pSrc[j]==INVALID)
					break;

				tPoly.m_indices[j]  = pSrc[j];
				tPoly.m_neighbor[j] = pSrc[navMesh.m_pHeader->m_iNVP+j];
				tPoly.m_iNumVerts++;
			}

			pSrc += navMesh.m_pHeader->m_iNVP * 2;
		}

		// store vertexs
		for ( unsigned int i=0; i<navMesh.m_pHeader->m_iNumVerts; i++)
		{
			const Int3& iv = m_pMesh->m_verts[i];
			Vector3&    v  = navMesh.m_pHeader->m_pVerts[i];

			v[0] = navMesh.m_pHeader->m_vMin.x + iv[0] * navMesh.m_pHeader->m_fCellSize;
			v[1] = navMesh.m_pHeader->m_vMin.y + iv[1] * navMesh.m_pHeader->m_fCellSize;
			v[2] = navMesh.m_pHeader->m_vMin.z + iv[2] * m_pMesh->m_fCellHeight;

			if( v.z < -1000.f || v.y > 4096.f || v.x > 4096.f)
				printf( "\nBugVertex: %10.2f,%10.2f,%10.2f;", v.x, v.y, v.z);

		}

		// Store Detail polys and vertices
		unsigned int vBase = 0;
		for (unsigned int i=0; i<navMesh.m_pHeader->m_iNumPolys; i++)
		{
			CStaticNavMesh::SStaticPolyDetail& dtl   = navMesh.m_pHeader->m_pDetailPolys[i];
			const int vertBase       = m_pDMesh->m_subMeshs[i].m_vertsOffset;
			const int ndv			 = m_pDMesh->m_subMeshs[i].m_numVerts;
			const int nv			 = navMesh.m_pHeader->m_pPolys[i].m_iNumVerts;	
			dtl.m_iVertBase = vBase;
			dtl.m_iNumVerts = ndv - nv;
			dtl.m_iTriBase  = m_pDMesh->m_subMeshs[i].m_trisOffset;
			dtl.m_iNumTris  = m_pDMesh->m_subMeshs[i].m_numTris;

			// Copy vertices except the first 'nv' verts which are equal to nav poly verts
			if ( ndv - nv)
			{
				memcpy( &navMesh.m_pHeader->m_pDetailVerts[vBase], &m_pDMesh->m_verts[(vertBase+nv)], sizeof(Vector3)*(ndv-nv));
				vBase += ndv - nv;
			}
		}

		// Store triangles
		memcpy( navMesh.m_pHeader->m_pDetailTris, m_pDMesh->m_tris.data(), sizeof(unsigned char)*4*m_pDMesh->m_numTris);

		// Store quadtree member for search
		float x0 = navMesh.m_pHeader->m_vMin.x,
			y0 = navMesh.m_pHeader->m_vMin.y,
			x1 = navMesh.m_pHeader->m_vMax.x,
			y1 = navMesh.m_pHeader->m_vMax.y;

        quad_tree_rect navArea( x0, x1, y0, y1);
		navMesh.m_quadTree->create( navArea, 7);
		for ( unsigned int i=0; i<navMesh.m_pHeader->m_iNumPolys; i++)
		{
			quad_tree_rect rect2D;
			navMesh.GetPolyBoundingBox( i, rect2D);

			navMesh.m_pHeader->m_pQuadTreeMember[i].value = i;
			navMesh.m_pHeader->m_pQuadTreeMember[i].key = rect2D;

			navMesh.m_quadTree->add_member( &navMesh.m_pHeader->m_pQuadTreeMember[i]);
		}

		// 构建路编号
		// ConstructRoadIDs( navMesh);

		// 标记顶点
		MarkApexVertex( navMesh);

		return true;
	}

	// 标记路号
	void CStaticMeshTiledBuilder::MarkRoadId( vector<UINT>& oRoadIds, const NavMeshFilter& filter, const CStaticNavMesh& navMesh, set<PolyRef>& polyIDs, int roadNum)
	{
		// 赋值路号
		for ( set<PolyRef>::iterator it=polyIDs.begin(); it!=polyIDs.end(); it++)
			oRoadIds[*it] = roadNum;

		// 获取相连多边形ID
		set<PolyRef> tNextPolyIDs;
		for ( set<PolyRef>::iterator it=polyIDs.begin(); it!=polyIDs.end(); it++)
		{
			int           childID[8];
			int           numChild;
			navMesh.GetPassAbleNeighbor( filter, childID, numChild, *it);
			for ( int j=0; j<numChild; j++)
			{
				int childRef = childID[j];
				if( oRoadIds[childRef] == INVALID)
				{
					tNextPolyIDs.insert( childRef);
				}
				else
				{
					A_ASSERT( oRoadIds[childRef]==roadNum);
				}
			}
		}

		// 指向所有子PolyIDs
		polyIDs.swap( tNextPolyIDs);
	}

	// 构建路编号(加速无路径查询)
	void CStaticMeshTiledBuilder::ConstructRoadIDs(  vector<UINT>& oRoadIds, const CStaticNavMesh& navMesh, const NavMeshFilter& filter)
	{
		// 初始化
		oRoadIds.resize( navMesh.m_pHeader->m_iNumPolys, -1);

		int roadNumber = 0;
		for( size_t i=0; i<navMesh.m_pHeader->m_iNumPolys; i++)
		{
			if( oRoadIds[i] == -1)
			{
				set<PolyRef> tPolyIDs;
				tPolyIDs.insert( i);

				// 循环 层级标记
				while( tPolyIDs.size())
				{
					MarkRoadId(  oRoadIds, filter, navMesh, tPolyIDs, roadNumber);
				}

				roadNumber++;
			}
		}
	}

	// 标记顶端点
	void CStaticMeshTiledBuilder::MarkApexVertex( CStaticNavMesh& navMesh)
	{
		navMesh.m_pHeader->m_iNumApexVerts = 0;

		vector<bool> marked( navMesh.m_pHeader->m_iNumVerts, false);
		std::memset( navMesh.m_pHeader->m_pVertFlags, 0, sizeof(BYTE)*navMesh.m_pHeader->m_iNumVerts);

		for( size_t i=0; i<navMesh.m_pHeader->m_iNumPolys; i++)
		{
			const CStaticNavMesh::SStaticPoly* poly = navMesh.GetPolyByRef( i); A_ASSERT( poly);

			// 标记所有点
			for ( UINT j=0; j<poly->m_iNumVerts; j++)
			{
				if( !marked[ poly->m_indices[j]])
				{
					if( IsApexPoint( navMesh, i, poly->m_indices[j]))
					{
						navMesh.m_pHeader->m_pVertFlags[poly->m_indices[j]] |= CStaticNavMesh::VF_APEX;
						navMesh.m_pHeader->m_iNumApexVerts++;
					}

					marked[ poly->m_indices[j]] = true;
				}
			}
		}
	}

	// 是否为顶端点
	bool CStaticMeshTiledBuilder::IsApexPoint( CStaticNavMesh& navMesh, PolyRef polyID, UINT pointIdx)
	{
		set<PolyRef> checked;		// 已被检测的多边形
		set<PolyRef> unchecked;		// 未检测的
		unchecked.insert( polyID);

		while ( unchecked.size())
		{
			set<PolyRef> nextUncheked;
			for( set<PolyRef>::iterator it=unchecked.begin(); it!=unchecked.end(); it++)
			{
				PolyRef leftPoly, rightPoly;
				navMesh.GetPolyNeighborByPoint( *it, pointIdx, leftPoly, rightPoly);
				if( leftPoly==INVALID || rightPoly==INVALID)
					return true;

				// 记录已检测
				checked.insert( *it);

				if( checked.find( leftPoly)==checked.end())
					nextUncheked.insert( leftPoly);

				if( checked.find( rightPoly)==checked.end())
					nextUncheked.insert( rightPoly);
			}

			unchecked.swap( nextUncheked);
		}

		return false;
	}

	// 批处理
	void CStaticMeshTiledBuilder::TileBatching( STile* tile, vector<STile*>& tiles, vector<RcPolyMesh*>& polyMeshs, vector<SRcPolyMeshDetail*>& polyMeshDetails, bool isEnd)
	{
		tiles.push_back( tile);

		if( tiles.size() == 256 || isEnd)
		{
			RcPolyMesh*		tPolyMesh		= NULL;
			SRcPolyMeshDetail*  tPolyMeshDetail = NULL;
			MergeTilesMesh( tPolyMesh, tPolyMeshDetail, tiles);
			polyMeshs.push_back( tPolyMesh);
			polyMeshDetails.push_back( tPolyMeshDetail);

			// 清除Tile内存
			SAFE_DELETE_VECTOR( tiles);
		}
	}

	// 初始化Config
	bool CStaticMeshTiledBuilder::InitCfg( const SBuildParam& params)
	{
		// Init build configuration from GUI
		memset( &m_rcConfig, 0, sizeof( m_rcConfig));

		// 区块大小 高度
		m_rcConfig.m_fCellSize		 = params.m_fCellSize;
		m_rcConfig.m_fCellHeight	 = params.m_fCellHeight;

		m_rcConfig.m_fWalkableSlope  = params.m_fAgentMaxSlope;
		m_rcConfig.m_iWalkableHeight = (int)ceilf( params.m_fAgentHeight / params.m_fCellHeight);
		m_rcConfig.m_iWalkableClimb  = (int)ceilf( params.m_fAgentClimb / params.m_fCellHeight);
		m_rcConfig.m_iWalkableRadius = (int)ceilf( params.m_fAgentRadius / params.m_fCellSize);

		m_rcConfig.m_iMaxEdgeLen     = (int)( params.m_fEdgeMaxLen / params.m_fCellSize);
		m_rcConfig.m_fMaxSimplificationError = params.m_fEdgeMaxError;
		m_rcConfig.m_iMinRegionSize	 = (int)rcSqr( params.m_fRegionMinSize);
		m_rcConfig.m_iMergeRegionSize= (int)rcSqr( params.m_fRegionMergeSize);
		m_rcConfig.m_iMaxVertsPerPolyon=(int)params.m_iVertesPerPoly;
		m_rcConfig.m_iTileSize         = params.m_iTileSize;
		m_rcConfig.m_iBorderSize	   = m_rcConfig.m_iWalkableRadius * 2 + 2;	
		m_rcConfig.m_fDetailSampleDist = params.m_fDetailSampleDist < 0.9f ? 0 : params.m_fCellSize * params.m_fDetailSampleDist;
		m_rcConfig.m_fDetailSampleMaxError = params.m_fCellHeight * params.m_fDetailSampleMaxError;

		// Set the area where the navigation will be build
		// Here the bounds of the input mesh are used, but the 
		// area could be specifed by an user defined box, etc
		m_rcConfig.m_vMin = params.m_vMin;
		m_rcConfig.m_vMax = params.m_vMax;

		RcCalcGridSize( m_rcConfig.m_iWidth, m_rcConfig.m_iHeight, m_rcConfig.m_vMin, m_rcConfig.m_vMax, m_rcConfig.m_fCellSize);

		return true;
	}

	// 初始化ChunkMesh
	bool CStaticMeshTiledBuilder::InitChunkMesh( const CStaticMeshTiledBuilder::SBuildParam& params)
	{
		// Build chunky triangle mesh for local polygon queries
		quad_tree_rect buildRect( params.m_vMin[0], params.m_vMax[0], params.m_vMin[1], params.m_vMax[1]);

		m_pChunkMesh = new_ SRcChunkyTriMesh( buildRect);
		if( !m_pChunkMesh)
			return false;

		// create chunk triangle mesh
		if( !RcCreateChunkyTriMesh( params.m_pVerts, params.m_pTris, params.m_iTris, m_pChunkMesh))
			return false;

		return true;
	}

	// 根据索引构建Tile
	bool CStaticMeshTiledBuilder::BuildTile( STile& tile, int xIdx, int yIdx, const CStaticMeshTiledBuilder::SBuildParam& params)
	{
		// Initialize per tile config
		SRcConfig cfg = m_rcConfig;
		cfg.m_iWidth  = m_rcConfig.m_iTileSize + m_rcConfig.m_iBorderSize * 2;
		cfg.m_iHeight = m_rcConfig.m_iTileSize + m_rcConfig.m_iBorderSize * 2;

		STile& tTile = tile;

		// calculate the per tile bounding box
		cfg.m_vMin[0] = m_rcConfig.m_vMin[0] + (xIdx*m_rcConfig.m_iTileSize - m_rcConfig.m_iBorderSize)     * m_rcConfig.m_fCellSize;
		cfg.m_vMin[1] = m_rcConfig.m_vMin[1] + (yIdx*m_rcConfig.m_iTileSize - m_rcConfig.m_iBorderSize)     * m_rcConfig.m_fCellSize;
		cfg.m_vMax[0] = m_rcConfig.m_vMin[0] + ((xIdx+1)*m_rcConfig.m_iTileSize + m_rcConfig.m_iBorderSize) * m_rcConfig.m_fCellSize;
		cfg.m_vMax[1] = m_rcConfig.m_vMin[1] + ((yIdx+1)*m_rcConfig.m_iTileSize + m_rcConfig.m_iBorderSize) * m_rcConfig.m_fCellSize;

		// Get chunk
		quad_tree_rect searchRect( cfg.m_vMin[0], cfg.m_vMax[0], cfg.m_vMin[1], cfg.m_vMax[1]);

		vector<int> triIndexs; triIndexs.reserve( 2048);
		const int nTri = RcGetChunkInRect( m_pChunkMesh, searchRect, triIndexs);
		if( !nTri)
			return false;

		// step 2. Rasterize input polygon soup
		// Allocate voxel heighfield where we rasterize our input data to.
		SRcHeightField* pSolid = new_ SRcHeightField;
		if( !pSolid)
			return false;

		if( !RcCreateHeightField( *pSolid, cfg.m_iWidth, cfg.m_iHeight, cfg.m_vMin,  cfg.m_vMax, cfg.m_fCellSize, cfg.m_fCellHeight))
			return false;

		Int3 pTris[MaxTrianglePerSearch];
		for ( int i=0; i<nTri; i++)
		{
			pTris[i][0] = params.m_pTris[triIndexs[i]*3+0];
			pTris[i][1] = params.m_pTris[triIndexs[i]*3+1];
			pTris[i][2] = params.m_pTris[triIndexs[i]*3+2];
		}

		vector<BYTE> triangleAreaIDs(MaxTrianglePerSearch);
		memset( triangleAreaIDs.data(), 0, MaxTrianglePerSearch * sizeof(BYTE));

		RcMarkWalkableTriangles( triangleAreaIDs.data(), params.m_pVerts, params.m_iVerts, (const int*)pTris, nTri, cfg.m_fWalkableSlope);
		RcRasterizeTriangles( params.m_pVerts, params.m_iVerts, (const int*)pTris, triangleAreaIDs.data(), nTri, *pSolid, cfg.m_iWalkableClimb);

		// Step3. Filter walkables surfaces
		// Once all geometry is rasterized, we do initial pass of filtering to 
		// remove unwanted overhangs caused by the conservative rasterization
		// as well as filter spans where the character cannot possibly stand.
		//RcFilterWaterSapns( *pSolid, params.m_waterZone, cfg);
		RcFilterLowHangingWalkableObstacles( cfg.m_iWalkableClimb, *pSolid);
		RcFilterLedgeSpans( cfg.m_iWalkableHeight, cfg.m_iWalkableClimb, *pSolid);
		RcFilterWalkableLowHeightSpans( cfg.m_iWalkableHeight, *pSolid);

		// Step4. Partition walkable surface to simple regions.
		// Compact the heightfield so that it is faster to handle from now on.
		// This will result more cache coherent data as well as neighbours
		// between walkable cells will be calculated.
		SRcCompactHeightField* pChf = new_ SRcCompactHeightField;
		if( !pChf)
			return false;

		std::memset( pChf, 0, sizeof(SRcCompactHeightField));

		if( !RcBuildCompactHeightfield( cfg.m_iWalkableHeight, cfg.m_iWalkableClimb, *pSolid, *pChf))
			return false;

		// Erode the walkabe area by agent radius
		if( !RcErodeWalkableArea( cfg.m_iWalkableRadius, *pChf))
			return false;

		// Mark Areas
		for( size_t i=0; i<params.m_convexVolumes.size(); i++)
			RcMarkConvexPolyArea( params.m_convexVolumes[i], *pChf);

		if( false/*cfg.m_monotonePartitioning*/)
		{
			// Partition the walkable surface into simple regions without holes.
			// Monotone partitioning does not need distancefield.
			//if( !RcBuildRegionsMonotone())
			//	return false;
		}
		else
		{
			// Prepare for region partitioning, by calculating distance field along the walkable surface
			if( !RcBuildDistanceField( *pChf))
				return false;

			// Partition the walkable surface into simple regions without holes
			if( !RcBuildRegions( *pChf, cfg.m_iBorderSize, cfg.m_iMinRegionSize, cfg.m_iMergeRegionSize))
			{
				// Build Navigation: could not build regions
				return false;
			} 
		}

		// step5 Trace and simplify region contours
		SRcContourSet* pCSet = new_ SRcContourSet;
		if( !pCSet)
			return false;

		memset( pCSet, 0, sizeof( SRcContourSet));

		if ( !RcBuildContours( *pChf, cfg.m_fMaxSimplificationError, cfg.m_iMaxEdgeLen, *pCSet))
			return false;

		if( !pCSet->m_iNumContours)
		{
			SAFE_DELETE( pCSet);	
			return false;
		}

		// step 6. Build polygons mesh from contours
		tTile.m_pMesh = new_ RcPolyMesh;
		if( !tTile.m_pMesh)
			return false;

		if ( !RcBuildPolyMesh( *pCSet, cfg.m_iMaxVertsPerPolyon, *tTile.m_pMesh))
		{
			SAFE_DELETE( tTile.m_pMesh);

			return false;
		}

		// Step7. Create detail mesh which allows to access approximate height on each polygon
		tTile.m_pDMesh = new_ SRcPolyMeshDetail;
		if( !tTile.m_pDMesh)
			return false;

		bool testFlag = false;
		if( !RcBuildPolyMeshDetail( *tTile.m_pMesh, *pChf, cfg.m_fDetailSampleDist, cfg.m_fDetailSampleMaxError, *tTile.m_pDMesh, testFlag))
		{
			SAFE_DELETE( tTile.m_pDMesh);

			return false;
		}

		if( testFlag)
		{
			printf( "找到你了 %d, %d", xIdx, yIdx);
		}

		SAFE_DELETE( pSolid);
		SAFE_DELETE( pChf);
		SAFE_DELETE( pCSet); 	

		return true;
	}

	// 构建块Mesh
	bool CStaticMeshTiledBuilder::MergeTilesMesh( RcPolyMesh*& outMesh, SRcPolyMeshDetail*& outMeshD,  vector<CStaticMeshTiledBuilder::STile*>& tiles)
	{
		// Merge per tile poly and detail meshes
		vector<RcPolyMesh*> pMmerge(tiles.size());
		vector<SRcPolyMeshDetail*> pDmmerge(tiles.size());

		int iMerge = 0;
		for ( size_t i=0; i<tiles.size(); i++)
		{
			if (tiles[i]->m_pMesh)
			{
				pMmerge[iMerge] =  tiles[i]->m_pMesh;
				pDmmerge[iMerge] = tiles[i]->m_pDMesh;
				iMerge++;
			}
		}

		// 合并
		RcMergePolyMeshes( pMmerge, iMerge, *outMesh);
		RcMergePolyMeshDetails( pDmmerge, iMerge, *outMeshD);

		return true;
	}

	// 重置
	void CStaticMeshTiledBuilder::Reset()
	{
		SAFE_DELETE(m_pChunkMesh);
		SAFE_DELETE(m_pMesh);
		SAFE_DELETE(m_pDMesh);
	}
}