#include "Physics/AresRecast.h"

namespace Ares
{
	static const int VERTEX_BUCKET_COUNT = (1<<12);

	// 三角化
	inline int RcPrev(int i, int n) { return i-1 >= 0 ? i-1 : n-1; }
	inline int RcNext(int i, int n) { return i+1 < n ? i+1 : 0; }

	// 面积
	inline int area2(const int* a, const int* b, const int* c)
	{
		return (b[0] - a[0]) * (c[1] - a[1]) - (c[0] - a[0]) * (b[1] - a[1]);
	}

	inline bool collinear(const int* a, const int* b, const int* c)
	{
		return area2(a, b, c) == 0;
	}

	//	Exclusive or: true iff exactly one argument is true.
	//	The arguments are negated to ensure that they are 0/1
	//	values.  Then the bitwise Xor operator may apply.
	//	(This idea is due to Michael Baldwin.)
	inline bool xorb(bool x, bool y)
	{
		return !x ^ !y;
	}

	// Returns true iff c is strictly to the left of the directed
	// line through a to b.
	inline bool left(const int* a, const int* b, const int* c)
	{
		return area2(a, b, c) < 0;
	}

	inline bool leftOn(const int* a, const int* b, const int* c)
	{
		return area2(a, b, c) <= 0;
	}

	//	Returns true iff ab properly intersects cd: they share
	//	a point interior to both segments.  The properness of the
	//	intersection is ensured by using strict leftness.
	bool intersectProp(const int* a, const int* b, const int* c, const int* d)
	{
		// Eliminate improper cases.
		if (collinear(a,b,c) || collinear(a,b,d) ||
			collinear(c,d,a) || collinear(c,d,b))
			return false;

		return xorb(left(a,b,c), left(a,b,d)) && xorb(left(c,d,a), left(c,d,b));
	}

	// Returns T iff (a,b,c) are collinear and point c lies 
	// on the closed segement ab.
	static bool between(const int* a, const int* b, const int* c)
	{
		if (!collinear(a, b, c))
			return false;
		// If ab not vertical, check betweenness on x; else on y.
		if (a[0] != b[0])
			return	((a[0] <= c[0]) && (c[0] <= b[0])) || ((a[0] >= c[0]) && (c[0] >= b[0]));
		else
			return	((a[1] <= c[1]) && (c[1] <= b[1])) || ((a[1] >= c[1]) && (c[1] >= b[1]));
	}

	// Returns true iff segments ab and cd intersect, properly or improperly.
	static bool intersect(const int* a, const int* b, const int* c, const int* d)
	{
		if (intersectProp(a, b, c, d))
			return true;
		else if (between(a, b, c) || between(a, b, d) ||
			between(c, d, a) || between(c, d, b))
			return true;
		else
			return false;
	}

	static bool vequal(const int* a, const int* b)
	{
		return a[0] == b[0] && a[1] == b[1];
	}

	// Returns T iff (v_i, v_j) is a proper internal *or* external
	// diagonal of P, *ignoring edges incident to v_i and v_j*.
	static bool Diagonalie(int i, int j, int n, const vector<int>& verts, int* indices)
	{
		const int* d0 = &verts[(indices[i] & 0x0fffffff) * 4];
		const int* d1 = &verts[(indices[j] & 0x0fffffff) * 4];

		// For each edge (k,k+1) of P
		for (int k = 0; k < n; k++)
		{
			int k1 = RcNext(k, n);
			// Skip edges incident to i or j
			if (!((k == i) || (k1 == i) || (k == j) || (k1 == j)))
			{
				const int* p0 = &verts[(indices[k] & 0x0fffffff) * 4];
				const int* p1 = &verts[(indices[k1] & 0x0fffffff) * 4];

				if (vequal(d0, p0) || vequal(d1, p0) || vequal(d0, p1) || vequal(d1, p1))
					continue;

				if (intersect(d0, d1, p0, p1))
					return false;
			}
		}
		return true;
	}

