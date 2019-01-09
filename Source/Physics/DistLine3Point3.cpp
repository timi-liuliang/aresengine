#include <Physics/DistLine3Point3.h>

namespace Ares
{
	// static distance queries
	float DistLine3Point3::Get()
	{
		return sqrtf( GetSquared());
	}

	// get distance squared
	float DistLine3Point3::GetSquared()
	{
		// Õ∂”∞‘ŸÀ„æ‡¿Î
		Vector3 diff	  = m_point - m_line.m_orig;
		m_lineParameter	  = Vector3Dot( m_line.m_dir, diff);

		m_closePointPoint = m_point;
		m_closePointLine  = m_line.m_orig + m_lineParameter * m_line.m_dir;

		diff			  = m_closePointPoint - m_closePointLine;

		return diff.LengthSq();
	}
}