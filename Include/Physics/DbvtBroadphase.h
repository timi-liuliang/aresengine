#pragma once

#include "Dbvt.h"
#include "Broadphase.h"

namespace Ares
{
	// DbvtProxy
	struct DbvtBroadphaseProxy : BroadphaseProxy
	{
		DbvtNode<DbvtBroadphaseProxy*>*	m_leaf;

		// 构造函数
		DbvtBroadphaseProxy( const Rect3& aabb, void* userPtr, UINT collisionFilterGroup, UINT collisionFilterMask)
			: BroadphaseProxy( aabb, userPtr, collisionFilterGroup, collisionFilterMask)
		{}
	};

	//--------------------------------------
	// DefaultBroadphase 2012-11-19 帝林
	//--------------------------------------
	class DbvtBroadphase : public Broadphase
	{
	public:
		// 索引
		enum
		{
			Dynamic_Set = 0,	// Dynamic set index
			Fixed_Set,			// Fixed set index
		};

	public:
		DbvtBroadphase();
		virtual ~DbvtBroadphase();

		// 创建代理
		virtual BroadphaseProxy* CreateProxy( const Rect3& aabb, int shapeType, void* userPtr, UINT collisionFilterGroup, UINT collisionFilterMask, void* multiSapProxy);

		// 删除代理
		virtual void DestroyProxy( BroadphaseProxy* proxy);

		// 设置更新轴向包围盒
		virtual void SetAABB( BroadphaseProxy* proxy, const Rect3& aabb);

		// 几何体检测
		virtual bool ShapeIntersectionTest( const Shape* shape, vector<BroadphaseProxy*>& result);

		// 获取重叠几何对
		virtual OverlappingPairCache* GetOverlappingPairCache(){ return m_paircache; }

	private:
		Dbvt<DbvtBroadphaseProxy*>		m_dbvt;			// Dynamics bounding volume tree
		OverlappingPairCache*			m_paircache;	// Pair cache
	};
}