	// Returns true iff the diagonal (i,j) is strictly internal to the 
	// polygon P in the neighborhood of the i endpoint.
	static bool	InCone(int i, int j, int n, const vector<int>& verts, int* indices)
	{
		const int* pi = &verts[(indices[i] & 0x0fffffff) * 4];
		const int* pj = &verts[(indices[j] & 0x0fffffff) * 4];
		const int* pi1 = &verts[(indices[RcNext(i, n)] & 0x0fffffff) * 4];
		const int* pin1 = &verts[(indices[RcPrev(i, n)] & 0x0fffffff) * 4];

		// If P[i] is a convex vertex [ i+1 left or on (i-1,i) ].
		if (leftOn(pin1, pi, pi1))
			return left(pi, pj, pin1) && left(pj, pi, pi1);
		// Assume (i-1,i,i+1) not collinear.
		// else P[i] is reflex.
		bool result = !(leftOn(pi, pj, pi1) && leftOn(pj, pi, pin1));

		return result;
	}

	// Returns T iff (v_i, v_j) is a proper internal
	// diagonal of P.
	static bool diagonal(int i, int j, int n, const vector<int>& verts, int* indices)
	{
		return InCone(i, j, n, verts, indices) && Diagonalie(i, j, n, verts, indices);
	}

	int Triangulate(int n, const vector<int>& verts, int* indices, int* tris)
	{
		int ntris = 0;
		int* dst = tris;

		// The last bit of the index is used to indicate if the vertex can be removed.
		for (int i = 0; i < n; i++)
		{
			int i1 = RcNext(i, n);
			int i2 = RcNext(i1, n);
			if (diagonal(i, i2, n, verts, indices))
				indices[i1] |= 0x80000000;
		}

		while (n > 3)
		{
			int minLen = -1;
			int mini = -1;
			for (int i = 0; i < n; i++)
			{
				int i1 = RcNext(i, n);
				if (indices[i1] & 0x80000000)
				{
					const int* p0 = &verts[(indices[i] & 0x0fffffff) * 4];
					const int* p2 = &verts[(indices[RcNext(i1, n)] & 0x0fffffff) * 4];

					int dx = p2[0] - p0[0];
					int dy = p2[1] - p0[1];
					int len = dx*dx + dy*dy;

					if (minLen < 0 || len < minLen)
					{
						minLen = len;
						mini = i;
					}
				}
			}

			// should not happen
			if (mini == -1)
			{
				//return -ntris;
				return ntris;
			}

			int i = mini;
			int i1 = RcNext(i, n);
			int i2 = RcNext(i1, n);

			*dst++ = indices[i] & 0x0fffffff;
			*dst++ = indices[i1] & 0x0fffffff;
			*dst++ = indices[i2] & 0x0fffffff;
			ntris++;

			// Removes P[i1] by copying P[i+1]...P[n-1] left one index.
			n--;
			for (int k = i1; k < n; k++)
				indices[k] = indices[k+1];

			if (i1 >= n) i1 = 0;
			i = RcPrev(i1,n);
			// Update diagonal flags.
			if (diagonal(RcPrev(i, n), i1, n, verts, indices))
				indices[i] |= 0x80000000;
			else
				indices[i] &= 0x0fffffff;

			if (diagonal(i, RcNext(i1, n), n, verts, indices))
				indices[i1] |= 0x80000000;
			else
				indices[i1] &= 0x0fffffff;
		}

		// Append the remaining triangle.
		*dst++ = indices[0] & 0x0fffffff;
		*dst++ = indices[1] & 0x0fffffff;
		*dst++ = indices[2] & 0x0fffffff;
		ntris++;

		return ntris;
	}

	inline int ComputeVertexHash(int x, int y, int z)
	{
		const unsigned int h1 = 0x8da6b343; // Large multiplicative constants;
		const unsigned int h2 = 0xd8163841; // here arbitrarily chosen primes
		const unsigned int h3 = 0xcb1ab31f;
		unsigned int n = h1 * x + h2 * y + h3 * z;
		return (int)(n & (VERTEX_BUCKET_COUNT-1));
	}


	static int AddVertex( int x, int y, int z, vector<Int3>& verts, vector<int>& firstVert, vector<int>& nextVert, int& nv)
	{
		int bucket = ComputeVertexHash(x, y, 0);
		int i = firstVert[bucket];

		while (i != -1)
		{
			const Int3& v = verts[i];
			if (v[0] == x  && v[1] == y && (abs(v[2] - z) <= 2))
				return i;
			i = nextVert[i]; // next
		}

		// Could not find, create new.
		i = nv; nv++;
		verts[i]	= Int3( x, y, z);
		nextVert[i] = firstVert[bucket];
		firstVert[bucket] = i;

		if( z < -10000)
			printf( "Cao %d, %d, %d, %d", x, y, z, bucket);

		return i;
	}

