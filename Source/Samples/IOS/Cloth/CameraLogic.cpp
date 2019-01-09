#include <Core/AresMath.h>
#include "CameraLogic.h"

// 摄像机左右旋转绽放 上下旋转缩放 半径增加缩短缩放
#define CAMERA_ROTATIONY_SCALAR  0.1f
#define CAMERA_ROTATIONZ_SCALAR  0.3f
#define CAMERA_RADIUS_SCALAR     0.01f

namespace Ares
{
	// 构造函数
	CameraLogic::CameraLogic()
		: CameraSystem()
		, m_vPos( Vector3( 0.f, 5.f, 3.25f))
	{
		m_cameras.assign( NULL);

		m_fSpeed = 2.5f;

		m_cameras[CT_Normal] = new_ CCamera;

		CCamera* pCamera = m_cameras[CT_Normal];
		if( pCamera)
		{
			pCamera->SetPos( &m_vPos);
			pCamera->SetLookAt( Vector3::Zero);
		}

		m_vpOrthoProj = MakeShaderParam( GetOrtho());
	}

	// 析构函数
	CameraLogic::~CameraLogic()
	{
	}

	// 是否可用
	bool CameraLogic::IsValid()
	{
		return true;
	}

	// 根据输入 控制摄像机
	void CameraLogic::FrameMove(float fTime)
	{
		CCamera* pCamera = m_cameras[CT_Normal];
		if ( pCamera)
		{
			pCamera->SetPos( &m_vPos);

			pCamera->FrameMove( fTime);

			UpdateSPAB();
		}
	}

	// 更新Shader参数
	void CameraLogic::UpdateSPAB()
	{
		m_params.m_skyViewProj = m_cameras[CT_Normal]->GetSkyViewProj();

		*m_spab[SP_View]	  = GetView();
		*m_spab[SP_ViewProj]  = GetViewProj();
		*m_spab[SP_SkyViewProj] = m_cameras[CT_Normal]->GetSkyViewProj();
		*m_spab[SP_Pos]		  = GetPosition();
		*m_spab[SP_Near]	  = GetNear();
		*m_spab[SP_Far]		  = GetFar();
		*m_spab[SP_Direction] = m_cameras[CT_Normal]->GetDirection();
	}

	// 设置ProjParams
	void CameraLogic::SetProjParams(float fovW, int screenWidth, int screenHeight, float fNearPlane, float fFarPlane,  RenderDeviceType type)
	{
		CCamera* pCamera = m_cameras[CT_Normal];

		//ICamera* pCamera		   = m_pCameraManager->GetCameraByName( "Normal");
		//ICamera* pCmaeraReflection = m_pCameraManager->GetCameraByName( "Reflection");
		//ICamera* pCameraFollow	   = m_pCameraManager->GetCameraByName( "Follow");

		//pCmaeraReflection->SetProjParams( fovW/aspect, screenWidth, screenHeight, fNearPlane, fFarPlane);
		//pCameraFollow->SetProjParams( fovW/aspect, screenWidth, screenHeight, fNearPlane, fFarPlane);

		// 设置投影矩阵
		float aspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
		pCamera->SetProjParams(  fovW/aspect, screenWidth, screenHeight, fNearPlane, fFarPlane, type);
	}

	// 设置远近截面
	void  CameraLogic::SetNearFar( float fNear, float fFar)
	{
		CCamera* pCamera = m_cameras[CT_Normal];

		//ICamera* pCamera		   = m_pCameraManager->GetCameraByName( "Normal");
		//ICamera* pCmaeraReflection = m_pCameraManager->GetCameraByName( "Reflection");
		//ICamera* pCameraFollow	   = m_pCameraManager->GetCameraByName( "Follow");	

		// 设置投影矩阵
		pCamera->SetNear(  fNear);
		pCamera->SetFar(  fFar);

		// 设置投影矩阵
		//pCmaeraReflection->SetNear(  fNear);
		//pCmaeraReflection->SetFar(  fFar);

		// 设置远近截面
		//pCameraFollow->SetNear(  fNear);
		//pCameraFollow->SetFar(  fFar);
	}

