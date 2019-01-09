#include <Physics/RigidBody.h>

namespace Ares
{
	// Constructs
	RigidBody::RigidBody( const RigidBodyConstructionInfo& info)
	{
		SetupRigidBody( info);
	}

	// Constructs
	RigidBody::RigidBody( float mass, MotionState* motionState, Shape* collisionShape, const Vector3& localInertia)
	{
		RigidBodyConstructionInfo info( mass, motionState, collisionShape, localInertia);
		
		SetupRigidBody( info);
	}

	// Apply impulse
	void RigidBody::ApplyImpulse( const Vector3& impulse, const Vector3& relPos)
	{
		if( m_inverseMass != 0.f)
		{
			ApplyCentralImpulse( impulse);
			if( m_angularFactor.Length())
			{
				ApplyTorqueImpulse( Vector3Cross( relPos, impulse * m_linearFactor));
			}
		}
	}

	// Apply central impulse
	void RigidBody::ApplyCentralImpulse( const Vector3& impulse)
	{
		m_linearVeloctiy += impulse * m_linearFactor * m_inverseMass;
	}

	// Apply TorqueImpulse
	void RigidBody::ApplyTorqueImpulse( const Vector3& torque)
	{
		m_angularVelocity += m_invInertiaTensorWorld.Rotate( torque * m_angularFactor);
	}

	// Get velocity in local point
	Vector3 RigidBody::GetVelocityInLocalPoint( const Vector3& relPos) const
	{
		return m_linearVeloctiy + Vector3Cross( m_angularVelocity, relPos);
	}

	// Setup RigidBody
	void RigidBody::SetupRigidBody( const RigidBodyConstructionInfo& constructionInfo)
	{
		m_internalType = COT_RigidBody;

		m_linearVeloctiy		= Vector3::Zero;
		m_angularVelocity		= Vector3::Zero;
		m_angularFactor			= Vector3( 1.f, 1.f, 1.f);
		m_linearFactor			= Vector3( 1.f, 1.f, 1.f);
		m_gravity				= Vector3::Zero;
		m_gravityAcceleration	= Vector3::Zero;
		m_totalForce			= Vector3::Zero;
		m_totalTorque			= Vector3::Zero;

		SetDamping( constructionInfo.m_linearDamping, constructionInfo.m_angularDamping);

		m_linearSleepingThreshold	= constructionInfo.m_linearSleepingThreshold;
		m_angularSleepingThreshold  = constructionInfo.m_angularSleepingThreshold;
		m_optionalMotionState		= constructionInfo.m_motionState;
		m_contactSolverType			= 0;
		m_frictionSolverType		= 0;
		m_additionalDamping			= constructionInfo.m_additionalDamping;
		m_additionalDampingFactor	= constructionInfo.m_additionalDampingFactor;

		m_additionalLinearDampingThresholdSqr = constructionInfo.m_additionalLinearDampingThresholdSqr;
		m_additionalAngularDampingThresholdSqr= constructionInfo.m_additionalAngularDampingThresholdSqr;
		m_additionalAngularDampingFactor	  = constructionInfo.m_additionalAngularDampingFactor;

		if( m_optionalMotionState)
			m_optionalMotionState->GetWorldTransform( m_worldTransform);
		else
			m_worldTransform = constructionInfo.m_startWorldTransform;

		m_interpolationWorldTransform = m_worldTransform;
		m_interpolationLinearVelocity = Vector3::Zero;
		m_interpolationAngularVelocity= Vector3::Zero;

		// moved to collision object
		m_friction			= constructionInfo.m_friction;
		m_rollingFriction	= constructionInfo.m_rollingFriction;
		m_restitution		= constructionInfo.m_restitution;

		SetCollisionShape( constructionInfo.m_collisionShape);

		SetMassProps( constructionInfo.m_mass, constructionInfo.m_localInertia);
		UpdateInertiaTensor();

		m_rigidBodyFlags = 0;

		m_deltaLinearVelocity = Vector3::Zero;
		m_deltaAngularVelocity= Vector3::Zero;
		m_invMass			  = m_inverseMass * m_linearFactor;
		m_pushVelocity		  = Vector3::Zero;
		m_turnVelocity		  = Vector3::Zero;
	}

	// Set damping(阻尼)
	void RigidBody::SetDamping( float linDamping, float angDamping)
	{
		m_linearDamping  = clamp( linDamping, 0.f, 1.f);
		m_angularDamping = clamp( angDamping, 0.f, 1.f);
	}

	// 设置重力
	void RigidBody::SetGravity( const Vector3& gravity)
	{
		if( m_inverseMass!=0.f)
			m_gravity = gravity * 1.f / m_inverseMass;
		
		m_gravityAcceleration = gravity;
	}

	// 设置质量属性
	void RigidBody::SetMassProps( float mass, const Vector3& inertia)
	{
		if( mass==0.f)
		{
			m_collisionFlags |= CF_StaticObject;
			m_inverseMass	  = 0.f;
		}
		else
		{
			m_collisionFlags &= (~CF_StaticObject);
			m_inverseMass	  = 1.f / mass;
		}

		// F = m * a
		m_gravity = mass * m_gravityAcceleration;
		m_invInertiaLocal.x = inertia.x!=0.f ? 1.f/inertia.x : 0.f;
		m_invInertiaLocal.y = inertia.y!=0.f ? 1.f/inertia.y : 0.f;
		m_invInertiaLocal.z = inertia.z!=0.f ? 1.f/inertia.z : 0.f;

		m_invMass = m_linearFactor * m_inverseMass;
	}

	// 计算
	float RigidBody::ComputeImpulseDenominator( const Vector3& pos, const Vector3& normal) const
	{
		Vector3 r0 = pos - GetCenterOfMassPosition();
		Vector3 c0 = Vector3Cross( r0, normal);
		Vector3 vec= Vector3Cross( GetInvInertiaTensorWorld().Rotate( c0), r0);

		return m_inverseMass * Vector3Dot( normal, vec);
	}

	// Update inertia tensor
	void RigidBody::UpdateInertiaTensor()
	{
		//m_invInertiaTensorWorld = m_worldTransform.GetBasis().Scaled( m_invInertiaLocal);
	}
}