#pragma once

#include <Physics/Shapes.h>

namespace Ares
{
	//----------------------------------------------
	// distance between vector(point) and triangle 
	//				2011-04-27  ╣шаж
	struct DistVector3Triangle3
	{
		const Vector3&	m_vector;
		const Triangle3&	m_triangle;

		Vector3			m_closePointVector;
		Vector3			m_closePointTriangle;

		float			m_triangleBary[3];		// closestTriangle = sum{i=0}^2 bary[i]*tri.vert[i]
	
		// constructor
		DistVector3Triangle3( const Vector3& vector, const Triangle3& triangle);

		// static distance queries
		float Get();
		float GetSquared();
	};

}