	// 设置开角
	void  CameraLogic::SetFov( float fov)
	{
		CCamera* pCamera = m_cameras[CT_Normal];
		//ICamera* pCamera		   = m_pCameraManager->GetCameraByName( "Normal");
		//ICamera* pCmaeraReflection = m_pCameraManager->GetCameraByName( "Reflection");
		//ICamera* pCameraFollow	   = m_pCameraManager->GetCameraByName( "Follow");	

		// 设置投影矩阵
		pCamera->SetFov(  fov);
		//pCmaeraReflection->SetFov(  fov);
		//pCameraFollow->SetFov(  fov);
	}

	// 设置上方向
	void  CameraLogic::SetUpDir( float x, float y, float z)
	{
		CCamera* pCamera = m_cameras[CT_Normal];
		//ICamera* pCamera		   = m_pCameraManager->GetCameraByName( "Normal");
		//ICamera* pCmaeraReflection = m_pCameraManager->GetCameraByName( "Reflection");
		//ICamera* pCameraFollow	   = m_pCameraManager->GetCameraByName( "Follow");	

		// 设置投影矩阵
		pCamera->SetUpDir(  x, y, z);
		//pCmaeraReflection->SetUpDir(  x, y, z);
		//pCameraFollow->SetUpDir( x, y, z);
	}


	//-------------------------------------------
	// For Render IO
	//-------------------------------------------
	// 获取观察投影矩阵
	Matrix44& CameraLogic::GetViewProj()
	{
		if( m_active==CT_Sky)
			return m_cameras[CT_Normal]->GetSkyViewProj();

		return m_cameras[CT_Normal]->GetViewProj();
	}

	// 获取摄像机位置
	Vector3& CameraLogic::GetPosition()
	{
		return (Vector3&)m_cameras[CT_Normal]->GetPosition();
	}

	// 获取近截面
	float CameraLogic::GetNear()
	{
		return m_cameras[CT_Normal]->GetNear();
	}

	// 获取远截面
	float CameraLogic::GetFar()
	{
		return m_cameras[CT_Normal]->GetFar();
	}

	// 获取朝向
	Vector3& CameraLogic::GetFacing()
	{
		return m_cameras[CT_Normal]->GetDirection();
	}

	// 获取观察矩阵
	Matrix44& CameraLogic::GetView()
	{
		if( m_active==CT_Sky)
			return m_cameras[CT_Normal]->GetSkyView();

		return m_cameras[CT_Normal]->GetView();
	}

	// 获取投影矩阵
	Matrix44& CameraLogic::GetProj()
	{
		return m_cameras[CT_Normal]->GetProj();
	}

	// 正交矩阵
	const Matrix44& CameraLogic::GetOrtho()
	{
		static Matrix44 orthoOffset;

		orthoOffset = m_cameras[CT_Normal]->GetOrtho();

		return orthoOffset;
	}

	// 观察正交矩阵
	const Matrix44& CameraLogic::GetViewOrtho()
	{
		static Matrix44 viewProjOffset;

		viewProjOffset = m_cameras[CT_Normal]->GetViewOrtho();

		return viewProjOffset;
	}

	// 更新反射摄像机
	void CameraLogic::SwitchToReflectionCamera()
	{
		/*ICamera* pCamera = m_pCameraManager->GetCameraByName( "Normal");

		// 首先更新( 默认1.f平面)
		Vector3 point ( 0.f, 0.f, 1.f);
		Vector3 normal( 0.f, 0.f, 1.f);
		Plane3   plane( point, normal);*/

		//m_pCameraManager->SwitchToReflectionCamera( pCamera, plane);
	}

	// 获取活动摄像机搜寻包围盒
	const Rect3& CameraLogic::GetSearchRect()
	{
		return m_cameras[CT_Normal]->GetFrustem().GetAABB();
	}

	// 获取活动摄像机平截头体
	Frustum3* CameraLogic::GetFrustum()
	{
		return &m_cameras[CT_Normal]->GetFrustem();
	}

	// 获取正交投影
	ShaderParamPtr& CameraLogic::GetVpOrthoProj()
	{
		*m_vpOrthoProj = m_cameras[CT_Normal]->GetOrtho();

		return m_vpOrthoProj;
	}
}