#include "MOLogic.h"
#include <Core/AresMath.h>
#include <Engine/AresRoot.h>

// 摄像机左右旋转绽放 上下旋转缩放 半径增加缩短缩放
#define CAMERA_ROTATIONY_SCALAR  0.1f
#define CAMERA_ROTATIONZ_SCALAR  0.3f
#define CAMERA_RADIUS_SCALAR     0.005f

namespace Ares
{
	// 构造函数
	MOCameraLogic::MOCameraLogic()
		: CameraSystem()
		, m_vPos( Vector3( 0.f, 20.f, 10.f))
		, m_bindLookAt( Vector3( 0.f, 0.f, 0.f))
		, m_xOffset( 0.f)
		, m_yOffset( 0.f)
	{
		m_screenOffset.SetIdentity();
		m_fSpeed = 5.f;


		m_cameras[CT_Normal] = new_ FxEditCamera;
		FxEditCamera* pCamera = dynamic_cast<FxEditCamera*>( m_cameras[CT_Normal]);
		if( pCamera)
		{
			pCamera->SetPos( &m_vPos);
			pCamera->SetLookAt( &Vector3( 0.f, 0.f, 0.f));
			pCamera->SetBindPos( &m_bindLookAt);
		}

		m_vpViewProj  = MakeShaderParam( GetViewProj());
		m_vpOrthoProj = MakeShaderParam( GetOrtho());
	}

	// 析构函数
	MOCameraLogic::~MOCameraLogic()
	{
	}

	// 是否可用
	bool MOCameraLogic::IsValid()
	{
		POINT cp;		// 当前鼠标位置
		RECT  cr;		// 窗口区域
		GetCursorPos( &cp);
		ScreenToClient( MOLogic_RenderSystem.GetRenderSettings().m_hwnd, &cp);
		GetClientRect( MOLogic_RenderSystem.GetRenderSettings().m_hwnd, &cr);

		if( cp.x<cr.left || cp.x>cr.right)
			return false;

		if( cp.y<cr.top || cp.y>cr.bottom)
			return false;

		return true;
	}

	// 鼠标移动消息
	bool MOCameraLogic::mouseMoved( const OIS::MouseEvent &arg)
	{
		OnMouseMove();

		return true;
	}

	// 鼠标移动
	void MOCameraLogic::OnMouseMove()
	{
		FxEditCamera* pCamera = dynamic_cast<FxEditCamera*>( m_cameras[CT_Normal]);
		if ( pCamera)
		{
			if( MOLogic_Input->IsMouseDown(OIS::MB_Right))		// 鼠标移动且右键按下
			{
				pCamera->UpdateCameraDir( MOLogic_Input->GetMouseState().Y.rel * CAMERA_RADIUS_SCALAR, MOLogic_Input->GetMouseState().X.rel * CAMERA_ROTATIONY_SCALAR * CAMERA_ROTATIONY_SCALAR);
			}

			if( MOLogic_Input->IsMouseDown(OIS::MB_Left))
			{
				pCamera->ChangeDefaultRadius( ((float)MOLogic_Input->GetMouseState().Y.rel) * 0.0025f * MOLogic_ModelEdit->GetModelRadius());
				pCamera->UpdateCameraDir( 0.f, MOLogic_Input->GetMouseState().X.rel * CAMERA_ROTATIONY_SCALAR * CAMERA_ROTATIONY_SCALAR);
			}

			if( MOLogic_Input->IsMouseDown( OIS::MB_Middle))
			{
				float xOffset = MOLogic_Input->GetMouseState().X.rel *  2.f / MOLogic_RenderWindow->GetWidth();
				float yOffset = MOLogic_Input->GetMouseState().Y.rel * -2.f / MOLogic_RenderWindow->GetHeight();
				AddScreenOffset( xOffset, yOffset);
			}
			else if(MOLogic_Input->GetMouseState().Z.rel )
				pCamera->ChangeDefaultRadius( ((float)MOLogic_Input->GetMouseState().Z.rel) * -0.0015f * MOLogic_ModelEdit->GetModelRadius());
		}
	}

	// 根据输入 控制摄像机
	void MOCameraLogic::FrameMove(float fTime)
	{
		FxEditCamera* pCamera = dynamic_cast<FxEditCamera*>( m_cameras[CT_Normal]);
		if ( pCamera)
		{
			pCamera->FrameMove( fTime);

			UpdateSPAB();
		}
	}

	// 更新Shader参数
	void MOCameraLogic::UpdateSPAB()
	{
		*m_spab[SP_View]		= GetView();
		*m_spab[SP_ViewProj]	= GetViewProj();
		*m_spab[SP_SkyViewProj] = m_cameras[CT_Normal]->GetSkyViewProj();
		*m_spab[SP_Pos]			= GetPosition();
		*m_spab[SP_NearFar]		= float2( GetNear(), GetFar());
		*m_spab[SP_Direction]	= m_cameras[CT_Normal]->GetDirection();
	}

	// 设置默认半径
	void MOCameraLogic::SetDefaultRadius( float radius)
	{
		FxEditCamera* pCamera = dynamic_cast<FxEditCamera*>( m_cameras[CT_Normal]);
		if( pCamera)
			pCamera->SetDefaultRadius( radius);
	}

