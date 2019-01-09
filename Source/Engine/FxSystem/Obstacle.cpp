#include <Engine/FxSystem/AresFxParticle.h>
#include <Engine/FxSystem/Obstacle.h>

namespace Ares
{
	// constructor
	FxObstacle::FxObstacle( FxZone* zone, FxModifier::EModifierTrigger trigger, float bouncingRatio, float friction)
		: FxModifier(  INTERSECT_ZONE | ENTER_ZONE | EXIT_ZONE, INTERSECT_ZONE, true, true, zone),
		  m_bouncingRatio( bouncingRatio),
		  m_friction( friction)
	{
		SetTrigger( trigger);
	}

	// a pure method that modifies the particle
	void FxObstacle::Modify( FxParticle& particle, float deltaTime)
	{
		Vector3 velocity = particle.GetPosition() - particle.GetOldPosition();

		if( deltaTime != 0.f)
			velocity *= 1.f /deltaTime;
		else
			velocity.x = velocity.y = velocity.z = 0.f;

		float dist = Vector3Dot( velocity, *m_normal);

		*m_normal *= dist;
		velocity -= *m_normal;
		velocity *= m_friction;
		*m_normal *= m_bouncingRatio;
		velocity -= *m_normal;

		particle.SetVelocity( velocity);
		particle.SetPosition( *m_intersection);
	}

	// a method that handles particles on the wrong side of this modifier zone
	void FxObstacle::ModifyWrongSide( FxParticle& particle, bool inside) const
	{

	}
}