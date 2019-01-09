#include "SCLogic.h"
#include <Core/AresMath.h>

// 摄像机左右旋转绽放 上下旋转缩放 半径增加缩短缩放
#define CAMERA_ROTATIONY_SCALAR  0.1f
#define CAMERA_ROTATIONZ_SCALAR  0.3f
#define CAMERA_RADIUS_SCALAR     0.01f

namespace Ares
{
	// 构造函数
	CameraLogic::CameraLogic()
		: CameraSystem()
	{
		m_cameras.assign( NULL);

		m_vpOrthoProj = MakeShaderParam();
	}

	// 析构函数
	CameraLogic::~CameraLogic()
	{
	}

	// 是否可用
	bool CameraLogic::IsValid()
	{
		POINT cp;		// 当前鼠标位置
		RECT  cr;		// 窗口区域
		GetCursorPos( &cp);
		ScreenToClient( SELogic_RenderSystem.GetRenderSettings().m_hwnd, &cp);
		GetClientRect(  SELogic_RenderSystem.GetRenderSettings().m_hwnd, &cr);

		if( cp.x<cr.left || cp.x>cr.right)
			return false;

		if( cp.y<cr.top || cp.y>cr.bottom)
			return false;

		return true;
	}

	// 鼠标移动消息
	bool CameraLogic::mouseMoved( const OIS::MouseEvent &arg)
	{
		LiberityViewCamera* pCamera = dynamic_cast<LiberityViewCamera*>( m_cameras[CT_Normal]);
		if ( pCamera)
		{
			if( SCLogic_Input->IsMouseDown(OIS::MB_Right))		// 鼠标移动且右键按下
			{
				pCamera->UpdateCameraDir( SCLogic_Input->GetMouseState().Y.rel * CAMERA_RADIUS_SCALAR, SCLogic_Input->GetMouseState().X.rel * CAMERA_ROTATIONZ_SCALAR * CAMERA_ROTATIONY_SCALAR);
			}

			if(SCLogic_Input->GetMouseState().Z.rel )
			{
				pCamera->ChangeDefaultRadius( (float)SCLogic_Input->GetMouseState().Z.rel );
			}
		}

		return true;
	}

	// 根据输入 控制摄像机
	void CameraLogic::FrameMove(float fTime)
	{
		UpdateSPAB();
	}

	// 更新Shader参数
	void CameraLogic::UpdateSPAB()
	{
		if( m_cameras[CT_Normal])
		{
			m_params.m_skyViewProj = m_cameras[CT_Normal]->GetSkyViewProj();

			*m_spab[SP_View]	  = GetView();
			*m_spab[SP_ViewProj]  = GetViewProj();
			*m_spab[SP_SkyViewProj] = m_cameras[CT_Normal]->GetSkyViewProj();
			*m_spab[SP_Pos]		  = GetPosition();
			*m_spab[SP_NearFar]	  = float2( GetNear(), GetFar());
			*m_spab[SP_Right]	  = GetFrustum()->GetRight();
			*m_spab[SP_Up]		  = GetFrustum()->GetUp();
			*m_spab[SP_Front]	  = GetFacing();
			*m_spab[SP_Direction] = m_cameras[CT_Normal]->GetDirection();
		}
	}

	// 设置远近截面
	void  CameraLogic::SetNearFar( float fNear, float fFar)
	{
		Camera* pCamera = m_cameras[CT_Normal];
		if( pCamera)
		{
			// 设置投影矩阵
			pCamera->SetNear(  fNear);
			pCamera->SetFar(  fFar);
		}
	}

	// 设置开角
	void  CameraLogic::SetFov( float fov)
	{
		if( m_cameras[CT_Normal])
		{
			m_cameras[CT_Normal]->SetFov( fov);
		}
	}

	// 设置上方向
	void  CameraLogic::SetUpDir( float x, float y, float z)
	{
		Camera* pCamera = m_cameras[CT_Normal];
		if( pCamera)
		{
			// 设置投影矩阵
			pCamera->SetUpDir(  x, y, z);
		}
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