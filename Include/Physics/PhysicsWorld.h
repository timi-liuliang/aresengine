#pragma once

#include "CollisionWorld.h"
#include "SegmentDetect.h"

namespace Ares
{
	//-----------------------------------------
	// 默认物理世界 2012-11-20
	//-----------------------------------------
	class RigidBody;
	class ActionInterface;
	class PhysicsWorld : public CollisionWorld
	{
	public:
		PhysicsWorld( Broadphase* broadphase=NULL);
		~PhysicsWorld();

		// 设置重力
		void SetGravity( const Vector3& gravity);

		// 获取重力
		const Vector3& GetGravity() { return m_gravity; }

		// 线段检测
		bool SegmentDetect( Segment3& segment, const SegmentDetectConfig& detectConfig, SegmentDetectResult& result);

		// 点选最近几何体
		bool RaycastSingle( const Vector3& rayOrig, const Vector3& rayDir, float distance, CollisionObject*& object);

		// 查询区域内三角形
		void RegionQueryTriangles( vector<Triangle3>& oResult, const Rect3& region);

		// 查询区域内对象
		void RegionQueryCollisionObjects( vector<CollisionObject*>& results, const Shape* region);

		// Add Action
		void AddAction( ActionInterface* action);

		// 添加交通工具
		void AddVehicle( ActionInterface* vehicle);

		// 添加刚体
		void AddRigidBody( RigidBody* body);

		// 刷新
		virtual void FrameMove( float timeStep, int maxSubSteps=1, float fixedTimeStep=1.f/60.f);

	private:
		// 更新
		void UpdateActions( float timeStep);

		// 单帧模拟
		void InternalSingleStepSimulation( float timeStep);

		// 保存运动状态
		virtual void SaveKinematicState( float timeStep);

		// 应用重力
		void ApplyGravity();

		// 同步移动状态
		virtual void SynchronizeMotionStates();

		// 
		void PredictUnconstraintMotion( float timeStep);

		// 创建接触
		void CreatePredictiveContacts( float timeStep);

		// 执行碰撞检测
		void PerformCollisionDetection();

		// 计算模拟
		void CalculateSimulationIslands();

	private:
		Vector3						m_gravity;				// 重力
		float						m_localTime;			// 本地时间
		vector<ActionInterface*>	m_actions;				// 活动对象
		vector<RigidBody*>			m_nonStaticRigidBodys;	// 非静态刚体
	};
	typedef s_ptr<PhysicsWorld> PhysicsWorldPtr;
}