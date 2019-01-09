#include <Physics/Collide.h>

namespace Ares
{
	CollideAlgorithm* Collide::m_dispatch[ST_Total][ST_Total];
	static Collide			   g_collide;						// 执行默认初始化

	// 构造函数
	Collide::Collide()
	{
		std::memset( m_dispatch, 0, sizeof(void*)*ST_Total*ST_Total);

		SetCollideAlgorithm( ST_Segment3, ST_Rect3,				new_ CollideSegment3Rect3);
		SetCollideAlgorithm( ST_Segment3, ST_KdtTriangleMesh,	new_ CollideSegment3KdtTriangleMesh);
		SetCollideAlgorithm( ST_Segment3, ST_HeightField,		new_ CollideSegment3HeightField);
		SetCollideAlgorithm( ST_Rect3,    ST_Rect3,				new_ CollideRect3Rect3);
		SetCollideAlgorithm( ST_Capsule,  ST_Capsule,			new_ CollideCapsule3Capsule3);
		SetCollideAlgorithm( ST_Capsule,  ST_Compount,			new_ CollideCompountCompount);
		SetCollideAlgorithm( ST_Compount, ST_Compount,			new_ CollideCompountCompount);
		SetCollideAlgorithm( ST_Rect3,	  ST_Frustum,			new_ CollideFrustum3Rect3);
		SetCollideAlgorithm( ST_Box3,	  ST_Rect3,			    new_ CollideBox3Rect3);
	}

	// 析构函数
	Collide::~Collide()
	{
		for( int i=0; i<ST_Total; i++)
			for( int j=0; j<ST_Total; j++)
				SAFE_DELETE( m_dispatch[i][j]);
	}

	// 设置碰撞算法
	bool Collide::SetCollideAlgorithm( ShapeType type0, ShapeType type1, CollideAlgorithm* algorithm)
	{
		if( type0 < type1)
		{
			SAFE_DELETE( m_dispatch[type0][type1]);
			m_dispatch[type0][type1] = algorithm;
		}
		else
		{
			SAFE_DELETE( m_dispatch[type0][type1]);
			m_dispatch[type1][type0] = algorithm;
		}

		return true;
	}

	// 相交测试
	bool Collide::IntersectionTest( const Shape* shape1, const Shape* shape2)
	{
		if( shape1->GetShapeType() < shape2->GetShapeType())
			return m_dispatch[shape1->GetShapeType()][shape2->GetShapeType()]->IntersectionTest( shape1, shape2);
		else
			return m_dispatch[shape2->GetShapeType()][shape1->GetShapeType()]->IntersectionTest( shape2, shape1);
	}

	// 相交测试
	bool Collide::IntersectionTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2)
	{
		if( shape1->GetShapeType() < shape2->GetShapeType())
			return m_dispatch[shape1->GetShapeType()][shape2->GetShapeType()]->IntersectionTest( shape1, q1, t1, shape2, q2, t2);
		else
			return m_dispatch[shape2->GetShapeType()][shape1->GetShapeType()]->IntersectionTest( shape2, q2, t2, shape1, q1, t1);
	}

	// 进行碰撞检测
	bool Collide::ContactTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2, CollideContactList& oContactList, bool clear/*=true*/)
	{
		if( clear)
			oContactList.clear();

		if( shape1->GetShapeType() < shape2->GetShapeType())
			return m_dispatch[shape1->GetShapeType()][shape2->GetShapeType()]->ContactTest( shape1, q1, t1, shape2, q2, t2, oContactList);

		if( m_dispatch[shape2->GetShapeType()][shape1->GetShapeType()]->ContactTest( shape2, q2, t2, shape1, q1, t1, oContactList))
		{
			for( size_t i=0; i<oContactList.size(); i++)
			{
				oContactList[i].m_normal = -oContactList[i].m_normal;
				std::swap( oContactList[i].m_point0, oContactList[i].m_point1);
			}

			return true;
		}

		return false;
	}
}