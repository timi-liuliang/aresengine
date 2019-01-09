#include <Physics/AresRecast.h>
#include <Core/AresMath.h>

namespace Ares
{
	// 辅助三角培分
	struct SRcHeightPatch
	{
		vector<WORD>	m_data;		// 高度数据
		int				m_xmin;		// x坐标 min
		int				m_ymin;		// y坐标 min ( 包围盒的底端)
		int				m_width;	// 宽
		int				m_height;	// 高

		SRcHeightPatch()
			: m_xmin( 0), m_ymin( 0), m_width( 0), m_height( 0)
		{}
	};

	// 干吗的？
	static int CircumCircle(const float xp, const float yp, const float x1, const float y1, const float x2, const float y2,
		const float x3, const float y3,
		float& xc, float& yc, float& rsqr)
	{
		static const float EPSILON = 1e-6f;

		const float fabsy1y2 = abs(y1-y2);
		const float fabsy2y3 = abs(y2-y3);

		/* Check for coincident points */
		if (fabsy1y2 < EPSILON && fabsy2y3 < EPSILON)
			return 0;

		if (fabsy1y2 < EPSILON)
		{
			const float m2 = - (x3-x2) / (y3-y2);
			const float mx2 = (x2 + x3) / 2.0f;
			const float my2 = (y2 + y3) / 2.0f;
			xc = (x2 + x1) / 2.0f;
			yc = m2 * (xc - mx2) + my2;
		}
		else if (fabsy2y3 < EPSILON)
		{
			const float m1 = - (x2-x1) / (y2-y1);
			const float mx1 = (x1 + x2) / 2.0f;
			const float my1 = (y1 + y2) / 2.0f;
			xc = (x3 + x2) / 2.0f;
			yc = m1 * (xc - mx1) + my1;
		}
		else
		{
			const float m1 = - (x2-x1) / (y2-y1);
			const float m2 = - (x3-x2) / (y3-y2);
			const float mx1 = (x1 + x2) / 2.0f;
			const float mx2 = (x2 + x3) / 2.0f;
			const float my1 = (y1 + y2) / 2.0f;
			const float my2 = (y2 + y3) / 2.0f;
			xc = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);
			if (fabsy1y2 > fabsy2y3)
				yc = m1 * (xc - mx1) + my1;
			else
				yc = m2 * (xc - mx2) + my2;
		}

		float dx,dy;

		dx = x2 - xc;
		dy = y2 - yc;
		rsqr = dx*dx + dy*dy;

		dx = xp - xc;
		dy = yp - yc;
		const float drsqr = dx*dx + dy*dy;

