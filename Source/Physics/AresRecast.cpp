#include "Physics/AresRecast.h"
#include "Physics/Shapes.h"

namespace Ares
{
	// 构造函数
	RcPolyMesh::RcPolyMesh()
	{
		m_iNumVerts		= 0;
		m_numPolys		= 0;
		m_iNVP			= 6;
	}

	// 获取多边形顶点数
	int RcPolyMesh::CountPolyVerts( const UINT* p, const int nvp)
	{
		for ( int i = 0; i < nvp; ++i)
			if( p[i] == RC_MESH_NULL_IDX)
				return i;

		return nvp;
	}


	// 辅助函数(是否重叠)
	static bool OverlapBounds( const Vector3& vMin0, const Vector3& vMax0,  const Vector3& vMin1, const Vector3& vMax1)
	{
		bool bOverlap = true;

		bOverlap = (vMin0.x > vMax1.x || vMax0.x < vMin1.x) ? false : bOverlap;
		bOverlap = (vMin0.y > vMax1.y || vMax0.y < vMin1.y) ? false : bOverlap;
		bOverlap = (vMin0.z > vMax1.z || vMax0.z < vMin1.z) ? false : bOverlap;

		return bOverlap;
	}

	// clip polygon
	static int ClipPoly( Vector3* pOut, const Vector3* pIn, int n, float pnx, float pny, float pd)
	{
		float d[12];

		for ( int i=0; i<n; i++)
			d[i] = pnx * pIn[i].x + pny * pIn[i].y + pd;

		int m = 0;
		for ( int i=0, j=n-1; i<n; j=i, ++i)
		{
			bool ina = d[j] >= 0;
			bool inb = d[i] >= 0;

			// 两点横跨格子,求交点
			if( ina != inb)
			{
				// s 为跨越比例
				float s = d[j] / ( d[j]-d[i]);
				
				pOut[m] = pIn[j] + ( pIn[i] - pIn[j]) * s;
				
				m++;
			}

			if( inb)
			{
				pOut[m] = pIn[i];

				m++;
			}
		}

		return m;
	}

	// 内存池分配Span
	static SRcSpan* AllocateSpan( SRcHeightField& hf)
	{
		// if running out of memory, allocate new page and update the freelist
		if ( !hf.m_pFreeList || !hf.m_pFreeList->m_pNext)
		{
			// Create new page, Allocate memory for the new pool
			const int iSize = ( sizeof(SRcSpanPool) - sizeof(SRcSpan)) + sizeof(SRcSpan)*RC_SPANS_PER_POOL;

			SRcSpanPool* pPool = reinterpret_cast<SRcSpanPool*>( new_ unsigned char[iSize]);
			if ( !pPool)
				return NULL;

			pPool->m_pNext = NULL;

			// Add the pool into the list of pools
			pPool->m_pNext = hf.m_pPools;
			hf.m_pPools = pPool;

			// Add new items to the free list
			SRcSpan* pFreeList = hf.m_pFreeList;
			SRcSpan* pHead     = &pPool->m_items[0];
			SRcSpan* it        = &pPool->m_items[RC_SPANS_PER_POOL];
			do 
			{
				--it;
				it->m_pNext = pFreeList;
				pFreeList   = it;
			} 
			while( it!=pHead);
			hf.m_pFreeList = it;
		}

		// pop item from in front of the free list
		SRcSpan* pIt = hf.m_pFreeList;
		hf.m_pFreeList = hf.m_pFreeList->m_pNext;

		return pIt;
	}

	// Free Span
	static void FreeSpan( SRcHeightField& hf, SRcSpan* pPtr)
	{
		if( !pPtr)
			return;

		// Add the node in front of the free list
		pPtr->m_pNext = hf.m_pFreeList;
		hf.m_pFreeList = pPtr;
	}

	// Add span to Heightfield
	static void AddSpan( SRcHeightField& hf, int x, int y, unsigned int sMin, unsigned int sMax, unsigned char area, const int flagMergeThr)
	{
		int index = x + y * hf.m_iWidth;

		SRcSpan* pSpan = AllocateSpan( hf);
		pSpan->m_iMinHeight = sMin;
		pSpan->m_iMaxHeight = sMax;
		pSpan->m_areaID     = area;
		pSpan->m_pNext      = NULL;

		// Empty cell, Add he first span
		if ( !hf.m_pSpans[index])
		{
			hf.m_pSpans[index] = pSpan;
			return;
		}

		SRcSpan* pPrev = NULL;
		SRcSpan* pCurr = hf.m_pSpans[index];

		// Insert and merge spans( 从下向上排序,链表)
		while( pCurr)
		{
			// current span is further than the new span, break
			if( pCurr->m_iMinHeight > pSpan->m_iMaxHeight)
				break;

			else if( pCurr->m_iMaxHeight < pSpan->m_iMinHeight)
			{
				// current span is before the new span advance
				pPrev = pCurr;
				pCurr = pCurr->m_pNext;
			}
			else
			{
				// Merge spans
				if( pCurr->m_iMinHeight < pSpan->m_iMinHeight)
					pSpan->m_iMinHeight = pCurr->m_iMinHeight;
				if( pCurr->m_iMaxHeight > pSpan->m_iMaxHeight)
					pSpan->m_iMaxHeight = pCurr->m_iMaxHeight;

				// Merge flags
				if( abs( (int)pSpan->m_iMaxHeight - (int)pCurr->m_iMaxHeight) <= flagMergeThr)
					pSpan->m_areaID = std::max<BYTE>( pCurr->m_areaID, pSpan->m_areaID);

				// remove current span
				SRcSpan* pNext = pCurr->m_pNext;
				FreeSpan( hf, pCurr);
				if( pPrev)
					pPrev->m_pNext = pNext;
				else
					hf.m_pSpans[index] = pNext;

				pCurr = pNext;
			}
		}

		// Insert new span
		if( pPrev)
		{
			pSpan->m_pNext = pPrev->m_pNext;
			pPrev->m_pNext = pSpan;
		}
		else
		{
			pSpan->m_pNext = hf.m_pSpans[index];
			hf.m_pSpans[index] = pSpan;
		}
	}

	// Calculates grid size based on bounding box and grid cell size
	// width     - grid width
	// height    - grid height
	// vMin vMax - bounding box
	// cellSize  - grid cell size
	void RcCalcGridSize( int& width, int& height, const Vector3& vMin, const Vector3& vMax, const float& cellSize)
	{
		width  = (int)(( vMax[0] - vMin[0])/cellSize + 0.5f);
		height = (int)(( vMax[1] - vMin[1])/cellSize + 0.5f);
	}

	// Creates and initializes new heightfield
	// Params:
	// outHF	  - heightfield to initialize
	// width	  - width of the heightfield
	// height	  - height of the heightfield
	// bMin, bMax - bounding box of the heightfield
	// cellSize   - grid cell size
	// cellHeight - grid cell height
	bool RcCreateHeightField( SRcHeightField& outHF, int width, int height, const Vector3& vMin, const Vector3& vMax, float cellSize, float cellHeight)
	{
		outHF.m_iWidth  = width;
		outHF.m_iHeight = height;
		outHF.m_vMin    = vMin;
		outHF.m_vMax    = vMax;
		outHF.m_fCellSize   = cellSize;
		outHF.m_fCellHeight = cellHeight;

		outHF.m_pSpans  = new_ SRcSpan*[width*height];
		if( !outHF.m_pSpans)
			return false;

		// 初始化
		std::memset( outHF.m_pSpans, 0, sizeof( SRcSpan*) * width * height);

		return true;
	}

