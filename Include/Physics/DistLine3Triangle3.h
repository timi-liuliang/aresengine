#pragma once

#include <Physics/Shapes.h>

namespace Ares
{
	//---------------------------------------
	// calcute the distance between triangle
	// and line
	// <<3D游戏引擎设计.第二版>> page 651
	//				--2011-04-27		帝林 
	struct DistLine3Triangle3
	{
		const Line3&		m_line;
		const Triangle3&	m_triangle;
		
		Vector3			m_closePointLine;
		Vector3			m_closePointTriangle;

		// information about the closet points
		float				m_lineParam;			// closestLine	   = line origin+param*line.dir
		float				m_triangleBary[3];		// closestTriangle = sum{i=0}^2 bary[i]*tri.vert[i]


		// construction
		DistLine3Triangle3( const Line3& line, const Triangle3& triangle);

		// static distance queries
		float GetSuared();
	};
}