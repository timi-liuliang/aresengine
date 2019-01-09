#include "Physics/AresRaycastVehicle.h"
#include <Physics/MotionState.h>

namespace Ares
{
	// 车轮接触点
	struct WheelContactPoint
	{
		RigidBody*		m_body0;
		RigidBody*		m_body1;
		Vector3			m_frictionPositionWorld;
		Vector3			m_frictionDirectionWorld;
		float			m_jacDiagABInv;
		float			m_maxImpulse;

		WheelContactPoint( RigidBody* body0, RigidBody* body1, const Vector3& frictionPosWorld, const Vector3& frictionDirectionWorld, float maxImpulse)
			: m_body0( body0)
			, m_body1( body1)
			, m_frictionPositionWorld( frictionPosWorld)
			, m_frictionDirectionWorld( frictionDirectionWorld)
			, m_maxImpulse( maxImpulse)
		{
			float denom0 = body0->ComputeImpulseDenominator( frictionPosWorld, frictionDirectionWorld);
			float denom1 = body1->ComputeImpulseDenominator( frictionPosWorld, frictionDirectionWorld);
			float relaxation = 1.f;
			m_jacDiagABInv   = relaxation / ( denom0 + denom1);
		}
	};

	// 静态函数
	RigidBody& ActionInterface::GetFixedBody()
	{
		static RigidBody sFixed( 0, 0, 0);
		sFixed.SetMassProps( 0.f, Vector3::Zero);

		return sFixed;
	}

	// 获取轮子Transform
	const Transform& RaycastVehicle::GetWheelTransformWS( int idx) const
	{
		return m_wheels[idx].m_worldTransform;
	}

	// 更新
	void RaycastVehicle::UpdateAction( PhysicsWorld* physicsWorld, float deltaTimeStep)
	{
		UpdateVehicle( deltaTimeStep);
	}

	// 更新车辆
	void RaycastVehicle::UpdateVehicle( float timeStep)
	{
		for ( size_t i=0; i<m_wheels.size(); i++)
		{
			UpdateWheelTransform( i, false);
		}

		// 当前速度km/hour
		const Transform& chassisTransform = GetChassisWorldTransform();
		Vector3 forward;
		if( Vector3Dot( forward, GetRigidBody()->GetLinearVelocity()) < 0.f)
			m_currentVehicleSpeedKmHour = GetRigidBody()->GetLinearVelocity().Length() * -3.6f;
		else
			m_currentVehicleSpeedKmHour = GetRigidBody()->GetLinearVelocity().Length() * 3.6f;

		// simulate suspension
		for ( size_t i=0; i<m_wheels.size(); i++)
		{
			float depth = RayCast( m_wheels[i]);
		}

		UpdateSuspension( timeStep);

		for( size_t i=0; i<m_wheels.size(); i++)
		{
			Wheel& wheel			= m_wheels[i];
			float  suspensionForce	= min( wheel.m_wheelSuspensionForce, wheel.m_maxSuspensionForce);
		
			Vector3 impulse			= wheel.m_raycastInfo.m_contactNormalWS * suspensionForce * timeStep;
			Vector3	relpos			= wheel.m_raycastInfo.m_contactPostionWS - GetRigidBody()->GetCenterOfMassPosition();

			GetRigidBody()->ApplyImpulse( impulse, relpos);
		}

		UpdateFriction( timeStep);

		for ( size_t i=0; i<m_wheels.size(); i++)
		{
			Wheel&  wheel   = m_wheels[i];
			Vector3 relPos  = wheel.m_raycastInfo.m_hardPositionWS - GetRigidBody()->GetCenterOfMassPosition();
			Vector3	velocity= GetRigidBody()->GetVelocityInLocalPoint( relPos);
			if( wheel.m_raycastInfo.m_isInContact)
			{
				const Transform& chassisWorldTransform = GetChassisWorldTransform();
				wheel.m_deltaRotation = 0.01f;
				wheel.m_rotation	  += wheel.m_deltaRotation;
			}
			else
			{
				wheel.m_rotation += wheel.m_deltaRotation;
			}

			// Damping of rotation when not in contact
			wheel.m_deltaRotation  *= 0.99f;
		}
	}

