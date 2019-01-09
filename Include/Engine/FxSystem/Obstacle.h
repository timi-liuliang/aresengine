#pragma once

#include <Engine/FxSystem/AresFxModifier.h>

namespace Ares
{
	//--------------------------------
	// Obstacle 2011-9-19  ╣шаж
	//--------------------------------
	class FxObstacle : public FxModifier
	{
	public:
		// constructor
		FxObstacle( FxZone* zone=NULL, FxModifier::EModifierTrigger trigger=FxModifier::INTERSECT_ZONE, float bouncingRatio = 1.f, float friction=1.f);

	private:
		// a pure method that modifies the particle
		virtual void Modify( FxParticle& particle, float deltaTime);

		// a method that handles particles on the wrong side of this modifier zone
		virtual void ModifyWrongSide( FxParticle& particle, bool inside) const;

	private:
		float	m_bouncingRatio;
		float	m_friction;
	};
}