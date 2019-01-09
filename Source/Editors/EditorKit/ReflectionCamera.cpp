#include "ReflectionCamera.h"

namespace Ares
{
	// 获取观察投影矩阵
	Matrix44&  CReflectionCamera::GetViewProj()
	{
		// 因为是镜射, 在转换到镜头坐标系后要做个左右对调的动作.
		Matrix44 matMirror;
		Matrix44Scaling( matMirror, -1.f, 1.f, 1.f);
	
		m_matViewProj = m_matView * matMirror * m_matProj;

		return m_matViewProj;
	}

	// 获取摄像机天空盒矩阵
	Matrix44&  CReflectionCamera::GetSkyViewProj()
	{
		static Matrix44 mirrorSkyBox;
		Matrix44Scaling( mirrorSkyBox, -1.f, 1.f, 1.f);

		// 镜像天空盒矩阵
		mirrorSkyBox = m_matSkyView * mirrorSkyBox * m_matUnitProj;

		return mirrorSkyBox;
	}
}