	struct SRcEdge
	{
		UINT	vert[2];		// 两顶点
		UINT	polyEdge[2];	// 两边
		UINT	poly[2];		// 两多边形
	};

	// 邻接
	static bool BuildMeshAdjacency( vector<UINT>& polys, const int npolys, const int nverts, const int vertsPerPoly)
	{
		// Based on code by Eric Lengyel from:
		// http://www.terathon.com/code/edges.php

		int maxEdgeCount = npolys*vertsPerPoly;
		vector<UINT> firstEdge(nverts + maxEdgeCount, INVALID);
		unsigned int* nextEdge = firstEdge.data() + nverts;
		int edgeCount = 0;

		vector<SRcEdge> edges(maxEdgeCount);

		// Invalida indices are marked as 0xffff, the following code
		// handles them just fine.
		for (int i = 0; i < npolys; ++i)
		{
			UINT* t = &polys[i*vertsPerPoly*2];
			for (int j = 0; j < vertsPerPoly; ++j)
			{
				UINT v0 = t[j];
				UINT v1 = (j+1 >= vertsPerPoly || t[j+1] == INVALID) ? t[0] : t[j+1];
				if (v0 < v1)
				{
					SRcEdge& edge = edges[edgeCount];
					edge.vert[0] = v0;
					edge.vert[1] = v1;
					edge.poly[0] = static_cast<UINT>(i);
					edge.polyEdge[0] = static_cast<UINT>(j);
					edge.poly[1]     = static_cast<UINT>(i);
					edge.polyEdge[1] = 0;
					// Insert edge
					nextEdge[edgeCount] = firstEdge[v0];
					firstEdge[v0] = edgeCount;
					edgeCount++;
				}
			}
		}

		for (int i = 0; i < npolys; ++i)
		{
			UINT* t = &polys[i*vertsPerPoly*2];
			for (int j = 0; j < vertsPerPoly; ++j)
			{
				UINT v0 = t[j];
				UINT v1 = (j+1 >= vertsPerPoly || t[j+1] == INVALID) ? t[0] : t[j+1];
				if (v0 > v1)
				{
					for ( UINT e = firstEdge[v1]; e != INVALID; e = nextEdge[e])
					{
						SRcEdge& edge = edges[e];
						if (edge.vert[1] == v0 && edge.poly[0] == edge.poly[1])
						{
							edge.poly[1]     = static_cast<UINT>(i);
							edge.polyEdge[1] = static_cast<UINT>(j);
							break;
						}
					}
				}
			}
		}

		// Store adjacency
		for (int i = 0; i < edgeCount; ++i)
		{
			const SRcEdge& e = edges[i];
			if (e.poly[0] != e.poly[1])
			{
				UINT* p0 = &polys[e.poly[0]*vertsPerPoly*2];
				UINT* p1 = &polys[e.poly[1]*vertsPerPoly*2];
				p0[vertsPerPoly + e.polyEdge[0]] = e.poly[1];
				p1[vertsPerPoly + e.polyEdge[1]] = e.poly[0];
			}
		}

		return true;
	}

	static void pushFront(int v, int* arr, int& an)
	{
		an++;
		for (int i = an-1; i > 0; --i) arr[i] = arr[i-1];
		arr[0] = v;
	}

	static void pushBack(int v, int* arr, int& an)
	{
		arr[an] = v;
		an++;
	}

	inline bool uleft(const Int3& a, const Int3& b, const Int3& c)
	{
		return ( b[0]-a[0]) * ( c[1]-a[1]) - (c[0]- a[0]) * (b[1]-a[1]) < 0;
	}