	// Sets the Walkable flag for every triangle whose slope is below the
	// max walkable slope angle
	// Params:
	// pFlags   - (out) array of triangle flags
	// pverts   - array of vertices
	// numVerts - vertex count
	// pTris    - array of triangle vertex indices
	// numTris  - triangle count
	// walkableSlope - max slop angle in degrees
	void RcMarkWalkableTriangles( unsigned char* areas, const Vector3* pVerts, int numVerts, const int* pTris, int numTris, float walkableSlope)
	{
		// 角度转弧度
		const float walkableThr = cosf( walkableSlope/180.f * PI);

		for( int i=0; i<numTris; i++)
		{
			const int* pTri = &pTris[i*3];

			// 计算法线
			Triangle3 tri( pVerts[pTri[0]], pVerts[pTri[1]], pVerts[pTri[2]]);
			Vector3   normal = tri.GetNormal();

			// check if the face is walkable
			if( abs( normal.z) > walkableThr)
				areas[i] = RC_WALK_AREA;
		}
	}

	// Rasterize a Triangle
	static void  RasterizeTri(const Vector3& v0, const Vector3& v1,const Vector3& v2, unsigned char area, SRcHeightField& hf, 
							  const Vector3& vMin,const Vector3& vMax, const float cellSize, const float ics, const float ich, const int flagMergeThr)
	{
		const float bz = vMax.z - vMin.z;

		// Calculate the bounding box of the triangle
		Vector3  tMin, tMax;

		tMin = v0;
		tMax = v0;
		tMin = Vector3Min( tMin, v1);
		tMin = Vector3Min( tMin, v2);
		tMax = Vector3Max( tMax, v1);
		tMax = Vector3Max( tMax, v2);

		// If the triangle does not touch the bounding box of the heightfield, skip the triangle
		if( !OverlapBounds( tMin, tMax, vMin, vMax))
			return;

		// Calculate the footprint of the triangle on the grid
		int x0 = (int)(( tMin.x - vMin.x) * ics);
		int y0 = (int)(( tMin.y - vMin.y) * ics);
		int x1 = (int)(( tMax.x - vMin.x) * ics);
		int y1 = (int)(( tMax.y - vMin.y) * ics);
		x0 = Clamp( x0, 0, hf.m_iWidth-1);
		y0 = Clamp( y0, 0, hf.m_iHeight-1);
		x1 = Clamp( x1, 0, hf.m_iWidth-1);
		y1 = Clamp( y1, 0, hf.m_iHeight-1);

		// clip the triangle into all grid cells it touches
		Vector3 in[7],
				out[7],
				inrow[7];

		for ( int y=y0; y<=y1; y++)
		{
			// clip polygon to row
			in[0] = v0;
			in[1] = v1;
			in[2] = v2;
			int nvrow = 3;
			const float cy = vMin.y + y * hf.m_fCellSize;
			nvrow = ClipPoly( out, in, nvrow, 0.f, 1.f, -cy);
			if( nvrow < 3)
				continue;
			nvrow = ClipPoly( inrow, out, nvrow, 0, -1, cy+hf.m_fCellSize);
			if( nvrow < 3)
				continue;

			for ( int x=x0; x<=x1; x++)
			{
				// clip polygon to  column
				int nv = nvrow;
				const float cx = vMin.x + x * hf.m_fCellSize;
				nv = ClipPoly( out, inrow, nv, 1, 0, -cx);
				if( nv<3)
					continue;
				nv = ClipPoly( in, out, nv, -1, 0, cx + hf.m_fCellSize);
				if( nv<3)
					continue;

				// Calculate min and max of the span height
				float sMin = in[0].z, sMax=in[0].z;
				for ( int i=1; i<nv; i++)
				{
					sMin = min( sMin, in[i].z);
					sMax = max( sMax, in[i].z);
				}

				// height
				sMin -= vMin.z;
				sMax -= vMin.z;

				// Skip the span if it is outside the heightfield box
				if( sMax < 0.f)
					continue;

				if( sMin>bz)
					continue;

				// clamp the span to the heightfield bounding box
				if( sMin<0.f)
					sMin = 0;
				if( sMax>bz)
					sMax = bz;

				// snap the span to the heightfield height grid
				unsigned short spanMinHeight = (unsigned short)Clamp( (int)floorf( sMin*ich), 0, RC_SPAN_MAX_HEIGHT);
				unsigned short spanMaxHeight = (unsigned short)Clamp( (int)ceilf(  sMax*ich), (int)(spanMinHeight+1), RC_SPAN_MAX_HEIGHT);

				AddSpan( hf, x, y, spanMinHeight, spanMaxHeight, area, flagMergeThr);
			}
		}
	}

	// Rasterizes a triangle into  heightfield spans.
	// Params:
	// pVerts   - array of vertices
	// numVerts - vertex count
	// pTris    - array of triangle vertex indices
	// numTris  - triangle count
	// pFlags   - array of triangle flags( uses walkable)
	// Solid    - heightfield where the triangles are rasterized
	void RcRasterizeTriangles( const Vector3* pVerts, int numVerts, const int* pTris, const unsigned char* areas, int numTris, SRcHeightField& solid, const int flagMergeThr)
	{
		const float ics = 1.f / solid.m_fCellSize;
		const float ich = 1.f / solid.m_fCellHeight;

		// Rasterize triangles
		for ( int i=0; i<numTris; i++)
		{
			//  Get three vertex
			const Vector3& v0 = pVerts[pTris[i*3+0]];
			const Vector3& v1 = pVerts[pTris[i*3+1]];
			const Vector3& v2 = pVerts[pTris[i*3+2]];

			RasterizeTri( v0, v1, v2, areas[i], solid, solid.m_vMin, solid.m_vMax, solid.m_fCellSize, ics, ich, flagMergeThr);
		}
	}

	// Removes walkable flag from all spans that are in water
	void RcFilterWaterSapns( SRcHeightField& solid, WaterZoneMgr* waterZone, const SRcConfig& cfg)
	{
		if( !waterZone) return;

		const int w = solid.m_iWidth;
		const int h = solid.m_iHeight;

		// Mark border span
		for ( int y=0; y<h; y++)
			for ( int x=0; x<w; x++)
				for ( SRcSpan* pSpan = solid.m_pSpans[x+y*w]; pSpan; pSpan=pSpan->m_pNext)
				{
					// skip non walkable spans
					if( pSpan->m_areaID == RC_NULL_AREA)
						continue;

					// 计算中心点
					//Vector3 minPoint = solid.m_vMin + Vector3( x*cfg.m_fCellSize, y*cfg.m_fCellSize, pSpan->m_iMinHeight*cfg.m_fCellHeight);
					//Vector3 maxPoint = solid.m_vMin + Vector3( x*cfg.m_fCellSize, y*cfg.m_fCellSize, pSpan->m_iMaxHeight*cfg.m_fCellHeight) + Vector3( cfg.m_fCellSize, cfg.m_fCellSize, 0.f);
					Vector3 spanCenter = solid.m_vMin + Vector3( (x+0.5f)*cfg.m_fCellSize, (y+0.5f)*cfg.m_fCellSize, pSpan->m_iMinHeight*cfg.m_fCellHeight);

					// The current span is in water
					if ( waterZone->IsPointIn( spanCenter))
						pSpan->m_areaID = RC_NULL_AREA;
				}
	}

