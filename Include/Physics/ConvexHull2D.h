#pragma once

#include <vector>
#include <Core/AresMath.h>

using namespace std;

namespace Ares
{
	//-------------------------------------------------------------
	//  2维凸包 (默认顺时针) 
	//  <<计算几何.算法与应用.第二版>> chapter 1
	//							    2011-01-21
	//-------------------------------------------------------------
	class  ConvexHull2
	{
	public:
		ConvexHull2();
		~ConvexHull2();

		// 通过顶点集构建ConvexHull
		bool Build( vector<Vector2>& vertices);

		// 是否包含某点
		bool IsContain( const Vector2& vertex);

	private:
		vector<Vector2>		m_vertices;		// 顺时针排放
	};
}