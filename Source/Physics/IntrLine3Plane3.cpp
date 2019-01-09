#include <Physics/IntrLine3Plane3.h>

namespace Ares
{
	// construction
	IntrLine3Plane3::IntrLine3Plane3( const Line3& line, const Plane3& plane)
		: m_line( line),
		  m_plane( plane)
	{}

	// test intersection query
	bool IntrLine3Plane3::Test()
	{
		float DdN = Vector3Dot( m_line.m_dir, m_plane.normal);
		float signedDistance = m_plane.DistanceTo( m_line.m_orig);
		if ( std::abs(DdN) > ZERO_TOLERANCE)
		{
			// The line is not parallel to the plane, so they must intersect.
			m_distance = -signedDistance/DdN;
			if( m_distance>=0.f)
			{
				m_intrPoint= m_line.m_orig + m_distance * m_line.m_dir;

				return true;
			}
		}

		// The Line and plane are parallel.  Determine if they are numerically
		// close enough to be coincident.
		if ( std::abs(signedDistance) <= ZERO_TOLERANCE)
		{
			// The line is coincident with the plane, so choose t = 0 for the
			// parameter.
			m_distance = 0.f;
			m_intrPoint= m_line.m_orig;

			return true;
		}

		return false;
	}
}