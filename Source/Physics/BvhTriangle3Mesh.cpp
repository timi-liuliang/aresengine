//#include "Physics/Shapes/Algorithm/Intersect.h"
//#include "Physics/Shapes/Algorithm/IntrCapsule3Triangle3.h"
//#include "Physics/Shapes/Algorithm/IntrBox3Triangle3.h"
//#include "Physics/Shapes/Algorithm/IntrBox3Sphere3.h"
//#include "Physics/Shapes/Algorithm/IntrRect3Rect3.h"
//#include <Core/AresMacros.h>
//#include <Physics/Shapes/BvhTriangle3Mesh.h>
//#include <sstream>
//
//namespace Ares
//{
//	// 构造函数
//	BvhTriangleMesh::BvhTriangleMesh()
//		: Shape( ST_UnKnown)
//	{
//		m_data	   = NULL;
//
//		Reset();
//	}
//
//	BvhTriangleMesh::~BvhTriangleMesh()
//	{
//		Reset();
//	}
//
//	// constructor AABB bounding box
//	void BvhTriangleMesh::BuildAABB( Rect3& box, const RTTransform& trans) const
//	{
//		box = m_header.m_boundingBox;
//
//		box.Transform( &trans.GetMatrix());
//	}
//
//	// 重置数据
//	void BvhTriangleMesh::Reset()
//	{
//		m_header.m_version  = 1;
//		m_header.m_numTris  = 0;
//		m_header.m_numVerts = 0;
//
//		m_header.m_dataSize		  = 0;
//		m_header.m_vertsOffset	  = 0;
//		m_header.m_trisOffset	  = 0;
//		m_header.m_triIdxsOffset  = 0;
//		m_header.m_gridDataOffset = 0;
//
//		SAFE_DELETE_ARRAY( m_data);
//
//		m_verts    = NULL;
//		m_tris     = NULL;
//		m_triIdxs  = NULL;		// 三角形索引
//		m_gridData = NULL;		// 格子数据
//
//		m_gridInfos.clear();
//	}
//
//	// 与Capsule进行碰撞检测
//	int  BvhTriangleMesh::CheckCapsuleColl( const Capsule3& capsule, vector<SCapsuleCollInfo>& outInfo)
//	{
//		Rect3 tRect3;
//		capsule.BuildAABB( tRect3);
//
//		IntrRect3Rect3 intrRR( tRect3, m_header.m_boundingBox);
//		intrRR.Test();
//		tRect3 = intrRR.m_intrRect;
//
//		// 得到应记录的所有网格
//		set<const Face*> tTriIds;
//		set<const Face*> tWithout;
//		GetFace( tTriIds, tWithout, tRect3);
//
//		SCapsuleCollInfo tCollInfo;
//		for ( set<const Face*>::const_iterator it = tTriIds.begin(); it!=tTriIds.end(); ++it)
//		{
//			// 射线与三角形碰撞
//			Triangle3 triangle( m_verts[(*it)->m_idx[0]], m_verts[(*it)->m_idx[1]], m_verts[(*it)->m_idx[2]]);
//
//			IntrCapsule3Triangle3 intrCT( capsule, triangle);
//			if( intrCT.Test())
//			{
//				tCollInfo.m_triCollPoint = intrCT.m_intrPointTriangle;
//				tCollInfo.m_collNormal   = intrCT.m_intrPointTriangle - intrCT.m_intrPointCapsule;
//				tCollInfo.m_triNormal	 = triangle.GetNormal();
//
//				outInfo.push_back( tCollInfo);
//			}
//		}
//
//		return outInfo.size();
//	}
//
//	// Interect with box3
//	bool BvhTriangleMesh::CheckOBBColl( const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& dir, float& length, bool bCalcMin/*=true*/)
//	{
//		// 计算前初始化
//		if( !bCalcMin)
//			length = 0.f;
//
//		Vector3 axis2 = dir;
//		axis2.Normalize();
//
//		// 构建有向包围盒
//		vector<Box3> obbs;
//		BuildOBB( obbs, v0, v1, v2, v3, axis2, length, length/*m_header.m_edgeLength*/);
//		
//		set<const Face*> tWithout;
//		for ( size_t i=0; i<obbs.size(); i++)
//		{
//			const Box3& tBox = obbs[i];
//
//			// 得到应记录的所有网格
//			set<const Face*> tTriIds;
//			GetFace( tTriIds, tWithout, tBox);
//
//			bool stopFlag = false;
//			for ( set<const Face*>::const_iterator it = tTriIds.begin(); it!=tTriIds.end(); ++it)
//			{
//				// 射线与三角形碰撞
//				Triangle3 triangle = GetTriangle( *it);
//
//				IntrBox3Triangle3 intrBT( tBox, triangle);
//				if( intrBT.Test())
//				{
//					intrBT.Find();
//
//					for ( int j=0; j<intrBT.m_quantity; j++)
//					{
//						Vector3 proj = ( intrBT.m_akPoint[j] - v0);
//						float tLength = Vector3Dot( proj, dir);
//
//						if( bCalcMin)
//							length = std::min<float>( length, tLength);
//						else
//							length = std::max<float>( length, tLength);
//					}
//
//					stopFlag = true;
//				}
//			}
//
//			if( stopFlag) 
//				return true;
//		}
//
//		return false;
//	}
//
//	// 构建OBB
//	void BvhTriangleMesh::BuildOBB( vector<Box3>& obbs, const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& dir, float length, float segmentLength)
//	{
//		obbs.clear();
//
//		int numSection = static_cast<int>( std::ceil( std::abs( length / segmentLength)));
//
//		Vector3 tVector[4];
//		for ( float i=0; i<numSection; i++)
//		{
//			Vector3 extLength = i * dir * segmentLength;
//			tVector[0] = v0 + extLength;
//			tVector[1] = v1 + extLength;
//			tVector[2] = v2 + extLength;
//			tVector[3] = v3 + extLength;
//
//			// 构建有向包围盒
//			Vector3 center = (tVector[1] + tVector[3] + dir * segmentLength) / 2.f,
//					axis0  = (tVector[0] - tVector[3]) / 2.f,
//					axis1  = (tVector[2] - tVector[3]) / 2.f,
//					axis2  = dir;
//
//			float  extent0 = axis0.Length();
//			float  extent1 = axis1.Length();
//
//			axis0 /= extent0;
//			axis1 /= extent1;
//
//			obbs.push_back(Box3( center, axis0, axis1, axis2, extent0, extent1, segmentLength / 2.f));
//		}
//	}
//
//	// 点选
//	bool BvhTriangleMesh::Pick(  const Line3& line, float& dist, Vector3* _intersectPos) const
//	{
//		// 与其中的三角形进行判断
//		float distance = 1e30f;
//
//		Rect3 tRect3;
//		line.BuildAABB( tRect3, RTTransform());
//
//		IntrRect3Rect3 intrRR( tRect3, m_header.m_boundingBox);
//		intrRR.Test();
//		tRect3 = intrRR.m_intrRect;
//
//		// 得到应记录的所有网格
//		set<const Face*> tTriIds;
//		set<const Face*> tWithout;
//		GetFace( tTriIds, tWithout, tRect3);
//
//		for ( set<const Face*>::const_iterator it = tTriIds.begin(); it!=tTriIds.end(); ++it)
//		{
//			// 射线与三角形碰撞
//			Triangle3 triangle = GetTriangle( *it);
//
//			float tDist = 1e30f;
//			if( Intersect( line, triangle, tDist))
//			{
//				if( tDist >= 0.f)
//					distance = std::min<float>( distance, tDist);
//			}
//		}
//
//		if( distance < 1e30f)
//		{
//			dist = distance;
//
//			if( _intersectPos)
//				*_intersectPos = line.m_orig + line.m_dir * dist;
//
//			return true;
//		}
//
//		return false;
//	}
//
//	// 设置数据
//	bool BvhTriangleMesh::Init( size_t numVertexs, Vector3* vertsData, size_t numTris, int* trisData, const Matrix44* matInit/*=NULL*/, float edgeLength/*=2.f*/)
//	{
//		Reset();
//
//		m_header.m_numVerts  = numVertexs;
//		m_header.m_numTris   = numTris;
//		m_header.m_edgeLength= edgeLength;
//
//		if( !numTris || !m_header.m_numVerts)
//			return false;
//
//		// 顶点偏移
//		m_header.m_vertsOffset = 0;
//		m_header.m_dataSize   += sizeof(Vector3) * m_header.m_numVerts;
//
//		m_header.m_trisOffset =  m_header.m_dataSize;
//		m_header.m_dataSize  += sizeof(Face) * m_header.m_numTris;
//
//		// 引用 
//		m_verts = vertsData;
//		m_tris  = (Face*)trisData;
//
//		if( matInit)
//		{
//			m_verts = new_ Vector3[m_header.m_numVerts];
//
//			for ( size_t i=0; i<m_header.m_numVerts; i++)
//			{
//				Vector3* tVert = (Vector3*)vertsData + i;
//
//				m_verts[i] = matInit->TransformVector3( *tVert);
//			}
//		}
//
//		// 临时变量,用于数据统计
//		map<string, vector<int>> tGridTriIdxs;
//
//		// 放置每一个三角形
//		Rect3& boundingBox = m_header.m_boundingBox;
//		for ( size_t i=0; i<m_header.m_numTris; i++)
//		{
//			const Face& face = m_tris[i];
//
//			Rect3 tRect3;
//
//			// 获取包围盒
//			for ( int j=0; j<3; j++)
//			{
//				const Vector3& v = m_verts[face.m_idx[j]];
//
//				tRect3.UnionPoint( v);
//				boundingBox.UnionPoint( v);
//			}
//
//			// 得到应记录的所有网格
//			int minx = (int)(tRect3.x0 / edgeLength);
//			int maxx = (int)(tRect3.x1 / edgeLength);
//
//			int miny = (int)(tRect3.y0 / edgeLength);
//			int maxy = (int)(tRect3.y1 / edgeLength);
//
//			int minz = (int)(tRect3.z0 / edgeLength);
//			int maxz = (int)(tRect3.z1 / edgeLength);
//
//			for ( int z=minz; z<=maxz; z++)
//			{			
//				for ( int y=miny; y<=maxy; y++)
//				{
//					for ( int x=minx; x<=maxx; x++)
//					{
//						string index; 
//						if( GetGridId( x, y, z, index))
//							tGridTriIdxs[index].push_back(i);
//					}
//				}
//			}
//		}
//
//		// 整理
//		size_t tTotalTri = 0;
//		for( map<string, vector<int>>::iterator it=tGridTriIdxs.begin(); it!=tGridTriIdxs.end(); it++)
//		{
//			tTotalTri += it->second.size();
//		}
//
//		// 分配空间, 填充数据
//		m_header.m_triIdxsOffset  =  m_header.m_dataSize;
//		m_header.m_dataSize      += sizeof(size_t) * tTotalTri;
//		m_header.m_gridDataOffset =  m_header.m_dataSize;
//		m_header.m_dataSize		 += sizeof(SGridInfo) * tGridTriIdxs.size();
//
//		m_header.m_numGrids = tGridTriIdxs.size();
//
//		// 分配数据
//		m_data = new_ unsigned char[m_header.m_dataSize];
//
//		memcpy( m_data + m_header.m_vertsOffset, m_verts, sizeof( Vector3) * m_header.m_numVerts);
//		if( matInit)
//			SAFE_DELETE_ARRAY( m_verts);
//
//		m_verts = (Vector3*)(m_data + m_header.m_vertsOffset);
//
//		memcpy( m_data+m_header.m_trisOffset, m_tris, sizeof( Face) * m_header.m_numTris);
//		m_tris = (Face*)(m_data+m_header.m_trisOffset);
//
//		m_triIdxs = (size_t*)(m_data+m_header.m_triIdxsOffset);
//		m_gridData= (SGridInfo*)(m_data+m_header.m_gridDataOffset);
//
//		// 索引计算
//		size_t tOffset = 0;
//		int    i=0;
//		for( map<string, vector<int>>::iterator it=tGridTriIdxs.begin(); it!=tGridTriIdxs.end(); it++)
//		{
//			strcpy( m_gridData[i].m_id, it->first.c_str());
//			m_gridData[i].m_baseOffset = tOffset;
//			m_gridData[i].m_numTris    = it->second.size();
//
//			for ( size_t j=0; j<it->second.size(); j++)
//			{
//				m_triIdxs[tOffset] = it->second[j];
//				tOffset++;
//			}
//
//			i++;
//		}
//
//		// 更新gridInfos数据
//		for ( int i=0; i<m_header.m_numGrids; i++)
//		{
//			m_gridInfos[m_gridData[i].m_id] = &m_gridData[i];
//		}
//
//		return true;
//	}
//
//	// 获取索引
//	bool  BvhTriangleMesh::GetGridId( int x, int y, int z, string& outId) const
//	{
//		static ostringstream oss;
//
//		oss.str("");
//		oss << x << '_' << y << '_' << z;
//		outId = oss.str();
//
//		return true;
//	}
//
//	// 获取格子包围球
//	void BvhTriangleMesh::GetGridSphere3( int x, int y, int z, Sphere3& sphere3)
//	{
//		sphere3.m_radius = m_header.m_edgeLength / 2.f;
//		// 得到应记录的所有网格
//		float minx = (x<=0 ? x-1 : x) * m_header.m_edgeLength;
//		float maxx = (x>=0 ? x+1 : x) * m_header.m_edgeLength;
//
//		float miny = (y<=0 ? y-1 : y) * m_header.m_edgeLength;
//		float maxy = (y>=0 ? y+1 : y) * m_header.m_edgeLength;
//
//		float minz = (z<=0 ? z-1 : z) * m_header.m_edgeLength;
//		float maxz = (z<=0 ? z+1 : z) * m_header.m_edgeLength;
//
//		sphere3.m_center = Vector3( minx+maxx, miny+maxy, minz+maxz) * 0.5f;
//		sphere3.m_radius = max(max( maxx-minx, maxy-miny), maxz-minz) * 0.5f;
//	}
//
//	// 获取AABB包围盒中所有面
//	size_t BvhTriangleMesh::GetFace( set<const Face*>& out, set<const Face*>& without, const Rect3& boundingBox) const
//	{
//		IntrRect3Rect3 intrRR( boundingBox, m_header.m_boundingBox);
//		intrRR.Test();
//
//		// 得到应记录的所有网格
//		int minx = (int)(intrRR.m_intrRect.x0 / m_header.m_edgeLength);
//		int maxx = (int)(intrRR.m_intrRect.x1 / m_header.m_edgeLength);
//
//		int miny = (int)(intrRR.m_intrRect.y0 / m_header.m_edgeLength);
//		int maxy = (int)(intrRR.m_intrRect.y1 / m_header.m_edgeLength);
//
//		int minz = (int)(intrRR.m_intrRect.z0 / m_header.m_edgeLength);
//		int maxz = (int)(intrRR.m_intrRect.z1 / m_header.m_edgeLength);
//
//		out.clear();
//
//		for ( int z=minz; z<=maxz; z++)			
//			for ( int y=miny; y<=maxy; y++)
//				for ( int x=minx; x<=maxx; x++)
//				{
//					string index; 
//					if( GetGridId( x, y, z, index))
//					{
//						GridInfo::const_iterator it = m_gridInfos.find( index);
//						if( it != m_gridInfos.end())
//						{
//							SGridInfo* tInfo = it->second;
//							for ( size_t i=0; i<tInfo->m_numTris; i++)
//							{
//								const Face* tIdxs = &m_tris[m_triIdxs[tInfo->m_baseOffset+i]];
//
//								if( without.find( tIdxs)==without.end())
//								{
//									out.insert( tIdxs);
//									without.insert( tIdxs);
//								}
//							}
//						}
//					}
//				}
//
//		return out.size();
//	}
//
//	// 获取OBB中的所有面
//	int BvhTriangleMesh::GetFace( set<const Face*>& out, set<const Face*>& without, const Box3& box3)
//	{
//		Rect3 box3AABB;
//		box3.BuildAABB( box3AABB);
//
//		IntrRect3Rect3 intrRR( box3AABB, m_header.m_boundingBox);
//		intrRR.Test();
//
//		// 得到应记录的所有网格
//		int minx = (int)(intrRR.m_intrRect.x0 / m_header.m_edgeLength);
//		int maxx = (int)(intrRR.m_intrRect.x1 / m_header.m_edgeLength);
//
//		int miny = (int)(intrRR.m_intrRect.y0 / m_header.m_edgeLength);
//		int maxy = (int)(intrRR.m_intrRect.y1 / m_header.m_edgeLength);
//
//		int minz = (int)(intrRR.m_intrRect.z0 / m_header.m_edgeLength);
//		int maxz = (int)(intrRR.m_intrRect.z1 / m_header.m_edgeLength);
//
//		out.clear();
//
//		for ( int z=minz; z<=maxz; z++)			
//			for ( int y=miny; y<=maxy; y++)
//				for ( int x=minx; x<=maxx; x++)
//				{
//					string  index;
//					Sphere3 sphere3; 
//					GetGridSphere3( x, y, z, sphere3);
//					IntrBox3Sphere3 intrBS( box3, sphere3);
//					if( GetGridId( x, y, z, index) && intrBS.Test())
//					{
//						GridInfo::const_iterator it = m_gridInfos.find( index);
//						if( it != m_gridInfos.end())
//						{
//							SGridInfo* tInfo = it->second;
//							for ( size_t i=0; i<tInfo->m_numTris; i++)
//							{
//								const Face* tIdxs = &m_tris[m_triIdxs[tInfo->m_baseOffset+i]];
//								if( without.find( tIdxs)==without.end())
//								{
//									out.insert( tIdxs);
//									without.insert( tIdxs);
//								}
//							}
//						}
//					}
//				}
//
//		return out.size();
//	}
//
//	// 根据面数据组织三角形
//	Triangle3 BvhTriangleMesh::GetTriangle( const Face* face) const
//	{
//		A_ASSERT( face);
//
//		return Triangle3( m_verts[face->m_idx[0]], m_verts[face->m_idx[1]], m_verts[face->m_idx[2]]);
//	}
//
//	// 加载
//	bool BvhTriangleMesh::Load( const char* fileName)
//	{
//		// 重置
//		Reset();
//
//		FILE* fileHandle = fopen( fileName, "rb");
//		if( fileHandle)
//		{
//			// 头标识
//			fread( &m_header, sizeof( m_header), 1, fileHandle);
//
//			// 数据 
//			if( m_header.m_dataSize)
//			{
//				m_data = new_ unsigned char[ m_header.m_dataSize];
//				fread( m_data, m_header.m_dataSize, 1, fileHandle);
//			}
//
//
//			fflush( fileHandle);
//			fclose( fileHandle);
//
//			m_verts = (Vector3*)(m_data+m_header.m_vertsOffset);
//			m_tris  = (Face*)(m_data+m_header.m_trisOffset);
//			m_triIdxs = (size_t*)(m_data+m_header.m_triIdxsOffset);
//			m_gridData= (SGridInfo*)(m_data+m_header.m_gridDataOffset);
//
//			// 更新gridInfos数据
//			for ( int i=0; i<m_header.m_numGrids; i++)
//			{
//				m_gridInfos[m_gridData[i].m_id] = &m_gridData[i];
//			}
//
//			return true;
//		}
//
//		return false;
//	}
//
//	// 保存
//	void BvhTriangleMesh::Save( const char* fileName)
//	{
//		FILE* fileHandle = fopen( fileName, "wb");
//		if( fileHandle)
//		{
//			// 版本号
//			fwrite( &m_header, sizeof( m_header), 1, fileHandle);
//
//			// 三角形数据
//			fwrite( m_data, m_header.m_dataSize, 1, fileHandle);
//
//			fflush( fileHandle);
//			fclose( fileHandle);
//		}
//	}
//}