	// 更新指定车轮
	void RaycastVehicle::UpdateWheelTransform( int wheelIdx, bool interpolateTransform)
	{
		Wheel& wheel = m_wheels[wheelIdx];
		UpdateWheeltransformWS( wheel, interpolateTransform);

		Vector3			up		= -wheel.m_raycastInfo.m_wheelDirectionWS;
		const Vector3&	right	= wheel.m_raycastInfo.m_wheelAxisWS;
		Vector3			forward = Vector3Cross( up, right); forward.Normalize();
		Matrix44        spaceMat( right.x, forward.y, up.z, 0.f,
								  right.x, forward.y, up.z, 0.f,
								  right.x, forward.y, up.z, 0.f,
								  0.f,   0.f,  0.f, 0.f);


		Quat axisSpace;
		axisSpace.FromMatrix( spaceMat);

		// rotate around steering
		Quat  steeringOrig; steeringOrig.FromAxisAngle( up,		wheel.m_steering);
		Quat  rotatingOrig; rotatingOrig.FromAxisAngle( right,  wheel.m_rotation);


		wheel.m_worldTransform.SetQuat( axisSpace * steeringOrig * rotatingOrig);
		wheel.m_worldTransform.SetTrans( wheel.m_raycastInfo.m_hardPositionWS + wheel.m_raycastInfo.m_wheelDirectionWS * wheel.m_raycastInfo.m_suspensionLength);
	}

	// 射线检测
	float RaycastVehicle::RayCast( Wheel& wheel)
	{
		UpdateWheeltransformWS( wheel, false);

		float	depth		= -1.f;
		float	raylen		= wheel.GetSuspensionRestLength() + wheel.m_wheelRadius;
		Vector3 rayVector	= wheel.m_raycastInfo.m_wheelDirectionWS * raylen;
		const Vector3& source = wheel.m_raycastInfo.m_hardPositionWS;
		wheel.m_raycastInfo.m_contactPostionWS = source + rayVector;
		const Vector3& target = wheel.m_raycastInfo.m_contactPostionWS;

		float param = 0.f;

		VehicleRaycaster::Result raycastResults;
		void* object = m_raycaster->RayCast( source, target, raycastResults);
		wheel.m_raycastInfo.m_groundObject = NULL;
		if( object)
		{
			param = raycastResults.m_distFraction;
			depth = raylen * raycastResults.m_distFraction;
			wheel.m_raycastInfo.m_contactNormalWS = raycastResults.m_hitNormalInWorld;
			wheel.m_raycastInfo.m_isInContact	  = true;

			wheel.m_raycastInfo.m_groundObject = &GetFixedBody();

			float hitDistance = param * raylen;
			wheel.m_raycastInfo.m_suspensionLength = hitDistance - wheel.m_wheelRadius;

			float minSuspensionLength = wheel.GetSuspensionRestLength() - wheel.m_maxSuspensionTravelCm * 0.01f;
			float maxSuspensionLength = wheel.GetSuspensionRestLength() - wheel.m_maxSuspensionTravelCm * 0.01f;
			if( wheel.m_raycastInfo.m_suspensionLength < minSuspensionLength)
				wheel.m_raycastInfo.m_suspensionLength = minSuspensionLength;
			if( wheel.m_raycastInfo.m_suspensionLength > maxSuspensionLength)
				wheel.m_raycastInfo.m_suspensionLength = maxSuspensionLength;

			wheel.m_raycastInfo.m_contactPostionWS = raycastResults.m_hitPointInWorld;

			float denominator = Vector3Dot( wheel.m_raycastInfo.m_contactNormalWS, wheel.m_raycastInfo.m_wheelDirectionWS);

			Vector3 chassisVelocityAtContactPoint;
			Vector3 relPos = wheel.m_raycastInfo.m_contactPostionWS - GetRigidBody()->GetCenterOfMassPosition();
			chassisVelocityAtContactPoint = GetRigidBody()->GetVelocityInLocalPoint( relPos);

			float projVel = Vector3Dot( wheel.m_raycastInfo.m_contactNormalWS, chassisVelocityAtContactPoint);
			if( denominator>-0.1f)
			{
				wheel.m_suspensionRelativeVelocity = 0.f;
				wheel.m_clippedInvContactDotSuspension = 1.f / 0.1f;
			}
			else
			{
				float inv = -1.f / denominator;
				wheel.m_suspensionRelativeVelocity = projVel * inv;
				wheel.m_clippedInvContactDotSuspension = inv;
			}
		}
		else
		{
			//put wheel info as in rest position
			wheel.m_raycastInfo.m_suspensionLength = wheel.GetSuspensionRestLength();
			wheel.m_suspensionRelativeVelocity = 0.f;
			wheel.m_raycastInfo.m_contactNormalWS = - wheel.m_raycastInfo.m_wheelDirectionWS;
			wheel.m_clippedInvContactDotSuspension = 1.f;
		}

		return depth;
	}

	// 获取车底盘转换
	const Transform& RaycastVehicle::GetChassisWorldTransform()
	{
		return GetRigidBody()->GetCenterOfMassTransform();
	}

