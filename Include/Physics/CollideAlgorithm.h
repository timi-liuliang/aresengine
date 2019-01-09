#pragma once

#include "Physics/Shapes.h"

namespace Ares
{
	// 碰撞结果
	struct CollideContact
	{
		Vector3  m_normal;
		Vector3	 m_point0;
		Vector3	 m_point1;
	};
	typedef std::vector<CollideContact> CollideContactList;

	//------------------------------------------
	// 碰撞算法 2012-11-21 帝林
	//------------------------------------------
	struct CollideAlgorithm
	{
		// 相交测试
		virtual bool IntersectionTest( const Shape* shape1, const Shape* shape2);

		// 相交测试
		virtual bool IntersectionTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2)=0;

		// 接触测试
		virtual bool ContactTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2, CollideContactList& oContactList)=0;
	};

	//---------------------------------------
	// XenoCollide 2012-12-13 帝林
	//---------------------------------------
	struct CollideXeno : public CollideAlgorithm
	{
		// 相交测试
		virtual bool IntersectionTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2);

		// 接触测试
		virtual bool ContactTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2, CollideContactList& oContactList);
	};

	//------------------------------------------------
	// intersection test between Segment3 and Rect3
	//				2012-05-23      帝林
	struct CollideSegment3Rect3 : public CollideAlgorithm
	{
		// 相交测试
		virtual bool IntersectionTest( const Shape* shape1, const Shape* shape2);

		// construction
		virtual bool IntersectionTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2);

		// 接触测试
		virtual bool ContactTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2, CollideContactList& oContactList);
	};

	//------------------------------------------------
	// 射线与三角形模型 2012-11-28 帝林
	//------------------------------------------------
	struct CollideSegment3KdtTriangleMesh : public CollideAlgorithm
	{
		// construction
		virtual bool IntersectionTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2);

		// 接触测试
		virtual bool ContactTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2, CollideContactList& oContactList);
	};

	//------------------------------------------------
	// 射线与高度图 2012-11-30 帝林
	//------------------------------------------------
	struct CollideSegment3HeightField : public CollideAlgorithm
	{
		// 相交检测
		virtual bool IntersectionTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2);

		// 接触测试
		virtual bool ContactTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2, CollideContactList& oContactList);
	};

	//---------------------------------------------
	// Box3 with Rect3 2013-7-9 帝林
	//---------------------------------------------
	struct CollideBox3Rect3 : public CollideAlgorithm
	{
		// 相交测试
		virtual bool IntersectionTest( const Shape* shape1, const Shape* shape2);

		// 相交检测
		virtual bool IntersectionTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2);

		// 接触测试
		virtual bool ContactTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2, CollideContactList& oContactList);

	};

	//---------------------------------------------
	// Rect3 with Rect3 2012-12-13 帝林
	//---------------------------------------------
	struct CollideRect3Rect3 : public CollideAlgorithm
	{
		// 相交测试
		virtual bool IntersectionTest( const Shape* shape1, const Shape* shape2);

		// 相交检测
		virtual bool IntersectionTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2);

		// 接触测试
		virtual bool ContactTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2, CollideContactList& oContactList);
	};

	//---------------------------------------------
	// Rect3 with Frustum 2013-7-6 帝林
	//---------------------------------------------
	struct CollideFrustum3Rect3 : public CollideAlgorithm
	{
		// 相交测试
		virtual bool IntersectionTest( const Shape* shape1, const Shape* shape2);

		// 相交检测
		virtual bool IntersectionTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2);

		// 接触测试
		virtual bool ContactTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2, CollideContactList& oContactList);
	};

	//---------------------------------------------
	// Capsule3 with Capsule3 2012-12-18 帝林
	//---------------------------------------------
	struct CollideCapsule3Capsule3 : public CollideAlgorithm
	{
		// 相交检测
		virtual bool IntersectionTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2);

		// 接触测试
		virtual bool ContactTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2, CollideContactList& oContactList);
	};

	//---------------------------------------------
	// Rect3 with Rect3 2012-12-13 帝林
	//---------------------------------------------
	struct CollideCompountCompount : public CollideAlgorithm
	{
		// 相交检测
		virtual bool IntersectionTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2);

		// 接触测试
		virtual bool ContactTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2, CollideContactList& oContactList);
	};
}