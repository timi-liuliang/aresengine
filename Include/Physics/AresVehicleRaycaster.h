#pragma once

#include <Core/AresMath.h>

namespace Ares
{
	//---------------------------------------
	// VehicleRaycaster 2013-9-6 帝林
	//---------------------------------------
	struct VehicleRaycaster
	{
		struct Result
		{
			Vector3		m_hitPointInWorld;
			Vector3		m_hitNormalInWorld;
			float		m_distFraction;

			// 析构函数
			Result::Result() : m_distFraction( -1.f)
			{}
		};

		// 射线检测
		virtual void* RayCast( const Vector3& from, const Vector3& to, Result& result)=0;
	};
}