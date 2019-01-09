//
// Copyright (c) 2009 Mikko Mononen memon@inside.org
// Copyright (c) 2011 帝林  http://www.51play.com	
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

#pragma once

// 注：此头文件可不对外发布

#include <Core/AresCore.h>
#include "Physics/WaterZoneMgr.h"
#include "Physics/AresStaticNavMesh.h"

namespace Ares
{
	// the units of the parameters are specified in parenthesis as follows
	// (vx) voxels, (wu) world units
	struct SRcConfig
	{
		int			m_iWidth   , m_iHeight;			// Dimensions of the rasterized heightfield(vx)
		float		m_fCellSize, m_fCellHeight;		// Grid cell size and height (wu)
		Vector3     m_vMin,      m_vMax;			// Grid Bounds(wu)

		float       m_fWalkableSlope;				// Maximum walkable slope angle in degrees.
		int         m_iWalkableHeight;				// Minimum height where the agent can still walk(vx)
		int         m_iWalkableClimb;				// Maximum height between grid cells the agent can climb(vx)
		int			m_iTileSize;					// Width and height of a tile(vx)
		int         m_iBorderSize;					// Non-navigation border around the heightfield(vx)
		int         m_iWalkableRadius;				// Radius of the agent in cells(vx)
		int         m_iMinRegionSize;				// Minimum regions size. Smaller regions will be deleted(vx)
		int         m_iMergeRegionSize;				// Minimum regions size. Smaller regions will be merged(vx)
		float       m_fMaxSimplificationError;		// Maximum distance error from contour to cells
		int         m_iMaxEdgeLen;					// Maximum contour edge length(vx)
		int         m_iMaxVertsPerPolyon;			// max number of vertices per polygon
		float       m_fDetailSampleDist;			// Detail mesh sample spacing
		float       m_fDetailSampleMaxError;		// Detail mesh simplification max sample error

		// 构造函数
		SRcConfig()
		{
			std::memset( this, 0, sizeof(*this));
		}
	};

	// polymesh store a connected mesh of polygons.
	// the polygons are store in an array where each polygons takes
	// 'nvp*2' elements. The first 'nvp' elements are indices to vertices
	// and the second 'nvp' elements are indices to neighbour polygons.
	// if a polygon has less than 'bvp' vertices, the remaining indices are
	// set as -1. if an polygon edge does not have a neighbour, the
	// neighbour index is set to -1.
	// Vertices can be transformed into world space as follows
	//   x = m_min[0] + m_pVerts[i*3+0]*cs;
	//   y = m_min[1] + m_pVerts[i*3+1]*ch;
	//   z = m_min[2] + m_pVerts[i*3+2]*cs;
	struct RcPolyMesh
	{
		vector<Int3>	 m_verts;		// vertices of the mesh, 3 elements per vertex
		vector<UINT>	 m_polys;		// polygons of the mesh, nvp*2 elements per polygon
		vector<UINT>	 m_regs;		// regions of the polygons
		vector<BYTE>	 m_areaIDs;		// the area id assigned for each polygon [length:maxpolys]
		int				 m_iNumVerts;	// Number of vertices;
		int              m_numPolys;	// Number of polygons
		int              m_iNVP;		// Max number of vertices per polygon
		Vector3			 m_vMin, m_vMax;// Bounding box of the mesh
		float            m_fCellSize;	// Cell size
		float			 m_fCellHeight;	// Cell height
		int				 m_borderSize;	// The aabb border size used to

		RcPolyMesh();

		// 获取多边形顶点数
		static int CountPolyVerts( const UINT* p, const int nvp);
	};

	// Detail mesh generated form a polymesh
	// Each submesh represents a polygon in the polymesh and they are stored in
	// excatly same order. Each submesh is described as 4 values:
	// base vertex, vertex count, base triangle, triangle count. That is,
	//		const unsigned char* t = 
	//
	// If the input polygon has 'n' vertices, those vertices are first in the 
	// submesh vertex list. This allows to compres the mesh by not storing the 
	// first vertices and using the polymehs vertices instead
	struct RcDetailPoly
	{
		unsigned int    m_vertsOffset;	// 在添加此poly前,poly集拥有的顶点总数
		unsigned int    m_trisOffset;	// 在添加此poly前,poly集有的三角形数
		unsigned int	m_numVerts;	// 该多边形拥有的顶点数	
		unsigned int	m_numTris;		// 该多边形的三角形数
	};

