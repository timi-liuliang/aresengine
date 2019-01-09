#include <Physics/KdtTriangleMesh.h>
#include <Physics/KdtBuilder.h>
#include <Physics/Intersect.h>

BOOST_CLASS_EXPORT( Ares::KdtTriangleMesh)

namespace Ares
{
	// 栈 辅助RayTrace
	struct KdtStack
	{
		const KdtNode<int>*	m_node;
		float				m_far;
		float				m_near;
	};

	// 构造函数
	KdtTriangleMesh::KdtTriangleMesh()
		: Shape( ST_KdtTriangleMesh)
	{
		for ( int i=0; i<8; i++)
		{
			int rdx = i & 1;
			int rdy = (i>>1) & 1;
			int rdz = (i>>2) & 1;
			m_rayDir[i][0][0] = rdx, m_rayDir[i][0][1] = rdx ^ 1;
			m_rayDir[i][1][0] = rdy, m_rayDir[i][1][1] = rdy ^ 1;
			m_rayDir[i][2][0] = rdz, m_rayDir[i][2][1] = rdz ^ 1;
		}
	}

	// 析构函数
	KdtTriangleMesh::~KdtTriangleMesh()
	{

	}

	// 初始化
	bool KdtTriangleMesh::Init( size_t numVertexs, Vector3* vertsData, size_t numTris, int* trisData)
	{
		Reset();

		// 记录顶点数据
		m_verts.reserve( numVertexs);
		for( size_t i=0; i<numVertexs; i++)
		{
			m_verts.push_back( vertsData[i]);
			m_localAabb.UnionPoint( vertsData[i]);
		}
		
		m_tris.reserve( numTris);
		for( size_t i=0; i<numTris; i++)
		{
			Int3 triFace;
			triFace[0] = trisData[i*3+0];
			triFace[1] = trisData[i*3+1];
			triFace[2] = trisData[i*3+2];

			m_tris.push_back( triFace);
		}

		// 构建KdTree
		vector<Rect3>	keys;	keys.reserve( numTris);
		vector<int>		values; values.reserve( numTris);
		for( size_t i=0; i<numTris; i++)
		{
			values.push_back( i);
			
			Rect3 rect;
			rect.UnionPoint( m_verts[m_tris[i][0]]);
			rect.UnionPoint( m_verts[m_tris[i][1]]);
			rect.UnionPoint( m_verts[m_tris[i][2]]);
			keys.push_back( rect);
		}

		KdtBuilder<int> kdtBuilder( keys, values, m_kdt);

		return false;
	}

	// 射线检测
	bool KdtTriangleMesh::RayTrace( const Vector3& rayOrig, const Vector3& rayDir, float& dist, Vector3& normal) const
	{
		bool isIntersected = false;

		const KdtNode<int>* node = m_kdt.GetRoot();
		if( !node) return false;

		// 朝向求逆
		Vector3 dirInv( 1.f/rayDir.x, 1.f/rayDir.y, 1.f/rayDir.z);
		int			oct = ((rayDir.x<0)?1:0) + ((rayDir.y<0)?2:0)+((rayDir.z<0)?4:0);
		const int*	rdir= &m_rayDir[oct][0][0];

		float	tNear	= 0;
		float	tFar	= dist;
		int     stackPtr= 0;
		vector<KdtStack> stack(60);
		// traversal
		while( true)
		{
			while( !node->IsLeaf())
			{
				int axis = node->GetAxis();
				const KdtNode<int>* front = node->GetLeft( m_kdt.m_root) + rdir[axis*2];
				const KdtNode<int>* back  = node->GetLeft( m_kdt.m_root) + rdir[axis*2+1];
				float		split = (node->m_split - rayOrig[axis])*dirInv[axis];

				node = back;
				if( split<tNear) continue;
				node = front;
				if( split>tFar) continue;

				stack[stackPtr].m_far   = tFar;
				stack[stackPtr++].m_node= back;
				tFar				  = std::min<float>( tFar, split);
			}

			// leaf node found
			int memOffset = node->GetMemberOffset();
			int memCount  = node->GetMemberCount();
			for( int i=0; i<memCount; i++)
			{
				float tDist = dist;
				Triangle3 triangle = GetTriangle( m_kdt.m_memberList[memOffset+i]);
				if( Intersect( Line3( rayOrig, rayDir), triangle, tDist))
				{
					if( tDist>tNear && tDist<dist)
					{
						dist   = tDist;
						normal = triangle.GetNormal();

						isIntersected = true;
					}
				}

				//Vector3	  triNormal= triangle.GetNormal();
				//Vector3	  ao	   = triangle.m_v[0] - rayOrig;
				//if( Vector3Dot( ao, triNormal)<0)  continue;
				//Vector3	  bo	   = triangle.m_v[1] - rayOrig;
				//Vector3   co	   = triangle.m_v[2] - rayOrig;
				//Vector3	  v1c	   = Vector3Cross( bo, ao);
				//Vector3	  v2c	   = Vector3Cross( ao, co);
				//Vector3	  v0c	   = Vector3Cross( co, bo);
				//float	  nominator= Vector3Dot( ao, triNormal);

				//// 相交测试
				//float v0d = Vector3Dot( v0c, rayDir);
				//float v1d = Vector3Dot( v1c, rayDir);
				//float v2d = Vector3Dot( v2c, rayDir);
				//if( v0d>0.f && v1d>0.f && v2d>0.f)
				//{
				//	float t = nominator * ( 1.f/Vector3Dot( rayDir, triNormal));
				//	if( t<dist && t>=0)
				//	{
				//		dist   = t;
				//		normal = triNormal;

				//		isIntersected= true;
				//	}
				//}
			}

			// terminate, or pop node from stack
			if( (dist<tFar) || !stackPtr) break;
			node  = stack[--stackPtr].m_node;
			tNear = tFar;
			tFar  = stack[stackPtr].m_far;
		}

		return isIntersected;
	}

	// 构建AABB包围盒
	void KdtTriangleMesh::BuildAABB( Rect3& box, const Transform& trans) const 
	{
		box = m_localAabb;
	}

	// 获取包围盒中三角形索引
	void KdtTriangleMesh::GetTriIdxs( set<int>& triIdxs, const Rect3& aabb) const
	{
		vector<int> results;
		m_kdt.ShapeIntersectionTest( results, m_kdt.GetRoot(), &aabb);

		for( size_t i=0; i<results.size(); i++)
			triIdxs.insert( results[i]);
	}

	// 获取三角形
	Triangle3 KdtTriangleMesh::GetTriangle( size_t idx) const
	{
		A_ASSERT( idx<m_tris.size());

		return Triangle3( m_verts[m_tris[idx][0]], m_verts[m_tris[idx][1]], m_verts[m_tris[idx][2]]);
	}

	// 重置数据
	void KdtTriangleMesh::Reset()
	{
		m_verts.clear();
		m_tris.clear();
		m_localAabb.Reset();
	}
}