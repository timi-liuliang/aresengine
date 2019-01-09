#pragma once

#include "Physics/Shapes.h"

namespace Ares
{
	//-----------------------------------------
	// Intersection between triangle3 and Box3
	//			2011-07-19	╣шаж
	struct IntrBox3Triangle3
	{
		const Box3&		 m_box;	
		const Triangle3& m_triangle;

		int				 m_quantity;	// quantity of intersect vertices
		Vector3			 m_akPoint[7];	// The intersection for static query. A triangle 
										// can intersect a box in at most 7 vertices.
		// construction 
		IntrBox3Triangle3( const Box3& box, const Triangle3& triangle);

		// static test-intersection query
		bool Test();

		// static test-intersection query
		bool Find();
	};
}