	struct RcDetailTriangle
	{
		unsigned char  m_iV0Index;		// 顶点0索引
		unsigned char  m_iV1Index;		// 顶点1索引
		unsigned char  m_iV2Index;		// 顶点2索引
		unsigned char  m_flag;			// 标记( 暂未确定其意义)
	};

	struct SRcPolyMeshDetail
	{
		vector<Vector3>			m_verts;		// Pointer to all vertex data
		vector<RcDetailTriangle>m_tris;			// Pointer to all triangle data
		vector<RcDetailPoly>	m_subMeshs;		// Pointer to all mesh data
		int						m_numMeshs;		// Number of polygons;
		int						m_numVerts;		// Number of total vertices
		int						m_numTris;		// Number of triangles

		// 构造函数
		SRcPolyMeshDetail()
		{
			m_numMeshs = 0;
			m_numTris  = 0;
			m_numVerts = 0;
		}

		// 析构函数
		~SRcPolyMeshDetail()
		{
		}
	};

	// HeightField span
	struct SRcSpan
	{
		unsigned int  m_iMinHeight;	// Sapn min height
		unsigned int  m_iMaxHeight;	// Span max height	
		unsigned int  m_areaID;		// The area id assigned to the span
		SRcSpan*      m_pNext;		// Next span in column
	};

	enum RCAreaType
	{	
		RC_NULL_AREA		= 0,	// Represents the null area. When a data element is given this value it is considered to no longer be assigned to a usable area.  (E.g. It is unwalkable.)
		RC_WALK_AREA		= 1,	// The default area id used to indicate a walkable polygon. This is also the maximum allowed area id, and the only non-null area id  recognized by some steps in the build process. 
		RC_ROAD_AREA		= 2,
		RC_WATER_AREA		= 3,
		RC_TOTAL_AREA_TYPE	= 32,
	};

	enum ERcSpanFlags
	{
		EM_RC_REACHABLE  = 0x02,	// can reach

		// The value returned by #RcGetCon if the specified direction is not connected
		// to another span. (Has no neighbor.)
		RC_NOT_CONNECTED = 0x3f,

		/// An value which indicates an invalid index within a mesh.
		/// @note This does not necessarily indicate an error.
		/// @see rcPolyMesh::polys
		RC_MESH_NULL_IDX = INVALID,
	};

	// If heightfield region ID has the following bit set, the region is on border area
	// and excluded from many calculations.
	static const unsigned short RC_BORDER_REG = 0x8000;

	// If contour region ID has the following bit set, the vertex will be later
	// removed in order to match the segments and vertices at tile boundaries.
	static const int RC_BORDER_VERTEX = 0x10000;

	/// Defines the number of bits allocated to rcSpan::smin and rcSpan::smax.
	static const int RC_SPAN_HEIGHT_BITS = 16;
	/// Defines the maximum value for rcSpan::smin and rcSpan::smax.
	static const int RC_SPAN_MAX_HEIGHT = (1<<RC_SPAN_HEIGHT_BITS)-1;

	static const int RC_SPANS_PER_POOL = 2048;


	// Memory pool used for quick span allocation
	struct SRcSpanPool
	{
		SRcSpanPool* m_pNext;		// Pointer to next pool
		SRcSpan      m_items[1];	// Array of spans( size RC_SPANS_PER_POOL)
	};

	// Dynamic span-heightfield
	struct SRcHeightField
	{
		int			m_iWidth, m_iHeight;	// Dimension(尺寸) of the heightfield
		Vector3     m_vMin,   m_vMax;		// Bounding box of the heightfield
		float       m_fCellSize,    
					m_fCellHeight;			// Cell size and height
		SRcSpan**   m_pSpans;				// Height field of spans ( m_iWidth * m_iHeight)		
		SRcSpanPool*m_pPools;				// Linked list of span pools
		SRcSpan*    m_pFreeList;			// Pointer to next free span

		// 构造函数
		SRcHeightField()
		{
			m_iWidth = m_iHeight = 0;
			m_pSpans	= NULL;
			m_pPools	= NULL;
			m_pFreeList = NULL;
		}

