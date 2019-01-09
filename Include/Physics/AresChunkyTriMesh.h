#pragma once

#include <Core/AresMath.h>
#include <Physics/QuadTree.hpp>

using namespace boost;

namespace Ares
{
	#define MaxTrianglePerSearch  0xffff

	// chunky mesh
	struct SRcChunkyTriMesh
	{
		QuadTree<int>            m_quadTree;		// 四叉树
		quad_tree_member<int>*   m_pQuadTreeMember; // 四叉树成员

		// 构造函数
		SRcChunkyTriMesh( quad_tree_rect& rect)
		{
			m_quadTree.create( rect, 7);
		}

		// 析构函数
		~SRcChunkyTriMesh()
		{
			SAFE_DELETE_ARRAY( m_pQuadTreeMember);
		}
	};

	// Creates partitioned triangle mesh( AABB tree)
	// where each node contains at max trisPerChunk triangles.
	bool RcCreateChunkyTriMesh( const Vector3* verts, const int* tris, int nTris ,SRcChunkyTriMesh* pOut);

	
	// return s the chunk indices which touch the input rectable
	int  RcGetChunkInRect( SRcChunkyTriMesh* cm, quad_tree_rect searchRect, vector<int>& ids);
}