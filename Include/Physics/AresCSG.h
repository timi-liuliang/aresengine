#ifndef CSG_BOX3_TRIANGLE3_H
#define CSG_BOX3_TRIANGLE3_H

#include "Box3.h"
#include "Triangle3.h"
#include "IntrBox3Triangle3.h"

namespace Ares
{
	//--------------------------------------
	// CSGBox3Triangle3 2013-7-3 帝林
	//--------------------------------------
	struct CSGBox3Triangle3
	{
		// 交
		static void Intersection( vector<Vector3>& vertexs, vector<WORD>& indices, const Box3& box3, const Triangle3& triangle);

		// 并

		// 差
	};

	//--------------------------------------
	// CSGAABB3Frustum3 2013-8-12 帝林
	//--------------------------------------
	struct CSGAABB3Frustum3
	{
		// 求交
		static void Intersection();
	};
}

#endif