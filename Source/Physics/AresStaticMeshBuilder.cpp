#include "Physics/Navigation/AresStaticMeshBuilder.h"
#include "Physics/Navigation/AresStaticNavMesh.h"
#include <stdlib.h>

#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }

namespace Ares
{
	// 构造函数
	CStaticMeshBuilder::CStaticMeshBuilder()
	{
		m_bKeepInterResults = false;
		m_pTriFlags			= NULL;
		m_pSolid			= NULL;
		m_pCHF				= NULL;
		m_pCSet				= NULL;
		m_pMesh				= NULL;
		m_pDMesh			= NULL;
	}

	// 构建
	bool CStaticMeshBuilder::HandleBuild( const CStaticNavMesh::SBuildParam& params)
	{
		if( !params.m_pVerts || !params.m_pTris)
			return false;

		// 重置( 保险)
		CleanUp();

		// 
		// step 1. Initialize build config
		//

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
		m_rcConfig.m_fDetailSampleDist = params.m_fDetailSampleDist < 0.9f ? 0 : params.m_fCellSize * params.m_fDetailSampleDist;
		m_rcConfig.m_fDetailSampleMaxError = params.m_fCellHeight * params.m_fDetailSampleMaxError;

		// Set the area where the navigation will be build
		// Here the bounds of the input mesh are used, but the 
		// area could be specifed by an user defined box, etc
		m_rcConfig.m_vMin = params.m_vMin;
		m_rcConfig.m_vMax = params.m_vMax;

		RcCalcGridSize( m_rcConfig.m_iWidth, m_rcConfig.m_iHeight, m_rcConfig.m_vMin, m_rcConfig.m_vMax, m_rcConfig.m_fCellSize);

		//
		// step 2. Rasterize input polygon soup
		//
		// Allocate voxel heighfield where we rasterize our input data to.
		m_pSolid = new_ SRcHeightField;
		if( !m_pSolid)
			return false;

		if( !RcCreateHeightField( *m_pSolid, m_rcConfig.m_iWidth, m_rcConfig.m_iHeight, m_rcConfig.m_vMin, 
								   m_rcConfig.m_vMax, m_rcConfig.m_fCellSize, m_rcConfig.m_fCellHeight))
			return false;

		// Allocate array that can hold triangle flags.
		// If you have multiple meshs you need to process, allocate
		// and array which can hold the max number of triangles you need to process
		m_pTriFlags = new_ unsigned char[params.m_iTris];
		if( !m_pTriFlags)
			return false;

		// Find Triangles which are walkable based on their slope and rasterize them.
		// If you input data is multiple meshs, you can transform them here, calculate
		// the flags for each of the meshes and rasterize them
		memset( m_pTriFlags, 0, params.m_iTris * sizeof( unsigned char));
		RcMarkWalkableTriangles( m_pTriFlags, params.m_pVerts, params.m_iVerts, params.m_pTris, params.m_iTris, m_rcConfig.m_fWalkableSlope);
		RcRasterizeTriangles( params.m_pVerts, params.m_iVerts, params.m_pTris, m_pTriFlags, params.m_iTris, *m_pSolid);

		// 清除标记数据( 下面不再使用)
		if( !m_bKeepInterResults)
			SAFE_DELETE_ARRAY( m_pTriFlags);

		//
		// Step3. Filter walkables surfaces
		//

		// Once all geometry is rasterized, we do initial pass of filtering to 
		// remove unwanted overhangs caused by the conservative rasterization
		// as well as filter spans where the character cannot possibly stand.
		RcFilterLedgeSpans( m_rcConfig.m_iWalkableHeight, m_rcConfig.m_iWalkableClimb, *m_pSolid);
		RcFilterWalkableLowHeightSpans( m_rcConfig.m_iWalkableHeight, *m_pSolid);

		//
		// Step4. Partition walkable surface to simple regions.
		//

		// Compact the heightfield so that it is faster to handle from now on.
		// This will result more cache coherent data as well as neighbours
		// between walkable cells will be calculated.
		m_pCHF = new_ SRcCompactHeightField;
		if( !m_pCHF)
			return false;

		if( !RcBuildCompactHeightfield( m_rcConfig.m_iWalkableHeight, m_rcConfig.m_iWalkableClimb, *m_pSolid, *m_pCHF))
			return false;

		// clear
		if ( !m_bKeepInterResults)
		{
			delete m_pSolid;
			m_pSolid = 0;
		}

		// Prepare for region partitioning, by calculating distance field along the walkable surface
		if( !RcBuildDistanceField( *m_pCHF))
			return false;

		// Partition the walkable surface into simple regions without holes
		if( !RcBuildRegions( *m_pCHF, m_rcConfig.m_iBorderSize, m_rcConfig.m_iMinRegionSize, m_rcConfig.m_iMergeRegionSize))
		{
			// Build Navigation: could not build regions
		} 

		//
		// step5 Trace and simplify region contours
		//

		m_pCSet = new_ SRcContourSet;
		if( !m_pCSet)
			return false;

		if ( !RcBuildContours( *m_pCHF, m_rcConfig.m_fMaxSimplificationError, m_rcConfig.m_iMaxEdgeLen, *m_pCSet))
			return false;
	
		//
		// step 6. Build polygons mesh from contours
		//
		m_pMesh = new_ RcPolyMesh;

		if( !m_pMesh)
			return false;

		if ( !RcBuildPolyMesh( *m_pCSet, m_rcConfig.m_iMaxVertsPerPolyon, *m_pMesh))
			return false;

		//
		// Step7. Create detail mesh which allows to access approximate height on each polygon
		//
		m_pDMesh = new_ SRcPolyMeshDetail;
		
		if( !m_pDMesh)
			return false;

		if( !RcBuildPolyMeshDetail( *m_pMesh, *m_pCHF, m_rcConfig.m_fDetailSampleDist, m_rcConfig.m_fDetailSampleMaxError, *m_pDMesh))
			return false;

		if( !m_bKeepInterResults)
		{
			delete  m_pCHF;
			delete  m_pCSet;

			m_pCHF  = 0;
			m_pCSet = 0;
		}

		return true;
	}

	// 重置
	void CStaticMeshBuilder::CleanUp()
	{
		SAFE_DELETE_ARRAY(m_pTriFlags);
		SAFE_DELETE_ARRAY(m_pSolid);
		SAFE_DELETE_ARRAY(m_pCHF);
		SAFE_DELETE_ARRAY(m_pCSet);
		SAFE_DELETE(m_pMesh);
		SAFE_DELETE(m_pDMesh);
	}
}