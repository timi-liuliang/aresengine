#include <Physics/IntrSegment3Sphere3.h>

namespace Ares
{
	// constructor
	IntrSegment3Sphere3::IntrSegment3Sphere3( const Segment3& seg, const Sphere3& sphere)
		: m_segment( seg)
		, m_sphere( sphere)
	{
		m_quantity = 0;
	}

	// test if intersection
	bool IntrSegment3Sphere3::Test()
	{
		Vector3 diff = m_segment.m_orig - m_sphere.m_center;
		float a0 = Vector3Dot( diff, diff) - m_sphere.m_radius * m_sphere.m_radius;
		float a1 = Vector3Dot( m_segment.m_dir, diff);
		float discr = a1*a1 - a0;
		if (discr < 0.f)
			return false;

		float tmp0 = m_segment.m_extent*m_segment.m_extent + a0;
		float tmp1 = 2.f*a1*m_segment.m_extent;
		float qm   = tmp0 - tmp1;
		float qp   = tmp0 + tmp1;
		if (qm*qp <= 0.f)
		{
			return true;
		}

		return qm>0.f && std::abs(a1)<m_segment.m_extent;
	}

	// test and find intr points
	bool IntrSegment3Sphere3::Find()
	{
		Vector3 diff = m_segment.m_orig - m_sphere.m_center;
		float a0 = Vector3Dot( diff, diff) - m_sphere.m_radius*m_sphere.m_radius;
		float a1 = Vector3Dot( m_segment.m_dir, diff);
		float discr = a1*a1 - a0;
		if (discr < 0.f)
		{
			m_quantity = 0;

			return false;
		}

		float tmp0 = m_segment.m_extent*m_segment.m_extent + a0;
		float tmp1 = 2.f*a1*m_segment.m_extent;
		float qm = tmp0 - tmp1;
		float qp = tmp0 + tmp1;
		float root;
		if (qm*qp <= 0.f)
		{
			root = sqrtf(discr);
			m_segmentParam[0] = (qm > 0.f ? -a1 - root : -a1 + root);
			m_intrPoints[0] = m_segment.m_orig + m_segmentParam[0] * m_segment.m_dir;
			m_quantity = 1;
			//mIntersectionType = IT_POINT;
			
			return true;
		}

		if (qm > 0.f && std::abs(a1) < m_segment.m_extent)
		{
			if (discr >= FLT_SMALL)
			{
				root = sqrtf(discr);
				m_segmentParam[0] = -a1 - root;
				m_segmentParam[1] = -a1 + root;
				m_intrPoints[0] = m_segment.m_orig + m_segmentParam[0] * m_segment.m_dir;
				m_intrPoints[1] = m_segment.m_orig + m_segmentParam[1] * m_segment.m_dir;
				m_quantity = 2;
				//mIntersectionType = IT_SEGMENT;
			}
			else
			{
				m_segmentParam[0] = -a1;
				m_intrPoints[0] = m_segment.m_orig + m_segmentParam[0] * m_segment.m_dir;
				m_quantity = 1;
				//mIntersectionType = IT_POINT;
			}
		}
		else
		{
			m_quantity = 0;
			//mIntersectionType = IT_EMPTY;
		}

		return m_quantity > 0;
	}
}