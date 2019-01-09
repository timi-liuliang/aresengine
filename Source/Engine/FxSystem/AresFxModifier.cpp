#include <Core/AresCore.h>
#include <Engine/FxSystem/AresFxModifier.h>
#include <Engine/FxSystem/AresFxZone.h>
#include <Engine/FxSystem/AresFxParticle.h>

namespace Ares
{
	// constructor
	FxModifier::FxModifier(int availableTriggers, EModifierTrigger trigger, bool needIntersection, bool needNormal, FxZone* zone)
		: m_availableTriggers( availableTriggers),
		  m_trigger( trigger),
		  m_zone( zone),
		  m_needsIntersection( needIntersection),
		  m_needsNormal( needNormal),
		  m_full( false),
		  m_active( true)//,
		 // m_local( false)
	{
		m_intersection = new_ Vector3;
		m_normal	   = new_ Vector3;
	}

	// destructor
	FxModifier::~FxModifier()
	{
		SAFE_DELETE( m_intersection);
		SAFE_DELETE( m_normal);
	}

	// sets the trigger of this modifier
	bool FxModifier::SetTrigger( EModifierTrigger trigger)
	{
		if( trigger && m_availableTriggers)
		{
			m_trigger = trigger;

			return true;
		}

		return false;
	}

	// Begin Process Group
	void FxModifier::EndProcess( FxParticleSystem& group)
	{
		// Restores the active state of the modifier
		m_active = m_savedActive;
	}

	// End Process Group
	void FxModifier::BeginProcess( FxParticleSystem& group)
	{
		m_savedActive = m_active;

		if( !m_active)
			return;

		// If buffers of the modifier in the group are not ready
		// the modifier is made incative for the frame
		//if( !PrepareBuffers( group))
		//	m_active = false;
	}

	// Process Particle
	void FxModifier::Process( FxParticle& particle, float deltaTime)
	{
		Vector3 tNormal;
		Vector3 tIntersection;

		switch( m_trigger)
		{
		case ALWAYS:
			{
				Modify( particle, deltaTime);
			}
			break;

		case INSIDE_ZONE:
			{
				if( !m_zone || m_zone->Contains( particle.GetPosition()))
					Modify( particle, deltaTime);
				else
					ModifyWrongSide( particle, true);
			}
			break;

		case OUTSIDE_ZONE:
			{
				if( !m_zone)
					return;
				else if( !m_zone->Contains( particle.GetPosition()))
					Modify( particle, deltaTime);
				else
					ModifyWrongSide( particle, false);
			}
			break;

		case INTERSECT_ZONE:
			{
				if( !m_zone)
					return;

				if( m_zone->Intersects( particle.GetOldPosition(), 
										particle.GetPosition(), 
										m_needsIntersection ? &tIntersection : NULL,
										m_needsNormal ? &tNormal : NULL))
				{
					*m_intersection = tIntersection;
					*m_normal	   = tNormal;

					Modify( particle, deltaTime);
				}
			}
			break;

		case ENTER_ZONE:
			{
				if( !m_zone)
					return;

				if( m_zone->Contains( particle.GetOldPosition()))
					ModifyWrongSide( particle, true);
				else if( m_zone->Intersects( particle.GetOldPosition(), 
											 particle.GetPosition(), 
											 m_needsIntersection ? &tIntersection : NULL,
											 m_needsNormal ? &tNormal : NULL))
				{
					*m_intersection = tIntersection;
					*m_normal	   = tNormal;

					Modify( particle, deltaTime);
				}
			}
			break;

		case EXIT_ZONE:
			{
				if( !m_zone)
					return;

				if( !m_zone->Contains( particle.GetOldPosition()))
					ModifyWrongSide( particle, false);
				else if( m_zone->Intersects( particle.GetOldPosition(), 
											 particle.GetPosition(), 
											 m_needsIntersection ? &tIntersection : NULL,
											 m_needsNormal ? &tNormal : NULL))
				{
					*m_intersection = tIntersection;
					*m_normal	   = tNormal;

					Modify( particle, deltaTime);
				}
			}
			break;
		}
	}
}