		// 析构函数
		~SRcHeightField()
		{
			SAFE_DELETE_ARRAY( m_pSpans);

			while( m_pPools)
			{
				SRcSpanPool* pNext = m_pPools->m_pNext;
				delete[] reinterpret_cast<unsigned char*>( m_pPools);
				m_pPools = pNext;
			}
		}
	};

	struct SRcCompactCell
	{
		UINT m_index;		// Index to first span in column
		UINT m_count;		// number of spans in this column
	};

	struct RcCompactSpan
	{
		int m_z;				// bottom coordinate of the span
		int m_regID;			// Region ID
		int m_dist;			// Distance to border( 到边界的距离)
		int m_conn;			// connections to neighbour cells
		int m_height;			// Height of the span
	};

	// compact static heightfield
	struct SRcCompactHeightField
	{
		int				m_width, m_height;	// width and height of the heightfield
		int				m_spanCount;			// number of spans in the heightfield
		int				m_iwalkableHeight;
		int				m_iwalkableClimb;		// Agent properties
		int				m_borderSize;			// the aabb border size used during the build of the field
		unsigned int    m_iMaxDistance;			// max distance value stored in heightfield
		unsigned int    m_iMaxRegions;			// max region Id stored in heightfield
		Vector3 		m_vMin,   m_vMax;		// Bounding box of the heightfield
		float			m_cellSize;    
		float			m_cellHeight;			// Cell size and height

		vector<SRcCompactCell>	m_cells;		// Pointer to width*height cells
		vector<RcCompactSpan>	m_spans;		// Pointer to spans
		vector<BYTE>			m_areaIDs;		// Area id data [size:m_iSpanCount]

		// 构造函数
		SRcCompactHeightField()
		{
			m_iMaxDistance = 0;
			m_iMaxRegions  = 0;
		}
	};

	enum AreaConstanct
	{
		// Area border flag.
		// If a region ID has this bit set, then the associated element lies on
		// the border of an area.
		// (Used during the region and contour build process.)
		// @see rcCompactSpan::reg, #rcContour::verts, #rcContour::rverts
		RC_AREA_BORDER = 0x20000,
	};

	/// Contour build flags.
	/// @see RcBuildContours
	enum RcBuildContoursFlags
	{
		RC_CONTOUR_TESS_WALL_EDGES = 0x01,	// Tessellate solid (impassable) edges during contour simplification.
		RC_CONTOUR_TESS_AREA_EDGES = 0x02,	// Tessellate edges between areas during contour simplification.
	};

	enum ContoursConstant
	{
		/// Applied to the region id field of contour vertices in order to extract the region id.
		/// The region id field of a vertex may have several flags applied to it.  So the
		/// fields value can't be used directly.
		/// @see rcContour::verts, rcContour::rverts
		RC_CONTOUR_REG_MASK = 0xffff,
	};

	// contour : 轮廓,外形,周线,围线
	struct SRcContour
	{
		vector<int>		m_verts;		// vertex coordinates, each vertex contains 4 components
		int				m_iNumVerts;	// Number of vertices
		vector<int>		m_rawVerts;	// Raw vertex coordinates, each vertex contains 4 components
		int				m_iNumRawVerts;	// Number of raw vertices
		UINT			m_regionID;		// Region ID of the contour
		UINT			m_areaID;		// Area ID of the contour

		// 构造函数
		SRcContour()
		{
			m_iNumVerts    = 0;
			m_iNumRawVerts = 0;
			m_areaID	   = 0;
		}

		// 析构函数 
		~SRcContour()
		{
		}
	};

	// Contour set
	struct SRcContourSet
	{
		vector<SRcContour>  m_contours;		// Pointer to all contours
		int					m_iNumContours;		// Number of contours
		Vector3				m_vMin, m_vMax;		// Bounding box of the heightfield
		float				m_fCellSize,
							m_fCellHeight;		// Cell size and height
		int					m_width;			// The width of the set
		int					m_height;			// The hgith of the set
		int					m_borderSize;		// The AABB border size used to generate the source data from which the contours were derived

		// 构造函数
		SRcContourSet()
		{
			m_iNumContours = 0;
		}
	};

