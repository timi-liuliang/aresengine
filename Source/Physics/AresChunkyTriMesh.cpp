#include "Physics/AresChunkyTriMesh.h"

namespace Ares
{
	// Creates partitioned triangle mesh( AABB tree)
	// where each node contains at max trisPerChunk triangles.
	bool RcCreateChunkyTriMesh( const Vector3* verts, const int* tris, int nTris, SRcChunkyTriMesh* pOut)
	{
		pOut->m_pQuadTreeMember = new_ quad_tree_member<int>[nTris];

		for (int i = 0; i < nTris; i++)
		{		
			quad_tree_rect rect;
			const int* tri = &tris[i*3];

			// Calc triangle XY bounds.
			for (int j = 0; j < 3; ++j)
				rect.union_point( verts[tri[j]].x, verts[tri[j]].y);

			pOut->m_pQuadTreeMember[i].key   = rect;
			pOut->m_pQuadTreeMember[i].value = i;

			pOut->m_quadTree.add_member( &pOut->m_pQuadTreeMember[i]);
		}

		return true;
	}

	// return s the chunk indices which touch the input rectable
	int  RcGetChunkInRect( SRcChunkyTriMesh* cm, quad_tree_rect searchRect, vector<int>& ids)
	{
		int searchNum = cm->m_quadTree.search( searchRect, ids);

		return searchNum;
	}
}