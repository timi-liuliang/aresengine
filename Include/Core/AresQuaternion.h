#pragma once

#include "AresMath.h"

namespace Ares
{
	//------------------------------------
	// 四元数 2011-11-1 <摘自ogre> 帝林
	//------------------------------------
	class Quaternion
	{
	public:
		// 构造函数
		Quaternion( const D3DXMATRIX& rot);

		// 转换成矩阵
		void  ToRotationMatrix( D3DXMATRIX& rot);

	public:
		  float		x, y, z, w;		// 数值
	};
}