#include <Physics/DistSegment3Triangle3.h>
#include <Physics/DistLine3Triangle3.h>
#include <Physics/DistVector3Triangle3.h>

namespace Ares
{
	// construction
	DistSement3Triangle3::DistSement3Triangle3( const Segment3& segment, const Triangle3& triangle)
		: m_segment( segment),
		  m_triangle( triangle)
	{
	}

	// static distance queries
	float DistSement3Triangle3::GetSquared()
	{
		Line3 tLine(m_segment.m_orig, m_segment.m_dir);
		DistLine3Triangle3 kLTDist( tLine, m_triangle);

		float fSqrDist = kLTDist.GetSuared();

		m_segmentParam = kLTDist.m_lineParam;

		if( m_segmentParam >= -m_segment.m_extent)
		{
			if( m_segmentParam <= m_segment.m_extent)
			{
				m_closePointSegment = kLTDist.m_closePointLine;
				m_closePointTriangle= kLTDist.m_closePointTriangle;

				// bararycentric
				m_triangleBary[0] = kLTDist.m_triangleBary[0];
				m_triangleBary[1] = kLTDist.m_triangleBary[1];
				m_triangleBary[2] = kLTDist.m_triangleBary[2];
			}
			else
			{
				m_closePointSegment = m_segment.GetVertex1();

				DistVector3Triangle3 kVTDist( m_closePointSegment, m_triangle);
				fSqrDist = kVTDist.GetSquared();

				m_closePointTriangle = kVTDist.m_closePointTriangle;
				m_segmentParam		 = m_segment.m_extent;

				// bararycentric
				m_triangleBary[0] = kVTDist.m_triangleBary[0];
				m_triangleBary[1] = kVTDist.m_triangleBary[1];
				m_triangleBary[2] = kVTDist.m_triangleBary[2];
			}
		}
		else
		{
			m_closePointSegment = m_segment.GetVertex0();

			DistVector3Triangle3 kVTDist( m_closePointSegment, m_triangle);
			fSqrDist = kVTDist.GetSquared();

			m_closePointTriangle = kVTDist.m_closePointTriangle;
			m_segmentParam		 = -m_segment.m_extent;

			// bararycentric
			m_triangleBary[0] = kVTDist.m_triangleBary[0];
			m_triangleBary[1] = kVTDist.m_triangleBary[1];
			m_triangleBary[2] = kVTDist.m_triangleBary[2];
		}

		return fSqrDist;
	}

	// static distance queries
	float DistSement3Triangle3::Get()
	{
		float fSqrDist = GetSquared();

		return sqrtf( fSqrDist);
	}
}