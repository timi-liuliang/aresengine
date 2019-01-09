#include <Physics/PhysicsWorld.h>
#include <Physics/DistLine3Point3.h>
#include <Physics/AresActionInterface.h>
#include <Physics/RigidBody.h>
#include <Physics/AresDispatch.h>

namespace Ares
{
	// 构造函数
	PhysicsWorld::PhysicsWorld( Broadphase* broadphase)
		: CollisionWorld( broadphase)
		, m_localTime( 0.f)
		, m_gravity( Vector3( 0.f, 0.f, -9.8f))
	{

	}

	// 析构函数
	PhysicsWorld::~PhysicsWorld()
	{

	}

	// 设置重力
	void PhysicsWorld::SetGravity( const Vector3& gravity)
	{
		m_gravity = gravity;		
		for( size_t i=0; i<m_nonStaticRigidBodys.size(); i++)
		{
			RigidBody* body = m_nonStaticRigidBodys[i];
			if( body->IsActive() && !( body->GetFlags() & RigidBody::RF_DisableWorldGravity))
				body->SetGravity( gravity);
		}
	}

	// 线段检测
	bool PhysicsWorld::SegmentDetect( Segment3& segment, const SegmentDetectConfig& detectConfig, SegmentDetectResult& result)
	{
		ContactResult contactResult;
		if( CollisionWorld::ContactTest( &segment, contactResult))
		{
			if( detectConfig.m_detectFlag | SDF_NearestHitPoint)
			{
				float dist = FLT_BIG;
				for ( size_t i=0; i<contactResult.GetSize(); i++)
				{
					float tDist = ( contactResult[i].m_point-segment.GetVertex0()).Length();
					if( tDist<dist)
					{
						result.m_closetHitPoint = contactResult[i].m_point;
						result.m_closetHitNormal= contactResult[i].m_normal;

						dist = tDist;
					}
				}

				return true;
			}
		}

		return false;
	}

	// 点选最近几何体
	bool PhysicsWorld::RaycastSingle( const Vector3& rayOrig, const Vector3& rayDir, float distance, CollisionObject*& object)
	{
		float tDist = FLT_BIG;
		ContactResult contactResult;
		Segment3 segment( rayOrig, rayOrig+rayDir*distance);
		Line3    line( rayOrig, rayDir);
		vector<BroadphaseProxy*> broadphaseProxys;
		if( m_broadphase->ShapeIntersectionTest( &segment, broadphaseProxys))
		{
			// 与相关循环体进行碰撞
			for( vector<BroadphaseProxy*>::iterator it=broadphaseProxys.begin(); it!=broadphaseProxys.end(); it++)
			{
				CollisionObject* collisionObject = (CollisionObject*)((*it)->m_clientObject);
				if( collisionObject)
				{
					Rect3 worldBounds;
					collisionObject->BuildAABB( worldBounds);

					Vector3 tCenter = (worldBounds.GetMaxPoint() + worldBounds.GetMinPoint()) / 2.f;
					DistLine3Point3 dLP( line, tCenter);
					float tCDist  = dLP.GetSquared();
					if( tDist > tCDist)
					{
						object = collisionObject;

						tDist = tCDist;
					}				
				}
			}
		}

		return object ? true : false;
	}

	// 查询区域内三角形
	void PhysicsWorld::RegionQueryTriangles( vector<Triangle3>& oResult, const Rect3& region)
	{
		oResult.clear();

		vector<BroadphaseProxy*> broadphaseProxys;
		if( m_broadphase->ShapeIntersectionTest( &region, broadphaseProxys))
		{
			// 与相关循环体进行碰撞
			for( vector<BroadphaseProxy*>::iterator it=broadphaseProxys.begin(); it!=broadphaseProxys.end(); it++)
			{
				CollisionObject* objectB = (CollisionObject*)((*it)->m_clientObject);
				if( objectB)
				{
					Shape* queryShape = objectB->GetCollisionShape();
					if( queryShape)
					{
						switch( queryShape->GetShapeType())
						{
						case ST_KdtTriangleMesh:
							{
								KdtTriangleMesh* kdtMesh = dynamic_cast<KdtTriangleMesh*>( queryShape);
								if( kdtMesh)
								{
									set<int> tTriIds; tTriIds.clear();
									kdtMesh->GetTriIdxs( tTriIds, region);
									for ( set<int>::const_iterator it = tTriIds.begin(); it!=tTriIds.end(); ++it)
									{
										// 射线与三角形碰撞
										oResult.push_back( kdtMesh->GetTriangle( *it));
									}
								}
							}
							break;
						}
					}	
				}
			}
		}
	}

