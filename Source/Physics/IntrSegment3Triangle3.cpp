#include <Physics/IntrSegment3Triangle3.h>

namespace Ares
{
	// constructor
	IntrSegment3Triangle3::IntrSegment3Triangle3( const Segment3& seg, const Triangle3& tri)
		: m_segment( seg)
		, m_triangle( tri)
	{

	}

	// test-intersection query
	bool IntrSegment3Triangle3::Test()
	{
		// Find vectors for two edges sharing vert0
		Vector3 edge1 = m_triangle.m_v[1] - m_triangle.m_v[0];
		Vector3 edge2 = m_triangle.m_v[2] - m_triangle.m_v[0];

		// Begin calculating determinant
		Vector3 pVec = Vector3Cross( m_segment.m_dir, edge2);

		// If determinant is near zero, ray lies in plane of triangle
		float det = Vector3Dot( edge1, pVec);

		Vector3 tVec;
		if ( det > 0)
		{
			tVec = m_segment.m_orig - m_triangle.m_v[0];
		}
		else
		{
			tVec = m_triangle.m_v[0] - m_segment.m_orig;
			det  = - det;
		}

		if ( det < 0.0001f)
		{
			return false;
		}

		// ²âÊÔ±ß½ç
		float u = Vector3Dot( tVec, pVec);
		if ( u<0.0f || u>det)
		{
			return false;
		}

		// calculate v parameter and test bounds
		Vector3 qVec = Vector3Cross( tVec, edge1);

		float v = Vector3Dot( m_segment.m_dir, qVec);
		if ( v<0.0f || u+v>det)
		{
			return false;
		}

		// calculate t, scale parameters, ray intersects triangle
		float fDist = Vector3Dot( edge2, qVec);
		float fInvDet = 1.0f / det;
		fDist *= fInvDet;
		//*u     *= fInvDet;
		//*v     *= fInvDet;
		if( fDist>=-m_segment.m_extent && fDist<=m_segment.m_extent)
		{
			m_intrPoint = m_segment.m_orig + m_segment.m_dir * fDist;

			return true;
		}

		return false;
	}
}