	static int GetPolyMergeValue( UINT* pa, UINT* pb,const vector<Int3>& verts, int& ea, int& eb, const int nvp)
	{
		const int na = RcPolyMesh::CountPolyVerts( pa, nvp);
		const int nb = RcPolyMesh::CountPolyVerts( pb, nvp);

		// If the merged polygon would be too big, do not merge.
		if (na+nb-2 > nvp)
			return -1;

		// Check if the polygons share an edge.
		ea = -1;
		eb = -1;

		for (int i = 0; i < na; ++i)
		{
			UINT va0 = pa[i];
			UINT va1 = pa[(i+1) % na];
			if (va0 > va1)
				swap(va0, va1);
			for (int j = 0; j < nb; ++j)
			{
				UINT vb0 = pb[j];
				UINT vb1 = pb[(j+1) % nb];
				if (vb0 > vb1)
					swap(vb0, vb1);
				if (va0 == vb0 && va1 == vb1)
				{
					ea = i;
					eb = j;
					break;
				}
			}
		}

		// No common edge, cannot merge.
		if (ea == -1 || eb == -1)
			return -1;

		// Check to see if the merged polygon would be convex.
		UINT va, vb, vc;

		va = pa[(ea+na-1) % na];
		vb = pa[ea];
		vc = pb[(eb+2) % nb];
		if (!uleft( verts[va], verts[vb], verts[vc]))
			return -1;

		va = pb[(eb+nb-1) % nb];
		vb = pb[eb];
		vc = pa[(ea+2) % na];
		if (!uleft( verts[va], verts[vb], verts[vc]))
			return -1;

		va = pa[ea];
		vb = pa[(ea+1)%na];

		int dx = verts[va][0] - verts[vb][0];
		int dy = verts[va][1] - verts[vb][1];

		return dx*dx + dy*dy;
	}

	// 合并polygon
	static void MergePolys( UINT* pa, UINT* pb, int ea, int eb, UINT* tmp, const int nvp)
	{
		const int na = RcPolyMesh::CountPolyVerts( pa, nvp);
		const int nb = RcPolyMesh::CountPolyVerts( pb, nvp);

		// Merge polygons.
		memset( tmp, INVALID, sizeof(UINT)*nvp);
		int n = 0;
		// Add pa
		for (int i = 0; i < na-1; ++i)
			tmp[n++] = pa[(ea+1+i) % na];
		// Add pb
		for (int i = 0; i < nb-1; ++i)
			tmp[n++] = pb[(eb+1+i) % nb];

		std::memcpy( pa, tmp, sizeof(UINT)*nvp);
	}