	// 查询区域内对象
	void PhysicsWorld::RegionQueryCollisionObjects( vector<CollisionObject*>& results, const Shape* region)
	{
		vector<BroadphaseProxy*> broadphaseProxys;
		if( m_broadphase->ShapeIntersectionTest( region, broadphaseProxys))
		{
			for ( size_t i=0; i<broadphaseProxys.size(); i++)
			{
				results.push_back( (CollisionObject*)(broadphaseProxys[i]->m_clientObject));
			}
		}
	}

	// 添加Vehicle
	void PhysicsWorld::AddAction( ActionInterface* action)
	{
		m_actions.push_back( action);
	}

	// 添加交通工具
	void PhysicsWorld::AddVehicle( ActionInterface* vehicle)
	{
		AddAction( vehicle);
	}

	// 添加刚体
	void PhysicsWorld::AddRigidBody( RigidBody* body)
	{
		if( !body->IsStaticOrKinematicObject() && !( body->GetFlags() & RigidBody::RF_DisableWorldGravity))
			body->SetGravity( m_gravity);
		
		if( body->GetCollisionShape())
		{
			if( body->IsStaticObject())
				m_nonStaticRigidBodys.push_back( body);
			else
				body->SetActivationState( CollisionObject::CF_IslandSleeping);
		}

		bool  isDynamic				= !(body->IsStaticObject() || body->IsKinematicObject());
		short collisionFilterGroup	= isDynamic ? short(BroadphaseProxy::Filter_Default) : short(BroadphaseProxy::Filter_Static);
		short collisionFilterMask	= isDynamic ? short(BroadphaseProxy::Filter_All) : short(BroadphaseProxy::Filter_All ^ BroadphaseProxy::Filter_Static);

		AddCollisionObject( body, collisionFilterGroup, collisionFilterMask);
	}

	// 刷新
	void PhysicsWorld::FrameMove(  float timeStep, int maxSubSteps, float fixedTimeStep)
	{
		int numSimulationSubSteps = 0;
		if( maxSubSteps)
		{
			m_localTime += timeStep;
			if( m_localTime>=fixedTimeStep)
			{
				numSimulationSubSteps = int( m_localTime/fixedTimeStep);
				m_localTime -= numSimulationSubSteps * fixedTimeStep;
			}
			else
			{
				// variable timestep
				fixedTimeStep = timeStep;
				m_localTime   = timeStep;
				if( timeStep==0.f)
				{
					numSimulationSubSteps = 0;
					maxSubSteps			  = 0;
				}
				else
				{
					numSimulationSubSteps = 1;
					maxSubSteps			  = 1;
				}
			}
		}

		// 模拟
		if( numSimulationSubSteps)
		{
			// clamp the number of substeps, to prevent simulation grinding spiralling down to a halt
			int clampedSimulationSteps = numSimulationSubSteps>maxSubSteps ? maxSubSteps : numSimulationSubSteps;

			// 更新运动状态
			SaveKinematicState( fixedTimeStep * clampedSimulationSteps);

			// 应用重力
			ApplyGravity();

			for ( int i=0; i<clampedSimulationSteps; i++)
			{
				InternalSingleStepSimulation( fixedTimeStep);
				SynchronizeMotionStates();
			}
		}
		else
		{
			SynchronizeMotionStates();
		}
	}

	// 单帧模拟
	void PhysicsWorld::InternalSingleStepSimulation( float timeStep)
	{/*
		// Apply gravity, predict motion
		PredictUnconstraintMotion( timeStep);

		// Modify the dispatch info
		DispatcherInfo&	dispatchInfo = GetDispatchInfo();
		dispatchInfo.m_timeStep = timeStep;
		dispatchInfo.m_stepCount= 0;

		// Contacts
		CreatePredictiveContacts( timeStep);

		// Perform collision detection
		PerformCollisionDetection();

		// what !!!
		CalculateSimulationIslands();

		// Solve contact and other joint constraints
		GetSolverInfo().m_timeStep = timeStep;
		SolveConstraints( GetSolverInfo());

		// Integrate transforms
		IntegrateTransforms( timeStep);

		// Update Vehicle simulation
		UpdateActions( timeStep);

		// Update state?
		UpdateActivationState( timeStep);*/
	}