	/// Allows the formation of walkable regions that will flow over low lying 
	/// objects such as curbs, and up structures such as stairways. 
	/// 
	/// Two neighboring spans are walkable if: <tt>rcAbs(currentSpan.smax - neighborSpan.smax) < waklableClimb</tt>
	/// 
	/// @warning Will override the effect of #rcFilterLedgeSpans.  So if both filters are used, call
	/// #rcFilterLedgeSpans after calling this filter. 
	///
	/// @see rcHeightfield, rcConfig
	void RcFilterLowHangingWalkableObstacles( int walkableClimb, SRcHeightField& solid)
	{
		const int w = solid.m_iWidth;
		const int h = solid.m_iHeight;
		for (int y=0; y<h; ++y)
		{
			for (int x=0; x<w; ++x)
			{
				SRcSpan* preSpan		  = NULL;
				bool     previousWalkable = false;
				unsigned char previousArea= RC_NULL_AREA;

				for( SRcSpan* pSpan = solid.m_pSpans[x+y*w]; pSpan; preSpan=pSpan, pSpan=pSpan->m_pNext)
				{
					const bool walkable = pSpan->m_areaID != RC_NULL_AREA;

					// If current span is not walkable, but there is walkable span just below it, 
					// mark the span above it walkable too
					if( !walkable && previousWalkable)
					{
						if( std::abs( (int)pSpan->m_iMaxHeight - (int)preSpan->m_iMaxHeight) <= walkableClimb)
							pSpan->m_areaID = previousArea;
					}

					// Copy walkable flag so that it can't proagate past multiple non-walkable objects
					previousWalkable = walkable;
					previousArea	 = pSpan->m_areaID;
				}
			}
		}
	}

	// Removes walkable flag from all spans that are at ledges. this filtering
	// removes possible overestimation of the conservative voxelization so that
	// the resulting mesh will not have regions hanging in air over ledges
	// Params:
	// walkableHeight  - minimum height where the agent can still walk
	// walkableClimb   - maximum height between grid cells the agent can climb
	// solid           - heightfield describing the solid space
	void RcFilterLedgeSpans( const int walkableHeight, const int walkableClimb,SRcHeightField& solid)
	{
		const int w = solid.m_iWidth;
		const int h = solid.m_iHeight;

		const int MAX_HEIGHT = 0xffff;

		// Mark border span
		for ( int y=0; y<h; y++)
			for ( int x=0; x<w; x++)
				for ( SRcSpan* pSpan = solid.m_pSpans[x+y*w]; pSpan; pSpan=pSpan->m_pNext)
				{
					// skip non walkable spans
					if( pSpan->m_areaID == RC_NULL_AREA)
						continue;

					const int bottom = (int)pSpan->m_iMaxHeight;
					const int top    = pSpan->m_pNext ? (int)pSpan->m_pNext->m_iMinHeight : MAX_HEIGHT;

					// Find neightbours minimum height
					int tMinHeight = MAX_HEIGHT;

					// Min and max height of accessible neighbours
					int asmin	= pSpan->m_iMaxHeight;
					int asmax	= pSpan->m_iMaxHeight;

					for ( int dir=0; dir<4; dir++)
					{
						int dx = x + RcGetDirOffsetX( dir);
						int dy = y + RcGetDirOffsetY( dir);

						// skip neighbours which are out of bounds
						if( dx < 0 || dy < 0 || dx >= w || dy >= h)
						{
							tMinHeight = min( tMinHeight, -walkableClimb - bottom);
							continue;
						}

						// From minus infinity to the first span
						SRcSpan* ns   = solid.m_pSpans[dx + dy*w];
						int      nBot = -walkableClimb;
						int      nTop = ns ? (int)ns->m_iMinHeight : MAX_HEIGHT;

						// skip neightbour if the gap between the spans is too small
						if( min( top, nTop) - max( bottom, nBot) > walkableHeight)
							tMinHeight = min( tMinHeight, nBot - bottom);

						// reset of the spans
						for ( ns = solid.m_pSpans[dx + dy*w]; ns; ns = ns->m_pNext)
						{
							nBot = (int)ns->m_iMaxHeight;
							nTop =  ns->m_pNext ? (int)ns->m_pNext->m_iMinHeight : MAX_HEIGHT;

							// skip neightbour if the gap between the spans is too small
							if( min( top, nTop) - max( bottom, nBot) > walkableHeight)
							{
								tMinHeight = min( tMinHeight, nBot - bottom);

								// find min/max accessible neighbour height
								if( abs( nBot-bottom) <= walkableClimb)
								{
									if( nBot<asmin) asmin=nBot;
									if( nBot>asmin) asmax=nBot;
								}
							}
						}
					}

					// The current span is close to a ledge if the drop to any
					// neighbour span is less than the walkableClimb.
					if (tMinHeight < -walkableClimb)
						pSpan->m_areaID = RC_NULL_AREA;

					// If the difference between all neighbours is too large,,
					// we are at steep slope, mark the span as ledge
					if( (asmax-asmin)>walkableClimb)
						pSpan->m_areaID = RC_NULL_AREA;
				}
	}

	// remove walkable flags from all spans which have smaller than 'walkableHeight'
	// clearane above them
	// Params:
	// walkable height	- minimum height where the agent can still walk
	// solid            - heightfield describing the solid space
	void RcFilterWalkableLowHeightSpans( int walkableHeight, SRcHeightField& solid)
	{
		const int w = solid.m_iWidth;
		const int h = solid.m_iHeight;
		const int MAX_HEIGHT = 0xFFFF;

		// remove walkable flag from spans which do not have enough
		// space above them for the agent to stand there
		for ( int y=0; y<h; y++)
			for ( int x =0; x<w; x++)
				for ( SRcSpan* pSpan=solid.m_pSpans[x+y*w]; pSpan; pSpan=pSpan->m_pNext)
				{
					const int bottom = (int)pSpan->m_iMaxHeight;
					const int top    = (int)pSpan->m_pNext ? (int)pSpan->m_pNext->m_iMinHeight : MAX_HEIGHT;

					if(( top-bottom) <= walkableHeight)
						pSpan->m_areaID = RC_NULL_AREA;
				}
	}

	// Get span count by flag
	static int RcGetHeightFieldSpanCount( SRcHeightField& hf)
	{
		const int w = hf.m_iWidth;
		const int h = hf.m_iHeight;

		int spanCount = 0;
		for ( int y=0; y<h; y++)
		{
			for ( int x=0; x<w; x++)
			{
				for ( SRcSpan* pSpan = hf.m_pSpans[x+y*w]; pSpan; pSpan = pSpan->m_pNext)
				{
					if( pSpan->m_areaID != RC_NULL_AREA)
						spanCount++;
				}
			}
		}

		return spanCount;
	}

	// 设置链接
	static inline void RcSetCon( RcCompactSpan& span, int dir, int i)
	{
		const UINT shift = (UINT)dir*6;
		const UINT con   = span.m_conn;

		span.m_conn = (con & ~(0x3f << shift)) | (((unsigned int)i & 0x3f) << shift);
	}

