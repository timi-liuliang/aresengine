#include <Engine/FxSystem/AresFxEmitter.h>
#include <Core/AresKitFunction.h>
#include <algorithm>
#include <Engine/FxSystem/AresFxParticle.h>
#include <Engine/FxSystem/AresFxZone.h>

namespace Ares
{
	// constructor
	FxEmitter::FxEmitter()
		: m_zone( NULL),
		  m_full( true),
		  m_tank( -1),
		  m_flow( 0.f),
		  m_forceMin( 0.f),
		  m_forceMax( 0.f),
		  m_fraction( Random( 0.f, 1.f)),
		  m_active( true)
	{

	}

	// Changes the number of particles in this emitter's tank
	void FxEmitter::ChangeTank(int deltaTank)
	{
		if( m_tank >= 0)
		{
			m_tank += deltaTank;
			if( m_tank < 0)
				m_tank = 0;
		}
	}

	// Changes the flow of particles of this emitter
	void FxEmitter::ChangeFlow( float deltaFlow)
	{
		if( m_flow >= 0.f)
		{
			m_flow += deltaFlow;
			if( m_flow < 0.f)
				m_flow = 0.f;
		}
	}

	// Sets the force of this Emitter
	void FxEmitter::SetForce( float min, float max)
	{
		m_forceMin = min;
		m_forceMax = max;
	}

	// Sets the zone of this emitter
	// If the zone is NULL, the default zone will be used( A point centered at the origin)
	void FxEmitter::SetZone( FxZone* zone, bool full)
	{
		//if( m_zone == NULL)
		//	m_zone = GetDefaultZone();

		m_zone = zone;
		m_full = full;
	}

	// Tells whether this emitter is sleeping or not
	bool FxEmitter::IsSleeping() const
	{
		return (m_tank == 0) || (m_flow == 0.f);
	}

	// Get default zone
	static FxZone* GetDefaultZone()
	{
		//static D3DXVECTOR3 defaultZone;

		return NULL;
	}

	// update number
	size_t FxEmitter::UpdateNumber( float deltaTime)
	{
		int numBorn;
		if( m_flow < 0.f)
		{
			numBorn = (std::max)( 0, m_tank);
			m_tank  = 0;
		}
		else if( m_tank != 0)
		{
			m_fraction += m_flow * deltaTime;
			numBorn = (int)m_fraction;
			if( m_tank >= 0)
			{
				numBorn = (std::min)( m_tank, numBorn);
				m_tank -= numBorn;
			}

			m_fraction -= numBorn;
		}
		else
			numBorn = 0;

		return (size_t)numBorn;
	}

	// 发射粒子
	void FxEmitter::Emit(FxParticle& particle) const
	{
		m_zone->GeneratePosition( particle, m_full);
		GenerateVelocity( particle);
	}

	// 为粒子生成速度
	void FxEmitter::GenerateVelocity( FxParticle& particle) const
	{
		GenerateVelocity( particle, Random( m_forceMin, m_forceMax)/particle.GetParamCurrentValue( FxModel::PARAM_MASS));
	}
}