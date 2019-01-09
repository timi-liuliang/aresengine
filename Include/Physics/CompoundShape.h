#pragma once

#include "Rect3.h"

namespace Ares
{
	// CompoundShapeChild
	struct CompoundShapeChild
	{
		Transform		m_transform;
		Shape*			m_shape;

		// 构造函数
		CompoundShapeChild() : m_shape( NULL)
		{}

		// 序列化
		template <typename Archive> void serialize( Archive& ar, const unsigned int version)
		{
			ar & m_transform;
			ar & m_shape;
		}
	};

	//----------------------------------------------------
	// CompoundShape 2012-9-10 帝林
	//----------------------------------------------------
	class CompoundShape : public Shape
	{
	public:
		CompoundShape();
		~CompoundShape();

		// 添加子外形
		void AddChildShape( const Transform& localTransform, Shape* shape);

		// 获取子外形数量
		int GetNumChildShapes() const { return static_cast<int>(m_children.size()); }

		// 根据索引获取子外形
		Shape* GetChildShape( int idx) { return m_children[idx].m_shape; }

		// 根据索引获取子外形
		const Shape* GetChildShape( int idx) const { return m_children[idx].m_shape; }

		// 获取子外形本地转换
		Transform& GetChildTransform( int idx) { return m_children[idx].m_transform; }

		// 获取子外形本地转换
		const Transform& GetChildTransform( int idx) const { return m_children[idx].m_transform; }

		// 构建AABB包围盒
		virtual void BuildAABB( Rect3& box, const Transform& trans) const;

		// 计算运动物体惯性
		virtual Vector3 CalculateLocalInertia( float mass) { A_ASSERT(false); return Vector3::Zero; }

		// 序列化
		template <typename Archive> void serialize( Archive& ar, const unsigned int version)
		{
			ar & boost::serialization::base_object<Shape>(*this);
			ar & m_localAABB;
			ar & m_children;
		}

	private:
		// 更新本地包围盒
		void RefreshLocalBoundingBox();

	private:
		Rect3						m_localAABB;		// 本地包围盒
		vector<CompoundShapeChild>	m_children;			// 子外形
	};
}