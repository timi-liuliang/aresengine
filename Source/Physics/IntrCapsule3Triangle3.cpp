#include <Physics/IntrCapsule3Triangle3.h>
#include <Physics/DistSegment3Triangle3.h>
#include <Physics/DistVector3Triangle3.h>

namespace Ares
{
	// construction
	IntrCapsule3Triangle3::IntrCapsule3Triangle3( const Capsule3& capsule, const Triangle3& triangle)
		: m_capsule( capsule),
		  m_triangle( triangle)
	{
	}

	// test intersection query
	bool IntrCapsule3Triangle3::Test()
	{
		// Çò
		if( m_capsule.m_segment.m_extent == 0.f)
		{
			DistVector3Triangle3 DistVT( m_capsule.m_segment.GetVertex0(), m_triangle);
			float dist = DistVT.Get(); 
			if( dist<m_capsule.m_radius)
			{
				m_intrPointTriangle = DistVT.m_closePointTriangle;
				if( dist>1e-5f)
					m_intrPointCapsule  = DistVT.m_closePointTriangle - (DistVT.m_closePointVector - DistVT.m_closePointTriangle) * ( (m_capsule.m_radius-dist)/dist);
				else
					m_intrPointCapsule  = DistVT.m_closePointTriangle - m_capsule.m_radius * m_triangle.GetNormal();
			}
		}
		else
		{
			DistSement3Triangle3 DistST( m_capsule.m_segment, m_triangle);
			float dist = DistST.Get(); 
			if( dist < m_capsule.m_radius)
			{
				m_intrPointTriangle = DistST.m_closePointTriangle;

				if( dist > 1e-5f)
					m_intrPointCapsule  = DistST.m_closePointTriangle - (DistST.m_closePointSegment - DistST.m_closePointTriangle) * ( (m_capsule.m_radius-dist)/dist);
				else
					m_intrPointCapsule  = DistST.m_closePointTriangle - m_capsule.m_radius * m_triangle.GetNormal();

				return true;
			}
		}

		return false;
	}
}