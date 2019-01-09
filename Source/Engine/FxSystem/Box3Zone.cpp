#include <Engine/FxSystem/Box3Zone.h>
#include <Core/AresKitFunction.h>

BOOST_CLASS_EXPORT( Ares::FxBox3Zone)

namespace Ares
{
	FxBox3Zone::FxBox3Zone()
		: FxZone( Vector3::Zero)
	{
	}

	// constructor
	FxBox3Zone::FxBox3Zone( const Box3& box)
		: FxZone( Vector3::Zero)
		, m_box3( box)
	{

	}

	// Checks whether a point is within the zone
	bool FxBox3Zone::Contains( const Vector3& point) const
	{
		return false;
	}

	// Check whether a line intersects the zone
	bool FxBox3Zone::Intersects( const Vector3& v0, const Vector3& v1, Vector3* intersection, Vector3* normal)const
	{
		return false;
	}

	// Generates a position inside this zone for a given particle
	void FxBox3Zone::GeneratePosition( FxParticle& particle, bool full) const
	{
		Vector3 position= m_box3.m_center;
		for( int i=0; i<3; i++)
		{
			float extent0  = Random( m_box3.m_extent[i], - m_box3.m_extent[i]);
			position     += extent0 * m_box3.m_axis[i];
		}

		particle.SetPosition( position);
	}
}