	// Build compact representation of the heightfield
	// Params:
	// walkableHeight - minmum height where the agent can still walk
	// walkableClimb  - maximum height between grid cells the agent can climb
	// hf             - heightfield to be compacted
	// chf            - compact heightfield representing the open space
	// Return false if operation run out of memory
	bool RcBuildCompactHeightfield( const int walkableHeight, const int walkableClimb, SRcHeightField& hf, SRcCompactHeightField& chf)
	{
		const int w			= hf.m_iWidth;
		const int h			= hf.m_iHeight;
		const int spanCount = RcGetHeightFieldSpanCount( hf);

		// fill in header
		chf.m_width	 = w;
		chf.m_height	 = h;
		chf.m_spanCount = spanCount;
		chf.m_iwalkableHeight = walkableHeight;
		chf.m_iwalkableClimb  = walkableClimb;
		chf.m_iMaxRegions = 0;
		chf.m_vMin = hf.m_vMin;
		chf.m_vMax = hf.m_vMax;

		// 包围盒增加walkableHeight
		chf.m_vMax.z += walkableHeight*hf.m_fCellHeight;
		chf.m_cellSize   = hf.m_fCellSize;
		chf.m_cellHeight = hf.m_fCellHeight;
		chf.m_cells.resize(w*h);
		memset( chf.m_cells.data(), 0, sizeof( SRcCompactCell)*w*h);

		chf.m_spans.resize(spanCount);
		memset( chf.m_spans.data(), 0, sizeof( RcCompactSpan)*spanCount);

		// areas
		chf.m_areaIDs.resize(spanCount);
		memset( chf.m_areaIDs.data(), RC_NULL_AREA, sizeof(BYTE)*spanCount);

		const int MAX_HEIGHT = 0xfffffff;

		// Fill in cells and spans( 填充cells and spans)
		int idx = 0;
		for ( int y=0; y<h; y++)
		{
			for ( int x=0; x<w; x++)
			{
				const SRcSpan* pSpan = hf.m_pSpans[x+y*w];

				// If there are no spans at this cell, just leave the data to index=0, count=0
				if( !pSpan)
					continue;

				SRcCompactCell& cell = chf.m_cells[x+y*w];
				cell.m_index = idx;
				cell.m_count = 0;
				while ( pSpan)
				{
					if( pSpan->m_areaID != RC_NULL_AREA)
					{
						const int bottom = (int)pSpan->m_iMaxHeight;
						const int top    = (int)pSpan->m_pNext ? (int)pSpan->m_pNext->m_iMinHeight : MAX_HEIGHT;

						chf.m_spans[idx].m_z			= Clamp( bottom, 0, MAX_HEIGHT);
						chf.m_spans[idx].m_height		= Clamp( top - bottom, 0, 0xffff);
						chf.m_areaIDs[idx]				= pSpan->m_areaID;

						idx++;
						cell.m_count++;
					}
					pSpan = pSpan->m_pNext;
				}
			}
		}

		// Find neighbour connections
		const int MAX_LAYERS	   = RC_NOT_CONNECTED - 1;
		int		  tooHighNeighbour = 0;
		for ( int y=0; y<h; y++)
		{
			for ( int x=0; x<w; x++)
			{
				const SRcCompactCell& cell = chf.m_cells[x+y*w];
				for ( int i=(int)cell.m_index, ni=(int)(cell.m_index+cell.m_count); i<ni; i++)
				{
					RcCompactSpan& tSpan = chf.m_spans[i];
					for ( int dir=0; dir<4; dir++)
					{
						RcSetCon( tSpan, dir, RC_NOT_CONNECTED);
						const int nx = x + RcGetDirOffsetX( dir);
						const int ny = y + RcGetDirOffsetY( dir);

						// First check that the neighbour cell is in bounds
						if( nx < 0 || ny<0 || nx>=w || ny>=h)
							continue;

						// Iterate over all neighbour spans and check if any of the is
						// accessible from current cell
						const SRcCompactCell& nc = chf.m_cells[nx+ny*w];
						for ( int k=(int)nc.m_index, nk=(int)(nc.m_index+nc.m_count); k<nk; k++)
						{
							const RcCompactSpan& ns = chf.m_spans[k];
							const int bottom = max( tSpan.m_z, ns.m_z);
							const int top    = min( tSpan.m_z + tSpan.m_height, ns.m_z + ns.m_height);

							// check that the gap between the spans is walkable
							// and that the climb height between the gaps is not too high
							if( ( top - bottom)>=walkableHeight && abs( (int)ns.m_z - (int)tSpan.m_z) <= walkableClimb)
							{
								// Mark direction as walkable
								const int lidx = k-(int)nc.m_index;
								if( lidx<0 || lidx>MAX_LAYERS)
								{
									tooHighNeighbour = max( tooHighNeighbour, lidx);
									continue;
								}

								RcSetCon( tSpan, dir, lidx);
								break;
							}
						}
					}
				}
			}
		}

		if (tooHighNeighbour > MAX_LAYERS)
		{
			// Warning
		}

		return true;
	}

	// Basically any spans that are closer to a boundary or obstruction than the specified radius
	// are marked as unwalkable
	//
	// This method is usually called immediately after the heightfield has been built
	// See RcCompactHeightfield, RcBuildCompactHeightField RcConfig::walkableRadius
	bool RcErodeWalkableArea( int radius, SRcCompactHeightField& chf)
	{
		const int w = chf.m_width;
		const int h = chf.m_height;

		// Init distance
		vector<BYTE> dist(chf.m_spanCount);
		std::memset( dist.data(), 0xff, sizeof(BYTE)*chf.m_spanCount);

		// Mark  boundary cells
		for ( int y=0; y<h; y++)
		{
			for( int x=0; x<w; x++)
			{
				const SRcCompactCell& c = chf.m_cells[x+y*w];
				for (int i = (int)c.m_index, ni = (int)(c.m_index+c.m_count); i < ni; ++i)
				{
					if (chf.m_areaIDs[i] == RC_NULL_AREA)
					{
						dist[i] = 0;
					}
					else
					{
						const RcCompactSpan& s = chf.m_spans[i];
						int nc = 0;
						for (int dir = 0; dir < 4; ++dir)
						{
							if (RcGetCon(s, dir) != RC_NOT_CONNECTED)
							{
								const int nx = x + RcGetDirOffsetX(dir);
								const int ny = y + RcGetDirOffsetY(dir);
								const int nidx = (int)chf.m_cells[nx+ny*w].m_index + RcGetCon(s, dir);

								int xa = (int)chf.m_cells[nx+ny*w].m_index;
								int xcount = chf.m_cells[nx+ny*w].m_count;
								int xb = RcGetCon(s, dir);


								if (chf.m_areaIDs[nidx] != RC_NULL_AREA)
								{
									nc++;
								}
							}
						}
						// At least one missing neighbour.
						if (nc != 4)
							dist[i] = 0;
					}
				}
			}
		}

		unsigned char nd;

		// Pass 1
		for (int y = 0; y < h; ++y)
		{
			for (int x = 0; x < w; ++x)
			{
				const SRcCompactCell& c = chf.m_cells[x+y*w];
				for (int i = (int)c.m_index, ni = (int)(c.m_index+c.m_count); i < ni; ++i)
				{
					const RcCompactSpan& s = chf.m_spans[i];

					if (RcGetCon(s, 0) != RC_NOT_CONNECTED)
					{
						// (-1,0)
						const int ax = x + RcGetDirOffsetX(0);
						const int ay = y + RcGetDirOffsetY(0);
						const int ai = (int)chf.m_cells[ax+ay*w].m_index + RcGetCon(s, 0);
						const RcCompactSpan& as = chf.m_spans[ai];
						nd = (unsigned char)min((int)dist[ai]+2, 255);
						if (nd < dist[i])
							dist[i] = nd;

						// (-1,-1)
						if (RcGetCon(as, 3) != RC_NOT_CONNECTED)
						{
							const int aax = ax + RcGetDirOffsetX(3);
							const int aay = ay + RcGetDirOffsetY(3);
							const int aai = (int)chf.m_cells[aax+aay*w].m_index + RcGetCon(as, 3);
							nd = (unsigned char)min((int)dist[aai]+3, 255);
							if (nd < dist[i])
								dist[i] = nd;
						}
					}
					if (RcGetCon(s, 3) != RC_NOT_CONNECTED)
					{
						// (0,-1)
						const int ax = x + RcGetDirOffsetX(3);
						const int ay = y + RcGetDirOffsetY(3);
						const int ai = (int)chf.m_cells[ax+ay*w].m_index + RcGetCon(s, 3);
						const RcCompactSpan& as = chf.m_spans[ai];
						nd = (unsigned char)min((int)dist[ai]+2, 255);
						if (nd < dist[i])
							dist[i] = nd;

						// (1,-1)
						if (RcGetCon(as, 2) != RC_NOT_CONNECTED)
						{
							const int aax = ax + RcGetDirOffsetX(2);
							const int aay = ay + RcGetDirOffsetY(2);
							const int aai = (int)chf.m_cells[aax+aay*w].m_index + RcGetCon(as, 2);
							nd = (unsigned char)min((int)dist[aai]+3, 255);
							if (nd < dist[i])
								dist[i] = nd;
						}
					}
				}
			}
		}

		// Pass 2
		for (int y = h-1; y >= 0; --y)
		{
			for (int x = w-1; x >= 0; --x)
			{
				const SRcCompactCell& c = chf.m_cells[x+y*w];
				for (int i = (int)c.m_index, ni = (int)(c.m_index+c.m_count); i < ni; ++i)
				{
					const RcCompactSpan& s = chf.m_spans[i];

					if ( RcGetCon(s, 2) != RC_NOT_CONNECTED)
					{
						// (1,0)
						const int ax = x + RcGetDirOffsetX(2);
						const int ay = y + RcGetDirOffsetY(2);
						const int ai = (int)chf.m_cells[ax+ay*w].m_index + RcGetCon(s, 2);
						const RcCompactSpan& as = chf.m_spans[ai];
						nd = (unsigned char)min((int)dist[ai]+2, 255);
						if (nd < dist[i])
							dist[i] = nd;

						// (1,1)
						if ( RcGetCon(as, 1) != RC_NOT_CONNECTED)
						{
							const int aax = ax + RcGetDirOffsetX(1);
							const int aay = ay + RcGetDirOffsetY(1);
							const int aai = (int)chf.m_cells[aax+aay*w].m_index + RcGetCon(as, 1);
							nd = (unsigned char)min((int)dist[aai]+3, 255);
							if (nd < dist[i])
								dist[i] = nd;
						}
					}
					if ( RcGetCon(s, 1) != RC_NOT_CONNECTED)
					{
						// (0,1)
						const int ax = x + RcGetDirOffsetX(1);
						const int ay = y + RcGetDirOffsetY(1);
						const int ai = (int)chf.m_cells[ax+ay*w].m_index + RcGetCon(s, 1);
						const RcCompactSpan& as = chf.m_spans[ai];
						nd = (unsigned char)min((int)dist[ai]+2, 255);
						if (nd < dist[i])
							dist[i] = nd;

						// (-1,1)
						if ( RcGetCon(as, 0) != RC_NOT_CONNECTED)
						{
							const int aax = ax + RcGetDirOffsetX(0);
							const int aay = ay + RcGetDirOffsetY(0);
							const int aai = (int)chf.m_cells[aax+aay*w].m_index + RcGetCon(as, 0);
							nd = (unsigned char)min((int)dist[aai]+3, 255);
							if (nd < dist[i])
								dist[i] = nd;
						}
					}
				}
			}
		}

		const unsigned char thr = (unsigned char)(radius*2);
		for (int i = 0; i < chf.m_spanCount; ++i)
			if (dist[i] < thr)
				chf.m_areaIDs[i] = RC_NULL_AREA;

		return true;
	}

