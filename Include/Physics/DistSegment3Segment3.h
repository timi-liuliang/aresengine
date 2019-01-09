#pragma once

#include <Physics/Shapes.h>

namespace Ares
{
	//------------------------------------------------
	// Distance between segment and segment 2012-12-18
	//------------------------------------------------
	struct DistSegment3Segment3
	{
		const Segment3&		m_segment0;
		const Segment3&	    m_segment1;

		Vector3				m_closePointSegment0;
		Vector3				m_closePointSegment1;

		// information about the closet points
		float				m_segment0Param;			// closestSegment  = set.origin+param*seg.m_dir
		float				m_segment1Param;			// closestSegment  = set.origin+param*seg.m_dir

		// construction
		DistSegment3Segment3( const Segment3& segment0, const Segment3& segment1);

		// static distance queries
		float GetSquared();

		// static distance queries
		float Get() { return sqrtf( GetSquared()); }
	};
}