	// 移除顶点1.mesh 2.顶点索引 3.最大三角形数
	static bool RemoveVertex(RcPolyMesh& mesh, const unsigned short rem, const int maxTris)
	{
		const int nvp = mesh.m_iNVP;

		// Count number of polygons to remove.
		int nrem = 0;
		for (int i = 0; i < mesh.m_numPolys; ++i)
		{
			UINT* p		 = &mesh.m_polys[i*nvp*2];
			const int nv = RcPolyMesh::CountPolyVerts( p, nvp);
			for (int j = 0; j < nv; ++j)
				if (p[j] == rem) 
					nrem++;
		}

		int nedges = 0;
		vector<int> edges(nrem*nvp*4);
		int nhole = 0;
		vector<int> hole(nrem*nvp);
		int nhreg = 0;
		vector<int> hreg(nrem*nvp);
		int nharea = 0;
		vector<int> harea(nrem*nvp);

		for (int i = 0; i < mesh.m_numPolys; ++i)
		{
			UINT* p = &mesh.m_polys[i*nvp*2];
			const int nv = RcPolyMesh::CountPolyVerts(p, nvp);
			bool hasRem = false;
			for (int j = 0; j < nv; ++j)
			{
				if (p[j] == rem) 
					hasRem = true;
			}

			if (hasRem)
			{
				// Collect edges which does not touch the removed vertex.
				for (int j = 0, k = nv-1; j < nv; k = j++)
				{
					if (p[j] != rem && p[k] != rem)
					{
						int* e = &edges[nedges*4];
						e[0] = p[k];
						e[1] = p[j];
						e[2] = mesh.m_regs[i];
						e[3] = mesh.m_areaIDs[i];
						nedges++;
					}
				}
				// Remove the polygon.
				UINT* p2 = &mesh.m_polys[(mesh.m_numPolys-1)*nvp*2];
				std::memcpy( p, p2, sizeof(UINT)*nvp);
				std::memset( p+nvp, INVALID, sizeof(UINT)*nvp);
				mesh.m_regs[i]		= mesh.m_regs[mesh.m_numPolys-1];
				mesh.m_areaIDs[i]	= mesh.m_areaIDs[mesh.m_numPolys-1];
				mesh.m_numPolys--;
				--i;
			}
		}

		// Remove vertex.(移除顶点)
		for (int i = (int)rem; i < mesh.m_iNumVerts; ++i)
		{
			mesh.m_verts[i] = mesh.m_verts[i+1];
		}
		mesh.m_verts.pop_back();
		mesh.m_iNumVerts--;

		// Adjust indices to match the removed vertex layout.
		for (int i = 0; i < mesh.m_numPolys; ++i)
		{
			UINT* p = &mesh.m_polys[i*nvp*2];
			const int nv = RcPolyMesh::CountPolyVerts( p, nvp);
			for (int j = 0; j < nv; ++j)
				if (p[j] > rem) p[j]--;
		}

		for (int i = 0; i < nedges; ++i)
		{
			if (edges[i*4+0] > rem) edges[i*4+0]--;
			if (edges[i*4+1] > rem) edges[i*4+1]--;
		}

		if (nedges == 0)
			return true;

		// Start with one vertex, keep appending connectd segments to the end of the hole
		pushBack( edges[0], hole.data(), nhole);
		pushBack( edges[2], hreg.data(), nhreg);
		pushBack( edges[3], harea.data(), nharea);
		while (nedges)
		{
			bool match = false;

			for (int i = 0; i < nedges; ++i)
			{
				const int ea = edges[i*4+0];
				const int eb = edges[i*4+1];
				const int r  = edges[i*4+2];
				const int a  = edges[i*4+3];
				bool add = false;
				if (hole[0] == eb)
				{
					pushFront(ea, hole.data(), nhole);
					pushFront(r, hreg.data(), nhreg);
					pushFront(a, harea.data(), nharea);
					add = true;
				}
				else if (hole[nhole-1] == ea)
				{
					pushBack(eb, hole.data(), nhole);
					pushBack(r, hreg.data(), nhreg);
					pushBack(a, harea.data(), nharea);
					add = true;
				}
				if (add)
				{
					// Remove edge.
					edges[i*4+0] = edges[(nedges-1)*4+0];
					edges[i*4+1] = edges[(nedges-1)*4+1];
					edges[i*4+2] = edges[(nedges-1)*4+2];
					edges[i*4+3] = edges[(nedges-1)*4+3];
					--nedges;
					match = true;
					--i;
				}
			}

			if (!match)
				break;
		}
		
		vector<int> tris(nhole*3);
		vector<int> tverts(nhole*4);
		vector<int> thole(nhole);

		// Generate temp vertex array for triangulation.
		for (int i = 0; i < nhole; ++i)
		{
			const int pi = hole[i];
			tverts[i*4+0] = mesh.m_verts[pi][0];
			tverts[i*4+1] = mesh.m_verts[pi][1];
			tverts[i*4+2] = mesh.m_verts[pi][2];
			tverts[i*4+3] = 0;
			thole[i] = i;
		}

		if( nhole < 3)
			return true;

		// Triangulate the hole.
		int ntris = Triangulate(nhole, tverts, &thole[0], tris.data());
		if( ntris <=0)
			ntris = -ntris;

		// Merge the hole triangles back to polygons.
		vector<UINT>polys((ntris+1)*nvp);
		vector<WORD>pregs( ntris);
		vector<BYTE>pareas(ntris);

		UINT* tmpPoly = &polys[ntris*nvp];

		// Build initial polygons.
		int npolys = 0;
		memset( polys.data(), INVALID, ntris*nvp*sizeof(UINT));
		for (int j = 0; j < ntris; ++j)
		{
			int* t = &tris[j*3];
			if (t[0] != t[1] && t[0] != t[2] && t[1] != t[2])
			{
				polys[npolys*nvp+0] = static_cast<UINT>(hole[t[0]]);
				polys[npolys*nvp+1] = static_cast<UINT>(hole[t[1]]);
				polys[npolys*nvp+2] = static_cast<UINT>(hole[t[2]]);
				pregs[npolys] = hreg[t[0]];
				pareas[npolys]= (BYTE)(harea[t[0]]);
				npolys++;
			}
		}
		if (!npolys)
			return true;

		// Merge polygons.
		if (nvp > 3)
		{
			while (true)
			{
				// Find best polygons to merge.
				int bestMergeVal = 0;
				int bestPa, bestPb, bestEa, bestEb;

				for (int j = 0; j < npolys-1; ++j)
				{
					UINT* pj = &polys[j*nvp];
					for (int k = j+1; k < npolys; ++k)
					{
						UINT* pk = &polys[k*nvp];
						int ea, eb;
						int v = GetPolyMergeValue(pj, pk, mesh.m_verts, ea, eb, nvp);
						if (v > bestMergeVal)
						{
							bestMergeVal = v;
							bestPa = j;
							bestPb = k;
							bestEa = ea;
							bestEb = eb;
						}
					}
				}

				if (bestMergeVal > 0)
				{
					// Found best, merge.
					UINT* pa = &polys[bestPa*nvp];
					UINT* pb = &polys[bestPb*nvp];
					MergePolys( pa, pb, bestEa, bestEb, tmpPoly, nvp);
					memcpy( pb, &polys[(npolys-1)*nvp], sizeof(UINT)*nvp);
					pregs[bestPb] = pregs[npolys-1];
					pareas[bestPb]= pareas[npolys-1];
					npolys--;
				}
				else
				{
					// Could not merge any polygons, stop.
					break;
				}
			}
		}

		// Store polygons.
		for (int i = 0; i < npolys; ++i)
		{
			if (mesh.m_numPolys >= maxTris) break;
			UINT* p = &mesh.m_polys[mesh.m_numPolys*nvp*2];
			memset( p, INVALID, sizeof(UINT)*nvp*2);
			for (int j = 0; j < nvp; ++j)
				p[j] = polys[i*nvp+j];
			mesh.m_regs[mesh.m_numPolys]	= pregs[i];
			mesh.m_areaIDs[mesh.m_numPolys]	= pareas[i];
			mesh.m_numPolys++;

			if( mesh.m_numPolys>maxTris)
				return false;
		}

		return true;
	}