	// 保存运动状态
	void PhysicsWorld::SaveKinematicState( float timeStep)
	{/*
		for ( int i=0; i<m_collisionObjects.size(); i++)
		{
			RigidBody* rigidBoxy = dynamic_cast<RigidBody*>( m_collisionObjects[i]);
			if( rigidBoxy && rigidBoxy->GetActivationState()!=CollisionObject::CF_IslandSleeping)
			{
				if( rigidBoxy->IsKinematicObject())
					rigidBoxy->SaveKinematicState( timeStep);
			}
		}*/
	}

	// 应用重力
	void PhysicsWorld::ApplyGravity()
	{/*
		for ( int i=0; i<m_nonStaticRigidBodys.size(); i++)
		{
			RigidBody* body = m_nonStaticRigidBodys[i];
			if( body->IsActive())
			{
				body->ApplyGravity();
			}
		}*/
	}

	// 同步移动状态
	void PhysicsWorld::SynchronizeMotionStates()
	{/*
		if( m_synchronizeAllMotionStates)
		{
			// Synchronize all collision objects
			for( int i=0; i<m_collisionObjects.size(); i++)
			{
				RigidBody* body = dynamic_cast<RigidBody*>( m_collisionObjects[i]);
				if( body)
					SynchronizeSingleMotionState( body);
			}
		}
		else
		{
			for( int i=0; i<m_nonStaticRigidBodys.size(); i++)
			{
				RigidBody* body = m_nonStaticRigidBodys[i];
				if( body->IsActive())
					SynchronizeSingleMotionState( body);
			}
		}*/
	}

	// 
	void PhysicsWorld::PredictUnconstraintMotion( float timeStep)
	{/*
		for( int i=0; i<m_nonStaticRigidBodys.size(); i++)
		{
			RigidBody* body = m_nonStaticRigidBodys[i];
			if( !body->IsStaticOrKinematicObject())
			{
				body->ApplyImpulse( timeStep);

				body->PredictIntegratedTransform( timeStep, body->GetInterpolationWorldTransform());
			}
		}*/
	}

	// 创建接触
	void PhysicsWorld::CreatePredictiveContacts( float timeStep)
	{/*
		for ( int i=0; i<m_predictiveManifolds.size(); i++)
		{
			PresistentManifold* manifold = m_predictiveManifolds[i];
			this->m_dispatcher1->ReleaseManifold( manifold);
		}
		m_predictiveManifolds.clear();

		Transform predictedTrans;
		for ( int i=0; i<m_nonStaticRigidBodys.size(); i++)
		{
			RigidBody* body = m_nonStaticRigidBodys[i];
			body->SetHitFraction( 1.f);

			if( body->IsActive() && (!body->IsStaticOrKinematicObject()))
			{
				body->PredictIntegratedTransform( timeStep, predictedTrans);
				float squareMotion = ( predictedTrans.GetTrans()-body->GetWorldTransform().GetTrans()).Length();

				if( GetDispatchInfo().m_useContinuous && body->GetCcdSquareMotionThreshold() && body->GetCcdSquareMotionThreshold() < squareMotion)
				{
					if( body->GetCollisionShape()->IsConvex())
					{

					}
				}
			}
		}*/
	}

	// 更新
	void PhysicsWorld::UpdateActions( float timeStep)
	{
		foreach_( ActionInterface* action, m_actions)
		{
			action->UpdateAction( this, timeStep);
		}
	}

	// 执行碰撞检测
	void PhysicsWorld::PerformCollisionDetection()
	{/*
		DispatcherInfo& dispatchInfo = GetDispatchInfo();

		UpdateAABBs();

		ComputeOverlappingPairs();

		Dispatcher* dispatcher = GetDipatcher();
		if( dispatcher)
		{
			dispatcher->DispatchAllCollisionPairs( m_broadphasePairCache->GetOverlappingPairCache(), dispathInfo, m_dispatch1);
		}*/
	}

	// 计算模拟
	void PhysicsWorld::CalculateSimulationIslands()
	{

	}
}