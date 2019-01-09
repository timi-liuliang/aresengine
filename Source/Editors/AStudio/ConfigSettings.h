#pragma once

namespace Ares
{
	// 摄像机配置结构体
	struct CameraSettings
	{
		float m_farPlane;		// 远平面
		float m_speed;			// 移动速度

		// 构造函数
		CameraSettings()
			: m_farPlane( 180.f)
			, m_speed( 3.f)
		{}
	};
}