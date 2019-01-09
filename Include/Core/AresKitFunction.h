#pragma once

#include <string>
#include <Core/AresPlatform.h>

using namespace std;

namespace Ares
{
	#define ARES_DEFAULT   ((UINT) -1)

	//---------------------------------------------------------------
	//	2010-02-26 15:53 -- 未知              --帝林 
	//---------------------------------------------------------------

	// 生成0-1之间的随机数
	inline float RandZeroToOne() 
	{
		return float( double(rand()) / double(RAND_MAX));
	}

	// 生成-1 到 1 之间的随机数
	inline float RandMinusOneToOne()
	{
		return float( double( rand()) / double(RAND_MAX) * 2.0f - 1.0f );
	}

	// 强转
	inline unsigned long FtoDW(float f) 
	{ 
		return (unsigned long&)f; 
	}

	// 范围之间的随机数
	inline float Random(float f1, float f2)	
	{ 
		if(f1 == f2) 
		{
			return f1; 
		}

		return f1 + RandZeroToOne() * (f2 - f1); 
	}

	// clamp
	template<class T>
	inline T Clamp(T v, T low, T high) 
	{ 
		if( low > v) return low; else if (high < v) return high;   return v;
	}


	//----------------------------------------------------------
	// QuickSort() 快速排序
	//----------------------------------------------------------
	template<class T, class FUNC>
	inline void QuickSort(T* array, int members, const FUNC& pred)
	{
		int h = 1;

		// find the largest h value possible
		while( (h*3+1) < members)
		{
			h = 3 * h + 1;
		}

		// while h remains larger than 0
		while( h > 0)
		{
			// for each set of elements (there are h sets)
			for(int i=h-1; i<members; i++)
			{
				T B = array[i];
				int j =i;

				// compare the element at B to the one before it in the set if they are
				// out of order continue this loop, moving elements "back" to make room for B
				// to be inserted
				for(j=i; (j>=h)&&pred(B, array[j-h]); j-=h)
				{
					array[j] = array[j-h];
				}

				// insert B into the correct place
				array[j] = B;
			}

			// all sets h-sorted, now decrease set size
			h = h/3;
		}
	}
}
