#pragma once

#include "Physics/Shapes.h"

namespace Ares
{
	//------------------------------------
	// IntrAxis
	//				2011-07-19  ╣шаж
	class IntrAxis
	{
	public:
		// get triangle3 projection
		static void GetProjection( const Vector3& rkAxis, const Triangle3& rkTriangle, float& fMin, float& fMax);

		// projections
		static void GetProjection( const Vector3& rkAxis, const Box3& rkBox, float& fMin, float& fMax);
	};


	//-------------------------------------------------------------------------
	// Miscellaneous support
	// The input and output polygons are stored in akP. The size of akp is
	// assumed to be large enough to store the clipped convex polygon vertices.
	// For now the maximum array size is 8 to support the current intersection
	// algorithms.
	void ClipConvexPolygonAgainstPlane( const Vector3& rkNormal, float fConstant, int& rQuantity, Vector3* akp);
}