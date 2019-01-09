#pragma once

#include "Physics/Shapes.h"

namespace Ares
{
	//------------------------------------------------
	// Intersection between Segment3 and triangle3 
	//					2012-11-22 ╣шаж
	//------------------------------------------------
	struct IntrSegment3Triangle3
	{
		const Segment3&		m_segment;
		const Triangle3&	m_triangle;

		Vector3				m_intrPoint;

		// constructor
		IntrSegment3Triangle3( const Segment3& seg, const Triangle3& tri);

		// test-intersection query
		bool Test();
	};
}