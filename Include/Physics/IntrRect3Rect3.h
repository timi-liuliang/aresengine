#pragma once

#include "Physics/Shapes.h"

namespace Ares
{
	//------------------------------------------------
	// intersection test between Rect3 and Rect3
	//				2012-05-23      µ€¡÷
	struct IntrRect3Rect3
	{
		Rect3			m_intrRect;	// ΩªºØ

		const Rect3&	m_rect0;
		const Rect3&	m_rect1;

		// construction
		IntrRect3Rect3( const Rect3& rect0, const Rect3& rect1);

		// test intersection query
		bool  Test();
	};
}