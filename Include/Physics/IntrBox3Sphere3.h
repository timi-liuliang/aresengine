#pragma once

#include "Physics/Shapes.h"

namespace Ares
{
	//----------------------------------------
	// IntrBox3Sphere3	2012-9-7 帝林
	//----------------------------------------
	struct IntrBox3Sphere3
	{
		Vector3			m_intrPoint;	// 相交点

		const Box3&		m_box3;			// 盒子
		const Sphere3&	m_sphere3;		// 球

		// constructor
		IntrBox3Sphere3( const Box3& box3, const Sphere3& sphere);

		// test intersection query
		bool Test();
	};
}