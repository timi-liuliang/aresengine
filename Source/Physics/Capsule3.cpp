#include <Physics/Capsule3.h>

BOOST_CLASS_EXPORT( Ares::Capsule3)

namespace Ares
{
	// construction
	Capsule3::Capsule3()
		: Shape( ST_Capsule)
	{
	}

	// initialized
	Capsule3::Capsule3( const Segment3& segment, float radius)
		: Shape( ST_Capsule)
		, m_segment( segment),
		  m_radius( radius)
	{
	}

	// 设置参数
	void Capsule3::Set( const Segment3& segment, float radius)
	{
		m_segment = segment;
		m_radius  = radius;
	}

	// constructor AABB bounding box
	void Capsule3::BuildAABB( Rect3& box, const Transform& trans) const
	{
		Vector3 radiusExtent( m_radius, m_radius, m_radius);

		box.UnionPoint( trans.TransformVector3( m_segment.GetVertex0()));
		box.UnionPoint( trans.TransformVector3( m_segment.GetVertex1()));

		box.UnionPoint( box.GetMaxPoint() + radiusExtent);
		box.UnionPoint( box.GetMinPoint() - radiusExtent);
	}

	// 获取支撑点
	Vector3 Capsule3::GetSupportPoint( const Vector3& normal) const
	{
		Vector3 tNormal = normal; tNormal.Normalize();
		if( Vector3Dot( tNormal, m_segment.GetVertex0()) > Vector3Dot( tNormal, m_segment.GetVertex1()))
			return m_segment.GetVertex0() + tNormal * m_radius;
		else
			return m_segment.GetVertex1() + tNormal * m_radius;
	}

	// 执行转换
	void Capsule3::DoTransform( const Transform& transform)
	{
		m_segment.DoTransform( transform);
	}
}