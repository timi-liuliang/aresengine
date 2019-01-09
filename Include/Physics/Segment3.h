#pragma once

#include <Core/AresMath.h>
#include "Rect3.h"

namespace Ares
{
	//--------------------------------------------------------------------
	// segment3 2011-04-27
	// The segment is represented as p+t*D, where p is the segment origin,
	// D is a unit-length direction vector and |t| <= e. The value e is 
	// referred to as the extent of the segment. the end points of the 
	// segment are P-e*D and P+e*D. the user must ensure that the direction
	// vector is unit-length. the representation for a segment is analogous
	// to that for an oriented bounding box. P is the center, D is the axis
	// direction, and e is the extent.
	struct Segment3 : public Shape
	{
		Vector3		m_orig;			// segment's center point
		Vector3		m_dir;			// direction
		float		m_extent;

		// construction uninitialized
		Segment3();

		// initialized
		Segment3( const Vector3& orig, const Vector3& dir, float extent);

		// initialized use two endpoints
		Segment3( const Vector3& end0, const Vector3& end1);

		// initialized use two endpoints
		void Set( const Vector3& end0, const Vector3& end1);

		// end points
		Vector3 GetVertex1() const;

		// neg end points
		Vector3 GetVertex0() const;

		// Length
		float Length() const { return m_extent * 2.f; }

		// 构建AABB包围盒
		virtual void BuildAABB( Rect3& box, const Transform& trans) const;

		// 构建AABB包围盒组
		void BuildAABBArray( vector<Rect3>& aabbArrays, const Transform& trans) const;

		// 计算运动物体惯性
		virtual Vector3 CalculateLocalInertia( float mass) { A_ASSERT(false); return Vector3::Zero; }

		// 执行转换
		void DoTransform( const Transform& transform);

		// 序列化
		template <typename Archive> void serialize( Archive& ar, const unsigned int version)
		{
			ar & boost::serialization::base_object<Shape>(*this);
			ar & m_orig;
			ar & m_dir;
			ar & m_extent;
		}
	};
}