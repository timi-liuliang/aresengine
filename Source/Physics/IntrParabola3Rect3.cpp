#include <Physics/IntrParabola3Rect3.h>
#include <Physics/IntrParabola3Plane3.h>

namespace Ares
{
	// construction
	IntrParabola3Rect3::IntrParabola3Rect3( const Parabola3& curve, const Rect3& rect3)
		: m_curve( curve)
		, m_rect( rect3)
	{

	}

	// test intersection query
	bool IntrParabola3Rect3::Test()
	{
		// 对Rect3的六个边分别进行检测
		Plane3 planes[6] = 
		{
			Plane3( m_rect.m_min, Vector3::XAxis),
			Plane3( m_rect.m_min, Vector3::YAxis),
			Plane3( m_rect.m_min, Vector3::ZAxis),
			Plane3( m_rect.m_max, Vector3::XAxis),
			Plane3( m_rect.m_max, Vector3::YAxis),
			Plane3( m_rect.m_max, Vector3::ZAxis)
		};

		for( int i=0; i<6; i++)
		{
			IntrParabola3Plane3 intrPP( m_curve, planes[i]);
			if( intrPP.Test())
			{
				for( UINT j=0; j<intrPP.m_numIntrPoint; j++)
				{
					if( m_rect.IsPointIn( intrPP.m_intrPoints[j]))
						return true;
				}
			}
		}

		return false;
	}
}