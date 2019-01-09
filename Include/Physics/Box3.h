#pragma once

#include "Rect3.h"

namespace Ares
{
	//------------------------------------------------------------------
	// Box3 2011-07-12   
	// A box has center C, axis directions U[0], U[1], and U[2] ( all 
	// unit-length vectors), and extents e[0], e[1], and e[2] ( all 
	// nonnegative numbers). A point x = C + y[0]*U[0] + y[1]*U[1] + 
	// y[2]*U[2] is inside or on the box wheneve |y[i]| <= e[i] for all i
	struct Box3 : public Shape
	{
		Vector3		m_center;	// 中心点
		Vector3		m_axis[3];	// 朝向,轴朝向必须是规范正交的
		float		m_extent[3];// 非负 

		// construction (uninitialized)
		Box3();

		// initialized
		Box3( const Vector3& center, const Vector3& axis0, const Vector3& axis1, const Vector3& axis2, float extent0, float extent1, float extent2);

		// Set
		void Set( const Vector3& center, const Vector3& axis0, const Vector3& axis1, const Vector3& axis2, float extent0, float extent1, float extent2);

		// 构建AABB包围盒
		virtual void BuildAABB( Rect3& box, const Transform& trans) const;

		// 构建八个顶点
		void BuildEightPoints( Vector3* points) const;

		// 计算运动物体惯性
		virtual Vector3 CalculateLocalInertia( float mass);

		// 判断点是否在有向包围盒内
		virtual bool IsPointIn( const Vector3& point) const;

		// 判断AABB包围盒(写法错误，望纠正)
		bool  IsAABBOverlap( const Rect3& rect) const;

		// 执行转换
		void DoTransform( const Transform& transform);

		// transform by matrix
		void  TransformByMatrix( const Matrix44& matrix);

		// Serialize
		template< typename Archive> void serialize( Archive& ar, const unsigned int version)
		{
			ar & boost::serialization::base_object<Shape>(*this);
			ar & m_center;
			ar & m_axis;
			ar & m_extent;
		}
	};
}