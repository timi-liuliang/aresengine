#pragma once

#include "Physics/Shapes.h"

namespace Ares
{
	//--------------------------------------------------------
	// intersection test between ProjectileMotion3 and Triangle3
	//					2013-3-14  帝林
	struct IntrProjectileMotion3Triangle3
	{
		float					m_intrTime[2];		// 曲线使用
		Vector3					m_intrPoints[2];
		UINT					m_numIntrPoint;

		const Triangle3&		 m_triangle;		// 三角形
		const Parabola3& m_curve;			// 抛体曲线

		// construction
		IntrProjectileMotion3Triangle3( const Parabola3& curve, const Triangle3& triangle);

		// test intersection query
		bool Test();
	};
}