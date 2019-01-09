#pragma once

#include "Physics/Shapes.h"

namespace Ares
{
	// compute an oriented bounding box of the points the box axis is confirm
	Box3 ContOrientedBox( const Vector3& axis0, const Vector3& axis1, const Vector3& axis2, int numPoints, const Vector3* points);
}