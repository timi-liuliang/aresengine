#include "Physics/Bezier3.h"

namespace Ares
{
	namespace Curves
	{
		//---------------------------------------------------------------
		// Bezier3 2011-07-12  帝林
		// 主要参考：<<DirectX 实时渲染技术详解>> 第21章节 贝赛尔面片
		// 
		// 说明：
		//	由一组控制点来表述的曲线(此处我暂只给出四个控制点的实现)
		// out 返回点
		// v1,v2,v3,v4 控制点
		// amount      控制量
		Vector3 Bezier3( Vector3& out, const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float amount)
		{
			// amount
			float  s  = amount;
			float _s = 1.f - amount;

			out = v1 *   _s * _s * _s +
				v2 *  3.f *  s * _s * _s +
				v3 *  3.f *  s *  s * _s +
				v4 *    s *  s *  s;

			return out;
		}

		// 求Bezier3中的切线( 求导)
		Vector3 Beizer3Tangent( Vector3& out, const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float amount)
		{
			// amount
			float s  = amount;
			float _s = 1.f - amount;
			float s2 = amount * amount;

			float b1 = -3.f * _s   * _s;
			float b2 =  3.f - 12.f * s   + 9.f * s2;
			float b3 =  6.f * s    - 9.f * s2;
			float b4 =  3.f * s2;

			out = v1 * b1 +
				v2 * b2 +
				v3 * b3 +
				v4 * b4;

			return out;
		}
	}
}