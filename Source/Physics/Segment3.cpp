#include "Physics/Segment3.h"

BOOST_CLASS_EXPORT( Ares::Segment3)

namespace Ares
{
	// construction uninitialized
	Segment3::Segment3()
		: Shape( ST_Segment3)
	{
	}

	// initialized
	Segment3::Segment3( const Vector3& orig, const Vector3& dir, float extent)
		: Shape( ST_Segment3) 
		, m_orig( orig),
		  m_dir( dir),
		  m_extent( extent)
	{
		m_dir.Normalize();
	}

	// initialized use two endpoints
	Segment3::Segment3( const Vector3& end0, const Vector3& end1)
		: Shape( ST_Segment3)
	{
		Set( end0, end1);
	}

	// initialized use two endpoints
	void Segment3::Set( const Vector3& end0, const Vector3& end1)
	{
		m_orig = 0.5f * ( end0 + end1);
		m_dir  = end1 - end0;
		m_extent = 0.5f * m_dir.Length();
		m_dir.Normalize();
	}

	// end points
	Vector3 Segment3::GetVertex1() const
	{
		return m_orig + m_extent * m_dir;
	}

	// neg end points
	Vector3 Segment3::GetVertex0() const
	{
		return m_orig - m_extent * m_dir;
	}


	// 构建AABB包围盒
	void Segment3::BuildAABB( Rect3& box, const Transform& trans) const
	{
		box.Reset();
		box.UnionPoint( GetVertex0());
		box.UnionPoint( GetVertex1());
	}

	// 构建AABB包围盒组
	void Segment3::BuildAABBArray( vector<Rect3>& aabbArrays, const Transform& trans) const
	{
		Rect3 box;
		box.Reset();
		box.UnionPoint( GetVertex0());
		box.UnionPoint( GetVertex1());

		aabbArrays.push_back( box);
	}

	// 执行转换
	void Segment3::DoTransform( const Transform& transform)
	{
		m_orig = transform.TransformVector3( m_orig);
		m_dir  = transform.m_rotation.Rotate( m_dir);
	}
}