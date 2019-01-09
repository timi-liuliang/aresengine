#include <Physics/AresCSG.h>

namespace Ares
{
	// 求交
	void CSGBox3Triangle3::Intersection(vector<Vector3>& vertexs, vector<WORD>& indices, const Box3& box3, const Triangle3& triangle)
	{
		vertexs.clear();
		indices.clear();

		IntrBox3Triangle3 intrBT( box3, triangle);
		if( intrBT.Test())
		{
			intrBT.Find();
			for( int i=0; i<intrBT.m_quantity; i++)
				vertexs.push_back( intrBT.m_akPoint[i]);
			
			for ( int i=2; i<intrBT.m_quantity; i++)
			{
				indices.push_back( 0);
				indices.push_back( i-1);
				indices.push_back( i);
			}
		}
	}

	// 求交
	void CSGAABB3Frustum3::Intersection()
	{

	}
}