	// The value of spacial parameters are in world units.
	void RcMarkConvexPolyArea( const RcConvexVolume& volume, SRcCompactHeightField& chf)
	{
		if( !volume.m_shape) return;

		Rect3 aabb;
		volume.m_shape->BuildAABB( aabb, Transform());
		const Vector3& bmin = aabb.m_min;
		const Vector3& bmax = aabb.m_max;

		int minx = (int)((bmin[0]-chf.m_vMin[0])/chf.m_cellSize);
		int miny = (int)((bmin[1]-chf.m_vMin[1])/chf.m_cellSize);
		int minz = (int)((bmin[2]-chf.m_vMin[2])/chf.m_cellHeight);
		int maxx = (int)((bmax[0]-chf.m_vMin[0])/chf.m_cellSize);
		int maxy = (int)((bmax[1]-chf.m_vMin[1])/chf.m_cellSize);
		int maxz = (int)((bmax[2]-chf.m_vMin[2])/chf.m_cellHeight);

		if( maxx<0) return;
		if( minx>=chf.m_width) return;
		if( maxy<0) return;
		if( miny>=chf.m_height) return;

		if( minx<0) minx=0;
		if( maxx>=chf.m_width) maxx = chf.m_width-1;
		if( miny<0) miny = 0;
		if( maxy>=chf.m_height) maxy=chf.m_height-1;

		// TODO: Optimize.
		for (int y = miny; y <= maxy; ++y)
		{
			for (int x = minx; x <= maxx; ++x)
			{
				const SRcCompactCell& c = chf.m_cells[x+y*chf.m_width];
				for (int i = (int)c.m_index, ni = (int)(c.m_index+c.m_count); i < ni; ++i)
				{
					RcCompactSpan& s = chf.m_spans[i];
					if (chf.m_areaIDs[i] == RC_NULL_AREA)
						continue;

					if ((int)s.m_z >= minz && (int)s.m_z <= maxz)
					{
						Vector3 p;
						p[0] = chf.m_vMin[0] + (x+0.5f)*chf.m_cellSize;
						p[1] = chf.m_vMin[1] + (y+0.5f)*chf.m_cellSize; 
						p[2] = chf.m_vMin[2] + s.m_z * chf.m_cellHeight;

						if ( volume.m_shape->IsPointIn( p))
						{
							chf.m_areaIDs[i] = volume.m_area;
						}
					}
				}
			}
		}
	}

	// 辅助函数
	static int GetCornerHeight(int x, int y, int i, int dir, const SRcCompactHeightField& chf, bool& isBorderVertex)
	{
		const RcCompactSpan& s = chf.m_spans[i];
		int ch   = s.m_z;
		int dirp = (dir+1) & 0x3;

		unsigned int regs[4] = {0,0,0,0};

		regs[0] = s.m_regID | (chf.m_areaIDs[i]<<16);

		if ( RcGetCon(s, dir) != RC_NOT_CONNECTED)
		{
			const int ax = x + RcGetDirOffsetX(dir);
			const int ay = y + RcGetDirOffsetY(dir);
			const int ai = (int)chf.m_cells[ax+ay*chf.m_width].m_index + RcGetCon(s, dir);
			const RcCompactSpan& as = chf.m_spans[ai];
			ch = max(ch, as.m_z);
			regs[1] = as.m_regID | (chf.m_areaIDs[ai]<<16);

			if (RcGetCon(as, dirp) != RC_NOT_CONNECTED)
			{
				const int ax2 = ax + RcGetDirOffsetX(dirp);
				const int ay2 = ay + RcGetDirOffsetY(dirp);
				const int ai2 = (int)chf.m_cells[ax2+ay2*chf.m_width].m_index + RcGetCon(as, dirp);
				const RcCompactSpan& as2 = chf.m_spans[ai2];
				ch = max(ch, as2.m_z);
				regs[2] = as2.m_regID | (chf.m_areaIDs[ai2]<<16);
			}
		}

		if (RcGetCon(s, dirp) != RC_NOT_CONNECTED)
		{
			const int ax = x + RcGetDirOffsetX(dirp);
			const int ay = y + RcGetDirOffsetY(dirp);
			const int ai = (int)chf.m_cells[ax+ay*chf.m_width].m_index + RcGetCon(s, dirp);
			const RcCompactSpan& as = chf.m_spans[ai];
			ch = max(ch, as.m_z);
			regs[3] = as.m_regID | (chf.m_areaIDs[ai]<<16);
			if ( RcGetCon(as, dir) != RC_NOT_CONNECTED)
			{
				const int ax2 = ax + RcGetDirOffsetX(dir);
				const int ay2 = ay + RcGetDirOffsetY(dir);
				const int ai2 = (int)chf.m_cells[ax2+ay2*chf.m_width].m_index + RcGetCon(as, dir);
				const RcCompactSpan& as2 = chf.m_spans[ai2];
				ch = max(ch, as2.m_z);
				regs[2] = as2.m_regID | (chf.m_areaIDs[ai2]<<16);
			}
		}

		// Check if the vertex is special edge vertex, these vertices will be removed later.
		for (int j = 0; j < 4; ++j)
		{
			const int a = j;
			const int b = (j+1) & 0x3;
			const int c = (j+2) & 0x3;
			const int d = (j+3) & 0x3;

			// The vertex is a border vertex there are two same exterior cells in a row,
			// followed by two interior cells and none of the regions are out of bounds.
			const bool twoSameExts = (regs[a] & regs[b] & RC_BORDER_REG) != 0 && regs[a] == regs[b];
			const bool twoInts = ((regs[c] | regs[d]) & RC_BORDER_REG) == 0;
			const bool intsSameArea  = (regs[c]>>16) == (regs[d]>>16);
			const bool noZeros = regs[a] != 0 && regs[b] != 0 && regs[c] != 0 && regs[d] != 0;
			if (twoSameExts && twoInts && intsSameArea && noZeros)
			{
				isBorderVertex = true;
				break;
			}
		}

		return ch;
	}

