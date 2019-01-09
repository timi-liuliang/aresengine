#include <algorithm>
#include <Physics/ContBox3.h>

namespace Ares
{
	// compute an oriented bounding box of the points the box axis is confirm
	Box3 ContOrientedBox( const Vector3& axis0, const Vector3& axis1, const Vector3& axis2, int numPoints, const Vector3* points)
	{
		float   minExtent0 =  FLT_BIG;
		float	maxExtent0 = -FLT_BIG;
		float   minExtent1 =  FLT_BIG;
		float	maxExtent1 = -FLT_BIG;
		float   minExtent2 =  FLT_BIG;
		float	maxExtent2 = -FLT_BIG;
		for ( int i=0; i<numPoints; i++)
		{
			minExtent0 = std::min<float>( minExtent0, Vector3Dot( axis0, points[i]));
			maxExtent0 = std::max<float>( maxExtent0, Vector3Dot( axis0, points[i]));
			minExtent1 = std::min<float>( minExtent1, Vector3Dot( axis1, points[i]));
			maxExtent1 = std::max<float>( maxExtent1, Vector3Dot( axis1, points[i]));
			minExtent2 = std::min<float>( minExtent2, Vector3Dot( axis2, points[i]));
			maxExtent2 = std::max<float>( maxExtent2, Vector3Dot( axis2, points[i]));
		}

		float	extent0 = (maxExtent0 - minExtent0) * 0.5f;
		float	extent1 = (maxExtent1 - minExtent1) * 0.5f; 
		float	extent2 = (maxExtent2 - minExtent2) * 0.5f;
		Vector3 center	= ((maxExtent0+minExtent0) * axis0 + (maxExtent1+minExtent1) * axis1 + (maxExtent2+minExtent2) * axis2) / 6.f;

		return Box3( center, axis0, axis1, axis2, extent0, extent1, extent2);
	}
}