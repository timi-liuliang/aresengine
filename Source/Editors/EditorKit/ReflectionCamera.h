#pragma once

#include "BaseCamera.h"

namespace Ares
{
	//--------------------------------
	// 镜像摄像机   2010-10-12  帝林
	//--------------------------------
	class CReflectionCamera : public Camera
	{
	public:
		// 获取观察投影矩阵
		virtual Matrix44&  GetViewProj();

		// 获取摄像机天空盒矩阵
		virtual Matrix44&  GetSkyViewProj();
	};
}