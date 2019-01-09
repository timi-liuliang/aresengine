#include "Physics/Triangle3.h"

namespace Ares
{
	// construction
	Triangle3::Triangle3()
	{

	}

	// initialized
	Triangle3::Triangle3( const Vector3& v0, const Vector3& v1, const Vector3& v2)
	{
		m_v[0] = v0;
		m_v[1] = v1;
		m_v[2] = v2;
	}

	// initialized
	Triangle3::Triangle3( const Vector3	v[3])
	{
		m_v[0] = v[0];
		m_v[1] = v[1];
		m_v[2] = v[2];
	}

	// calc normal (unit-length)
	Vector3 Triangle3::GetNormal() const
	{
		Vector3 edge0 = m_v[1] - m_v[0];
		Vector3 edge1 = m_v[2] - m_v[0];

		// calc triangle normal
		Vector3 normal;
		normal = Vector3Cross( edge0, edge1);
		normal.Normalize();

		return normal;
	}

	// calc surface area
	float Triangle3::GetArea() const
	{
		// ´íÎó

		return 0.f;
	}

	// Determine whether point in this triangle
	bool Triangle3::IsVertexIn( const Vector3& p) const
	{
		// http://www.cnblogs.com/graphics/archive/2010/08/05/1793393.html
		// http://mathworld.wolfram.com/BarycentricCoordinates.html
		Vector3 v0 = m_v[1] - m_v[0];
		Vector3 v1 = m_v[2] - m_v[0];
		Vector3 v2 = p		- m_v[0];

		float dot00 = Vector3Dot( v0, v0);
		float dot01 = Vector3Dot( v0, v1);
		float dot02 = Vector3Dot( v0, v2);
		float dot11 = Vector3Dot( v1, v1);
		float dot12 = Vector3Dot( v1, v2);

		float inverDeno = 1 / (dot00 * dot11 - dot01 * dot01) ;

		float u = (dot11 * dot02 - dot01 * dot12) * inverDeno ;
		if (u < 0.f || u > 1.f) // if u out of range, return directly
			return false ;

		float v = (dot00 * dot12 - dot01 * dot02) * inverDeno ;
		if (v < 0.f || v > 1.f) // if v out of range, return directly
			return false ;

		return u + v <= 1.f;
	}
}