#pragma once

#include <Core/AresMath.h>
#include "CollisionObject.h"
#include "MotionState.h"

namespace Ares
{
	// The RigidBodyConstructionInfo structure provides information to create a rigid body, Setting mass to zero
	// creates a fixed(non-dynamic) rigid body, for dynamic objects, you can use the collision shape to approximate
	// the local inertia tensor, otherwise use the zero vector(default argument), you can use the motion state to
	// synchronize the world transform between physics and graphics objects. And if the motion state is provided, the
	// rigid body will initialize its initial world transform form the motion state, m_startWorldTransform is only
	// used when you don't provide a motion state;
	struct RigidBodyConstructionInfo
	{
		float			m_mass;					// 质量
		MotionState*	m_motionState;
		Transform		m_startWorldTransform;	// 转换
		Shape*			m_collisionShape;		// 外型
		Vector3			m_localInertia;
		float			m_linearDamping;
		float			m_angularDamping;
		float			m_friction;				// 摩擦力
		float			m_rollingFriction;		// 旋转摩擦力
		float			m_restitution;			
		float			m_linearSleepingThreshold;
		float			m_angularSleepingThreshold;
		bool			m_additionalDamping;
		float			m_additionalDampingFactor;
		float			m_additionalLinearDampingThresholdSqr;
		float			m_additionalAngularDampingThresholdSqr;
		float			m_additionalAngularDampingFactor;

		// 构造函数
		RigidBodyConstructionInfo( float mass, MotionState* motionState, Shape* shape, const Vector3& localInertia=Vector3::Zero)
			: m_mass( mass)
			, m_motionState( motionState)
			, m_collisionShape( shape)
			, m_localInertia( localInertia)
			, m_linearDamping( 0.f)
			, m_angularDamping( 0.f)
			, m_friction( 0.5f)
			, m_rollingFriction( 0.f)
			, m_restitution( 0.f)
			, m_linearSleepingThreshold( 0.8f)
			, m_angularSleepingThreshold( 1.f)
			, m_additionalDamping( false)
			, m_additionalDampingFactor( 0.005f)
			, m_additionalLinearDampingThresholdSqr( 0.01f)
			, m_additionalAngularDampingThresholdSqr( 0.01f)
			, m_additionalAngularDampingFactor( 0.01f)
		{
			m_startWorldTransform.SetIdentity();
		}
	};

	//------------------------------------------------------------
	// RigidBody 2013-4-7 帝林
	// A rigid body is the basic simulation object in the physics
	// world.
	// It has position and orientation data, along with first
	// derivatives. It can be integrated forward through time, and
	// have forces, torques and impulses (linear or angular) applied
	// to it. the rigid body manages its state and allows access 
	// through a set of methods.
	// A rigid body contains 64 worlds. it contains no virtual 
	// functions, so should take up exactly 64 worlds in memory
	// Of this total 15 words are padding, distributed among the  
	// Vector3 data members
	//------------------------------------------------------------
	class MotionState;
	class RigidBody : public CollisionObject
	{
	public:
		enum Flags
		{
			RF_DisableWorldGravity    = 1,
			RF_EnableGyropscopicForce = 2,
		};

	public:
		// Constructs
		RigidBody( const RigidBodyConstructionInfo& info);
		RigidBody( float mass, MotionState* motionState, Shape* collisionShape, const Vector3& localInertia=Vector3::Zero);

		// Apply impulse
		void ApplyImpulse( const Vector3& impulse, const Vector3& relPos);

		// Apply central impulse
		void ApplyCentralImpulse( const Vector3& impulse);

		// Apply TorqueImpulse
		void ApplyTorqueImpulse( const Vector3& torque);

		// Is static object
		bool IsStaticObject() const { return (m_collisionFlags & CF_StaticObject) != 0; }

		// Is static or kinematic object
		bool IsStaticOrKinematicObject() const { return (m_collisionFlags & ( CF_KinematicObject | CF_StaticObject)) != 0;}

	public:
		// MotionState allows to automatic synchronize the world transform for active objects
		MotionState* GetMotionState() { return m_optionalMotionState; }

		// Get inverse mass
		float GetInvMass() const { return m_inverseMass; }

		// Get mass center position
		const Transform& GetCenterOfMassTransform() const { return m_worldTransform; }

		// Get mass  center position
		const Vector3& GetCenterOfMassPosition()  const { return m_worldTransform.GetTrans(); }

		// Get Linear Velociry
		const Vector3& GetLinearVelocity() const { return m_linearVeloctiy; }

		// Get velocity in local point
		Vector3 GetVelocityInLocalPoint( const Vector3& relPos) const;

		// Get
		const Quat& GetInvInertiaTensorWorld() const { return m_invInertiaTensorWorld; }

		// 获取标记
		int GetFlags() const { return m_rigidBodyFlags; }

		// 设置重力
		void SetGravity( const Vector3& gravity);

		// Set damping(阻尼)
		void SetDamping( float linDamping, float angDamping);

		// 设置质量属性
		void SetMassProps( float mass, const Vector3& inertia);

		// 计算
		float ComputeImpulseDenominator( const Vector3& pos, const Vector3& normal) const;

	protected:
		// Setup RigidBody
		void SetupRigidBody( const RigidBodyConstructionInfo& constructionInfo);

		// Update inertia tensor
		void UpdateInertiaTensor();

	protected:
		Quat			m_invInertiaTensorWorld;				// inverse inertial world
		float			m_inverseMass;							// inverse of the mass 1.f/mass
		Vector3			m_linearFactor;							// 
		Vector3			m_acceleration;							// holds the acceleration of the rigid body. this value can be used to set acceleration due to gravity or any other constant acceleration
		MotionState*	m_optionalMotionState;					// allows to automatic synchronize the world transform for active objects
		Vector3			m_linearVeloctiy;						// linear veloctiy

		Vector3			m_angularVelocity;
		Vector3			m_angularFactor;
		Vector3			m_invMass;								// 质量的反

		Vector3			m_gravity;								// 重力
		Vector3			m_gravityAcceleration;					// 重和加速度
		Vector3			m_invInertiaLocal;
		Vector3			m_totalForce;							// 合力
		Vector3			m_totalTorque;							// 扭矩

		float			m_linearDamping;						// 线性阻尼
		float			m_angularDamping;						// 旋转阻尼

		float			m_linearSleepingThreshold;				// 临界值
		float			m_angularSleepingThreshold;				// 临界值

		int				m_contactSolverType;
		int				m_frictionSolverType;					// For experimental overriding of friction/contact solver func
	
		bool			m_additionalDamping;					// 额外阻尼
		float			m_additionalDampingFactor;				// 额外阻尼因子
		float			m_additionalLinearDampingThresholdSqr;	// 额外线性阻尼临界值
		float			m_additionalAngularDampingThresholdSqr; // 额外旋转阻尼临界值
		float			m_additionalAngularDampingFactor;		// 额外旋转阻尼因子

		int				m_rigidBodyFlags;						// 刚体标记
		Vector3			m_deltaLinearVelocity;					// 默认线性速度
		Vector3			m_deltaAngularVelocity;					// 角速度
		Vector3			m_pushVelocity;
		Vector3			m_turnVelocity;
	};
}