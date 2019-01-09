#pragma once

#include "BroadphaseProxy.h"
#include "OverlappingPairCache.h"
#include "Shape.h"

namespace Ares
{
	//------------------------------------------------------------------------------
	// BroadphaseInterface 2012-7-31  bullet
	// BroadphaseInterface class provides an interface to detect aabb-overlapping
	// object pairs. Some implementations for this broadphase interface include
	// AxisSweep3, DbvtBroadphase. The actual overlapping pair management, storage,
	// adding and removing of pairs is dealt by the overlappingPairCache class
	//------------------------------------------------------------------------------
	class Broadphase
	{
	public:
		// 虚析构函数
		virtual ~Broadphase() {}

		// 创建代理
		virtual BroadphaseProxy* CreateProxy( const Rect3& aabb, int shapeType, void* userPtr, UINT collisionFilterGroup, UINT collisionFilterMask, void* multiSapProxy)=0;
	
		// 删除代理
		virtual void DestroyProxy( BroadphaseProxy* proxy)=0;

		// 设置更新轴向包围盒
		virtual void SetAABB( BroadphaseProxy* proxy, const Rect3& aabb)=0;

		// 几何体检测
		virtual bool ShapeIntersectionTest( const Shape* shape, vector<BroadphaseProxy*>& result)=0;

		// 获取重叠几何对
		virtual OverlappingPairCache* GetOverlappingPairCache()=0;
	};
} 