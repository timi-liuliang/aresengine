#pragma once

#include "Physics/Parabola3.h"

namespace Ares
{
	//-----------------------------------------------------
	// Calculate the distance between parabola and point
	//					2013-3-16 帝林
	struct DistParabola3Point3
	{
		const Parabola3&	m_curve;
		const Vector3&		m_point;

		float				m_intrTime[2];		// 曲线使用
		Vector3				m_intrPoints[2];
		UINT				m_quantity;

		// constructor
		DistParabola3Point3( const Parabola3& curve, const Vector3& point)
			: m_curve( curve), m_point( point), m_quantity( 0)
		{}

		// static distance queries
		float Get();
	};
}