	// Builds connected convex polygon mesh from contour polygons
	// Params:
	// out			- (out) poly Mesh
	// nvp			- max number of vertices per polygon
	// contourSet	- contour set
	bool  RcBuildPolyMesh( SRcContourSet& cset, int nvp, RcPolyMesh& mesh)
	{
		// 记录包围盒
		mesh.m_vMin = cset.m_vMin;
		mesh.m_vMax = cset.m_vMax;

		// 记录块大小
		mesh.m_fCellSize   = cset.m_fCellSize;
		mesh.m_fCellHeight = cset.m_fCellHeight;
		mesh.m_borderSize  = cset.m_borderSize;

		int maxVertices = 0;
		int maxTris = 0;
		int maxVertsPerCont = 0;

		for (int i = 0; i < cset.m_iNumContours; ++i)
		{
			if( cset.m_contours[i].m_iNumVerts < 3)
				continue;

			maxVertices += cset.m_contours[i].m_iNumVerts;

			// n个顶点最多组成 n-2 个三角形
			maxTris			+= cset.m_contours[i].m_iNumVerts - 2;		
			maxVertsPerCont  = max(maxVertsPerCont, cset.m_contours[i].m_iNumVerts);
		}

		// 限制最大顶点数
		if (maxVertices >= 0xfffe)
			return false;

		// 标记
		vector<BYTE> vflags(maxVertices, 0);

		mesh.m_verts.resize(maxVertices, Int3( 0, 0, 0));
		mesh.m_polys.resize(maxTris*nvp*2, INVALID);
		mesh.m_regs.resize(maxTris, 0);

		// 多边形信息
		mesh.m_areaIDs.resize( maxTris, 0);

		mesh.m_iNumVerts = 0;
		mesh.m_numPolys = 0;
		mesh.m_iNVP = nvp;

		vector<int> nextVert(maxVertices, 0);
		vector<int> firstVert(VERTEX_BUCKET_COUNT, -1);
		vector<int> indices(maxVertsPerCont);
		vector<int> tris(maxVertsPerCont*3);
		vector<UINT>polys((maxVertsPerCont+1)*nvp);

		UINT* tmpPoly = &polys[maxVertsPerCont*nvp];
		for (int i = 0; i < cset.m_iNumContours; ++i)
		{
			SRcContour& cont = cset.m_contours[i];

			// Skip empty contours.
			if (cont.m_iNumVerts < 3)
				continue;

			// Triangulate contour
			for ( int j = 0; j < cont.m_iNumVerts; ++j)
				indices[j] = j;

			int ntris = Triangulate(cont.m_iNumVerts, cont.m_verts, &indices[0], &tris[0]);

			if (ntris <= 0)
				ntris = -ntris;

			// Add and merge vertices.
			for (int j = 0; j < cont.m_iNumVerts; ++j)
			{
				const int* v = &cont.m_verts[j*4];
				indices[j] = AddVertex( v[0], v[1], v[2], mesh.m_verts, firstVert, nextVert, mesh.m_iNumVerts);
				if (v[3] & RC_BORDER_VERTEX)
				{
					// This vertex should be removed.
					vflags[indices[j]] = 1;
				}
			}

			// Build initial polygons.
			int npolys = 0;
			std::memset(polys.data(), INVALID, maxVertsPerCont*nvp*sizeof(UINT));
			for (int j = 0; j < ntris; ++j)
			{
				int* t = &tris[j*3];
				if (t[0] != t[1] && t[0] != t[2] && t[1] != t[2])
				{
					polys[npolys*nvp+0] = static_cast<UINT>(indices[t[0]]);
					polys[npolys*nvp+1] = static_cast<UINT>(indices[t[1]]);
					polys[npolys*nvp+2] = static_cast<UINT>(indices[t[2]]);
					npolys++;
				}
			}
			if (!npolys)
				continue;

			// Merge polygons.
			if (nvp > 3)
			{
				while (true)
				{
					// Find best polygons to merge.
					int bestMergeVal = 0;
					int bestPa, bestPb, bestEa, bestEb;

					for (int j = 0; j < npolys-1; ++j)
					{
						UINT* pj = &polys[j*nvp];
						for (int k = j+1; k < npolys; ++k)
						{
							UINT* pk = &polys[k*nvp];
							int ea, eb;
							int v = GetPolyMergeValue(pj, pk, mesh.m_verts, ea, eb, nvp);
							if (v > bestMergeVal)
							{
								bestMergeVal = v;
								bestPa = j;
								bestPb = k;
								bestEa = ea;
								bestEb = eb;
							}
						}
					}

					if (bestMergeVal > 0)
					{
						// Found best, merge.
						// 合并 poly "pa" 与 pb
						UINT* polyA = &polys[bestPa*nvp];
						UINT* polyB = &polys[bestPb*nvp];
						MergePolys(polyA, polyB, bestEa, bestEb, tmpPoly, nvp);
						memcpy(polyB, &polys[(npolys-1)*nvp], sizeof(UINT)*nvp);
						npolys--;
					}
					else
					{
						// Could not merge any polygons, stop.
						break;
					}
				}
			}

			// Store polygons.
			for (int j = 0; j < npolys; ++j)
			{
				UINT* p = &mesh.m_polys[mesh.m_numPolys*nvp*2];
				UINT* q = &polys[j*nvp];
				for (int k = 0; k < nvp; ++k)
					p[k] = q[k];
				mesh.m_regs[mesh.m_numPolys]	= cont.m_regionID;
				mesh.m_areaIDs[mesh.m_numPolys]= cont.m_areaID;
				mesh.m_numPolys++;
			}
		}


		// Remove edge vertices.
		for (int i = 0; i < mesh.m_iNumVerts; ++i)
		{
			if (vflags[i])
			{
				if (!RemoveVertex(mesh, i, maxTris))
					goto failure;
				for (int j = i; j < mesh.m_iNumVerts-1; ++j)
					vflags[j] = vflags[j+1];
				--i;
			}
		}

		// Calculate adjacency.
		if (!BuildMeshAdjacency( mesh.m_polys, mesh.m_numPolys, mesh.m_iNumVerts, nvp))
			return false;

		// find portal edges
		if( mesh.m_borderSize>0)
		{
			const int w = cset.m_width;
			const int h = cset.m_height;
			for( int i=0; i<mesh.m_numPolys; i++)
			{
				UINT* p = &mesh.m_polys[i*2*nvp];
				for( int j=0; j<nvp; ++j)
				{
					if( p[j]==RC_MESH_NULL_IDX) break;

					// skip connected edges
					if( p[nvp+j]!=RC_MESH_NULL_IDX)
						continue;

					int nj = j+1;
					if (nj >= nvp || p[nj] == RC_MESH_NULL_IDX) nj = 0;
					const Int3& va = mesh.m_verts[p[j]];
					const Int3& vb = mesh.m_verts[p[nj]];

					if( va[0]==0 && vb[0]==0)
						p[nvp+j] = 0x8000 | 0;
					else if( va[1] == h && vb[1] == h)
						p[nvp+j] = 0x8000 | 1;
					else if( va[0] == w && vb[0] == w)
						p[nvp+j] = 0x8000 | 2;
					else if ( va[1] == 0 && vb[1] == 0)
						p[nvp+j] = 0x8000 | 3;
				}
			}
		}

		return true;

failure:

		return false;
	}

