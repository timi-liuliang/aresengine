#pragma once

#include <Core/AresMath.h>

namespace Ares
{
	//---------------------------------------------------
	// triangle3 :
	// the triangle is represented as an array of three 
	// vertices, v0, v1 and v2.
	struct Triangle3
	{
		Vector3		m_v[3];

		// construction
		Triangle3();

		// initialized
		Triangle3( const Vector3& v0, const Vector3& v1, const Vector3& v2);

		// initialized
		Triangle3( const Vector3 v[3]);

		// calc normal (unit-length)
		Vector3 GetNormal() const;

		// get surface area
		float GetArea() const;

		// Determine whether point in this triangle
		bool IsVertexIn( const Vector3& p) const;

		// 计算运动物体惯性
		virtual Vector3 CalculateLocalInertia( float mass) { A_ASSERT(false); return Vector3::Zero; }
	};
}