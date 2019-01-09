#include <Physics/XenoCollide.h>

namespace Xeno
{
	CollideNeg::CollideNeg( const Shape* s1, const Quat& q1, const Vector3& t1)
		: Shape( ST_UnKnown)
		, m_shape( s1)
		, m_quat( q1)
		, m_trans( t1)
	{

	}

	// 构建AABB包围盒
	void CollideNeg::BuildAABB( Rect3& box, const Transform& trans) const
	{
		A_ASSERT( FALSE);
	}

	// 获取支撑点
	Vector3 CollideNeg::GetSupportPoint( const Vector3& normal)
	{
		return -( m_quat.Rotate( m_shape->GetSupportPoint( (~m_quat).Rotate( -normal))) + m_trans);
	}

	// 获取中心点
	Vector3 CollideNeg::GetCenter()
	{
		return -( m_quat.Rotate( m_shape->GetCenter()) + m_trans);
	}

	CollideSum::CollideSum( const Shape* s1, const Quat& q1, const Vector3& t1, const Shape* s2, const Quat& q2, const Vector3& t2)
		: Shape( ST_UnKnown) 
		, m_shape1( s1)
		, m_shape2( s2)
		, m_quat1( q1)
		, m_quat2( q2)
		, m_trans1( t1)
		, m_trans2( t2)
	{

	}

	// 构建AABB包围盒
	void CollideSum::BuildAABB( Rect3& box, const Transform& trans) const
	{
		A_ASSERT( FALSE);
	}

	// 获取支撑点
	Vector3 CollideSum::GetSupportPoint( const Vector3& normal)
	{
		return m_quat1.Rotate( m_shape1->GetSupportPoint( (~m_quat1).Rotate( normal))) + m_trans1 + m_quat2.Rotate( m_shape2->GetSupportPoint( (~m_quat2).Rotate( normal))) + m_trans2;
	}

	// 获取中心点
	Vector3 CollideSum::GetCenter()
	{
		return m_quat1.Rotate( m_shape1->GetCenter()) + m_trans1 + m_quat2.Rotate( m_shape2->GetCenter()) + m_trans2;
	}
}