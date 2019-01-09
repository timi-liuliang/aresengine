#include <Physics/IntrParabola3Plane3.h>

namespace Ares
{
	// construction
	IntrParabola3Plane3::IntrParabola3Plane3( const Parabola3& curve, const Plane3& plane)
		: m_curve( curve)
		, m_plane( plane)
		, m_numIntrPoint( 0)
	{

	}

	// test intersection query
	bool IntrParabola3Plane3::Test()
	{
		// 根据平面公式与抛体公式解一元二次方程 ax^2+bx+c=0（a≠0）
		float a = Vector3Dot( m_plane.normal, m_curve.m_g) * 0.5f;
		float b = Vector3Dot( m_plane.normal, m_curve.m_v0);
		float c = Vector3Dot( m_plane.normal, m_curve.m_orig) - m_plane.d;

		// 1.平行
		if( a==0.f && b==0.f)
		{
			m_numIntrPoint = 0;

			return false;
		}

		// 2.一元一次方程
		if( a==0.f)
		{
			m_numIntrPoint = 1;
			m_intrTime[0]  = -c / b;
			m_curve.BuildPos( m_intrPoints[0], m_intrTime[0]);

			return true;
		}

		// 二元一次方程 Δ=b2-4ac
		float judge = b * b - 4.f * a * c;
		if( judge<0.f)
		{
			m_numIntrPoint = 0;

			return false;
		}

		if( judge > 0.f)
		{
			m_intrTime[0] = ( -b+sqrtf(judge)) / (2*a);
			m_intrTime[1] = ( -b-sqrtf(judge)) / (2*a);

			m_curve.BuildPos( m_intrPoints[0], m_intrTime[0]);
			m_curve.BuildPos( m_intrPoints[1], m_intrTime[1]);

			m_numIntrPoint = 2;

			return true;
		}

		if( judge == 0.f)
		{
			m_intrTime[0] = ( -b+sqrtf(judge)) / (2*a);
			m_curve.BuildPos( m_intrPoints[0], m_intrTime[0]);

			m_numIntrPoint = 1;

			return true;
		}

		A_ASSERT( FALSE);

		return true;
	}
}