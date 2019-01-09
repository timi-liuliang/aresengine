//#pragma once
//
//#include "BvhTriangle3Mesh.h"
//#include "Physics/Accelerators/QuadTree.hpp"
//#include <map>
//
//using namespace std;
//using namespace boost;
//
//namespace Ares
//{
//	//------------------------------------------
//	// 碰撞图 2011-06-29  帝林
//	// 使用网格分形
//	// 特点：
//	// 1. 静态的
//	// 2. 无法确切知道碰撞物属性
//	//------------------------------------------
//	class CCollMap
//	{
//		typedef map< string, quad_tree_member<BvhTriangleMesh*>> TreeMemberMap;
//
//		// 格子数据,记录三角形信息
//		struct SGridInfo
//		{
//			size_t  m_baseOffset;		// 基偏移
//			size_t  m_numTris;			// 三角形数量
//
//			// 构造函数
//			SGridInfo()
//				: m_baseOffset(0), m_numTris(0)
//			{}
//		};
//
//	public:
//		// 加载
//		void Load( const char* fileName);
//
//		// 点选( 默认检测距离1000单位)
//		bool Pick( const Line3& line, float& dist, Vector3& _intersectPos) const;
//
//		// capsule 检测
//		int  CheckCapsuleColl( const Capsule3& capsule, vector<SCapsuleCollInfo>& collInfo) const;
//
//		// obb检测, 下图为顶点顺序// 0  1// 3  2
//		int CheckOBBColl( const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& dir, float& length, bool bCalcMin=true) const;
//
//	public:
//		// 添加简模
//		bool AddCollMesh( const char* uniqueName, BvhTriangleMesh* triMesh);
//
//		// 删除简模
//		bool DeleteCollMesh( const char* uniqueName);
//
//	public:
//		// 构造函数
//		CCollMap();
//
//		// 析构函数
//		~CCollMap();
//
//		// 碰撞图生成( note:z轴向上)
//		void Init( Vector3* pVerts, int numVerts, int* pTris, int numTris);
//
//		// 设置场景大小
//		void SetBoundingBox( const Rect3& boundingBox);
//
//		// 设置高度图
//		void SetHeightField( const HeightField* heightField);
//
//		// 获取索引
//		bool  GetGridId( size_t x, size_t y, size_t& outId) const;
//
//		// 保存
//		void Save( const char* fileName);
//
//	private:
//		size_t				m_numVerts;			// 顶点数
//		int					m_numTris;			// 三角形数
//		Vector3*			m_verts;			// 顶点
//		Face*				m_tris;				// 三角形
//
//		size_t				m_width;			// 宽
//		size_t				m_height;			// 长
//		Rect3				m_boundingBox;		// 包围盒
//
//		size_t*				m_triIdxs;			// 三角形索引			
//
//		SGridInfo*			m_gridInfos;		// 格子信息
//
//		const HeightField*   m_heightField;		// 高度图
//		QuadTree<BvhTriangleMesh*>*  m_quadTree;		// 四叉树 
//		TreeMemberMap		  m_memberMap;		// 成员
//	};
//}