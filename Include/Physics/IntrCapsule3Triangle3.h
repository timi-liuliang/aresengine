#pragma once

#include "Physics/Shapes.h"

namespace Ares
{
	//------------------------------------------------
	// intersection test between capsule and triangle
	//				2011-04-28      ╣шаж
	struct IntrCapsule3Triangle3
	{
		Vector3			 m_intrPointCapsule;
		Vector3			 m_intrPointTriangle;

		const Capsule3&	 m_capsule;
		const Triangle3& m_triangle;

		// construction
		IntrCapsule3Triangle3( const Capsule3& capsule, const Triangle3& triangle);

		// test intersection query
		bool  Test();
	};
}