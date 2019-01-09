#pragma once

#include <vector>
#include "Contacts.h"
#include "Rect3.h"
#include "CollisionObject.h"
#include "Broadphase.h"
#include "Collide.h"
#include "AresDispatch.h"

using namespace std;

namespace Ares
{
	//-----------------------------------------------
	// CollisionWorld 2012-7-30 帝林  excerpt bullet
	// CollisionWorld is interface and container for
	// the collision detection.
	//-----------------------------------------------
	class CollisionWorld
	{
	public:
		// 更新AABB
		void UpdateSingleAABB( CollisionObject* colObj);

		// 更新所有AABB
		void UpdateAABBs();

		// 获取世界中碰撞物体数量
		int GetNumCollisionObjects() const { return (int)m_collisionObjects.size(); }

		// 获取碰撞对象指针
		CollisionObject* GetCollisionObject( int idx) { return m_collisionObjects[idx]; }

		// 添加CollisionObject
		virtual void AddCollisionObject( CollisionObject* collisionObject, UINT collisionFilterGroup=BroadphaseProxy::Filter_Default, UINT collisionFilterMask=BroadphaseProxy::Filter_All);

		// 移除碰撞对象
		virtual void RemoveCollisionObject( CollisionObject* collisionObject);

		// 获取Broadphase
		Broadphase* GetBroadphase() { return m_broadphase; }

		// 获取Broadphase
		const Broadphase* GetBroadphase() const { return m_broadphase; }

		// 获取信息
		DispatcherInfo& GetDispatchInfo() { return m_dispatchInfo; }

		// 接触检测(shape not in the collision world)
		virtual bool ContactTest( Shape* shape, ContactResult& result);

		// 碰撞检测(object in the collision world)
		virtual bool ContactTest( CollisionObject* object, ContactResult& result);

		// 碰撞检测(object in the collision world)
		virtual void ContactPairTest( CollisionObject* objectA, CollisionObject* objectB,  ContactResult& result);

		// 刷新
		virtual void FrameMove(  float timeStep, int maxSubSteps=1, float fixedTimeStep=1.f/60.f);

	protected:
		CollisionWorld( Broadphase* broadphase=NULL);
		virtual ~CollisionWorld();

	protected:
		Broadphase*					m_broadphase;				// 粗检测管理器
		DispatcherInfo				m_dispatchInfo;				// 调度信息
		vector<CollisionObject*>	m_collisionObjects;			// 所有的碰撞对象
	};
	typedef s_ptr<CollisionWorld>	CollisionWorldPtr;
}