	// 合并mesh
	bool  RcMergePolyMeshes( vector<RcPolyMesh*>& meshes, const int nmeshes, RcPolyMesh& mesh)
	{
		if (!nmeshes || !meshes.size())
			return true;

		mesh.m_iNVP        = meshes[0]->m_iNVP;
		mesh.m_fCellSize   = meshes[0]->m_fCellSize;
		mesh.m_fCellHeight = meshes[0]->m_fCellHeight;
		mesh.m_vMin = meshes[0]->m_vMin;
		mesh.m_vMax = meshes[0]->m_vMax;

		int maxVerts = 0;
		int maxPolys = 0;
		int maxVertsPerMesh = 0;
		for (int i = 0; i < nmeshes; ++i)
		{
			mesh.m_vMin = Vector3Min( mesh.m_vMin, meshes[i]->m_vMin);
			mesh.m_vMax = Vector3Max( mesh.m_vMax, meshes[i]->m_vMax);
			maxVertsPerMesh = std::max<int>(maxVertsPerMesh, meshes[i]->m_iNumVerts);
			maxVerts += meshes[i]->m_iNumVerts;
			maxPolys += meshes[i]->m_numPolys;
		}

		mesh.m_iNumVerts = 0;
		mesh.m_verts.resize(maxVerts);

		mesh.m_numPolys = 0;
		mesh.m_polys.resize(maxPolys*2*mesh.m_iNVP, INVALID);

		mesh.m_regs.resize(maxPolys, 0);
		mesh.m_areaIDs.resize( maxPolys, 0);

		vector<int> nextVert(maxVerts, 0);
		vector<int> firstVert(VERTEX_BUCKET_COUNT, INVALID);
		vector<UINT> vremap(maxVertsPerMesh, 0);

		for (int i = 0; i < nmeshes; ++i)
		{
			const RcPolyMesh* pmesh = meshes[i];

			const unsigned short ox = (unsigned short)floorf((pmesh->m_vMin[0]-mesh.m_vMin[0])/mesh.m_fCellSize+0.5f);
			const unsigned short oy = (unsigned short)floorf((pmesh->m_vMin[1]-mesh.m_vMin[1])/mesh.m_fCellSize+0.5f);

			for (int j = 0; j < pmesh->m_iNumVerts; ++j)
			{
				const Int3& v = pmesh->m_verts[j];
				vremap[j] = AddVertex( v[0]+ox, v[1]+oy, v[2], mesh.m_verts, firstVert, nextVert, mesh.m_iNumVerts);
			}

			for (int j = 0; j < pmesh->m_numPolys; ++j)
			{
				UINT*		tgt = &mesh.m_polys[mesh.m_numPolys*2*mesh.m_iNVP];
				const UINT* src = &pmesh->m_polys[j*2*mesh.m_iNVP];
				mesh.m_regs[mesh.m_numPolys]	 = pmesh->m_regs[j];
				mesh.m_areaIDs[mesh.m_numPolys] = pmesh->m_areaIDs[j];

				if( pmesh->m_areaIDs[j] == 0)
					int CNIMAT = 10;

				mesh.m_numPolys++;
				for (int k = 0; k < mesh.m_iNVP; ++k)
				{
					if (src[k] == INVALID) 
						break;

					tgt[k] = vremap[src[k]];
				}
			}
		}

		// Calculate adjacency.
		if (!BuildMeshAdjacency( mesh.m_polys, mesh.m_numPolys, mesh.m_iNumVerts, mesh.m_iNVP))
		{
			return false;
		}

		return true;
	}
}