	// 未知作用
	static void WalkContour(int x, int y, int i,SRcCompactHeightField& chf, unsigned char* flags, RcIntArray& points)
	{
		// Choose the first non-connected edge
		unsigned char dir = 0;
		while ((flags[i] & (1 << dir)) == 0)
			dir++;

		unsigned char startDir = dir;
		int starti = i;

		const unsigned char area = chf.m_areaIDs[i];

		int iter = 0;
		while (++iter < 40000)
		{
			if (flags[i] & (1 << dir))
			{
				// Choose the edge corner
				bool isBorderVertex = false;
				bool isAreaBorder	= false;
				int px = x;
				int py = y;
				int pz = GetCornerHeight(x, y, i, dir, chf, isBorderVertex);
				switch(dir)
				{
				case 0: py++; break;
				case 1: px++; py++; break;
				case 2: px++; break;
				}
				int r = 0;
				const RcCompactSpan& s = chf.m_spans[i];
				if ( RcGetCon(s, dir) != RC_NOT_CONNECTED)
				{
					const int ax = x + RcGetDirOffsetX(dir);
					const int ay = y + RcGetDirOffsetY(dir);
					const int ai = (int)chf.m_cells[ax+ay*chf.m_width].m_index + RcGetCon(s, dir);
					r = (int)chf.m_spans[ai].m_regID;
					if( area != chf.m_areaIDs[ai])
						isAreaBorder = true;
				}
				if (isBorderVertex)
					r |= RC_BORDER_VERTEX;
				if( isAreaBorder)
					r |= RC_AREA_BORDER;

				points.push_back(px);
				points.push_back(py);
				points.push_back(pz);
				points.push_back(r);

				flags[i] &= ~(1 << dir); // Remove visited edges
				dir = (dir+1) & 0x3;	 // Rotate CW
			}
			else
			{
				int ni = -1;
				const int nx = x + RcGetDirOffsetX(dir);
				const int ny = y + RcGetDirOffsetY(dir);
				const RcCompactSpan& s = chf.m_spans[i];
				if ( RcGetCon(s, dir) != RC_NOT_CONNECTED)
				{
					const SRcCompactCell& nc = chf.m_cells[nx+ny*chf.m_width];
					ni = (int)nc.m_index + RcGetCon(s, dir);
				}
				if (ni == -1)
				{
					// Should not happen.
					return;
				}
				x = nx;
				y = ny;
				i = ni;
				dir = (dir+3) & 0x3;	// Rotate CCW
			}

			if (starti == i && startDir == dir)
			{
				break;
			}
		}
	}

	// 求距离
	static float DistancePtSeg(int x, int y, int px, int py, int qx, int qy)
	{
	/*	float pqx = (float)(qx - px);
		float pqy = (float)(qy - py);
		float pqz = (float)(qz - pz);
		float dx = (float)(x - px);
		float dy = (float)(y - py);
		float dz = (float)(z - pz);
		float d = pqx*pqx + pqy*pqy + pqz*pqz;
		float t = pqx*dx + pqy*dy + pqz*dz;
		if (d > 0)
			t /= d;
		if (t < 0)
			t = 0;
		else if (t > 1)
			t = 1;
	
		dx = px + t*pqx - x;
		dy = py + t*pqy - y;
		dz = pz + t*pqz - z;
	
		return dx*dx + dy*dy + dz*dz;*/

		float pqx = (float)(qx - px);
		float pqy = (float)(qy - py);
		float dx = (float)(x - px);
		float dy = (float)(y - py);
		float d = pqx*pqx + pqy*pqy;
		float t = pqx*dx + pqy*dy;
		if (d > 0)
			t /= d;
		if (t < 0)
			t = 0;
		else if (t > 1)
			t = 1;
	
		dx = px + t*pqx - x;
		dy = py + t*pqy - y;
	
		return dx*dx + dy*dy;
	}

