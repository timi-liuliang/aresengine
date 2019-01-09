#include <Physics/IntrParabola3Plane3.h>
#include <Physics/DistVector3Triangle3.h>
#include <Physics/IntrParabola3Triangle3.h>

namespace Ares
{
	// construction
	IntrProjectileMotion3Triangle3::IntrProjectileMotion3Triangle3( const Parabola3& curve, const Triangle3& triangle)
		: m_curve( curve)
		, m_triangle( triangle)
		, m_numIntrPoint( 0)
	{

	}

	// test intersection query
	bool IntrProjectileMotion3Triangle3::Test()
	{
		Plane3 triPlan( m_triangle.m_v[0], m_triangle.GetNormal());
		IntrParabola3Plane3 intrPP( m_curve, triPlan);
		if( !intrPP.Test())
		{
			m_numIntrPoint = 0;

			return false;
		}

		m_numIntrPoint = 0;
		for( UINT i=0; i<intrPP.m_numIntrPoint; i++)
		{
			if( m_triangle.IsVertexIn( intrPP.m_intrPoints[i]))
			{
				m_intrPoints[m_numIntrPoint] = intrPP.m_intrPoints[i];
				m_intrTime[m_numIntrPoint]	 = intrPP.m_intrTime[i];
				m_numIntrPoint++;
			}
		}
	
		return m_numIntrPoint ? true : false;
	}
}