		return (drsqr <= rsqr) ? 1 : 0;
	}

	static int ptcmp(void* up, const void *v1, const void *v2)
	{
		const float* verts = (const float*)up;
		const float* p1 = &verts[(*(const int*)v1)*3];
		const float* p2 = &verts[(*(const int*)v2)*3];
		if (p1[0] < p2[0])
			return -1;
		else if (p1[0] > p2[0])
			return 1;
		else
			return 0;
	}

	// 三角剖分
	// Based on Paul Bourke's triangulate.c
	//  http://astronomy.swin.edu.au/~pbourke/terrain/triangulate/triangulate.c
	static void Delaunay(const int numVerts, vector<Vector3>& verts, RcIntArray& idx, RcIntArray& tris, RcIntArray& edges)
	{
		// Sort vertices
		idx.resize(numVerts);
		for (int i = 0; i < numVerts; ++i)
			idx[i] = i;

#ifdef WIN32
		qsort_s( &idx[0], idx.size(), sizeof(int), ptcmp, verts.data());
#else
        qsort_r(&idx[0], idx.size(), sizeof(int), verts.data(), ptcmp);
#endif

		// 查找polygon边界
		// Find the maximum and minimum vertex bounds.
		// This is to allow calculation of the bounding triangle
		float xmin = verts[0].x;
		float ymin = verts[0].y;
		float xmax = xmin;
		float ymax = ymin;
		for (int i = 1; i < numVerts; ++i)
		{
			xmin = min(xmin, verts[i][0]);
			xmax = max(xmax, verts[i][0]);
			ymin = min(ymin, verts[i][1]);
			ymax = max(ymax, verts[i][1]);
		}
		float dx = xmax - xmin;
		float dy = ymax - ymin;
		float dmax = (dx > dy) ? dx : dy;
		float xmiddle = (xmax + xmin) / 2.0f;
		float ymiddle = (ymax + ymin) / 2.0f;

		// Set up the supertriangle
		// This is a triangle which encompasses all the sample points.
		// The supertriangle coordinates are added to the end of the
		// vertex list. The supertriangle is the first triangle in
		// the triangle list.
		vector<Vector3> sv(3);

		sv[0][0] = xmiddle - 20 * dmax;
		sv[0][1] = ymiddle - dmax;
		sv[0][2] = 0;

		sv[1][0] = xmiddle;
		sv[1][1] = ymiddle + 20 * dmax;
		sv[1][2] = 0; 

		sv[2][0] = xmiddle + 20 * dmax;
		sv[2][1] = ymiddle - dmax;
		sv[2][2] = 0;

		tris.push_back(-3);
		tris.push_back(-2);
		tris.push_back(-1);
		tris.push_back(0); // not completed

		for (int i = 0; i < numVerts; ++i)
		{
			const float xp = verts[idx[i]][0];
			const float yp = verts[idx[i]][1];

			edges.resize(0);

			// Set up the edge buffer.
			// If the point (xp,yp) lies inside the circumcircle then the
			// three edges of that triangle are added to the edge buffer
			// and that triangle is removed.
			for (size_t j = 0; j < tris.size()/4; ++j)
			{
				int* t = &tris[j*4];

				if (t[3]) // completed?
					continue;

				const Vector3& v1 = t[0] < 0 ? sv[(t[0]+3)] : verts[idx[t[0]]];
				const Vector3& v2 = t[1] < 0 ? sv[(t[1]+3)] : verts[idx[t[1]]];
				const Vector3& v3 = t[2] < 0 ? sv[(t[2]+3)] : verts[idx[t[2]]];
				float xc,yc,rsqr;
				int inside = CircumCircle(xp, yp, v1[0],v1[1], v2[0],v2[1], v3[0],v3[1], xc,yc,rsqr);
				if (xc < xp && rcSqr(xp-xc) > rsqr)
					t[3] = 1;
				if (inside)
				{
					// Collect triangle edges.
					edges.push_back(t[0]);
					edges.push_back(t[1]);
					edges.push_back(t[1]);
					edges.push_back(t[2]);
					edges.push_back(t[2]);
					edges.push_back(t[0]);
					// Remove triangle j.
					t[0] = tris[tris.size()-4];
					t[1] = tris[tris.size()-3];
					t[2] = tris[tris.size()-2];
					t[3] = tris[tris.size()-1];
					tris.resize(tris.size()-4);
					j--;
				}
			}

			// Remove duplicate edges.
			const int ne = edges.size()/2;
			for (int j = 0; j < ne-1; ++j)
			{
				for (int k = j+1; k < ne; ++k)
				{
					// Dupe?, make null.
					if ((edges[j*2+0] == edges[k*2+1]) && (edges[j*2+1] == edges[k*2+0]))
					{
						edges[j*2+0] = 0;
						edges[j*2+1] = 0;
						edges[k*2+0] = 0;
						edges[k*2+1] = 0;
					}
				}
			}

			// Form new triangles for the current point
			// Skipping over any null.
			// All edges are arranged in clockwise order.
			for (int j = 0; j < ne; ++j)
			{
				if (edges[j*2+0] == edges[j*2+1]) continue;
				tris.push_back(edges[j*2+0]);
				tris.push_back(edges[j*2+1]);
				tris.push_back(i);
				tris.push_back(0); // not completed
			}
		}

		// Remove triangles with supertriangle vertices
		// These are triangles which have a vertex number greater than nv
		for ( size_t i = 0; i < tris.size()/4; ++i)
		{
			int* t = &tris[i*4];
			if (t[0] < 0 || t[1] < 0 || t[2] < 0)
			{
				t[0] = tris[tris.size()-4];
				t[1] = tris[tris.size()-3];
				t[2] = tris[tris.size()-2];
				t[3] = tris[tris.size()-1];
				tris.resize(tris.size()-4);
				i--;
			}
		}
		// Triangle vertices are pointing to sorted vertices, remap indices.
		for( size_t i = 0; i < tris.size(); ++i)
			tris[i] = idx[tris[i]];
	}

	static const unsigned RC_UNSET_HEIGHT = 0xffff;

	// 获取高度()
	static unsigned short GetHeight(const Vector3& pos, const float ics, const SRcHeightPatch& hp, const SRcCompactHeightField& chf)
	{
		int ix = (int)floorf( pos[0]*ics + 0.01f);
		int iy = (int)floorf( pos[1]*ics + 0.01f);
		ix = Clamp( ix-hp.m_xmin, 0, hp.m_width);
		iy = Clamp( iy-hp.m_ymin, 0, hp.m_height);
		unsigned short h = hp.m_data[ix+iy*hp.m_width];
		if( h==RC_UNSET_HEIGHT)
		{
			// Special case when data might be bad.
			// Find nearest neighbour pixel which has valid height.
			const int off[8*2] = { -1,0, -1,-1, 0,-1, 1,-1, 1,0, 1,1, 0,1, -1,1};
			float dmin = FLT_MAX;
			for (int i = 0; i < 8; ++i)
			{
				const int nx = ix+off[i*2+0];
				const int ny = iy+off[i*2+1];
				if (nx < 0 || ny < 0 || nx >= hp.m_width || ny >= hp.m_height) continue;
				const unsigned short nh = hp.m_data[nx+ny*hp.m_width];
				if (nh == RC_UNSET_HEIGHT) 
					continue;

				const float d = fabsf(nh*chf.m_cellHeight - pos[2]);
				if (d < dmin)
				{
					h = nh;
					dmin = d;
				}
			}
		}

		return h;
	}	

	// 获取高度数据
	// 参数2 : 前6个为多边形顶点索引,后6个为邻接多边线
	// 参数3 : 当前操作poly的顶点数
	// 参数4 : 
	static void GetHeightData(const SRcCompactHeightField& chf, const UINT* poly, const int numPoly, const vector<Int3>& verts, const int bs, SRcHeightPatch& hp, RcIntArray& stack)
	{
		// Floodfill the heightfield to get 2D height data,
		// starting at vertex locations as seeds.
		memset( hp.m_data.data(), 0, sizeof(WORD) * hp.m_width * hp.m_height);

		stack.resize(0);

		static const int	  offset[9*2]     = { 0,0, -1,-1, 0,-1, 1,-1, 1,0, 1,1, 0,1, -1,1, -1,0, };

		// Use poly vertices as seed points for the flood fill.
		for (int j = 0; j < numPoly; ++j)
		{
			int cx=0, cy=0, ci=-1;
			int dmin = RC_UNSET_HEIGHT;
			for( int k=0; k<9; ++k)
			{
				// 获得多边形顶点
				const int ax = verts[poly[j]][0] + offset[k*2+0];
				const int ay = verts[poly[j]][1] + offset[k*2+1];
				const int az = verts[poly[j]][2];
				// 不在heightPatch所在的范围内
				if ( ax < hp.m_xmin || ax >= (hp.m_xmin + hp.m_width) ||  ay < hp.m_ymin || ay >= (hp.m_ymin + hp.m_height))
					continue;

				const SRcCompactCell& c = chf.m_cells[(ax+bs)+(ay+bs)*chf.m_width];
				for (int i = (int)c.m_index, ni = (int)(c.m_index+c.m_count); i < ni; i++)
				{
					const RcCompactSpan& s = chf.m_spans[i];
					int d = abs( az - s.m_z);
					if ( d < dmin)
					{
						cx = ax;
						cy = ay;
						ci = i;
						dmin = d;
					}
				} 
			}
			
			if (ci != -1)
			{
				stack.push_back(cx);
				stack.push_back(cy);
				stack.push_back(ci);
			}
		}

		// Find center of the polygon using flood fill
		int pcx=0, pcy=0;
		for ( int j=0; j<numPoly; j++)
		{
			pcx += verts[poly[j]][0];
			pcy += verts[poly[j]][1];
		}
		pcx /= numPoly;
		pcy /= numPoly;

		for (size_t i = 0; i < stack.size(); i += 3)
		{
			int cx = stack[i+0];
			int cy = stack[i+1];
			int idx = cx-hp.m_xmin+(cy-hp.m_ymin)*hp.m_width;
			hp.m_data[idx] = 1;
		}

		while (stack.size() > 0)
		{
			int ci = stack.back(); stack.pop_back();
			int cy = stack.back(); stack.pop_back();
			int cx = stack.back(); stack.pop_back();

			// Check if close to center of the polygon
			if( abs(cx-pcx)<=1 && abs(cy-pcy)<=1)
			{
				stack.resize( 0);
				stack.push_back( cx);
				stack.push_back( cy);
				stack.push_back( ci);
				break;
			}

			const RcCompactSpan& cs = chf.m_spans[ci];
			for (int dir = 0; dir < 4; ++dir)
			{
				if ( RcGetCon(cs, dir) == RC_NOT_CONNECTED) 
					continue;

				const int ax = cx + RcGetDirOffsetX(dir);
				const int ay = cy + RcGetDirOffsetY(dir);

				if (ax < hp.m_xmin || ax >= (hp.m_xmin + hp.m_width) ||
					ay < hp.m_ymin || ay >= (hp.m_ymin + hp.m_height))
					continue;

				if ( hp.m_data[ax-hp.m_xmin+(ay-hp.m_ymin)*hp.m_width] != 0)
					continue;

				const int ai = (int)chf.m_cells[(ax+bs)+(ay+bs)*chf.m_width].m_index + RcGetCon(cs, dir);

				int idx = ax-hp.m_xmin+(ay-hp.m_ymin)*hp.m_width;
				hp.m_data[idx] = 1;

				stack.push_back(ax);
				stack.push_back(ay);
				stack.push_back(ai);
			}
		}	

		memset( hp.m_data.data(), 0xff, sizeof(WORD)*hp.m_width*hp.m_height);

		// Mark start locations
		for( size_t i=0; i<stack.size(); i+=3)
		{
			int cx = stack[i+0];
			int cy = stack[i+1];
			int ci = stack[i+2];
			int idx= cx-hp.m_xmin + (cy-hp.m_ymin) * hp.m_width;
			const RcCompactSpan& cs = chf.m_spans[ci];
			hp.m_data[idx] = cs.m_z;

			A_ASSERT( cs.m_z < 65535);
		}

		static const int RETRACT_SIZE = 256;
		int head = 0;

		while( head*3<(int)stack.size())
		{
			int cx = stack[head*3+0];
			int cy = stack[head*3+1];
			int ci = stack[head*3+2];
			head++;
			if (head >= RETRACT_SIZE)
			{
				head = 0;
				if (stack.size() > RETRACT_SIZE*3)
					memmove(&stack[0], &stack[RETRACT_SIZE*3], sizeof(int)*(stack.size()-RETRACT_SIZE*3));
				stack.resize(stack.size()-RETRACT_SIZE*3);
			}

			const RcCompactSpan& cs = chf.m_spans[ci];
			for (int dir = 0; dir < 4; ++dir)
			{
				if ( RcGetCon(cs, dir) == RC_NOT_CONNECTED) continue;

				const int ax = cx + RcGetDirOffsetX(dir);
				const int ay = cy + RcGetDirOffsetY(dir);

				if (ax < hp.m_xmin || ax >= (hp.m_xmin+hp.m_width) ||
					ay < hp.m_ymin || ay >= (hp.m_ymin+hp.m_height))
					continue;

				if (hp.m_data[ax-hp.m_xmin+(ay-hp.m_ymin)*hp.m_width] != RC_UNSET_HEIGHT)
					continue;

				const int ai = (int)chf.m_cells[(ax+bs)+(ay+bs)*chf.m_width].m_index + RcGetCon(cs, dir);

				const RcCompactSpan& as = chf.m_spans[ai];
				int idx = ax-hp.m_xmin+(ay-hp.m_ymin)*hp.m_width;
				hp.m_data[idx] = as.m_z;

				A_ASSERT( cs.m_z < 65535);

				stack.push_back(ax);
				stack.push_back(ay);
				stack.push_back(ai);
			}
		}
	}

	static float DistancePtSeg(const float* pt, const float* p, const float* q)
	{
		float pqx = q[0] - p[0];
		float pqy = q[1] - p[1];
		float pqz = q[2] - p[2];
		float dx = pt[0] - p[0];
		float dy = pt[1] - p[1];
		float dz = pt[2] - p[2];
		float d = pqx*pqx + pqy*pqy + pqz*pqz;
		float t = pqx*dx  + pqy*dy  + pqz*dz;
		if (d > 0)
			t /= d;
		if (t < 0)
			t = 0;
		else if (t > 1)
			t = 1;

		dx = p[0] + t*pqx - pt[0];
		dy = p[1] + t*pqy - pt[1];
		dz = p[2] + t*pqz - pt[2];

		return dx*dx + dy*dy + dz*dz;
	}

	static float DistancePtSeg2d(const Vector3& pt, const Vector3& p, const Vector3& q)
	{
		float pqx = q[0] - p[0];
		float pqy = q[1] - p[1];
		float dx = pt[0] - p[0];
		float dy = pt[1] - p[1];
		float d = pqx*pqx + pqy*pqy;
		float t = pqx*dx + pqy*dy;
		if (d > 0)
			t /= d;
		if (t < 0)
			t = 0;
		else if (t > 1)
			t = 1;

		dx = p[0] + t*pqx - pt[0];
		dy = p[1] + t*pqy - pt[1];

		return dx*dx + dy*dy;
	}

	static unsigned char GetEdgeFlags(const Vector3& va, const Vector3& vb, const vector<Vector3>& polyVerts, const int numPolyVerts)
	{
		// Return true if edge (va,vb) is part of the polygon.
		static const float thrSqr = (0.001f * 0.001f);
		for (int i = 0, j = numPolyVerts-1; i < numPolyVerts; j=i++)
		{
			if ( DistancePtSeg2d( va, polyVerts[j], polyVerts[i]) < thrSqr && 
				 DistancePtSeg2d( vb, polyVerts[j], polyVerts[i]) < thrSqr)
				return 1;
		}
		return 0;
	}

	static unsigned char GetTriFlags(const Vector3& va, const Vector3& vb, const Vector3& vc, const vector<Vector3>& polyVerts, const int numPolyVerts)
	{
		unsigned char flags = 0;

		flags |= GetEdgeFlags(va,vb, polyVerts, numPolyVerts) << 0;
		flags |= GetEdgeFlags(vb,vc, polyVerts, numPolyVerts) << 2;
		flags |= GetEdgeFlags(vc,va, polyVerts, numPolyVerts) << 4;

		return flags;
	}

	inline void vmin(float* mn, const float* v)
	{
		mn[0] = min(mn[0], v[0]);
		mn[1] = min(mn[1], v[1]);
		mn[2] = min(mn[2], v[2]);
	}

	inline void vmax(float* mx, const float* v)
	{
		mx[0] = max(mx[0], v[0]);
		mx[1] = max(mx[1], v[1]);
		mx[2] = max(mx[2], v[2]);
	}

	inline float vdot2(const Vector3& a, const Vector3& b)
	{
		return a[0]*b[0] + a[1]*b[1];
	}

	// 点到三角形距离
	static float distPtTri(const Vector3& p, const Vector3& a, const Vector3& b, const Vector3& c)
	{
		Vector3 v0, v1, v2;
		v0 = c-a;
		v1 = b-a;
		v2 = p-a;

		const float dot00 = vdot2(v0, v0);
		const float dot01 = vdot2(v0, v1);
		const float dot02 = vdot2(v0, v2);
		const float dot11 = vdot2(v1, v1);
		const float dot12 = vdot2(v1, v2);

		// Compute barycentric coordinates
		float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
		float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
		float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

		// If point lies inside the triangle, return interpolated y-coord.
		static const float EPS = 1e-4f;
		if (u >= -EPS && v >= -EPS && (u+v) <= 1+EPS)
		{
			float y = a[1] + v0[1]*u + v1[1]*v;
			return fabsf(y-p[1]);
		}
		return FLT_MAX;
	}

	static float DistToPoly(int numPolyVerts, const vector<Vector3>& polyVerts, const Vector3& p)
	{
		float dmin = FLT_MAX;
		int i, j, c = 0;
		for (i = 0, j = numPolyVerts-1; i < numPolyVerts; j = i++)
		{
			const Vector3& vi = polyVerts[i];
			const Vector3& vj = polyVerts[j];

			if (((vi.y > p[1]) != (vj.y > p[1])) &&
				(p[0] < (vj.x - vi.x) * ( p[1]-vi.y) / (vj.y-vi.y) + vi.x) )
				c = !c;
			dmin = min(dmin, DistancePtSeg2d( p, vj, vi));
		}
		return c ? -dmin : dmin;
	}

	// 到三角形的距离
	static float DistToTriMesh(const Vector3& p, const vector<Vector3>& verts, int nverts, const int* tris, int ntris)
	{
		float dmin = FLT_MAX;
		for (int i = 0; i < ntris; ++i)
		{
			const Vector3& va = verts[tris[i*4+0]];
			const Vector3& vb = verts[tris[i*4+1]];
			const Vector3& vc = verts[tris[i*4+2]];
			float d = distPtTri( p, va, vb, vc);
			if (d < dmin)
				dmin = d;
		}

		if (dmin == FLT_MAX) 
			return -1;
		
		return dmin;
	}

	// 参数1 - 输入的多边形(最少3边, 最多6边)
	static bool BuildPolyDetail(const vector<Vector3>& inPoly, const int numPolyVerts, unsigned short reg, const float sampleDist, const float sampleMaxError,
								const SRcCompactHeightField& chf, const SRcHeightPatch& hp, vector<Vector3>& verts, int& nverts, RcIntArray& tris,
								RcIntArray& edges, RcIntArray& idx,RcIntArray& samples)
	{
		static const int MAX_VERTS			= 255;
		static const int MAX_TRIS			= 521;
		static const int MAX_VERTS_PER_EDGE = 64;
		vector<float>	 edge((MAX_VERTS_PER_EDGE+1)*3);

		nverts = 0;

		for (int i = 0; i < numPolyVerts; ++i)
			verts[i] = inPoly[i];

		nverts = numPolyVerts;

		const float ics = 1.f / chf.m_cellSize;

		// Tesselate outlines.
		// This is done in separate pass in order to ensure
		// seamless height values across the ply boundaries.
		if ( sampleDist > 0)
		{ 
			for (int i = 0, j = numPolyVerts-1; i < numPolyVerts; j=i++)
			{
				Vector3 vj = inPoly[j];
				Vector3 vi = inPoly[i];
				bool swapped = false;

				// Make sure the segments are always handled in same order
				// using lexological sort or else there will be seams.
				if (fabsf(vj.x - vi.x) < 1e-6f)
				{
					if ( vj.y > vi.y)
					{
						swap( vj, vi);
						swapped = true;
					}
				}
				else
				{
					if ( vj.x > vi.x)
					{
						swap( vj, vi);
						swapped = true;
					}
				}

				// Create samples along the edge.
				float dx = vi.x - vj.x;
				float dy = vi.y - vj.y;
				float dz = vi.z - vj.z;

				// 求出边长
				float d = sqrtf(dx*dx + dy*dy);

				// 采样数量
				int nn = 1 + (int)floorf(d/sampleDist);

				// 合法性限定
				if (nn > MAX_VERTS_PER_EDGE) 
					nn = MAX_VERTS_PER_EDGE-1;

				if (nverts+nn >= MAX_VERTS)
					nn = MAX_VERTS-1-nverts;

				for (int k = 0; k <= nn; ++k)
				{//Debug:
					float u = (float)k/(float)nn;
					Vector3& pos = (Vector3&)edge[k*3];
					pos[0] = vj.x + dx*u;
					pos[1] = vj.y + dy*u;
					pos[2] = vj.z + dz*u;

					float height = GetHeight( pos, ics, hp, chf);
					if( height!=RC_UNSET_HEIGHT)
						pos[2] = height * chf.m_cellHeight;
					else
						printf( "\nBuildPolyDetail获取高度失败!");
				}
				// Simplify samples.
				int idx[MAX_VERTS_PER_EDGE] = {0,nn};
				int nidx = 2;
				for (int k = 0; k < nidx-1; )
				{
					const int a = idx[k];
					const int b = idx[k+1];
					const float* va = &edge[a*3];
					const float* vb = &edge[b*3];
					// Find maximum deviation along the segment.
					float maxd = 0;
					int maxi = -1;
					for (int m = a+1; m < b; ++m)
					{
						float d = DistancePtSeg(&edge[m*3],va,vb);
						if (d > maxd)
						{
							maxd = d;
							maxi = m;
						}
					}
					// If the max deviation is larger than accepted error,
					// add new point, else continue to next segment.
					if (maxi != -1 && maxd > (sampleMaxError * sampleMaxError) /*&& sqrtf(maxd) < 2 * sampleDist*/)
					{
						for (int m = nidx; m > k; --m)
							idx[m] = idx[m-1];
						idx[k+1] = maxi;
						nidx++;
					}
					else
					{
						++k;
					}
				}
				if( swapped)
				{
					for( int k=nidx-2; k>0; --k)
					{
						verts[nverts][0] = edge[idx[k]*3+0];
						verts[nverts][1] = edge[idx[k]*3+1];
						verts[nverts][2] = edge[idx[k]*3+2];
						nverts++;
					}
				}
				else
				{
					for (int k = 1; k < nidx-1; ++k)
					{
						verts[nverts][0] = edge[idx[k]*3+0];
						verts[nverts][1] = edge[idx[k]*3+1];
						verts[nverts][2] = edge[idx[k]*3+2];

						nverts++;
					}
				}
			}
		}

		// Tesselate the base mesh.
		edges.resize(0);
		tris.resize(0);

		Delaunay(nverts, verts, idx, tris, edges);

		if (sampleDist > 0)
		{
			// Create sample locations in a grid.
			Vector3 vMin, vMax;

			vMin = inPoly[0];
			vMax = inPoly[0];

			for (int i = 1; i < numPolyVerts; ++i)
			{
				vMin = Vector3Min( vMin, inPoly[i]);
				vMax = Vector3Max( vMax, inPoly[i]);
			}

			int x0 = (int)floorf( vMin.x / sampleDist);
			int x1 = (int)ceilf ( vMax.x / sampleDist);
			int y0 = (int)floorf( vMin.y / sampleDist);
			int y1 = (int)ceilf ( vMax.y / sampleDist);

			samples.resize(0);

			for (int y = y0; y < y1; ++y)
			{
				for (int x = x0; x < x1; ++x)
				{
					Vector3 pt;
					pt[0] = x * sampleDist;
					pt[1] = y * sampleDist;
					pt[2] = (vMax[2]+vMin[2])*0.5f;

					// Make sure the samples are not too close to the edges.
					if ( DistToPoly( numPolyVerts, inPoly, pt) > -sampleDist/2) 
						continue;

					samples.push_back(x);
					samples.push_back(y);
					samples.push_back( GetHeight( pt, ics, hp, chf));
					samples.push_back(0);
				}
			}

			// Add the samples starting from the one that has the most
			// error. The procedure stops when all samples are added
			// or when the max error is within treshold.
			const int nsamples = samples.size()/4;
			for (int iter = 0; iter < nsamples; ++iter)
			{
				// Find sample with most error.
				Vector3 bestpt( 0.f, 0.f, 0.f);
				float   bestd = 0;
				int     besti = -1;
				for (int i = 0; i < nsamples; ++i)
				{
					const int* s = &samples[i*4];
					if( s[3]) 
						continue;

					Vector3 pt;
					pt[0] = samples[i*4+0]*sampleDist;
					pt[1] = samples[i*4+1]*sampleDist;
					pt[2] = chf.m_vMin.z + samples[i*4+2]*chf.m_cellHeight;

					float d = DistToTriMesh(pt, verts, nverts, &tris[0], tris.size()/4);
					if (d < 0) continue; // did not hit the mesh.
					if (d > bestd)
					{
						bestd = d;
						besti = i;
						bestpt=pt;
					}
				}
				// If the max error is within accepted threshold, stop tesselating.
				if (bestd <= sampleMaxError || besti==-1)
					break;

				samples[besti*4+3] = 1;

				// Add the new sample point.
				verts[nverts] = bestpt;
				nverts++;

				// Create new triangulation.
				// TODO: Incremental add instead of full rebuild.
				edges.resize(0);
				tris.resize(0);
				idx.resize(0);
				Delaunay(nverts, verts, idx, tris, edges);

				if (nverts >= MAX_VERTS)
					break;
			}
		}

		const int ntris = tris.size()/4;
		if( ntris>MAX_TRIS)
		{
			tris.resize( MAX_TRIS*4);
		}

		return true;
	}

	// Builds detail triangle mesh for each polygon in the poly mesh.
	// Params:
	// mesh - (in) poly mesh to detail.
	// chf -  (in) compacy height field, used to query height for new vertices.
	// sampleDist - (in) spacing between height samples used to generate more detail into mesh.
	// sampleMaxError - (in) maximum allowed distance between simplified detail mesh and height sample.
	// pmdtl - (out) detail mesh.
	// Returns false if operation ran out of memory.
	bool RcBuildPolyMeshDetail(const RcPolyMesh& polyMesh, const SRcCompactHeightField& chf, const float sampleDist, const float sampleMaxError, SRcPolyMeshDetail& dmesh, bool& testFlag)
	{
		if ( polyMesh.m_iNumVerts == 0 || polyMesh.m_numPolys == 0)
			return true;

		const int   nvp		 = polyMesh.m_iNVP;
		const float cs		 = polyMesh.m_fCellSize;
		const float ch		 = polyMesh.m_fCellHeight;
		const Vector3& vOrig = polyMesh.m_vMin;
		const int  borderSize= polyMesh.m_borderSize;

		RcIntArray edges(64);
		RcIntArray tris(512);
		RcIntArray idx(512);
		RcIntArray stack(512);
		RcIntArray samples(512);
		vector<Vector3> verts(256);
		SRcHeightPatch hp;
		int nPolyVerts = 0;
		int maxhw = 0, maxhh = 0;
	
		vector<int>		bounds(polyMesh.m_numPolys*4);	// 记录所有多边形的xy边界	
		vector<Vector3> poly(nvp);						// 临时多边形

		// Find max size for a polygon area.
		for (int i = 0; i < polyMesh.m_numPolys; ++i)
		{
			const UINT* p = &polyMesh.m_polys[i*nvp*2];
			int& xmin = bounds[i*4+0];
			int& xmax = bounds[i*4+1];
			int& ymin = bounds[i*4+2];
			int& ymax = bounds[i*4+3];
			xmin = chf.m_width;
			xmax = 0;
			ymin = chf.m_height;
			ymax = 0;
			for (int j = 0; j < nvp; ++j)
			{
				if( p[j] == RC_MESH_NULL_IDX) 
					break;

				const Int3& v = polyMesh.m_verts[p[j]];
				xmin = min(xmin, v[0]);
				xmax = max(xmax, v[0]);
				ymin = min(ymin, v[1]);
				ymax = max(ymax, v[1]);
				nPolyVerts++;
			}
			xmin = max(0,xmin-1);
			xmax = min(chf.m_width,xmax+1);
			ymin = max(0,ymin-1);
			ymax = min(chf.m_height,ymax+1);
			if ( xmin >= xmax || ymin >= ymax) 
				continue;

			maxhw = max(maxhw, xmax-xmin);
			maxhh = max(maxhh, ymax-ymin);
		}

		hp.m_data.resize(maxhw*maxhh);

		// 初始化
		dmesh.m_numMeshs  = polyMesh.m_numPolys;
		dmesh.m_numVerts = 0;
		dmesh.m_numTris = 0;
		dmesh.m_subMeshs.resize( dmesh.m_numMeshs);

		int vcap = nPolyVerts+nPolyVerts/2;
		int tcap = vcap*2;

		dmesh.m_numVerts = 0;
		dmesh.m_verts.resize(vcap);

		dmesh.m_numTris = 0;
		dmesh.m_tris.resize( tcap);

		for (int i = 0; i < polyMesh.m_numPolys; ++i)
		{
			const UINT* tPoly = &polyMesh.m_polys[i*nvp*2];

			// Find polygon bounding box.
			int npoly = 0;
			for (int j = 0; j < nvp; ++j)
			{
				if( tPoly[j] == RC_MESH_NULL_IDX) 
					break;

				const Int3& v = polyMesh.m_verts[tPoly[j]];
				poly[j].x = v[0]*cs;
				poly[j].y = v[1]*cs;
				poly[j].z = v[2]*ch;

				npoly++;
			}

			// Get the height data from the area of the polygon.
			// polygon存储顺序为: x0, x1, y0, y1
			hp.m_xmin   = bounds[i*4+0];
			hp.m_ymin   = bounds[i*4+2];
			hp.m_width  = bounds[i*4+1]-bounds[i*4+0];
			hp.m_height = bounds[i*4+3]-bounds[i*4+2];
			GetHeightData(chf, tPoly, npoly, polyMesh.m_verts, borderSize, hp, stack);

			// Build detail mesh.
			int numVerts = 0;
			if (!BuildPolyDetail( poly, npoly, polyMesh.m_regs[i], sampleDist, sampleMaxError, chf, hp, verts, numVerts, tris, edges, idx, samples))
			{
				printf( "日，不是吧!");

				goto failure;
			}

			// Offset detail vertices, unnecassary?
			for (int j = 0; j < numVerts; ++j)
			{
				verts[j]    += vOrig;
				verts[j][2] += chf.m_cellHeight;
			}

			// Offset poly too, will be used to flag checking
			for ( int j=0; j<npoly; j++)
			{
				poly[j] += vOrig;
			}

			// Store detail submesh.
			const int ntris = tris.size()/4;

			// 记录pMeshs中记录的数据
			dmesh.m_subMeshs[i].m_vertsOffset = dmesh.m_numVerts;
			dmesh.m_subMeshs[i].m_trisOffset  = dmesh.m_numTris;
			dmesh.m_subMeshs[i].m_numVerts    = (unsigned int)numVerts;
			dmesh.m_subMeshs[i].m_numTris     = (unsigned int)ntris;

			// Store vertices, allocate more memory if necessary.
			if (dmesh.m_numVerts+numVerts > vcap)
			{
				while ( dmesh.m_numVerts + numVerts > vcap)
					vcap += 256;

				dmesh.m_verts.resize( vcap);
			}

			// 拷贝增加顶点
			for (int j = 0; j < numVerts; ++j)
			{
				dmesh.m_verts[dmesh.m_numVerts] = verts[j];
				dmesh.m_numVerts++;

				Vector3 temp = verts[j];
				if( temp.z < -1000.f || temp.y > 4096.f || temp.x > 4096.f || temp.z > 2000.f )
				{
					printf( "\n淡定，要淡定: %10.2f,%10.2f,%10.2f;", temp.x, temp.y, temp.z);
					testFlag = true;
				}
					
			}

			// Store triangles, allocate more memory if necessary.
			if (dmesh.m_numTris+ntris > tcap)
			{
				while (dmesh.m_numTris+ntris > tcap)
					tcap += 256;

				vector<RcDetailTriangle> newTris(tcap);
				if ( dmesh.m_numTris)
					memcpy( newTris.data(), dmesh.m_tris.data(), sizeof(RcDetailTriangle) * dmesh.m_numTris);

				dmesh.m_tris.swap(newTris);
			}

			for (int j = 0; j < ntris; ++j)
			{
				const int* t = &tris[j*4];
				dmesh.m_tris[dmesh.m_numTris].m_iV0Index = (unsigned char)t[0];
				dmesh.m_tris[dmesh.m_numTris].m_iV1Index = (unsigned char)t[1];
				dmesh.m_tris[dmesh.m_numTris].m_iV2Index = (unsigned char)t[2];
				dmesh.m_tris[dmesh.m_numTris].m_flag = GetTriFlags( verts[t[0]], verts[t[1]], verts[t[2]], poly, npoly);
				dmesh.m_numTris++;
			}
		}

		return true;

	failure:

		return false;
	}

	bool RcMergePolyMeshDetails( vector<SRcPolyMeshDetail*>& meshes, const int nmeshes, SRcPolyMeshDetail& mesh)
	{
		int maxVerts = 0;
		int maxTris = 0;
		int maxPolygons = 0;

		for (int i = 0; i < nmeshes; ++i)
		{
			if (!meshes[i]) continue;
			maxVerts    += meshes[i]->m_numVerts;
			maxTris     += meshes[i]->m_numTris;
			maxPolygons += meshes[i]->m_numMeshs;
		}

		mesh.m_numMeshs = 0;
		mesh.m_subMeshs.resize(maxPolygons);

		mesh.m_numTris = 0;
		mesh.m_tris.resize(maxTris);

		mesh.m_numVerts = 0;
		mesh.m_verts.resize( maxVerts);

		// Merge datas.
		for (int i = 0; i < nmeshes; ++i)
		{
			SRcPolyMeshDetail* pDetailMesh = meshes[i];
			if (!pDetailMesh) continue;

			// 拷贝多边形数据
			for (int j = 0; j < pDetailMesh->m_numMeshs; ++j)
			{
				RcDetailPoly* dst = &mesh.m_subMeshs[mesh.m_numMeshs];
				RcDetailPoly* src = &pDetailMesh->m_subMeshs[j];
				dst->m_vertsOffset = mesh.m_numVerts + src->m_vertsOffset;
				dst->m_numVerts    = src->m_numVerts;
				dst->m_trisOffset  = mesh.m_numTris + src->m_trisOffset;
				dst->m_numTris     = src->m_numTris;
				mesh.m_numMeshs++;
			}

			// 拷贝顶点数据
			for (int k = 0; k < pDetailMesh->m_numVerts; ++k)
			{
				mesh.m_verts[mesh.m_numVerts] = pDetailMesh->m_verts[k];
				mesh.m_numVerts++;

				//-------------------------------------------
				Vector3 temp = pDetailMesh->m_verts[k];
				if( temp.z < -1000.f || temp.y > 4096.f || temp.x > 4096.f)
					printf( "\n无奈了: %10.2f,%10.2f,%10.2f;", temp.x, temp.y, temp.z);

			}

			// 拷贝三角形数据
			for (int k = 0; k < pDetailMesh->m_numTris; ++k)
			{
				mesh.m_tris[mesh.m_numTris] = pDetailMesh->m_tris[k];
				mesh.m_numTris++;
			}
		}

		return true;
	}
}