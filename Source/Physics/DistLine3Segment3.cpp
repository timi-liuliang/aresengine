#include <Physics/Shapes/Algorithm/DistLine3Segment3.h>

namespace Ares
{
	// construction
	DistLine3Segment3::DistLine3Segment3( const Line3& line, const Segment3& segment)
		: m_line( line),
		  m_segment( segment)
	{
	}

	// static distance queries
	float DistLine3Segment3::GetSquared()
	{
		Vector3 kDiff = m_line.m_orig - m_segment.m_orig;

		float fA01 = Vector3Dot( -m_line.m_dir, m_segment.m_dir);
		float fB0  = Vector3Dot( kDiff, m_line.m_dir);
		float fC   = kDiff.LengthSq();
		float fDet = abs( 1.f - fA01 * fA01);
		float fB1, fS0, fS1, fSqrDist, fExtDet;

		if( fDet >= ZERO_TOLERANCE)
		{
			// the line and segment are not parallel
			fB1 = Vector3Dot( -kDiff, m_segment.m_dir);
			fS1 = fA01 * fB0 - fB1;
			fExtDet = m_segment.m_extent * fDet;

			if( fS1 >= -fExtDet)
			{
				if( fS1 <= fExtDet)
				{
					// two interior points are closet, one on the line and one on the segment
					float fInvDet = 1.f/fDet;
					fS0 = ( fA01*fB1 - fB0) * fInvDet;
					fS1 *= fInvDet;
					fSqrDist = fS0 * (fS0+fA01*fS1+2.f*fB0) + fS1*(fA01*fS0+fS1+2.f*fB1) + fC;
				}
				else
				{
					// the end point e1 of the segment and an interior point of the line are closest
					fS1 = m_segment.m_extent;
					fS0 = -( fA01*fS1+fB0);
					fSqrDist = -fS0*fS0 + fS1*(fS1+2.f*fB1) + fC;
				}
			}
			else
			{
				// the end point e0 of the segment and an interior point of the line are closest
				fS1 = -m_segment.m_extent;
				fS0 = -(fA01*fS1 + fB0);
				fSqrDist = -fS0 * fS0 + fS1 * ( fS1 + 2.f * fB1) + fC;
			}
		}
		else
		{
			// the line and segment are parallel. choose the closet pair so that one point is
			// at segment origin
			fS1 = 0.f;
			fS0 = -fB0;
			fSqrDist = fB0*fS0 + fC;
		}

		m_closePointLine = m_line.m_orig + fS0 * m_line.m_dir;
		m_closePointSegment = m_segment.m_orig + fS1 * m_segment.m_dir;

		m_lineParam = fS0;
		m_segmentParam = fS1;

		return abs( fSqrDist);
	}
}