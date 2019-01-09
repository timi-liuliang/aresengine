#pragma once

#include "Physics/Kdt.h"
#include "Triangle3.h"

namespace Ares
{
	//----------------------------------------
	// KdtTriangleMesh 2012-11-23 帝林
	//----------------------------------------
	class KdtTriangleMesh : public Shape
	{
	public:
		KdtTriangleMesh();
		~KdtTriangleMesh();

		// 初始化
		bool Init( size_t numVertexs, Vector3* vertsData, size_t numTris, int* trisData);

		// 射线检测
		bool RayTrace( const Vector3& rayOrig, const Vector3& rayDir, float& dist, Vector3& normal) const;

		// 获取包围盒中三角形索引
		void GetTriIdxs( set<int>& triIdxs, const Rect3& aabb) const;

		// 获取三角形
		Triangle3 GetTriangle( size_t idx) const;

		// 构建AABB包围盒
		virtual void BuildAABB( Rect3& box, const Transform& trans) const;

		// 计算运动物体惯性
		virtual Vector3 CalculateLocalInertia( float mass) { A_ASSERT(false); return Vector3::Zero; }

		// 重置数据
		void Reset();

		// 序列化
		template <typename Archive> void serialize( Archive& ar, const unsigned int version)
		{
			ar & boost::serialization::base_object<Shape>(*this);
			ar & m_kdt;
			ar & m_localAabb;
			ar & m_verts;
			ar & m_tris;
		}

	private:
		Kdt<int>		m_kdt;				// KDTree
		Rect3			m_localAabb;		// 包围盒
		vector<Vector3>	m_verts;			// 顶点
		vector<Int3>	m_tris;				// 三角形
		int				m_rayDir[8][3][2];	// 辅助射线跟踪
	};
}