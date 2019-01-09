#include <Engine/FxSystem/PointZone.h>
#include <Engine/FxSystem/AresFxParticle.h>

//BOOST_CLASS_EXPORT( Ares::FxPointZone)

namespace Ares
{
	// constructor
	FxPointZone::FxPointZone( const Vector3& position)
		: FxZone( position)
	{
		
	}

	// Checks whether a point is within the zone
	bool FxPointZone::Contains( const Vector3& point) const
	{
		return false;
	}

	// Check whether a line intersects the zone
	bool FxPointZone::Intersects( const Vector3& v0, const Vector3& v1, Vector3* intersection, Vector3* normal)const
	{
		return false;
	}

	// Generates a position inside this zone for a given particle
	void FxPointZone::GeneratePosition( FxParticle& particle, bool full) const
	{
		particle.SetPosition( m_position);
	}
}