	// 简化contour
	static void SimplifyContour(RcIntArray& points, RcIntArray& simplified, float maxError, int maxEdgeLen, const int buildFlags)
	{
		// Add initial points.
		bool hasConnections = false;
		for ( int i = 0; i < (int)points.size(); i += 4)
		{
			// 用最后的标志位进行判断
			if ((points[i+3] & RC_CONTOUR_REG_MASK) != 0)
			{
				hasConnections = true;
				break;
			}
		}

		if( hasConnections)
		{
			// The contour has some portals to other regions.
			// Add a new point to every location where the region changes.
			for (int i = 0, ni = points.size()/4; i < ni; ++i)
			{
				int ii = (i+1) % ni;
				const bool differentRegs = (points[i*4+3] & RC_CONTOUR_REG_MASK) != (points[ii*4+3] & RC_CONTOUR_REG_MASK);
				const bool areaBorders = (points[i*4+3] & RC_AREA_BORDER) != (points[ii*4+3] & RC_AREA_BORDER);
				if (differentRegs || areaBorders)
				{
					simplified.push_back(points[i*4+0]);
					simplified.push_back(points[i*4+1]);
					simplified.push_back(points[i*4+2]);
					simplified.push_back(i);
				}
			}    
		}


		if ( simplified.size()==0)
		{
			// If there is no connections at all,
			// create some initial points for the simplification process. 
			// Find lower-left and upper-right vertices of the contour.
			int llx = points[0];
			int lly = points[1];
			int llz = points[2];
			int lli = 0;
			int urx = points[0];
			int ury = points[1];
			int urz = points[2];
			int uri = 0;
			for ( size_t i = 0; i < points.size(); i += 4)
			{
				int x = points[i+0];
				int y = points[i+1];
				int z = points[i+2];
				if (x < llx || (x == llx && y < lly))
				{
					llx = x;
					lly = y;
					llz = z;
					lli = i/4;
				}
				if (x >= urx || (x == urx && y > ury))
				{
					urx = x;
					ury = y;
					urz = z;
					uri = i/4;
				}
			}
			simplified.push_back(llx);
			simplified.push_back(lly);
			simplified.push_back(llz);
			simplified.push_back(lli);

			simplified.push_back(urx);
			simplified.push_back(ury);
			simplified.push_back(urz);
			simplified.push_back(uri);
		}

		// Add points until all raw points are within
		// error tolerance to the simplified shape.
		const int pn = points.size()/4;
		for ( int i = 0; i < (int)simplified.size()/4; )
		{
			int ii = (i+1) % (simplified.size()/4);

			int ax = simplified[i*4+0];
			int ay = simplified[i*4+1];
			int ai = simplified[i*4+3];

			int bx = simplified[ii*4+0];
			int by = simplified[ii*4+1];
			int bi = simplified[ii*4+3];

			// Find maximum deviation from the segment.
			float maxd = 0;
			int	  maxi = -1;
			int	  ci, cinc, endi;

			// Traverse the segment in lexilogical order so that the
			// max deviation is calculated similarly when travering
			// opposite sements
			if( bx>ax || (bx==ax && by>ay))
			{
				cinc = 1;
				ci   = (ai+cinc) % pn;
				endi = bi;
			}
			else
			{
				cinc = pn - 1;
				ci   = (bi+cinc) % pn;
				endi = ai;
			}

			// Tesselate only outer edges.
			if ((points[ci*4+3] & RC_CONTOUR_REG_MASK) == 0 || (points[ci*4+3] &  RC_AREA_BORDER))
			{
				while (ci != endi)
				{
					float d = DistancePtSeg(points[ci*4+0], points[ci*4+1], ax, ay, bx, by);
					if (d > maxd)
					{
						maxd = d;
						maxi = ci;
					}
					ci = (ci+cinc) % pn;
				}
			}


			// If the max deviation is larger than accepted error,
			// add new point, else continue to next segment.
			if (maxi != -1 && maxd > (maxError*maxError))
			{
				// Add space for the new point.
				simplified.resize(simplified.size()+4);
				int n = ((int)simplified.size())/4;
				for (int j = n-1; j > i; --j)
				{
					simplified[j*4+0] = simplified[(j-1)*4+0];
					simplified[j*4+1] = simplified[(j-1)*4+1];
					simplified[j*4+2] = simplified[(j-1)*4+2];
					simplified[j*4+3] = simplified[(j-1)*4+3];
				}
				// Add the point.
				simplified[(i+1)*4+0] = points[maxi*4+0];
				simplified[(i+1)*4+1] = points[maxi*4+1];
				simplified[(i+1)*4+2] = points[maxi*4+2];
				simplified[(i+1)*4+3] = maxi;
			}
			else
			{
				++i;
			}
		}

		// Split too long edges.
		if (maxEdgeLen > 0 && (buildFlags & (RC_CONTOUR_TESS_AREA_EDGES | RC_CONTOUR_TESS_WALL_EDGES)) != 0)
		{
			for ( int i = 0; i < (int)simplified.size()/4; )
			{
				int ii = (i+1) % (simplified.size()/4);

				int ax = simplified[i*4+0];
				int ay = simplified[i*4+1];
				int ai = simplified[i*4+3];

				int bx = simplified[ii*4+0];
				int by = simplified[ii*4+1];
				int bi = simplified[ii*4+3];

				// Find maximum deviation from the segment.
				int maxi = -1;
				int ci = (ai+1) % pn;

				// Tessellate only outer edges or edges between areas
				bool tess = false;
				// Wall edges.
				if ((buildFlags & RC_CONTOUR_TESS_WALL_EDGES) && (points[ci*4+3] & RC_CONTOUR_REG_MASK) == 0)
					tess = true;
				// Edges between areas.
				if ((buildFlags & RC_CONTOUR_TESS_AREA_EDGES) && (points[ci*4+3] & RC_AREA_BORDER))
					tess = true;

				if( tess)
				{
					int dx = bx - ax;
					int dy = by - ay;
					if (dx*dx + dy*dy > maxEdgeLen*maxEdgeLen)
					{
						// Round based on the segments in lexilogical order so that the
						// max tesselation is consistent regardles in which direction
						// segments are traversed.
						const int n = bi < ai ? (bi+pn - ai) : (bi - ai);
						if (n > 1)
						{
							if (bx > ax || (bx == ax && by > ay))
								maxi = (ai + n/2) % pn;
							else
								maxi = (ai + (n+1)/2) % pn;
						}
					}
				}

				// If the max deviation is larger than accepted error,
				// add new point, else continue to next segment.
				if (maxi != -1)
				{
					// Add space for the new point.
					simplified.resize(simplified.size()+4);
					int n = ((int)simplified.size())/4;
					for (int j = n-1; j > i; --j)
					{
						simplified[j*4+0] = simplified[(j-1)*4+0];
						simplified[j*4+1] = simplified[(j-1)*4+1];
						simplified[j*4+2] = simplified[(j-1)*4+2];
						simplified[j*4+3] = simplified[(j-1)*4+3];
					}
					// Add the point.
					simplified[(i+1)*4+0] = points[maxi*4+0];
					simplified[(i+1)*4+1] = points[maxi*4+1];
					simplified[(i+1)*4+2] = points[maxi*4+2];
					simplified[(i+1)*4+3] = maxi;
				}
				else
				{
					++i;
				}
			}
		}

		for ( size_t i = 0; i < simplified.size()/4; ++i)
		{
			// The edge vertex flag is take from the current raw point,
			// and the neighbour region is take from the next raw point.
			const int ai = (simplified[i*4+3]+1) % pn;
			const int bi = simplified[i*4+3];
			simplified[i*4+3] = (points[ai*4+3] & (RC_CONTOUR_REG_MASK|RC_AREA_BORDER)) | (points[bi*4+3] & RC_BORDER_VERTEX);
		}

	}

	// 移除片段
	static void RemoveDegenerateSegments(RcIntArray& simplified)
	{
		// Remove adjacent vertices which are equal on xy-plane,
		// or else the triangulator will get confused.
		for ( size_t i = 0; i < simplified.size()/4; ++i)
		{
			int ni = i+1;
			if (ni >= ((int)simplified.size()/4))
				ni = 0;

			if (simplified[i*4+0] == simplified[ni*4+0] && simplified[i*4+1] == simplified[ni*4+1])
			{
				// Degenerate segment, remove.
				for (int j = i; j < (int)simplified.size()/4-1; ++j)
				{
					simplified[j*4+0] = simplified[(j+1)*4+0];
					simplified[j*4+1] = simplified[(j+1)*4+1];
					simplified[j*4+2] = simplified[(j+1)*4+2];
					simplified[j*4+3] = simplified[(j+1)*4+3];
				}
				simplified.pop_back();
			}
		}
	}

	// 计算面积
	static int CalcAreaOfPolygon2D(const int* verts, const int nverts)
	{
		int area = 0;
		for (int i = 0, j = nverts-1; i < nverts; j=i++)
		{
			const int* vi = &verts[i*4];
			const int* vj = &verts[j*4];
			area += vi[0] * vj[1] - vj[0] * vi[1];
		}
		return (area+1) / 2;
	}

	// 获取封闭索引
	static void GetClosestIndices(const int* vertsa, const int nvertsa, const int* vertsb, const int nvertsb,int& ia, int& ib)
	{
		int closestDist = 0xfffffff;
		for (int i = 0; i < nvertsa; ++i)
		{
			const int* va = &vertsa[i*4];
			for (int j = 0; j < nvertsb; ++j)
			{
				const int* vb = &vertsb[j*4];
				const int dx = vb[0] - va[0];
				const int dy = vb[1] - va[1];
				const int d = dx*dx + dy*dy;
				if (d < closestDist)
				{
					ia = i;
					ib = j;
					closestDist = d;
				}
			}
		}
	}

