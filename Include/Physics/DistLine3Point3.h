#pragma once

#include "Physics/Shapes.h"

namespace Ares
{
	//----------------------------------
	// Line3Point3 2012-8-1 ╣шаж
	//----------------------------------
	struct DistLine3Point3
	{
		const Line3&	m_line;
		const Vector3&	m_point;

		Vector3			m_closePointLine;
		Vector3			m_closePointPoint;

		float			m_lineParameter;	// m_closePointLine = m_line.m_orig + m_line.m_dir * m_lineParameter	

		// constructor
		DistLine3Point3( const Line3& line, const Vector3& point)
			: m_line(line), m_point(point)
		{}

		// static distance queries
		float Get();

		// get distance squared
		float GetSquared();
	};
}