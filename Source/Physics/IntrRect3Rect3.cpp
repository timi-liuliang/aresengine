#include <Physics/IntrRect3Rect3.h>

namespace Ares
{
	// construction
	IntrRect3Rect3::IntrRect3Rect3( const Rect3& rect0, const Rect3& rect1)
		: m_rect0( rect0),
		  m_rect1( rect1)
	{
	}

	// test intersection query
	bool IntrRect3Rect3::Test()
	{
		m_intrRect.m_min.x = std::max<float>( m_rect0.m_min.x, m_rect1.m_min.x);
		m_intrRect.m_min.y = std::max<float>( m_rect0.m_min.y, m_rect1.m_min.y);
		m_intrRect.m_min.z = std::max<float>( m_rect0.m_min.z, m_rect1.m_min.z);

		m_intrRect.m_max.x = std::min<float>( m_rect0.m_max.x, m_rect1.m_max.x);
		m_intrRect.m_max.y = std::min<float>( m_rect0.m_max.y, m_rect1.m_max.y);
		m_intrRect.m_max.z = std::min<float>( m_rect0.m_max.z, m_rect1.m_max.z);

		return ((m_intrRect.m_min.x <= m_intrRect.m_max.x) && (m_intrRect.m_min.y <= m_intrRect.m_max.y) && (m_intrRect.m_min.z <= m_intrRect.m_max.z)) ? true : false;
	}
}