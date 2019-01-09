//#pragma once
//
//#include "Line3.h"
//#include "Capsule3.h"
//#include "Box3.h"
//#include "Sphere3.h"
//#include "Triangle3.h"
//#include <string>
//#include <vector>
//#include <map>
//#include <set>
//
//using namespace std;
//
//namespace Ares
//{
//	// 面
//	struct Face
//	{
//		int m_idx[3];
//	};
//
//	// 碰撞信息
//	struct SCapsuleCollInfo
//	{
//		Vector3  m_triCollPoint;	// 碰撞点
//		Vector3  m_collNormal;		// 碰撞法线,包含穿刺深度信息
//		Vector3  m_triNormal;		// 三角形法线
//	};
//	
//	//------------------------------------
//	// collmesh   2011-09-02 帝林
//	//------------------------------------
//	class BvhTriangleMesh : public Shape
//	{
//	public:
//		// Header
//		struct SHeader
//		{
//			int				m_version;			// 文件版本
//			size_t			m_numVerts;			// 顶点数
//			size_t			m_numTris;			// 三角形数
//			int				m_numGrids;			// 格子数
//			float			m_edgeLength;		// 边长度
//
//			int				m_dataSize;			// 数据大小
//			int				m_vertsOffset;		// 顶点数据偏移
//			int				m_trisOffset;		// 三角形偏移
//			int				m_triIdxsOffset;	// 三角形索引偏移
//			int				m_gridDataOffset;	// 格子数据偏移
//
//			Rect3			m_boundingBox;		// 包围盒
//		};
//
//		// 格子数据,记录三角形信息
//		struct SGridInfo
//		{
//			char	m_id[64];			// 格子ID
//			size_t  m_baseOffset;		// 基偏移
//			size_t  m_numTris;			// 三角形数量
//		};
//
//	public:
//		// 构造函数
//		BvhTriangleMesh();
//
//		// 析构函数
//		~BvhTriangleMesh();
//
//		// constructor AABB bounding box
//		virtual void BuildAABB( Rect3& box, const RTTransform& trans) const;
//
//	public:
//		// 设置数据
//		bool Init( size_t numVertexs, Vector3* vertsData, size_t numTris, int* trisData, const Matrix44* matInit=NULL, float edgeLength=2.f);
//
//		// 射线检测( 默认检测距离1000单位)
//		bool Pick( const Line3& line, float& dist, Vector3* _intersectPos=NULL) const;
//
//		// 与Capsule进行碰撞检测
//		int  CheckCapsuleColl( const Capsule3& capsule, vector<SCapsuleCollInfo>& outInfo);
//
//		// Interect with box3
//		bool CheckOBBColl( const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& dir, float& length, bool bCalcMin/*=true*/);
//
//	public:
//		// 获取包围盒
//		const Rect3& GetBoundingBox() const { return m_header.m_boundingBox; }
//
//	public:
//		// 加载
//		bool Load( const char* fileName);
//
//		// 保存
//		void Save( const char* fileName);
//
//		// 重置数据
//		void Reset();
//
//		// 根据面数据组织三角形
//		Triangle3 GetTriangle( const Face* face) const;
//
//		// 获取索引
//		bool GetGridId( int x, int y, int z, string& outId) const;
//
//		// 获取格子包围球
//		void GetGridSphere3( int x, int y, int z, Sphere3& sphere3);
//
//		// 获取AABB包围盒中所有面
//		size_t GetFace( set<const Face*>& out, set<const Face*>& without, const Rect3& boundingBox) const;
//
//		// 获取OBB中的所有面
//		int GetFace( set<const Face*>& out, set<const Face*>& without, const Box3& box3);
//
//		// 构建OBB
//		void BuildOBB( vector<Box3>& obbs, const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& dir, float length, float segmentLength);
//
//	private:
//		typedef map<string, SGridInfo*> GridInfo;
//
//		SHeader			m_header;		// 文件头
//		unsigned char*	m_data;			// 数据
//
//		Vector3*		m_verts;		// 顶点
//		Face*			m_tris;			// 三角形
//		size_t*			m_triIdxs;		// 三角形索引
//		SGridInfo*		m_gridData;		// 格子数据
//
//		GridInfo        m_gridInfos;	// 格子索引,快速检索
//	};
//}