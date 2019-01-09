#pragma once

#include <vector>
#include <Core/AresMath.h>
#include "Rect3.h"

using namespace std;

namespace Ares
{
	//-----------------------------------
	// ProjectileMotion3 2013-3-14 帝林
	//-----------------------------------
	struct Parabola3
	{
		Vector3		m_orig;			// 起点
		Vector3		m_v0;			// 方向
		Vector3		m_g;			// 重力

		// construction
		Parabola3( const Vector3& orig,  const Vector3& v0, const Vector3& g);

		// get turn point
		float GetTurnPos( Vector3& oPos); 

		// build curent pos by time
		void BuildPos( Vector3& oPos, float t) const;

		// interpolate
		void Interpolate( vector<Vector3>& oPos, float t0, float t1, size_t intepolateNum) const;

		// 构建AABB包围盒组
		void BuildAABBArray( vector<Rect3>& aabbArrays, const Transform& trans, float t0, float t1, size_t num) const;

		// 计算与点的距离
		float DistanceTo( const Vector3& p) const;

		// 执行转换
		void DoTransform( const Transform& transform);
	};
}