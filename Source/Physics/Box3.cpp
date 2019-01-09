#include <algorithm>
#include <Physics/Box3.h>
#include <Physics/IntrBox3Sphere3.h>

BOOST_CLASS_EXPORT( Ares::Box3)

namespace Ares
{
	// construction (uninitialized)
	Box3::Box3()
		: Shape( ST_Box3)
	{

	}

	// initialized
	Box3::Box3( const Vector3& center, const Vector3& axis0, const Vector3& axis1, const Vector3& axis2, float extent0, float extent1, float extent2)
		: Shape( ST_Box3)
	{
		Set( center, axis0, axis1, axis2, extent0, extent1, extent2);
	}

	// Set
	void Box3::Set( const Vector3& center, const Vector3& axis0, const Vector3& axis1, const Vector3& axis2, float extent0, float extent1, float extent2)
	{
		m_center = center;

		m_axis[0] = axis0;
		m_axis[1] = axis1;
		m_axis[2] = axis2;

		m_axis[0].Normalize();
		m_axis[1].Normalize();
		m_axis[2].Normalize();

		m_extent[0] = std::abs(extent0);
		m_extent[1] = std::abs(extent1);
		m_extent[2] = std::abs(extent2);
	}

	// 构建AABB包围盒
	void Box3::BuildAABB( Rect3& box, const Transform& trans) const
	{
		boost::array< Vector3, 8> eightPoints;
		BuildEightPoints( eightPoints.data());

		for( size_t i=0; i<eightPoints.size(); i++)
			box.UnionPoint( eightPoints[i]);
	}

	// 构建八个顶点
	void Box3::BuildEightPoints( Vector3* points) const
	{
		Vector3 extAxis0 = m_extent[0] * m_axis[0];
		Vector3 extAxis1 = m_extent[1] * m_axis[1];
		Vector3 extAxis2 = m_extent[2] * m_axis[2];

		points[0] = m_center - extAxis0 - extAxis1 - extAxis2;
		points[1] = m_center + extAxis0 - extAxis1 - extAxis2;
		points[2] = m_center + extAxis0 + extAxis1 - extAxis2;
		points[3] = m_center - extAxis0 + extAxis1 - extAxis2;
		points[4] = m_center - extAxis0 - extAxis1 + extAxis2;
		points[5] = m_center + extAxis0 - extAxis1 + extAxis2;
		points[6] = m_center + extAxis0 + extAxis1 + extAxis2;
		points[7] = m_center - extAxis0 + extAxis1 + extAxis2;
	}

	// 判断点是否在有向包围盒内
	bool  Box3::IsPointIn( const Vector3& point) const
	{
		Vector3 tD = point - m_center;

		if( abs(Vector3Dot( tD, m_axis[0])) > m_extent[0])
			return false;

		if( abs(Vector3Dot( tD, m_axis[1])) > m_extent[1])
			return false;

		if( abs(Vector3Dot( tD, m_axis[2])) > m_extent[2])
			return false;

		return true;
	}

	// 判断AABB包围盒
	bool Box3::IsAABBOverlap( const Rect3& rect) const
	{
		Vector3 center;
		float   radius;
		rect.BuildBoundingSphere( center, radius);

		Sphere3 sphere( center, radius);
		IntrBox3Sphere3 intrBS( *this, sphere);

		return intrBS.Test();
	}

	// 执行转换
	void Box3::DoTransform( const Transform& transform)
	{
		m_center = transform.TransformVector3( m_center);

		m_axis[0] = transform.m_rotation.Rotate( m_axis[0]);
		m_axis[1] = transform.m_rotation.Rotate( m_axis[1]);
		m_axis[2] = transform.m_rotation.Rotate( m_axis[2]);
	}

	// transform by matrix
	void  Box3::TransformByMatrix( const Matrix44& matrix)
	{
		// center
		m_center = matrix.TransformVector3( m_center);

		// axis
		m_axis[0] = matrix.TransformVector3( m_axis[0]);
		m_axis[1] = matrix.TransformVector3( m_axis[1]);
		m_axis[2] = matrix.TransformVector3( m_axis[2]);
	}

	// 计算运动物体惯性
	Vector3 Box3::CalculateLocalInertia( float mass)
	{
		float lx = m_extent[0];
		float ly = m_extent[1];
		float lz = m_extent[2];

		return Vector3( mass/12.f * (ly*ly + lz*lz), mass/12.f * (lx*lx + lz*lz), mass/12.f * (lx*lx + ly*ly));
	}
}