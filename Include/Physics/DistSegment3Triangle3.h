#pragma once

#include "Physics/Shapes.h"

namespace Ares
{
	//----------------------------------------
	// distance between segment and triangle
	//			2011-04-27    ╣шаж
	struct DistSement3Triangle3
	{
		const Segment3&		m_segment;
		const Triangle3&	m_triangle;

		Vector3				m_closePointSegment;
		Vector3				m_closePointTriangle;

		// information about the closet points
		float				m_segmentParam;			// closestSegment	   = set origin+param*seg.dir
		float				m_triangleBary[3];		// closestTriangle = sum{i=0}^2 bary[i]*tri.vert[i]

		// construction
		DistSement3Triangle3( const Segment3& segment, const Triangle3& triangle);

		// static distance queries
		float Get();
		float GetSquared();
	};
}