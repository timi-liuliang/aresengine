#include <Physics/Intersect.h>
#include "Physics/Shapes.h"

namespace Ares
{
	// 3D矩形与平面 空间位置检测
	int PlaneClassify( const Rect3& rect, const Plane3& plane)
	{
		static int EM_PLANE_FRONT = 0, EM_PLANE_BACK = 1, EM_PLANE_INTERSECT = 2;

		Vector3 minPoint, maxPoint;

		// build two points based on the direction of the plane vector. minPoint
		// and maxPoint are the two  points on the rectangle furthest away from
		// each other along the plane normal

		if (plane.normal.x > 0.0f)
		{
			minPoint.x = (float)rect.m_min.x;
			minPoint.x = (float)rect.m_max.x;
		}
		else
		{
			minPoint.x = (float)rect.m_max.x;
			maxPoint.x = (float)rect.m_min.x;
		}

		minPoint.y = (float)rect.m_min.y;
		if (plane.normal.y > 0.0f)
		{
			maxPoint.y = (float)rect.m_max.y;
		}
		else
		{
			minPoint.y = (float)rect.m_max.y;
			maxPoint.y = (float)rect.m_min.y;
		}

		if (plane.normal.z > 0.0f)
		{
			minPoint.z = (float)rect.m_min.z;
			maxPoint.z = (float)rect.m_max.z;
		}
		else
		{
			minPoint.z = (float)rect.m_max.z;
			maxPoint.z = (float)rect.m_min.z;
		}

		// compute the signed distance from 
		// the plane to both points
		float dmin = plane.DistanceTo( minPoint);
		float dmax = plane.DistanceTo( maxPoint);

		// the rectangle intersects the plane if
		// one value is positive and the other is negative
		if (dmin * dmax < 0.0f)
		{
			return EM_PLANE_INTERSECT;
		}
		else if (dmin)
		{
			return EM_PLANE_FRONT;
		}

		return EM_PLANE_BACK;
	}

	// 射线与三角形碰撞
	bool Intersect( const Line3& line, const Triangle3& triangle, float& fDist)
	{
		// Find vectors for two edges sharing vert0
		Vector3 edge1 = triangle.m_v[1] - triangle.m_v[0];
		Vector3 edge2 = triangle.m_v[2] - triangle.m_v[0];

		// Begin calculating determinant
		Vector3 pVec = Vector3Cross( line.m_dir, edge2);

		// If determinant is near zero, ray lies in plane of triangle
		float det = Vector3Dot( edge1, pVec);

		Vector3 tVec;
		if ( det > 0)
		{
			tVec = line.m_orig - triangle.m_v[0];
		}
		else
		{
			tVec = triangle.m_v[0] - line.m_orig;
			det  = - det;
		}

		if ( det < 0.0001f)
		{
			return false;
		}

		// 测试边界
		float u = Vector3Dot( tVec, pVec);
		if ( u<0.0f || u>det)
		{
			return false;
		}

		// calculate v parameter and test bounds
		Vector3 qVec = Vector3Cross( tVec, edge1);

		float v = Vector3Dot( line.m_dir, qVec);
		if ( v<0.0f || u+v>det)
		{
			return false;
		}

		// calculate t, scale parameters, ray intersects triangle
		fDist = Vector3Dot( edge2, qVec);
		float fInvDet = 1.0f / det;
		fDist *= fInvDet;
		//*u     *= fInvDet;
		//*v     *= fInvDet;

		return true;
	}

	// global function
	bool WindowPointToRay( Vector3& rayOrig, Vector3& rayDir, LONG x, LONG y, UINT d3dBackBufferWidth, UINT d3dBackBufferHeight, const Matrix44& matView, const Matrix44& matProj)
	{
		// 得到dir在观察坐标空间内的表示
		Vector3 v;
		v.x =  ( 2.0f * x / d3dBackBufferWidth -1 ) / matProj._11;
		v.y = -( 2.0f * y / d3dBackBufferHeight - 1) / matProj._22;
		v.z = 1.0f;

		// 求逆矩阵
		Matrix44 matInverse;
		Matrix44Inverse( matInverse, matView);

		// transform the screen space pick ray into 3d space
		rayDir.x = v.x * matInverse._11 + v.y * matInverse._21 + v.z * matInverse._31;
		rayDir.y = v.x * matInverse._12 + v.y * matInverse._22 + v.z * matInverse._32;
		rayDir.z = v.x * matInverse._13 + v.y * matInverse._23 + v.z * matInverse._33;
		rayOrig.x = matInverse._41;
		rayOrig.y = matInverse._42;
		rayOrig.z = matInverse._43;

		return true;
	}

	// global function
	bool WindowPointToRay( Vector3& rayOrig, Vector3& rayDir, LONG x, LONG y, UINT d3dBackBufferWidth, UINT d3dBackBufferHeight, const Matrix44& matView, const Matrix44& matProj, const Matrix44& matOffset)
	{
		// 得到dir在观察坐标空间内的表示
		Vector3 v;
		v.x =  ( 2.0f * x/d3dBackBufferWidth - 1) - matOffset._41;
		v.y = -( 2.0f * y/d3dBackBufferHeight- 1) - matOffset._42;
		v.x /= matProj._11;
		v.y /= matProj._22;
		v.z = 1.0f;

		// 求逆矩阵
		Matrix44 matInverse;
		Matrix44Inverse( matInverse, matView);

		// transform the screen space pick ray into 3d space
		rayDir.x  = v.x * matInverse._11 + v.y * matInverse._21 + v.z * matInverse._31;
		rayDir.y  = v.x * matInverse._12 + v.y * matInverse._22 + v.z * matInverse._32;
		rayDir.z  = v.x * matInverse._13 + v.y * matInverse._23 + v.z * matInverse._33;
		rayOrig.x = matInverse._41;
		rayOrig.y = matInverse._42;
		rayOrig.z = matInverse._43;

		return true;
	}

	// 屏幕点转换到3D空间
	void ScreenPointTo3DSpace( Vector3& result, LONG x, LONG y, LONG screenWidth, LONG screenHeight, const Matrix44& transform)
	{
		/*result.x =  2.f * x / screenWidth - 1;
		result.y = -2.f * y / screenHeight- 1;
		result.z = 0.f;


		D3DXVec3TransformCoord( &testHihi, &testHihi, &testM);

		result = matInverse.TransformVector3( result);*/
	}
}