	// 合并cb 到 ca
	static bool MergeContours( SRcContour& ca, SRcContour& cb, int ia, int ib)
	{
		const int maxVerts = ca.m_iNumVerts + cb.m_iNumVerts + 2;
		vector<int> verts(maxVerts*4);

		int nv = 0;

		// Copy contour A.
		for (int i = 0; i <= ca.m_iNumVerts; ++i)
		{
			int* dst = &verts[nv*4];
			const int* src = &ca.m_verts[((ia+i)%ca.m_iNumVerts)*4];
			dst[0] = src[0];
			dst[1] = src[1];
			dst[2] = src[2];
			dst[3] = src[3];
			nv++;
		}

		// Copy contour B
		for (int i = 0; i <= cb.m_iNumVerts; ++i)
		{
			int* dst = &verts[nv*4];
			const int* src = &cb.m_verts[((ib+i)%cb.m_iNumVerts)*4];
			dst[0] = src[0];
			dst[1] = src[1];
			dst[2] = src[2];
			dst[3] = src[3];
			nv++;
		}

		ca.m_verts = verts;
		ca.m_iNumVerts = nv;

		cb.m_verts.clear();
		cb.m_iNumVerts = 0;

		return true;
	}

	// Builds simplified contours from the regions outlines.
	// Params:
	// chf        - (in) compact heightfield which has regions set.
	// maxError   - (in) maximum allowed distance between simplified countour and cells.
	// maxEdgeLen - (in) maximum allowed contour edge length in cells.
	// cset       - (out) Resulting contour set.
	// Returns false if operation ran out of memory.
	bool RcBuildContours(SRcCompactHeightField& chf, const float maxError, const int maxEdgeLen, SRcContourSet& cset, const int buildFlags)
	{
		const int w			 = chf.m_width;
		const int h			 = chf.m_height;
		const int borderSize = chf.m_borderSize;
	
		// boundig box
		cset.m_vMin = chf.m_vMin;
		cset.m_vMax = chf.m_vMax;
		if( borderSize > 0)
		{
			// If the heightfield was build with bordersize, remove the offset
			const float pad = borderSize * chf.m_cellSize;
			cset.m_vMin += Vector3( pad, pad, 0.f);
			cset.m_vMax -= Vector3( pad, pad, 0.f);
		}
	
		cset.m_fCellSize	= chf.m_cellSize;
		cset.m_fCellHeight	= chf.m_cellHeight;
		cset.m_width		= chf.m_width - chf.m_borderSize * 2;
		cset.m_height		= chf.m_height- chf.m_borderSize * 2;
		cset.m_borderSize	= chf.m_borderSize;

		// 分配
		int maxContours = std::max<INT>( chf.m_iMaxRegions, 8);
		cset.m_contours.resize(maxContours);
		cset.m_iNumContours = 0;

		// 标记
		vector<BYTE> flags( chf.m_spanCount);
	
		// Mark boundaries
		for ( int y=0; y<h; y++)
		{
			for ( int x=0; x<w; x++)
			{
				const SRcCompactCell& cell = chf.m_cells[x+y*w];
				for ( int i=(int)cell.m_index, ni=(int)(cell.m_index+cell.m_count); i<ni; i++)
				{
					unsigned char res = 0;
					const RcCompactSpan& span = chf.m_spans[i];

					if( !span.m_regID || ( span.m_regID & RC_BORDER_REG))
					{
						flags[i] = 0;
						continue;
					}

					for( int dir=0; dir<4; dir++)
					{
						unsigned short r = 0;
						if( RcGetCon( span, dir) != RC_NOT_CONNECTED)
						{
							const int ax = x + RcGetDirOffsetX( dir);
							const int ay = y + RcGetDirOffsetY( dir);
							const int ai = (int)chf.m_cells[ax+ay*w].m_index + RcGetCon( span, dir);
							r = chf.m_spans[ai].m_regID;
						}

						if( r == span.m_regID)
							res |= (1<<dir);
					}

					// Inverse, mark non connected edges
					flags[i] = res ^ 0xf;
				}
			}
		}

		RcIntArray verts(256);
		RcIntArray simplified(64);
					
		for( int y=0; y<h; y++)
		{
			for( int x=0; x<w; x++)
			{
				const SRcCompactCell& cell = chf.m_cells[x+y*w];
				for ( int i= (int)cell.m_index, ni=(int)(cell.m_index+cell.m_count); i<ni; i++)
				{
					if( flags[i] == 0 || flags[i] == 0xf)
					{
						flags[i] = 0;
						continue;
					}

					unsigned short reg = chf.m_spans[i].m_regID;
					if( !reg || (reg & RC_BORDER_REG))
						continue;

					const unsigned char area = chf.m_areaIDs[i];

					verts.resize( 0);
					simplified.resize( 0);

					WalkContour( x, y, i, chf, flags.data(), verts);
					SimplifyContour( verts, simplified, maxError, maxEdgeLen, buildFlags);
					RemoveDegenerateSegments( simplified);

					// Store region->contour remap info
					// Create contour
					if( simplified.size()/4 >= 3)
					{
						if( cset.m_iNumContours >= maxContours)
						{
							// Allocate more contours
							// This can happen when there are tiny holes in the heightfield
							maxContours*=2;
							cset.m_contours.resize( maxContours);
						}

						SRcContour* pCont = &cset.m_contours[cset.m_iNumContours++];

						pCont->m_iNumVerts = simplified.size() / 4;
						pCont->m_verts.resize(pCont->m_iNumVerts*4);
						memcpy( pCont->m_verts.data(), &simplified[0], sizeof(int) * pCont->m_iNumVerts * 4);
						if( borderSize>0)
						{
							// If the heightfield was build with bordersize, remove the offset
							for( int j=0; j<pCont->m_iNumVerts; j++)
							{
								int* v  = &pCont->m_verts[j*4];
								v[0]   -= borderSize;
								v[1]   -= borderSize;
							}
						}

						pCont->m_iNumRawVerts = verts.size() / 4;
						pCont->m_rawVerts.resize( pCont->m_iNumRawVerts*4);
						memcpy( pCont->m_rawVerts.data(), &verts[0], sizeof(int) * pCont->m_iNumRawVerts * 4);
						if( borderSize>0)
						{
							// If the heightfield was build with bordersize, remove the offset
							for ( int j=0; j<pCont->m_iNumRawVerts; j++)
							{
								int* v = &pCont->m_rawVerts[j*4];
								v[0] -= borderSize;
								v[1] -= borderSize;
							}
						}

						pCont->m_regionID = reg;
						pCont->m_areaID	  = area;

						A_ASSERT( area != RC_NULL_AREA);
					}
				}
			}
		}

		// Check and merge droppings
		// Sometimes the previous algorithms can fail and create several countours
		// per area. this pass will try to merge the holes into the main region
		for ( int i=0; i<cset.m_iNumContours; i++)
		{
			SRcContour& cont = cset.m_contours[i];

			// check if the contour is would backward
			if ( CalcAreaOfPolygon2D( cont.m_verts.data(), cont.m_iNumVerts)< 0)
			{
				// Find another contour which has the same region ID
				int mergeIdx = -1;
				for ( int j=0; j<cset.m_iNumContours; j++)
				{
					if( i==j)
						continue;

					if( cset.m_contours[j].m_iNumVerts && cset.m_contours[j].m_regionID == cont.m_regionID)
					{
						// Mark sure the polygon is correctly oriented
						if( CalcAreaOfPolygon2D( cset.m_contours[j].m_verts.data(), cset.m_contours[j].m_iNumVerts))
						{
							mergeIdx = j;
							break;
						}
					}
				}

				if( mergeIdx == -1)
				{

				}
				else
				{
					SRcContour& mCont = cset.m_contours[mergeIdx];

					// Merge by closest points
					int ia=0, ib=0;
					GetClosestIndices( mCont.m_verts.data(), mCont.m_iNumVerts, cont.m_verts.data(), cont.m_iNumVerts, ia, ib);
					if( !MergeContours( mCont, cont, ia, ib))
					{
						continue;
					}
				}
			}
		}

		return true;
	}
}