#include <Physics/IntrBox3Sphere3.h>

namespace Ares
{
	// constructor
	IntrBox3Sphere3::IntrBox3Sphere3( const Box3& box3, const Sphere3& sphere3)
		: m_box3( box3)
		, m_sphere3( sphere3)
	{}

	// test intersection query
	bool IntrBox3Sphere3::Test()
	{
		// test for intersection in the coordinate system of the box by
		// transforming the sphere into that coordinate system
		Vector3 diff = m_sphere3.m_center - m_box3.m_center;

		float ax = std::abs( Vector3Dot( diff, m_box3.m_axis[0]));
		float ay = std::abs( Vector3Dot( diff, m_box3.m_axis[1]));
		float az = std::abs( Vector3Dot( diff, m_box3.m_axis[2]));
		float dx = ax - m_box3.m_extent[0];
		float dy = ay - m_box3.m_extent[1];
		float dz = az - m_box3.m_extent[2];

		if ( ax <= m_box3.m_extent[0])
		{
			if ( ay <= m_box3.m_extent[1])
			{		
				if (az <= m_box3.m_extent[2])
					return true;															// Sphere center inside box.
				else
					return dz <= m_sphere3.m_radius;										// Potential sphere-face intersection with face z.
			}
			else
			{
				if (az <= m_box3.m_extent[2])
					return dy <= m_sphere3.m_radius;										// Potential sphere-face intersection with face y.
				else
					return dy*dy + dz*dz <= m_sphere3.m_radius*m_sphere3.m_radius;			// Potential sphere-edge intersection with edge formed by faces y and z.
			}
		}
		else
		{
			if (ay <= m_box3.m_extent[1])
			{
				if (az <= m_box3.m_extent[2])		
					return dx <= m_sphere3.m_radius;										// Potential sphere-face intersection with face x.
				else		
					return dx*dx + dz*dz <= m_sphere3.m_radius*m_sphere3.m_radius;			// Potential sphere-edge intersection with edge formed by faces x and z.
			}
			else
			{
				if (az <= m_box3.m_extent[2])			
					return dx*dx + dy*dy <= m_sphere3.m_radius*m_sphere3.m_radius;		   // Potential sphere-edge intersection with edge formed by faces x and y.
				else
					return dx*dx + dy*dy + dz*dz <= m_sphere3.m_radius*m_sphere3.m_radius; // Potential sphere-vertex intersection at corner formed by faces x,y,z.
			}
		}
	}
}