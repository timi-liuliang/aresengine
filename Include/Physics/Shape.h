#pragma once

#include <Core/AresTransform.h>
#include <Core/AresSerialization.h>

namespace Ares
{
	// 支持的Shape类型
	enum ShapeType
	{
		ST_Segment3		=  0,		// 射线
		ST_Rect3,					// 轴向包围盒
		ST_Capsule,					// 胶囊
		ST_KdtTriangleMesh,			// KDTree三角形网格
		ST_HeightField,				// 高度图
		ST_Frustum,					// 平截头体
		ST_Box3,					// 轴向包围盒
		ST_Compount,				// 组合类型
		ST_Total,					// 类型总数
		ST_UnKnown,					// 未知
	};

	//----------------------------------------------------
	// CollisionShape 2012-7-30 帝林
	// CollisionShape provides an interface for collision
	// shapes that can be shared among CollionObjects
	//----------------------------------------------------
	class Rect3;
	class Shape
	{
	public:
		// 构造函数
		Shape( ShapeType type)
			: m_type( type)
			, m_scale( 1.f, 1.f, 1.f)
		{}

		// 虚析构函数
		virtual ~Shape(){}

		// 返回模型类型
		int GetShapeType() const { return m_type; }

		// 是否为凸多边形
		bool IsConvex();

		// 构建AABB包围盒
		virtual void BuildAABB( Rect3& box, const Transform& trans) const=0;

		// 获取包围球
		virtual void BuildBoundingSphere( Vector3& center, float& radius) const;

		// 计算运动物体惯性
		virtual Vector3 CalculateLocalInertia( float mass)=0;

		// 设置缩放
		virtual void SetScale( const Vector3& scale) { m_scale=scale; }

		// 获取支撑点
		virtual Vector3 GetSupportPoint( const Vector3& normal) const;

		// 获取中心点
		virtual Vector3 GetCenter() const;

		// Checks whether a point is within the shape
		virtual bool IsPointIn( const Vector3& point) const;

		// 序列化
		template <typename Archive> void serialize( Archive& ar, const unsigned int version)
		{
			ar & m_scale;
		}

	protected:
		int			m_type;
		Vector3		m_scale;
	};
	typedef s_ptr<Shape> ShapePtr;
}