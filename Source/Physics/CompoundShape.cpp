#include <Physics/CompoundShape.h>

BOOST_CLASS_EXPORT( Ares::CompoundShape)

namespace Ares
{
	// 构造函数
	CompoundShape::CompoundShape()
		: Shape( ST_Compount)
	{
	}

	// 析构函数
	CompoundShape::~CompoundShape()
	{
		for( size_t i=0; i<m_children.size(); i++)
			SAFE_DELETE( m_children[i].m_shape);
	}

	// 添加子外形
	void CompoundShape::AddChildShape( const Transform& localTransform, Shape* shape)
	{
		CompoundShapeChild child;
		child.m_transform = localTransform;
		child.m_shape	  = shape;

		m_children.push_back( child);

		// Update the local bounding Box
		RefreshLocalBoundingBox();
	}

	// 更新本地包围盒
	void CompoundShape::RefreshLocalBoundingBox()
	{
		m_localAABB.Reset();
		for( size_t i=0; i<m_children.size(); i++)
		{
			Rect3 localAABB;
			m_children[i].m_shape->BuildAABB( localAABB, m_children[i].m_transform);

			m_localAABB.UnionPoint( localAABB.GetMinPoint());
			m_localAABB.UnionPoint( localAABB.GetMaxPoint());
		}
	}

	// 构建AABB包围盒
	void CompoundShape::BuildAABB( Rect3& box, const Transform& trans) const
	{
		box = m_localAABB;
		box.TransformByMatrix( trans.GetMatrix());
	}
}