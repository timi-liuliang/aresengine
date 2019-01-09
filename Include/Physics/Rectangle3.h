#pragma once

#include <Core/AresMath.h>

namespace Ares
{
	//------------------------------------------------------------------
	// Rectangle3 2011-07-12   
	// Points are R(s, t) = C + s0*u0 + s1*u1, where C is the center of 
	// the rectangle, u0 and u1 are unit-length and perpendicular axis.
	// the parameters s0 and s1 are constrained by |s0|<=e0 and |s1|,=e1
	// where e0>0 and e1>0 are called the extents of the rectangle
	struct Rectangle3
	{
		Vector3		m_center;	// 中心点
		Vector3		m_axis[2];	// 朝向,轴朝向必须是规范正交的
		float		m_extent[2];// 非负 

		// construction (uninitialized)
		Rectangle3();

		// initialized
		Rectangle3( const Vector3& center, const Vector3& axis0, const Vector3& axis1, float extent0, float extent1);
	};
}