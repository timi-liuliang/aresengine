#pragma once

namespace Ares
{
	//------------------------------------
	// Plane3 2012/1/7 帝林
	//------------------------------------
	class Plane3
	{
	public:
		// Constructors
		FORCEINLINE Plane3()
		{}

		// Constructors
		FORCEINLINE Plane3( FLOAT nx, FLOAT ny, FLOAT nz, FLOAT _d)
			: normal( nx, ny, nz), d( _d)
		{ }

		// Normal is specified, d=dot( N, P) where P is on the plane
		FORCEINLINE Plane3( const Vector3& rkP, const Vector3& rkNormal)
		{
			normal = rkNormal; normal.Normalize();
			d	   = Vector3Dot( rkNormal, rkP);
		}

		// Normalize plane (so that |normal| == 1)
		FORCEINLINE void Normalize(){ normal.Normalize(); }

		// Compute dis=Vector3Dot(n, p)-d where n is plane normal, p is a point.
		// and d is the plane constant. This is a signed distance. The sign of 
		// the return value is positive if the point is on the postive side of
		// the plane, negative if the point is on the negative side, and zero
		// if the point is on the plane
		FORCEINLINE FLOAT DistanceTo( const Vector3& p)const
		{
			return Vector3Dot( normal, p) - d;
		}

		// 通过Plane镜像
		INLINE Vector3 MirrorVector3( const Vector3& v) const
		{
			// 先计算点到平面的距离，再减去两倍的法线距离
			return v - normal * ( 2.f * DistanceTo( v));
		}

	public:
		Vector3	 normal;	// normal
		FLOAT	 d;			// constant
	};
}