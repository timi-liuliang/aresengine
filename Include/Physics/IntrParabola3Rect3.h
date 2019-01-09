#pragma once

#include "Physics/Shapes.h"

namespace Ares
{
	//--------------------------------------------------------
	// intersection test between Parabola3 and Rect3
	//					2013-3-14  帝林
	struct IntrParabola3Rect3
	{
		const Rect3&	 m_rect;			// AABB包围盒
		const Parabola3& m_curve;			// 抛体曲线

		// construction
		IntrParabola3Rect3( const Parabola3& curve, const Rect3& rect3);

		// test intersection query
		bool Test();
	};
}