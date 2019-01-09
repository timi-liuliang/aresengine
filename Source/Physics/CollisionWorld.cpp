#include <Physics/CollisionWorld.h>
#include <Physics/DbvtBroadphase.h>

namespace Ares
{
	// 构造函数
	CollisionWorld::CollisionWorld( Broadphase* broadphase)
	{
		m_broadphase = broadphase ? broadphase : new_ DbvtBroadphase;
	}

	// 析构函数
	CollisionWorld::~CollisionWorld()
	{
		while( m_collisionObjects.size())
			RemoveCollisionObject( m_collisionObjects.back());

		SAFE_DELETE( m_broadphase);
	}

	// 更新AABB
	void CollisionWorld::UpdateSingleAABB( CollisionObject* colObj)
	{
		// 重新计算AABB
		Rect3 boundingBox;
		int	  type  = colObj->GetCollisionShape() ? colObj->GetCollisionShape()->GetShapeType() : ST_UnKnown;
		colObj->BuildAABB( boundingBox);

		m_broadphase->SetAABB( colObj->GetBroadphaseProxy(), boundingBox);
	}

	// 更新所有AABB
	void CollisionWorld::UpdateAABBs()
	{
		for( size_t i=0; i<m_collisionObjects.size(); i++)
		{
			UpdateSingleAABB( m_collisionObjects[i]);
		}
	}

	// 添加CollisionObject
	void CollisionWorld::AddCollisionObject( CollisionObject* collisionObject, UINT collisionFilterGroup/*=BroadphaseProxy::Filter_Default*/, UINT collisionFilterMask/*=BroadphaseProxy::Filter_All*/)
	{
		A_ASSERT( collisionObject);

		// 保证碰撞对象尚未被添加
		if( std::find( m_collisionObjects.begin(), m_collisionObjects.end(), collisionObject) == m_collisionObjects.end())
		{
			m_collisionObjects.push_back( collisionObject);

			// 重新计算AABB
			Rect3 boundingBox;
			int	  type  = collisionObject->GetCollisionShape() ? collisionObject->GetCollisionShape()->GetShapeType() : ST_UnKnown;
			collisionObject->BuildAABB( boundingBox);

			// 粗检测阶段
			A_ASSERT( m_broadphase);
			BroadphaseProxy* bpp = m_broadphase->CreateProxy( boundingBox, type, collisionObject, collisionFilterGroup, collisionFilterMask, 0);
			collisionObject->SetBroadphaseProxy( bpp);
		}
	}

	// 移除碰撞对象
	void CollisionWorld::RemoveCollisionObject( CollisionObject* collisionObject)
	{
		BroadphaseProxy* bp = collisionObject->GetBroadphaseProxy();
		if( bp)
		{
			GetBroadphase()->DestroyProxy( bp);
			collisionObject->SetBroadphaseProxy( NULL);
		}

		vector<CollisionObject*>::iterator it = std::find( m_collisionObjects.begin(), m_collisionObjects.end(), collisionObject);
		if( it != m_collisionObjects.end())
			m_collisionObjects.erase( it);
	}

	// 几何体碰撞检测
	bool CollisionWorld::ContactTest( Shape* shape, ContactResult& result)
	{
		vector<BroadphaseProxy*> broadphaseProxys;
		if( m_broadphase->ShapeIntersectionTest( shape, broadphaseProxys))
		{
			// 与相关循环体进行碰撞
			for( vector<BroadphaseProxy*>::iterator it=broadphaseProxys.begin(); it!=broadphaseProxys.end(); it++)
			{
				CollisionObject* objectB = (CollisionObject*)((*it)->m_clientObject);
				if( objectB && objectB->GetCollisionShape())
				{
					CollideContactList collideContactList;
					if( Collide::ContactTest( shape, Quat::Identity, Vector3::Zero, objectB->GetCollisionShape(), objectB->GetTransform().GetQuat(), objectB->GetTransform().GetTrans(), collideContactList))
					{
						for( size_t i=0; i<collideContactList.size(); i++)
						{
							result.AddContact( NULL, objectB, collideContactList[i].m_normal, collideContactList[i].m_point0, collideContactList[i].m_point1);
						}
					}
				}
			}
		}

		return result.GetSize()>0;
	}

	// 碰撞检测(object in the collision world)
	bool CollisionWorld::ContactTest( CollisionObject* object, ContactResult& result)
	{
		if( !object->GetBroadphaseProxy())
			return false;

		Rect3 aabb;
		object->BuildAABB( aabb);

		vector<BroadphaseProxy*> broadphaseProxys;
		if( m_broadphase->ShapeIntersectionTest( &aabb,broadphaseProxys))
		{
			// 与相关循环体进行碰撞
			for( vector<BroadphaseProxy*>::iterator it=broadphaseProxys.begin(); it!=broadphaseProxys.end(); it++)
			{
				CollisionObject* collisionObject = (CollisionObject*)((*it)->m_clientObject);
				if( collisionObject != object && m_broadphase->GetOverlappingPairCache()->IsNeedBroadphaseCollision( object->GetBroadphaseProxy(), collisionObject->GetBroadphaseProxy()))
					ContactPairTest( object, collisionObject, result);
			}
		}

		return result.GetSize()>0;
	}

	// 碰撞检测(object in the collision world)
	void CollisionWorld::ContactPairTest( CollisionObject* objectA, CollisionObject* objectB,  ContactResult& result)
	{
		if( objectA->GetCollisionShape() && objectB->GetCollisionShape())
		{
			const Transform& transformA = objectA->GetTransform();
			const Transform& transformB = objectB->GetTransform();

			CollideContactList collideContactList;
			if( Collide::ContactTest( objectA->GetCollisionShape(), transformA.GetQuat(), transformA.GetTrans(), objectB->GetCollisionShape(), transformB.GetQuat(), transformB.GetTrans(), collideContactList))
			{
				for( size_t i=0; i<collideContactList.size(); i++)
					result.AddContact( objectA, objectB, collideContactList[i].m_normal, collideContactList[i].m_point0, collideContactList[i].m_point1);
			}
		}
	}

	// 刷新
	void CollisionWorld::FrameMove(  float timeStep, int maxSubSteps, float fixedTimeStep)
	{
		UpdateAABBs();
	}
}