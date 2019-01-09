#pragma once

#include "Physics/Shapes.h"

namespace Ares
{
	//--------------------------------------------
	// calc the distance between line and segment
	//					--2011-04-27   ╣шаж
	struct DistLine3Segment3
	{
		const Line3&		m_line;
		const Segment3&	    m_segment;

		Vector3			m_closePointLine;
		Vector3			m_closePointSegment;

		// information about the closet points
		float				m_lineParam;			// closestLine	   = line origin+param*line.dir
		float				m_segmentParam;			// closestSegment  = set.origin+param*seg.m_dir

		// construction
		DistLine3Segment3( const Line3& line, const Segment3& segment);

		// static distance queries
		float GetSquared();
	};
}