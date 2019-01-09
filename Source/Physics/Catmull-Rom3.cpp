#include "Physics/Catmull-Rom3.h"

namespace Ares
{
	namespace Curves
	{
		// 说明：
		//     Catmull-Rom样条可以使空间中的点序列变得平滑,它产生了一条
		// 能够经过原始路径中所有控制点的曲线(不像Bezier曲线,虽然比较平
		// 滑但是不能经过所有的控制点)。
		Vector3 CatmullRom3( Vector3& out, const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float amount)
		{
			// amount
			float u3 = amount * amount * amount;
			float u2 = amount * amount;
			float u1 = amount;

			out = v1 * ( -0.5f * u3 +         u2 - 0.5f * u1) +
				  v2 * (  1.5f * u3 + -2.5f * u2 + 1.f)		  +
				  v3 * ( -1.5f * u3 +   2.f * u2 + 0.5f * u1) +
				  v4 * (  0.5f * u3 -  0.5f * u2);

			return out;
		}

		// 求CatmullRom3 切线
		Vector3 CatmullRom3Tangent( Vector3& out, const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float amount)
		{
			// amount
			float s  = amount;
			float s2 = amount * amount;

			float b1 = -1.5f * s2 + 2.f * s - 0.5f;
			float b2 =  4.5f * s2 - 5.f * s;
			float b3 = -4.5f * s2 + 4.f * s + 0.5f;
			float b4 =  1.5f * s2 - s;

			out = v1 * b1 +
				v2 * b2 +
				v3 * b3 +
				v4 * b4;

			return out;
		}
	}
}