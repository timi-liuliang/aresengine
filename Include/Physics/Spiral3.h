#ifndef SPIRAL_H
#define SPIRAL_H

#include <Core/AresMath.h>

namespace Ares
{
	//--------------------------------
	// 螺旋线 2013-6-18 帝林
	//--------------------------------
	struct Spiral3
	{
		// 计算当前位置
		static void CalculatePos( Vector3& oResult, const Vector3& centerPos, const Vector3& centerDir, float radius, float angle);
	};
}

#endif