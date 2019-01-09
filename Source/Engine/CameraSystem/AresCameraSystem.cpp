#include <Engine/CameraSystem/AresCameraSystem.h>

namespace Ares
{
	// 构造函数
	CameraSystem::CameraSystem()
		: m_active( CT_Normal)
	{
		for( int type=SP_View; type<SP_Total; type++)
		{
			m_spab[type] = MakeShaderParam();
		}
	}

	// 析构函数
	CameraSystem::~CameraSystem()
	{

	}

	// 获取观察矩阵旋转的逆
	Matrix44 CameraSystem::GetViewRotationInverse()
	{
		Matrix44 rotView = GetView();
		rotView._41 = rotView._42 = rotView._43 = 0.f;

		Matrix44 invView;
		Matrix44Inverse( invView, rotView);

		return invView;
	}

	// 获取shader绑定参数
	ShaderParamPtr CameraSystem::GetSPAB( SPType type) const
	{
		return m_spab[type];
	}

	// 设置活跃摄像机
	void CameraSystem::SetActive( CameraType type)
	{
		m_active = type;
	}
}