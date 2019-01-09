#include <Physics/DistLine3Triangle3.h>
#include <Physics/DistLine3Segment3.h>

namespace Ares
{
	// input w must be a unit-length vector, the output vectors{ u, v} are unit length
	// and mutually perpendicular, and { U, V, W} is an orthonormal basis
	static void GenerateComplementBasis( Vector3& rkU, Vector3& rkV,const Vector3& rkW)
	{
		float fInvLength;

		if( abs( rkW.x > rkW.y))
		{
			fInvLength = 1.f / sqrtf( rkW.x * rkW.x + rkW.z * rkW.z);

			rkU.x = -rkW.z * fInvLength;
			rkU.y = 0.f;
			rkU.z =  rkW.x * fInvLength;

			rkV.x = rkW.y * rkU.z;
			rkV.y = rkW.z * rkU.x - rkW.x * rkU.z;
			rkV.z = -rkW.y * rkU.x;
		}
		else
		{
			fInvLength = 1.f / sqrtf( rkW.y * rkW.y + rkW.z * rkW.z);

			rkU.x = 0.f;
			rkU.y = rkW.z * fInvLength;
			rkU.z = -rkW.y * fInvLength;

			rkV.x = rkW.y * rkU.z - rkW.z * rkU.y;
			rkV.y = -rkW.x * rkU.z;
			rkV.z = rkW.x * rkU.y;
		}
	}

	// construction
	DistLine3Triangle3::DistLine3Triangle3( const Line3& line, const Triangle3& triangle)
		: m_line( line),
		  m_triangle( triangle)
	{
	}

	// static distance queries
	float DistLine3Triangle3::GetSuared()
	{
		Vector3 edge0 = m_triangle.m_v[1] - m_triangle.m_v[0];
		Vector3 edge1 = m_triangle.m_v[2] - m_triangle.m_v[0];

		// calc triangle normal
		Vector3 normal;
		normal = Vector3Cross( edge0, edge1);
		normal.Normalize();

		float fNdD = Vector3Dot( normal, m_line.m_dir);
		if( abs( fNdD) > ZERO_TOLERANCE)
		{
			// the line and triangle are not parallel, so the line intersects
			// the plane of the triangle
			Vector3  kDiff = m_line.m_orig - m_triangle.m_v[0];
			const Vector3& rkD   = m_line.m_dir;
			Vector3 kU, kV;

			GenerateComplementBasis( kU, kV, rkD);

			float fUdE0   = Vector3Dot( kU, edge0);
			float fUdE1   = Vector3Dot( kU, edge1);
			float fUdDiff = Vector3Dot( kU, kDiff);

			float fVdE0	  = Vector3Dot( kV, edge0);
			float fVdE1	  = Vector3Dot( kV, edge1);
			float fVdDiff = Vector3Dot( kV, kDiff);

			float fInvDet = 1.f / ( fUdE0*fVdE1 - fUdE1*fVdE0);

			// Barycentric coordinates for the point of intersection
			float fB1 = ( fVdE1*fUdDiff - fUdE1*fVdDiff) * fInvDet;
			float fB2 = ( fUdE0*fVdDiff - fVdE0*fUdDiff) * fInvDet;
			float fB0 = 1.f - fB1 - fB2;

			if( fB0 >= 0.f && fB1>=0.f && fB2 >= 0.f)
			{
				// line parameter for the point of intersection
				float fDdE0   = Vector3Dot( rkD, edge0);
				float fDdE1   = Vector3Dot( rkD, edge1);
				float fDdDiff = Vector3Dot( m_line.m_dir, kDiff);

				m_lineParam = fB1 * fDdE0 + fB2 * fDdE1 - fDdDiff;

				// barycentric coordinates for the point of intersection
				m_triangleBary[0] = fB0;
				m_triangleBary[1] = fB1;
				m_triangleBary[2] = fB2;

				// the intersection point is inside or on the triangle
				m_closePointLine = m_line.m_orig + m_lineParam * m_line.m_dir;
				
				m_closePointTriangle = m_triangle.m_v[0] + fB1 * edge0 + fB2 * edge1;

				return 0.f;
			}
		}

		// Either the line is not parallel to the triangle and the point of intersection
		// of the line and the plane of the triangle is outside the triangle or the line
		// and triangle are parallel. Regardless, the closest point on the triangle is on
		// an edge of the triangle. Compare the line to all three edges of the triangle
		float fSqrDist = 1e38f;
		for( int i0=2, i1=0; i1<3; i0=i1++)
		{
			Segment3 kSeg( m_triangle.m_v[i0], m_triangle.m_v[i1]);

			DistLine3Segment3 kLSDist( m_line, kSeg);
			float fSqrDistTmp = kLSDist.GetSquared();

			if( fSqrDistTmp < fSqrDist)
			{
				m_closePointLine = kLSDist.m_closePointLine;
				m_closePointTriangle = kLSDist.m_closePointSegment;

				fSqrDist = fSqrDistTmp;

				m_lineParam = kLSDist.m_lineParam;

				float ratio = kLSDist.m_segmentParam / kSeg.m_extent;
				
				// barycentric coordinates for the point of intersection
				m_triangleBary[i0] = 0.5f * ( 1.f - ratio);
				m_triangleBary[i1] = 1.f - m_triangleBary[i0];
				m_triangleBary[3-i0-i1] = 0.f;
			}
		}

		return fSqrDist;
	}
}