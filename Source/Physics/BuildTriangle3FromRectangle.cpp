#include <Physics/BuildTriangle3FromRectangle.h>

namespace Ares
{
	namespace Build
	{		
		// 通过矩形构建TriangleMesh 2011-10-21  帝林
		size_t BuildTriangle3FromRectangle( std::vector<Vector3>& out, const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& v3, float maxEdgeLength)
		{
			// 初始返回值
			size_t tTriNum = 0;

			if( maxEdgeLength < 1e-4f)
			{
				out.push_back( v0);
				out.push_back( v1);
				out.push_back( v2);
				out.push_back( v0);
				out.push_back( v2);
				out.push_back( v3);

				tTriNum += 2;
			}
			else
			{
				Vector3 vWidth  = v1 - v0;
				Vector3 vHeight = v3 - v0;

				int	  tNumWidthParts = (int)( vWidth.Length() / maxEdgeLength) + 1;
				int   tNumHeightParts= (int)( vHeight.Length()/ maxEdgeLength) + 1;

				Vector3 stepWidth = vWidth / (float)tNumWidthParts;
				Vector3 stepHeight= vHeight/ (float)tNumHeightParts;


				Vector3 tV0, tV1, tV2, tV3;
				for ( int i=0; i<tNumWidthParts; i++)
				{
					for ( int j=0; j<tNumHeightParts; j++)
					{
						tV0 = v0  + (float)i*stepWidth + (float)j*stepHeight;
						tV1 = tV0 + stepWidth;
						tV2 = tV1 + stepHeight;
						tV3 = tV0 + stepHeight;

						out.push_back( tV0);
						out.push_back( tV1);
						out.push_back( tV2);
						out.push_back( tV0);
						out.push_back( tV2);
						out.push_back( tV3);

						tTriNum += 2;
					}
				}
			}

			return tTriNum; 
		}
	}
}