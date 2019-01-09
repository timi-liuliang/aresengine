#include <Physics/ConvexHull2D.h>

namespace Ares
{
	// 构造函数
	ConvexHull2::ConvexHull2()
	{
	}

	// 析构函数
	ConvexHull2::~ConvexHull2()
	{
	}

	// 通过顶点集构建ConvexHull
	// http://www.cnblogs.com/Booble/archive/2011/02/28/1967179.html
	bool ConvexHull2::Build( vector<Vector2>& vertices)
	{
		if( vertices.size()<3)
			return false;

		m_vertices = vertices;

		// 1.确定一个肯定在凸包上的点
		int first = 0;
		for( int i=1; i<(int)vertices.size(); i++)
		{
			if( ( vertices[i].x<vertices[first].x) || ( ( vertices[i].x==vertices[first].x) && ( vertices[i].y<vertices[first].y)))
				first = i;
		}

		std::swap( m_vertices[0], m_vertices[first]);

		// 循环确定下一个点
		for( int curIdx=0, nextIdx=1; curIdx<(int)m_vertices.size(); )
		{
			// 求出nextIdx
			for( int i=0; i<(int)m_vertices.size(); i++)
			{
				if( i!=curIdx && i!=nextIdx)
				{
					Vector2 nextDir = m_vertices[nextIdx] - m_vertices[curIdx];
					Vector2 testDir = m_vertices[i]		  - m_vertices[curIdx];
					float   v2Cross   =  Vector2Cross( nextDir, testDir);
					if( v2Cross<0.f || ( v2Cross==0.f && testDir.Length()>nextDir.Length()))
						nextIdx = i;
				}
			}

			// Impossible
			if( curIdx==(int)m_vertices.size()-1 && nextIdx!=0)
				return false;

			if( nextIdx!=0)
			{
				curIdx++;
				swap( m_vertices[curIdx], m_vertices[nextIdx]);
				nextIdx = curIdx+1<(int)m_vertices.size() ? curIdx+1 : 0;

				// It's impossible
				if( !(nextIdx<(int)m_vertices.size()))
					return false;
			}
			else
			{
				m_vertices.erase( m_vertices.begin()+curIdx+1, m_vertices.end());
				
				return true;
			}
		}

		return false;
	}

	// 是否包含某点(以后改用二分查找)
	bool ConvexHull2::IsContain( const Vector2& vertex)
	{
		if( m_vertices.size() < 3)
			return false;

		for ( int j=m_vertices.size()-1, i=0; i<(int)m_vertices.size(); j=i++)
		{
			Vector2 nextDir = m_vertices[i] - m_vertices[j];
			Vector2 testDir = vertex	    - m_vertices[j];
			float   v2Cross   = Vector2Cross( nextDir, testDir);
			if( v2Cross<0.f || ( v2Cross==0.f && testDir.Length()>nextDir.Length()))
				return false;
		}

		return true;
	}
}