	// 更新车指定车轮
	void RaycastVehicle::UpdateWheeltransformWS( Wheel& wheel, bool isInterpolatedTransform)
	{
		wheel.m_raycastInfo.m_isInContact = false;
		
		Transform chassisTrans = GetChassisWorldTransform();
		if( isInterpolatedTransform && GetRigidBody()->GetMotionState())
		{
			GetRigidBody()->GetMotionState()->GetWorldTransform( chassisTrans);
		}

		wheel.m_raycastInfo.m_hardPositionWS   = chassisTrans.TransformVector3( wheel.m_chassisConnectionPointCS);
		wheel.m_raycastInfo.m_wheelDirectionWS = chassisTrans.GetQuat().Rotate( wheel.m_wheelDirectionCS);
		wheel.m_raycastInfo.m_wheelAxisWS	   = chassisTrans.GetQuat().Rotate( wheel.m_wheelAxisCS);
	}

	// 计算旋转摩擦
	float CalcRollingFriction( WheelContactPoint& contactPoint)
	{
		float j1 = 0.f;
		const Vector3& contactPosWorld = contactPoint.m_frictionPositionWorld;

		Vector3 relPos1 = contactPosWorld - contactPoint.m_body0->GetCenterOfMassPosition();
		Vector3 relPos2 = contactPosWorld - contactPoint.m_body1->GetCenterOfMassPosition();

		float maxImpulse= contactPoint.m_maxImpulse;

		Vector3 vel1 = contactPoint.m_body0->GetVelocityInLocalPoint( relPos1);
		Vector3 vel2 = contactPoint.m_body1->GetVelocityInLocalPoint( relPos2);
		Vector3 vel  = vel1 - vel2;

		float	vrel = Vector3Dot( contactPoint.m_frictionDirectionWorld, vel);

		// calculate j that moves us to zero relative velocity
		j1 = -vrel * contactPoint.m_jacDiagABInv;
		j1 = min( j1, maxImpulse);
		j1 = max( j1, maxImpulse);

		return j1;
	}

	// 更新
	void RaycastVehicle::UpdateSuspension( float deltaTime)
	{
		float chassisMass = 1.f / m_chassis->GetInvMass();
		for( int i=0; i<GetNumWheels(); i++)
		{
			Wheel& wheel = m_wheels[i];
			if ( wheel.m_raycastInfo.m_isInContact)
			{
				float force;
				//	Spring
				{
					float	susp_length	   = wheel.GetSuspensionRestLength();
					float	current_length = wheel.m_raycastInfo.m_suspensionLength;
					float   length_diff   = (susp_length - current_length);
					force = wheel.m_suspensionStiffness * length_diff * wheel.m_clippedInvContactDotSuspension;
				}

				// Damper
				{
					float projected_rel_vel = wheel.m_suspensionRelativeVelocity;

					float	susp_damping;
					if ( projected_rel_vel < 0.f)
						susp_damping = wheel.m_wheelsDampingCompression;
					else
						susp_damping = wheel.m_wheelsDampingRelaxation;
					force -= susp_damping * projected_rel_vel;
				}

				// RESULT
				wheel.m_wheelsSuspensionForce = max( force * chassisMass, 0.f);
			}
			else
			{
				wheel.m_wheelsSuspensionForce = 0.f;
			}
		}
	}

