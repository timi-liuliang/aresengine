#include "Physics/Rectangle3.h"

namespace Ares
{
	// construction (uninitialized)
	Rectangle3::Rectangle3()
	{

	}

	// initialized
	Rectangle3::Rectangle3( const Vector3& center, const Vector3& axis0, const Vector3& axis1, float extent0, float extent1)
	{
		m_center = center;
		
		m_axis[0] = axis0;
		m_axis[1] = axis1;

		m_extent[0] = extent0;
		m_extent[1] = extent1;
	}
}