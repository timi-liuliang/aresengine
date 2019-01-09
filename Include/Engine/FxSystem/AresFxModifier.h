#pragma once

#include <Core/AresMath.h>

namespace Ares
{
	//----------------------------------------------------
	// Modifier 2011-9-19 µ€¡÷
	//     A abstract class that definesa physical object 
	// acting on particles.
	//	   A modifier is first defined by a zone and a 
	// trigger to that zone If the particle triggers the 
	// modifier. the modifier's action is applied to the 
	// particle. An action can be anything that has effect 
	// on the particl's parameters, position, velocity, life...
	//     If no zone is attached to a modifier the zone
	// is considered to be the entire universe.
	//     Like an emitter, a modifer can either be used
	// automatically within a group or manually directly 
	// by the user.
	//----------------------------------------------------
	class FxParticleSystem;
	class FxZone;
	class FxParticle;
	class FxModifier
	{
	public:
		// ModifierTrigger...defining the triggers for modifier
		enum EModifierTrigger
		{
			ALWAYS         = 1 << 0,	// No trigger, a Particle is always modified
			INSIDE_ZONE    = 1 << 1,	// A Particle inside the zone
			OUTSIDE_ZONE   = 1 << 2,    // A Particle outside the zone
			INTERSECT_ZONE = 1 << 3,	// A Particle intersecting the zone
			ENTER_ZONE	   = 1 << 4,	// A Particle entering the zone
			EXIT_ZONE      = 1 << 5,	// A Particle exiting the zone
		};

	public:
		// constructor
		FxModifier(int availableTriggers = ALWAYS, EModifierTrigger trigger=ALWAYS, bool needIntersection=false, bool needNormal = false, FxZone* zone=NULL);

		// destructor
		virtual ~FxModifier();

		// Is active or not
		bool IsActive() const { return m_active; }

	public:
		// Begin Process Group
		void BeginProcess( FxParticleSystem& group);

		// End Process Group
		void EndProcess( FxParticleSystem& group);

		// Process Particle
		void Process( FxParticle& particle, float deltaTime);

	public:
		// sets the trigger of this modifier
		bool SetTrigger( EModifierTrigger trigger);

	private:
		// a pure method that modifies the particle
		virtual void Modify( FxParticle& particle, float deltaTime)=0;

		// a method that handles particles on the wrong side of this modifier zone
		virtual void ModifyWrongSide( FxParticle& particle, bool inside) const {}

	private:
		FxZone*				m_zone;			// «¯”Ú
		bool				m_full;			// 

		bool				m_active;
		mutable bool		m_savedActive;

	protected:
		Vector3*			m_intersection;
		Vector3*			m_normal;

	protected:
		bool				m_needsIntersection;
		bool				m_needsNormal;

		EModifierTrigger	m_trigger;

		const int			m_availableTriggers;
	};
}