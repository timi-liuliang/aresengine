#pragma once

#include <Core/AresMath.h>
#include "AresFxZone.h"
#include "AresFxParticle.h"

namespace Ares
{
	//------------------------------------------------
	// Emitter 2011-9-14  帝林
	//     An abstract class that defines an emitter 
	// An emitter has a flow and a tank of particles
	// The flow defines the rate at which particles 
	// are launched and the tank defines the total
	// number of particles the emitter can launched.
	// Note that the flow and the tank of an emitter 
	// are only used when the emitter emits automatically
	// from a group. but the user can also emit manually
	// outside a group.
	//------------------------------------------------
	class FxEmitter
	{
	public:
		// constructor
		FxEmitter();

		// destructor
		virtual ~FxEmitter() {}

	public:
		// update number
		size_t UpdateNumber( float deltaTime);

	public:
		// Sets this emitter active or not
		void SetActive( bool active) { m_active = active; }

		// Tells whether this emitter is active or not
		bool IsActive() const { return m_active; }

	public:
		// Sets the number of particles in this Emitter's tank
		void SetTank( int tank) { m_tank = tank; }

		// Changes the number of particles in this emitter's tank
		void ChangeTank(int deltaTank);

		// Gets the number of particles in this Emitter's tank
		int GetTank() const { return m_tank; }

		// Sets the flow of this emitter
		void SetFlow( float flow) { m_flow = flow; }

		// Changes the flow of particles of this emitter
		void ChangeFlow( float deltaFlow);

		// Gets the flow of this emitter
		float GetFlow() const { return m_flow; }

	public:
		// Sets the force of this Emitter
		void SetForce( float min, float max);

		// Gets the minimum force of this emitter
		float GetForceMin() const { return m_forceMin; }

		// Gets the maximum force of this emitter
		float GetForceMax() const { return m_forceMax; }

		// Sets the zone of this emitter
		// If the zone is NULL, the default zone will be used( A point centered at the origin)
		void SetZone( FxZone* zone, bool full=true);
	
		// 获取发射区域
		FxZone* GetZone() const { return m_zone; }

		// Tells whether this emitter emits in the whole zone or only at its borders
		bool IsFullZone() const { return m_full; }

		// 发射粒子
		void Emit( FxParticle& particle) const;

		// 为粒子生成速度
		void GenerateVelocity( FxParticle& particle) const;

	public:
		// Tells whether this emitter is sleeping or not
		bool IsSleeping() const;

		// Serialize
		template< typename Archive> void serialize( Archive& ar, const unsigned int version)
		{
			ar & m_zone;
			ar & m_full;
			ar & m_active;
			ar & m_tank;
			ar & m_flow;
			ar & m_forceMin;
			ar & m_forceMax;
			ar & m_fraction;
		}

	private:
		// Get default zone
		static FxZone* GetDefaultZone();

		// A pure method, that generates the volocity of the particle in function of a speed
		virtual void GenerateVelocity( FxParticle& particle, float speed) const=0;

	private:
		FxZone*			m_zone;			// Zone
		bool			m_full;
		bool			m_active;		// Is active

		int				m_tank;
		float			m_flow;

		float			m_forceMin;
		float			m_forceMax;
		mutable float	m_fraction;
	};
}