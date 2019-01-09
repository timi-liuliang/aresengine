#pragma once

#include <Core/AresMath.h>

namespace Ares
{
	//---------------------------------------
	// Sphere3 2012-9-7 帝林
	//---------------------------------------
	struct Sphere3
	{
		Vector3		m_center;		// 中心点
		float		m_radius;		// 半径

		// constructor
		Sphere3();

		// initialized
		Sphere3( const Vector3& center, float radius);

		// 计算运动物体惯性
		virtual Vector3 CalculateLocalInertia( float mass) { A_ASSERT(false); return Vector3::Zero; }
	};
}