	typedef vector<int> RcIntArray;

	// Compact span neighbour helpers
	static inline int RcGetCon( const RcCompactSpan& span, int dir)
	{
		const UINT shift = (UINT)dir*6;
		return ( span.m_conn >> shift) & 0x3F;
	}

	// 辅助函数 其中'&'运算符具有类似求余运算效果,防止数组越界 
	// 图示               3
	//                    |
	//                0<-- -->2
	//					  |					
	//                    1
	static inline int RcGetDirOffsetX(int dir)
	{
		const int offset[4] = { -1, 0, 1, 0, };
		return offset[dir&0x03];
	}

	static inline int RcGetDirOffsetY(int dir)
	{
		const int offset[4] = { 0, 1, 0, -1 };
		return offset[dir&0x03];
	}

	// 相减
	static inline void vsub(float* dest, const float* v1, const float* v2)
	{
		dest[0] = v1[0]-v2[0];
		dest[1] = v1[1]-v2[1];
		dest[2] = v1[2]-v2[2];
	}

	static inline float vdot(const float* v1, const float* v2)
	{
		return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
	}

	static inline void vcopy(float* dest, const float* v)
	{
		dest[0] = v[0];
		dest[1] = v[1];
		dest[2] = v[2];
	}

	template<class T> inline T rcSqr(T a) { return a*a; }

	// Calculates grid size based on bounding box and grid cell size
	// width     - grid width
	// height    - grid height
	// vMin vMax - bounding box
	// cellSize  - grid cell size
	void RcCalcGridSize( int& width, int& height, const Vector3& vMin, const Vector3& vMax, const float& cellSize);

	// Creates and initializes new heightfield
	// Params:
	// outHF	  - heightfield to initialize
	// width	  - width of the heightfield
	// height	  - height of the heightfield
	// bMin, bMax - bounding box of the heightfield
	// cellSize   - grid cell size
	// cellHeight - grid cell height
	bool RcCreateHeightField( SRcHeightField& outHF, int width, int height, const Vector3& vMin, const Vector3& vMax, float cellSize, float cellHeight);

	// Sets the Walkable flag for every triangle whose slope is below the
	// max walkable slope angle
	// Params:
	// pFlags   - (out) array of triangle flags
	// pverts   - array of vertices
	// numVerts - vertex count
	// pTris    - array of triangle vertex indices
	// numTris  - triangle count
	// walkableSlope - max slop angle in degrees
	void RcMarkWalkableTriangles( unsigned char* areas, const Vector3* pVerts, int numVerts, const int* pTris, int numTris, float walkableSlope);

	// Rasterizes a triangle into  heightfield spans.
	// Params:
	// pVerts   - array of vertices
	// numVerts - vertex count
	// pTris    - array of triangle vertex indices
	// numTris  - triangle count
	// areas    - The area id's of the triangles
	// Solid    - heightfield wher the triangles are rasterized
	void RcRasterizeTriangles( const Vector3* pVerts, int numVerts, const int* pTris, const unsigned char* areas, int numTris, SRcHeightField& solid, const int flagMergeThr=1);

	// Removes walkable flag from all spans that are in water
	void RcFilterWaterSapns( SRcHeightField& solid, WaterZoneMgr* waterZone, const SRcConfig& cfg);

	// Marks non-walkable spans as walkable if their maximum is within @p walkableClimp of a walkable neihbor
	// Params:
	// walkableClimb - Maximum ledge height that is considered to still be traversable
	// solid		 - A fully built heightfield (All spans have been added)
	void RcFilterLowHangingWalkableObstacles( int walkableClimb, SRcHeightField& solid);

	// Removes walkable flag from all spans that are at ledges. this filtering
	// removes possible overestimation of the conservative voxelization so that
	// the resulting mesh will not have regions hanging in air over ledges
	// Params:
	// walkableHeight  - minimum height where the agent can still walk
	// walkableClimb   - maximum height between grid cells the agent can climb
	// solid           - heightfield describing the solid space
	void RcFilterLedgeSpans( const int walkableHeight, const int walkableClimb, SRcHeightField& solid);

