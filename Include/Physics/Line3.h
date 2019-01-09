#pragma once

#include <Core/AresMath.h>
#include "Physics/Rect3.h"

namespace Ares
{
	//----------------------------------------------------------------
	// Line3 2011-04-27   
	// the line is represented as p+t*d where p is the line origin and
	// d is a unit-length direction vector, the user must ensure that 
	// the direction vector satisfies this condition
	struct Line3
	{
		Vector3		m_orig;		// 起点
		Vector3		m_dir;		// 朝向

		// construction
		Line3();

		// initialized
		Line3( const Vector3& orig, const Vector3& dir);

		// 构建AABB包围盒
		virtual void BuildAABB( Rect3& box, const Transform& trans) const;
	};
}