	// 更新摩擦力
	void RaycastVehicle::UpdateFriction( float timeStep)
	{
		// calculate the impulse, so that the wheels don;t move sidewards
		int numWheel = GetNumWheels();
		if( !numWheel)
			return;

		m_forwardWS.resize( numWheel);
		m_axle.resize( numWheel);
		m_forwardImpulse.resize( numWheel);
		m_sideImpulse.resize( numWheel);

		int numWheelsOnGround = 0;

		// collapse all those loops into one!
		for ( int i=0; i<GetNumWheels(); i++)
		{
			Wheel& wheel = m_wheels[i];
			const RigidBody* groundObject = (RigidBody*)wheel.m_raycastInfo.m_groundObject;
			if( groundObject)
				numWheelsOnGround++;

			m_sideImpulse[i]    = 0.f;
			m_forwardImpulse[i] = 0.f;
		}
/*
		for ( int i=0; i<GetNumWheels(); i++)
		{
			Wheel& wheel = m_wheels[i];
			RigidBody* groundObject = (RigidBody*)wheel.m_raycastInfo.m_groundObject;
			if( groundObject)
			{
				const Transform& wheelTrans = GetWheelTransformWS( i);
				Matrix33 wheelBasis0 = wheelTrans.GetBasis();
				m_axle[i] = Vector3( wheelBasis0[0][m_indexRightAxis], wheelBasis0[1][m_indexRightAxis], wheelBasis0[2][m_indexRightAxis]);
			
				const Vector3& surfNormalWS = wheel.m_raycastInfo.m_contactNormalWS;
				float proj					= Vector3Dot( m_axle[i], surfNormalWS);
				m_axle[i]				   -= surfNormalWS * proj;
				m_axle[i].Normalize();

				m_forwardWS[i] = Vector3Cross( surfNormalWS, m_axle[i]);
				m_forwardWS[i].Normalize();

				ResolveSingleBilateral( *m_chassis, wheel.m_raycastInfo.m_contactPostionWS, *groundObject, wheel.m_raycastInfo.m_contactPostionWS, 0.f, m_axle[i], m_sideImpulse[i], timeStep);
			
				m_sideImpulse[i] *= sideFrictionStiffness2;
			}
		}
		*/

		float sideFactor = 1.f;
		float fwdFactor  = 0.5f;
		bool  sliding    = false;
		{
			for ( int i=0; i<GetNumWheels(); i++)
			{
				Wheel& wheel = m_wheels[i];
				RigidBody* groundObject = (RigidBody*)(wheel.m_raycastInfo.m_groundObject);
				float rollingFriction = 0.f;
				if( groundObject)
				{
					if( wheel.m_engineForce!=0.f)
					{
						rollingFriction = wheel.m_engineForce * timeStep;
					}
					else
					{
						float defaultRollingFrictionImpulse = 0.f;
						float maxImpulse = wheel.m_brake ? wheel.m_brake : defaultRollingFrictionImpulse;
						WheelContactPoint contactPt( m_chassis, groundObject, wheel.m_raycastInfo.m_contactPostionWS, m_forwardWS[i], maxImpulse);
						rollingFriction	= CalcRollingFriction( contactPt);
					}
				}

				// switch between active rolling(throttle), braking and non-active rolling friction
				m_forwardImpulse[i]    = 0.f;
				m_wheels[i].m_skidInfo = 1.f;
				if( groundObject)
				{
					m_wheels[i].m_skidInfo= 1.f;

					float maximp = wheel.m_wheelsSuspensionForce * timeStep * wheel.m_frictionSlip;
					float maximpSide = maximp;

					float maximpSquared = maximp * maximpSide;


					m_forwardImpulse[i] = rollingFriction;//wheelInfo.m_engineForce* timeStep;

					float x = (m_forwardImpulse[i] ) * fwdFactor;
					float y = (m_sideImpulse[i] ) * sideFactor;

					float impulseSquared = (x*x + y*y);

					if (impulseSquared > maximpSquared)
					{
						sliding = true;

						float factor = maximp / sqrt( impulseSquared);

						m_wheels[i].m_skidInfo *= factor;
					}
				}
			}

			if (sliding)
			{
				for ( int i=0; i<GetNumWheels(); i++)
				{
					if(m_sideImpulse[i] != 0.f)
					{
						if (m_wheels[i].m_skidInfo< 1.f)
						{
							m_forwardImpulse[i] *=	m_wheels[i].m_skidInfo;
							m_sideImpulse[i]    *= m_wheels[i].m_skidInfo;
						}
					}
				}
			}

			// apply the impulses
			{
				for( int i = 0;i<GetNumWheels() ; i++)
				{
					Wheel& wheelInfo = m_wheels[i];
					Vector3 rel_pos = wheelInfo.m_raycastInfo.m_contactPostionWS -  m_chassis->GetCenterOfMassPosition();

					if ( m_forwardImpulse[i] != 0.f)
					{
						m_chassis->ApplyImpulse(m_forwardWS[i]*( m_forwardImpulse[i]),rel_pos);
					}
					if (m_sideImpulse[i] != 0.f)
					{
						class RigidBody* groundObject = (RigidBody*) m_wheels[i].m_raycastInfo.m_groundObject;

						Vector3 rel_pos2 = wheelInfo.m_raycastInfo.m_contactPostionWS - groundObject->GetCenterOfMassPosition();


						Vector3 sideImp = m_axle[i] * m_sideImpulse[i];

#if defined ROLLING_INFLUENCE_FIX // fix. It only worked if car's up was along Y - VT.
						btVector3 vChassisWorldUp = getRigidBody()->getCenterOfMassTransform().getBasis().getColumn(m_indexUpAxis);
						rel_pos -= vChassisWorldUp * (vChassisWorldUp.dot(rel_pos) * (1.f-wheelInfo.m_rollInfluence));
#else
						rel_pos[m_indexUpAxis] *= wheelInfo.m_rollInfluence;
#endif
						m_chassis->ApplyImpulse( sideImp, rel_pos);

						//apply friction impulse on the ground
						groundObject->ApplyImpulse( -sideImp, rel_pos2);
					}
				}
			}
		}
	}
}