	// remove walkable flags from all spans which have smaller than 'walkableHeight'
	// clearane above them
	// Params:
	// walkable height	- minimum height where the agent can still walk
	// solid            - heightfield describing the solid space
	void RcFilterWalkableLowHeightSpans( int walkableHeight, SRcHeightField& solid);

	// Build compact representation of the heightfield
	// Params:
	// walkableHeight - minmum height where the agent can still walk
	// walkableClimb  - maximum height between grid cells the agent can climb
	// hf             - heightfield to be compacted
	// chf            - compact heightfield representing the open space
	// Return false if operation run out of memory
	bool RcBuildCompactHeightfield( const int walkableHeight, const int walkableClimb, SRcHeightField& hf, SRcCompactHeightField& chf);

	// 区域凸包
	struct RcConvexVolume
	{
		ShapePtr	m_shape;	// 形状
		int			m_area;		// 区域ID
	};
	typedef vector<RcConvexVolume> VolumeList;

	// The value of spacial parameters are in world units.
	void RcMarkConvexPolyArea( const RcConvexVolume& volume, SRcCompactHeightField& chf);

	// Erodes the walkable area within the heightfield by the specified radius
	// Params
	// radius		- the radius of erosion
	// chf			- the populated compact heightfield to erode
	// returns true if the operation completed successfully
	bool RcErodeWalkableArea( int radius, SRcCompactHeightField& chf);

	// Builds distance field and stores it into the combat heightfield
	// Params:
	// chf - compact heightfield representing the open space
	// Returns false if operation run out of memory
	bool RcBuildDistanceField( SRcCompactHeightField& chf);

	// Divides the walkable heighfied into simple regions.
	// Each region has only one contour and no overlaps.
	// The regions are stored in the compact heightfield 'reg' field.
	// The regions will be shrinked by the radius of the agent.
	// The process sometimes creates small regions. The parameter
	// 'minRegionSize' specifies the smallest allowed regions size.
	// If the area of a regions is smaller than allowed, the regions is
	// removed or merged to neighbour region.
	// Params:
	// chf				 - compact heightfield representing the open space
	// walkableRadius	 - the radius of the agent
	// minRegionSize	 - the smallest allowed regions size.
	// maxMergeRegionSize- the largest allowed regions size which can be merged
	// return false if operation ran out of memory
	bool RcBuildRegions( SRcCompactHeightField& chf, int borderSize, int minRegionArea, int mergeRegionArea);

	// Builds simplified contours from the regions outlines.
	// Params:
	// chf        - (in) compact heightfield which has regions set.
	// maxError   - (in) maximum allowed distance between simplified countour and cells.
	// maxEdgeLen - (in) maximum allowed contour edge length in cells.
	// cset       - (out) Resulting contour set.
	// Returns false if operation ran out of memory.
	bool RcBuildContours(SRcCompactHeightField& chf, const float maxError, const int maxEdgeLen, SRcContourSet& cset, const int buildFlags=RC_CONTOUR_TESS_WALL_EDGES);

	// Builds connected convex polygon mesh from contour polygons
	// Params:
	// out			- (out) poly Mesh
	// nvp			- max number of vertices per polygon
	// contourSet	- contour set
	bool  RcBuildPolyMesh( SRcContourSet& cset, int nvp, RcPolyMesh& mesh);

	bool  RcMergePolyMeshes( vector<RcPolyMesh*>& meshes, const int nmeshes, RcPolyMesh& mesh);

	// Builds detail triangle mesh for each polygon in the poly mesh.
	// 根据三角培分,建立纯有三角形组成的mesh
	// Params:
	//	mesh - (in) poly mesh to detail.
	//	chf -  (in) compacy height field, used to query height for new vertices.
	//  sampleDist - (in) spacing between height samples used to generate more detail into mesh.
	//  sampleMaxError - (in) maximum allowed distance between simplified detail mesh and height sample.
	//	pmdtl - (out) detail mesh.
	// Returns false if operation ran out of memory.
	bool RcBuildPolyMeshDetail(const RcPolyMesh& mesh, const SRcCompactHeightField& chf, const float sampleDist, const float sampleMaxError, SRcPolyMeshDetail& dmesh, bool& testFlag);

	bool RcMergePolyMeshDetails( vector<SRcPolyMeshDetail*>& meshes, const int nmeshes, SRcPolyMeshDetail& mesh);
}