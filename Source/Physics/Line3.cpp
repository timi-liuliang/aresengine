#include "Physics/Line3.h"

namespace Ares
{
	// unitialized construction
	Line3::Line3()
	{
		// do nothing
	}

	// construction
	Line3::Line3(const Vector3& orig, const Vector3& dir)
		: m_orig( orig)
		, m_dir( dir)
	{
		m_dir.Normalize();
	}

	// ¹¹½¨AABB°üÎ§ºÐ
	void Line3::BuildAABB( Rect3& box, const Transform& trans) const
	{
		box.m_min = Vector3( -FLT_MAX,  -FLT_MAX,  -FLT_MAX);
		box.m_max = Vector3(  FLT_MAX,   FLT_MAX,   FLT_MAX);
	}
}