#include <Physics/XenoCollide.h>
#include <Physics/Collide.h>
#include <Physics/IntrRect3Rect3.h>
#include <Physics/Intersect.h>
#include <Physics/IntrSegment3Triangle3.h>
#include <Physics/IntrCapsule3Capsule3.h>

#define USE_XENO FALSE
using namespace Xeno;

namespace Ares
{ 
	// 相交测试
	bool CollideAlgorithm::IntersectionTest( const Shape* shape1, const Shape* shape2)
	{
		A_ASSERT( FALSE);

		return false;
	}

	// 相交测试
	bool CollideXeno::IntersectionTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2)
	{
		A_ASSERT( FALSE);

		return false;
	}

	// 接触测试
	bool CollideXeno::ContactTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2, CollideContactList& oContactList)
	{
		CollideContact oContact;
		static bool  gTrackingOn      = false;
		static float gAvgSupportCount = 0.f;
		
		INT supportCount = 0;

		static float kCollideEpsilon = 1e-3f;
		static INT	 callCount		 = 0;

		callCount++;

		CollideNeg neg( shape1, q1, t1);
		CollideSum diff( shape2, q2, t2, &neg, Quat( 0.f, 0.f, 0.f, 1.f), Vector3( 0.f, 0.f, 0.f));

		// v0 = center of Minkowski sum
		Vector3 v01 = q1.Rotate( shape1->GetCenter()) + t1;
		Vector3 v02 = q2.Rotate( shape2->GetCenter()) + t2;
		Vector3 v0  = v02 - v01; //v0.Normalize();

		// v1 = support in direction of origin
		Vector3 n = -v0;
		supportCount++;
		Vector3 v11 = TransformSupportVert( shape1, q1, t1, -n);
		Vector3 v12 = TransformSupportVert( shape2, q2, t2,  n);
		Vector3 v1  = v12 - v11;// v1.Normalize();

		if( Vector3Dot( v1, n) <=0.f)
		{
			oContact.m_normal = n;
		
			return false;
		}

		// v2 - support perpendicular to v1, v0
		n = Vector3Cross( v1, v0);
		if( n==Vector3::Zero)
		{
			n = v1-v0;
			n.Normalize();

			oContact.m_normal = n;
			oContact.m_point0 = v11;
			oContact.m_point1 = v12;
			oContactList.push_back( oContact);

			return true;
		}

		supportCount++;
		Vector3 v21 = TransformSupportVert( shape1, q1, t1, -n);
		Vector3 v22 = TransformSupportVert( shape2, q2, t2,  n);
		Vector3 v2  = v22 - v21; //v2.Normalize();
		if( Vector3Dot( v2, n)<=0.f)
		{
			oContact.m_normal = n;

			return false;
		}

		// Determine whether origin is on + or - side of plane ( v1, v0, v2)
		n = Vector3Cross( v1-v0, v2-v0);
		float dist = Vector3Dot( n, v0);

		A_ASSERT( n != Vector3::Zero);

		// If the origin is on the - side of the plane, reverse the direction of the plane
		if( dist>0.f)
		{
			std::swap(  v1,  v2);
			std::swap( v11, v21);
			std::swap( v12, v22);
			n = -n;
		}

		// Phase one : Identify a portal
		while( true)
		{
			// Obtain the support point in a direction perpendicular to the existing plane
			// Note: This point is guaranteed to lie off the plane
			supportCount++;
			Vector3 v31 = TransformSupportVert( shape1, q1, t1, -n);
			Vector3 v32 = TransformSupportVert( shape2, q2, t2, n);
			Vector3 v3  = v32 - v31; //v3.Normalize();
			if( Vector3Dot( v3, n)<=0.f)
			{
				oContact.m_normal = n;

				return false;
			}

			// If origin is outside( v1, v0, v3), then eliminate v2 and loop
			if( Vector3Dot( Vector3Cross( v1, v3), v0) < 0.f)
			{
				v2  = v3;
				v21 = v31;
				v22 = v32;
				n   = Vector3Cross( v1-v0, v3-v0);

				continue;
			}

			// If origin is outside( v3, v0, v2), then eliminate v1 and loop
			if( Vector3Dot( Vector3Cross( v3, v2), v0)<0.f)
			{
				v1  = v3;
				v11 = v31;
				v12 = v32;
				n   = Vector3Cross( v3-v0, v2-v0);

				continue;
			}

			bool hit = false;

			///
			// Phase two : Refine the portal
			INT phase2 = 0;

			// We are now inside of a wedge
			while( true)
			{
				phase2++;
				if( phase2>1)
				{
					static bool doneIt = false;
					if( !gTrackingOn && !doneIt)
					{
						doneIt = true;
						gTrackingOn = true;					
						ContactTest( shape1, q1, t1, shape2, q2, t2, oContactList);
						gTrackingOn = false;

						return false;
					}
				}

				// Compute normal of the wedge face
				n = Vector3Cross( v2-v1, v3-v1);

				// Can this happen??? Can it be handled more cleanly?
				if( n==Vector3::Zero)
				{
					A_ASSERT( FALSE);
					return true;
				}

				n.Normalize();

				// Compute distance from origin to wedge face
				float d = Vector3Dot( n, v1);

				// If the origin is inside the wedge, we have a hit
				if( d>=0 && !hit)
				{
					oContact.m_normal = n;

					// Compute the barycentric coordinates of the origin
					float b0 = Vector3Dot( Vector3Cross( v1, v2), v3);
					float b1 = Vector3Dot( Vector3Cross( v3, v2), v0);
					float b2 = Vector3Dot( Vector3Cross( v0, v1), v3);
					float b3 = Vector3Dot( Vector3Cross( v2, v1), v0);

					float sum = b0 + b1 + b2 + b3;
					if (sum <= 0)
					{
						b0 = 0.f;
						b1 = Vector3Dot( Vector3Cross( v2, v3), n);
						b2 = Vector3Dot( Vector3Cross( v3, v1), n);
						b3 = Vector3Dot( Vector3Cross( v1, v2), n);

						sum = b1 + b2 + b3;
					}

					float inv = 1.0f / sum;

					oContact.m_point0 = ( b0 * v01 + b1 * v11 + b2 * v21 + b3 * v31) * inv;
					oContact.m_point1 = ( b0 * v02 + b1 * v12 + b2 * v22 + b3 * v32) * inv;		

					// HIT!!!
					hit = true;
				}

				// Find the support point in the direction of the wedge face
				supportCount++;
				Vector3 v41 = TransformSupportVert( shape1, q1, t1, -n);
				Vector3 v42 = TransformSupportVert( shape2, q2, t2,  n);
				Vector3 v4  = v42 - v41; //v4.Normalize();

				float delta      =  Vector3Dot( v4-v3, n);
				float separation = -Vector3Dot( v4, n);

				// If the boundary is thin enough or the origin is outside the support pane for the newly discovered vertex, then
				// we can terminate
				if( delta<=kCollideEpsilon || separation>=0.f || phase2>300)
				{
					oContact.m_normal = n;

					// MISS!!! (We didn't move closer)
					static INT   maxPhase2    = 0;
					static INT   maxCallCount = 0;
					static float avg		  = 0.f;
					static INT   hitCount	  = 0;
					static float avgSupportCount = 0.f;

					if( hit)
					{
						hitCount++;

						avg = (avg * (hitCount-1) + phase2) / hitCount;
						avgSupportCount = (avgSupportCount * (hitCount-1) + supportCount) / hitCount;
						gAvgSupportCount = avgSupportCount;

						if (phase2 > maxPhase2)
						{
							maxPhase2 = phase2;
							maxCallCount = callCount;
						}
					}

					oContactList.push_back( oContact);

					return hit;
				}

				// Compute the tetrahedron dividing face (v4,v0,v1)
				float d1 = Vector3Dot( Vector3Cross( v4, v1), v0);

				// Compute the tetrahedron dividing face (v4,v0,v2)
				float d2 = Vector3Dot( Vector3Cross( v4, v2), v0);

				// Compute the tetrahedron dividing face (v4,v0,v3)
				float d3 = Vector3Dot( Vector3Cross( v4, v3), v2);

				if( d1<0.f)
				{
					if( d2<0.f)
					{
						// Inside d1 & inside d2 ==> eliminate v1
						v1  = v4;
						v11 = v41;
						v12 = v42;
					}
					else
					{
						// Inside d1 & outside d2 ==> eliminate v3
						v3  = v4;
						v31 = v41;
						v32 = v42;
					}
				}
				else
				{
					if( d3<0.f)
					{
						// Outside d1 & inside d3 ==> eliminate v2
						v2  = v4;
						v21 = v41;
						v22 = v42;
					}
					else
					{
						// Outside d1 & outside d3 ==> eliminate v1
						v1  = v4;
						v11 = v41;
						v12 = v42;
					}
				}
			}
		}

		return false;
	}

	// 相交测试
	bool CollideSegment3Rect3::IntersectionTest( const Shape* shape1, const Shape* shape2)
	{
		const Segment3* segm = dynamic_cast<const Segment3*>( shape1);
		const Rect3*    rect = dynamic_cast<const Rect3*>( shape2);

		Vector3 invDirection;
		int		sign[3];

		invDirection.x = 1 / segm->m_dir.x;
		invDirection.y = 1 / segm->m_dir.y;
		invDirection.z = 1 / segm->m_dir.z;
		sign[0] = invDirection.x < 0;
		sign[1] = invDirection.y < 0;
		sign[2] = invDirection.z < 0;

		// 临时变量
		float tmin, tmax, tymin, tymax, tzmin, tzmax;
        
        Vector3 params[2];
        params[0] = rect->m_min;
        params[1] = rect->m_max;
		tmin  = ( params[  sign[0]].x - segm->m_orig.x) * invDirection.x;
		tmax  = ( params[1-sign[0]].x - segm->m_orig.x) * invDirection.x;
		tymin = ( params[  sign[1]].y - segm->m_orig.y) * invDirection.y;
		tymax = ( params[1-sign[1]].y - segm->m_orig.y) * invDirection.y;

		// 一些判断 ( 很悲哀的说,这算法完全看不懂)
		if ( tmin > tymax || tymin > tmax)
			return false;

		if ( tymin > tmin)
			tmin = tymin;

		if ( tymax < tmax)
			tmax = tymax;

		tzmin = ( params[  sign[2]].z - segm->m_orig.z) * invDirection.z;
		tzmax = ( params[1-sign[2]].z - segm->m_orig.z) * invDirection.z;

		// 判断
		if ( tmin > tzmax || tzmin > tmax)
			return false;

		if ( tzmin > tmin)
			tmin = tzmin;

		if ( tzmax < tmax)
			tmax = tzmax;

		return ( tmin<segm->m_extent && tmax>-segm->m_extent);
	}

	// 射线与AABB包围盒碰撞  
	bool CollideSegment3Rect3::IntersectionTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2)
	{
		// not supported
		A_ASSERT( FALSE);

		return false;
	}

	// 接触测试
	bool CollideSegment3Rect3::ContactTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2, CollideContactList& oContactList)
	{
		// not supported
		A_ASSERT( FALSE);

		return false;
	}

	// construction
	bool CollideSegment3KdtTriangleMesh::IntersectionTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2)
	{
		return false;
	}

	// 接触测试
	bool CollideSegment3KdtTriangleMesh::ContactTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2, CollideContactList& oContactList)
	{
		bool  isContact				 = false;
		const Segment3*		   segm  = dynamic_cast<const Segment3*>( shape1);
		const KdtTriangleMesh* kdtTM = dynamic_cast<const KdtTriangleMesh*>( shape2);

		Vector3 rayOrig = segm->m_orig - segm->m_dir * segm->m_extent;
		Vector3 rayDir	= segm->m_dir;
		float	dist	= segm->m_extent * 2.f;	

		CollideContact oContact;
		if( kdtTM->RayTrace( rayOrig, rayDir, dist, oContact.m_normal))
		{
			oContact.m_point0 = rayOrig + rayDir * dist;
			oContact.m_point1 = oContact.m_point0;

			oContactList.push_back( oContact);

			isContact = true;
		}

		return isContact;
		/*

		// 与其中的三角形进行判断
		vector<Rect3> aabbArrays;
		segm->BuildAABBArray( aabbArrays, Transform());
		for( size_t i=0; i<aabbArrays.size(); i++)
		{
			//IntrRect3Rect3 intrRR( aabbArrays[i], kdtTM->GetBoundingBox());
			//intrRR.Test();
			//aabbArrays[i] = intrRR.m_intrRect;

			// 得到应记录的所有网格
			set<int> tTriIds; tTriIds.clear();
			kdtTM->GetTriIdxs( tTriIds, aabbArrays[i]);

			CollideContact oContact;
			for ( set<int>::const_iterator it = tTriIds.begin(); it!=tTriIds.end(); ++it)
			{
				// 射线与三角形碰撞
				Triangle3 triangle = kdtTM->GetTriangle( *it);

				IntrSegment3Triangle3 intrST( *segm, triangle);
				if( intrST.Test())
				{
					oContact.m_normal = triangle.GetNormal();
					oContact.m_point0 = intrST.m_intrPoint;
					oContact.m_point1 = intrST.m_intrPoint;
					oContactList.push_back( oContact);

					isContact = true;
				}
			}
		}	

		return isContact;*/
	}

	// 相交检测
	bool CollideSegment3HeightField::IntersectionTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2)
	{
		// not supported
		A_ASSERT( FALSE);

		return false;
	}

	// 接触测试
	bool CollideSegment3HeightField::ContactTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2, CollideContactList& oContactList)
	{
		bool  isContact			 = false;
		const Segment3*	   segm  = dynamic_cast<const Segment3*>( shape1);
		const HeightField* hf    = dynamic_cast<const HeightField*>( shape2);

		// 与其中的三角形进行判断
		vector<Rect3> aabbArrays;
		segm->BuildAABBArray( aabbArrays, Transform());
		for( size_t i=0; i<aabbArrays.size(); i++)
		{
			// 得到区域中三角形
			CollideContact oContact;
			vector<Triangle3> triangles;
			hf->GetTriangle( triangles, aabbArrays[i]);
			for ( size_t j=0; j<triangles.size(); j++)
			{
				IntrSegment3Triangle3 intrST( *segm, triangles[j]);
				if( intrST.Test())
				{
					oContact.m_normal = Vector3::Zero;
					oContact.m_point0 = intrST.m_intrPoint;
					oContact.m_point1 = intrST.m_intrPoint;
					oContactList.push_back( oContact);

					isContact = true;
				}
			}
		}	

		return isContact;
	}

	// 相交测试
	bool CollideBox3Rect3::IntersectionTest( const Shape* shape1, const Shape* shape2)
	{
		const Rect3& a = *dynamic_cast<const Rect3*>( shape1);
		const Box3&  b = *dynamic_cast<const Box3*>( shape2);

		return b.IsAABBOverlap( a);
	}

	// 相交检测
	bool CollideBox3Rect3::IntersectionTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2)
	{
		A_ASSERT( FALSE);

		return false;
	}

	// 接触测试
	bool CollideBox3Rect3::ContactTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2, CollideContactList& oContactList)
	{
		A_ASSERT( FALSE);

		return false;
	}


	// 相交测试
	bool CollideRect3Rect3::IntersectionTest( const Shape* shape1, const Shape* shape2)
	{
		const Rect3& a = *dynamic_cast<const Rect3*>( shape1);
		const Rect3& b = *dynamic_cast<const Rect3*>( shape2);

		return ( a.m_min.x<=b.m_max.x && a.m_max.x>=b.m_min.x && a.m_min.y<=b.m_max.y && a.m_max.y>=b.m_min.y && a.m_min.z<=b.m_max.z && a.m_max.z>=b.m_min.z);
	}

	// 相交检测
	bool CollideRect3Rect3::IntersectionTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2)
	{
		A_ASSERT( FALSE);

		return false;
	}

	// 接触测试
	bool CollideRect3Rect3::ContactTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2, CollideContactList& oContactList)
	{
		A_ASSERT( FALSE);

		return false;
	}

	// 相交测试
	bool CollideFrustum3Rect3::IntersectionTest( const Shape* shape1, const Shape* shape2)
	{
		const Rect3&	a = *dynamic_cast<const Rect3*>( shape1);
		const Frustum3& b = *dynamic_cast<const Frustum3*>( shape2);

		return b.IsAABBIn( a.m_min, a.m_max);
	}

	// 相交检测
	bool CollideFrustum3Rect3::IntersectionTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2)
	{
		A_ASSERT( FALSE);

		return false;
	}

	// 接触测试
	bool CollideFrustum3Rect3::ContactTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2, CollideContactList& oContactList)
	{
		A_ASSERT( FALSE);

		return false;
	}

	// 相交检测
	bool CollideCapsule3Capsule3::IntersectionTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2)
	{
		Capsule3 capsule1 = *(dynamic_cast<const Capsule3*>( shape1));
		Capsule3 capsule2 = *(dynamic_cast<const Capsule3*>( shape2));
		capsule1.DoTransform( Transform( q1, t1));
		capsule2.DoTransform( Transform( q2, t2));

		IntrCapsule3Capsule3 intrCC( capsule1, capsule2);

		return intrCC.Test();
	}

	// 接触测试
	bool CollideCapsule3Capsule3::ContactTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2, CollideContactList& oContactList)
	{
		Capsule3 capsule1 = *(dynamic_cast<const Capsule3*>( shape1));
		Capsule3 capsule2 = *(dynamic_cast<const Capsule3*>( shape2));
		capsule1.DoTransform( Transform( q1, t1));
		capsule2.DoTransform( Transform( q2, t2));

		DistSegment3Segment3 distSS( capsule1.m_segment, capsule2.m_segment);

		CollideContact oContact;
		float distance = distSS.Get();
		float sum	   = capsule1.m_radius + capsule2.m_radius;
		if( distance <= sum)
		{
			oContact.m_normal = distSS.m_closePointSegment0 - distSS.m_closePointSegment1;
			oContact.m_normal.Normalize();
			oContact.m_point0 = distSS.m_closePointSegment0 - oContact.m_normal * capsule1.m_radius;
			oContact.m_point1 = distSS.m_closePointSegment1 + oContact.m_normal * capsule2.m_radius;
			oContactList.push_back( oContact);

			return true;
		}

		return false;
	}

	// 相交检测
	bool CollideCompountCompount::IntersectionTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2)
	{
		A_ASSERT( FALSE);

		return false;
	}

	// 接触测试
	bool CollideCompountCompount::ContactTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2, CollideContactList& oContactList)
	{
		vector<const Shape*>shapes1;
		vector<Transform>	transforms1;
		vector<const Shape*>shapes2;
		vector<Transform>	transforms2;

		if( shape1->GetShapeType()==ST_Compount)
		{
			const CompoundShape* cs1 = dynamic_cast<const CompoundShape*>( shape1);
			for( int i=0; i<cs1->GetNumChildShapes(); i++)
			{
				shapes1.push_back( cs1->GetChildShape( i));
				transforms1.push_back( Transform( q1, t1) * cs1->GetChildTransform( i));
			}
		}
		else
		{
			shapes1.push_back( shape1);
			transforms1.push_back( Transform( q1, t1));
		}

		if( shape2->GetShapeType()==ST_Compount)
		{
			const CompoundShape* cs2 = dynamic_cast<const CompoundShape*>( shape2);
			for( int i=0; i<cs2->GetNumChildShapes(); i++)
			{
				shapes2.push_back( cs2->GetChildShape( i));
				transforms2.push_back( Transform( q2, t2) * (cs2->GetChildTransform( i)));
			}
		}
		else
		{
			shapes2.push_back( shape2);
			transforms2.push_back( Transform( q2, t2));
		}

		bool result = false;;
		for( size_t i=0; i<shapes1.size(); i++)
		{
			for( size_t j=0; j<shapes1.size(); j++)
			{
				result |= Collide::ContactTest( shapes1[i], transforms1[i].m_rotation, transforms1[i].m_origin, shapes2[j], transforms2[j].m_rotation, transforms2[j].m_origin, oContactList, false);
			}
		}

		return result;
	}
}