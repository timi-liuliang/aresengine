#pragma once

#include "Physics/Shapes.h"

namespace Ares
{
	//-------------------------------------
	// IntrSegment3Sphere3 2012-10-18 ╣шаж
	//-------------------------------------
	struct IntrSegment3Sphere3
	{
		int				m_quantity;			// intersection num
		Vector3			m_intrPoints[2];	// intersection points, most two
		float			m_segmentParam[2];	// segment t

		const Segment3&	m_segment;			// segment
		const Sphere3&	m_sphere;			// sphere

		// constructor
		IntrSegment3Sphere3( const Segment3& seg, const Sphere3& sphere);

		// test if intersection
		bool Test();

		// test and find intr points
		bool Find();
	};
}