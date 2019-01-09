#pragma once

#include <Core/AresMath.h>

namespace Ares
{
	namespace Curves
	{
		//---------------------------------------------------------------
		// Bezier3 2011-07-12  帝林
		// 主要参考：<<DirectX 实时渲染技术详解>> 第21章节 贝赛尔面片
		//			 伯恩斯坦多项式 ( Bernstein polynomials)
		// 
		// 说明：
		//	由一组控制点来表述的曲线(此处我暂只给出四个控制点的实现)
		// out 返回点
		// v1,v2,v3,v4 控制点
		// amount      控制量
		Vector3 Bezier3( Vector3& out, const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float amount);

		// 求Bezier3中的切线
		Vector3 Beizer3Tangent( Vector3& out, const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float amount);
	}
}