	// 使观察者位于屏幕中间
	void MOCameraLogic::Zoom( const Rect3 bounds)
	{
		FxEditCamera* pCamera = dynamic_cast<FxEditCamera*>( m_cameras[CT_Normal]);
		if( pCamera)
		{
			float   radius = ((bounds.GetMinPoint()-bounds.GetMaxPoint()).Length() * 0.5f);
			Vector3 center = ((bounds.GetMinPoint()+bounds.GetMaxPoint()) * 0.5f);

			pCamera->Zoom( radius);
				
			SetLookAt( center);
			AddScreenOffset( -m_xOffset, -m_yOffset);
		}
	}

	// 设置观察点
	void MOCameraLogic::SetLookAt( const Vector3& lookAt)
	{
		FxEditCamera* pCamera = dynamic_cast<FxEditCamera*>( m_cameras[CT_Normal]);
		if( pCamera)
		{
			m_bindLookAt = lookAt;
		}
	}

	// 添加ScreenOffset
	void MOCameraLogic::AddScreenOffset( float xOffset, float yOffset)
	{
		m_xOffset += xOffset;
		m_yOffset += yOffset;

		Matrix44Translation( m_screenOffset, m_xOffset, m_yOffset, 0.f);
	}

	// 设置ProjParams
	void MOCameraLogic::SetProjParams( float fovW, int screenWidth, int screenHeight, float fNearPlane, float fFarPlane)
	{
		FxEditCamera* pCamera = dynamic_cast<FxEditCamera*>( m_cameras[CT_Normal]);

		// 设置投影矩阵
		float aspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
		pCamera->SetProjParams(  fovW/aspect, screenWidth, screenHeight, fNearPlane, fFarPlane, D3D11);
	}

	// 设置远近截面
	void  MOCameraLogic::SetNearFar( float fNear, float fFar)
	{
		FxEditCamera* pCamera = dynamic_cast<FxEditCamera*>( m_cameras[CT_Normal]);

		// 设置投影矩阵
		pCamera->SetNear(  fNear);
		pCamera->SetFar(  fFar);
	}

	// 设置开角
	void  MOCameraLogic::SetFov( float fov)
	{
		FxEditCamera* pCamera = dynamic_cast<FxEditCamera*>( m_cameras[CT_Normal]);

		// 设置投影矩阵
		pCamera->SetFov(  fov);
	}

	// 设置上方向
	void  MOCameraLogic::SetUpDir( float x, float y, float z)
	{
		FxEditCamera* pCamera = dynamic_cast<FxEditCamera*>( m_cameras[CT_Normal]);

		// 设置投影矩阵
		pCamera->SetUpDir(  x, y, z);
	}


	//-------------------------------------------
	// For Render IO
	//-------------------------------------------
	// 获取观察投影矩阵
	Matrix44& MOCameraLogic::GetViewProj()
	{
		static Matrix44 viewProjOffset;

		FxEditCamera* pCamera = dynamic_cast<FxEditCamera*>( m_cameras[CT_Normal]);

		viewProjOffset = pCamera->GetViewProj() * m_screenOffset;

		return viewProjOffset;
	}

	// 获取正交投影
	ShaderParamPtr& MOCameraLogic::GetVpOrthoProj()
	{
		*m_vpOrthoProj = GetOrtho();

		return m_vpOrthoProj;
	}

	// 正交矩阵
	const Matrix44& MOCameraLogic::GetOrtho()
	{
		static Matrix44 orthoOffset;

		FxEditCamera* pCamera = dynamic_cast<FxEditCamera*>( m_cameras[CT_Normal]);

		orthoOffset = pCamera->GetOrtho() * m_screenOffset;

		return orthoOffset;
	}

	// 观察正交矩阵
	const Matrix44& MOCameraLogic::GetViewOrtho()
	{
		static Matrix44 viewProjOffset;

		FxEditCamera* pCamera = dynamic_cast<FxEditCamera*>( m_cameras[CT_Normal]);

		viewProjOffset = pCamera->GetViewOrtho()  * m_screenOffset;

		return viewProjOffset;
	}

	// 获取摄像机
	FxEditCamera* MOCameraLogic::GetNormalCamera()
	{
		return dynamic_cast<FxEditCamera*>( m_cameras[CT_Normal]);
	}

	// 获取摄像机位置
	Vector3& MOCameraLogic::GetPosition()
	{
		return (Vector3&)GetNormalCamera()->GetPosition();
	}

	// 获取近截面
	float MOCameraLogic::GetNear()
	{
		return GetNormalCamera()->GetNear();
	}

	// 获取右朝向
	Vector3	MOCameraLogic::GetRight()
	{
		return GetNormalCamera()->GetRight();
	}

	// 获取远截面
	float MOCameraLogic::GetFar()
	{
		return GetNormalCamera()->GetFar();
	}

	// 获取朝向
	Vector3& MOCameraLogic::GetFacing()
	{
		return GetNormalCamera()->GetDirection();
	}

	// 获取观察矩阵
	Matrix44& MOCameraLogic::GetView()
	{
		return GetNormalCamera()->GetView();
	}

	// 获取投影矩阵
	Matrix44& MOCameraLogic::GetProj()
	{
		return GetNormalCamera()->GetProj();
	}	

	// 获取活动摄像机搜寻包围盒
	const Rect3& MOCameraLogic::GetSearchRect()
	{
		return GetNormalCamera()->GetFrustem().GetAABB();
	}

	// 获取活动摄像机平截头体
	Frustum3* MOCameraLogic::GetFrustum()
	{
		return &GetNormalCamera()->GetFrustem();
	}
}