#pragma once

#include <Physics/Rect3.h>

namespace Ares
{
	enum BroadphaseNativeTypes
	{
		BPNT_Box,
		BPNT_Invalid,
		BPNT_Max
	};

	//--------------------------------------------------------
	// BroadphaseProxy 2012-7-30 ժ bullet
	// The BroadphaseProxy is the main class that can be used
	// with the bullet broadphases. It stores collision shape
	// type information, collision filter information and a
	// client object, typically a CollisionObject or RigidBody
	//--------------------------------------------------------
	class BroadphaseProxy
	{
	public:
		// optional filtering to cull potential collisions
		enum CollisionFilterGroups
		{
			Filter_Default		= 1,
			Filter_Static		= 2,
			Filter_Kinematic	= 4,
			Filter_Debris		= 8,
			Filter_SensorTrigger= 16,
			Filter_Character	= 32,
			Filter_All			= -1			// All bits sets: Filter_Default | Filter_Static | Filter_Kinematic | Filter_Debris | Filter_SensorTrigger
		};

		void*			m_clientObject;			// Usually the client CollisionObject or Rigidbody class
		UINT			m_collisionFilterGroup;
		UINT			m_collisionFilterMask;
		Rect3			m_aabb;

		// 构造函数
		BroadphaseProxy( const Rect3& aabb, void* userPtr, UINT collisionFilterGroup, UINT collisionFilterMask)
			: m_aabb( aabb)
			, m_clientObject( userPtr)
			, m_collisionFilterGroup( collisionFilterGroup)
			, m_collisionFilterMask( collisionFilterMask)
		{}
	};
}