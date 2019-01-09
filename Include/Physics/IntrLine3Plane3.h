#pragma once

#include "Physics/Shapes.h"

namespace Ares
{
	//------------------------------------------------
	// intersection test between line3 and plane3
	//				2012-6-7      ╣шаж
	struct IntrLine3Plane3
	{
		float			 m_distance;
		Vector3			 m_intrPoint;

		const Line3&	 m_line;
		const Plane3&	 m_plane;

		// construction
		IntrLine3Plane3( const Line3& line, const Plane3& plane);

		// test intersection query
		bool  Test();
	};
}