#include "BaseCamera.h"
#include <Engine/AresRoot.h>

#define HALF_PI PI / 2.f

namespace Ares
{
	// 构造函数
	CCamera::CCamera()
		: m_screenWidth( 0.f)
		, m_screenHeight( 0.f)
	{
		m_vLookAt   = Vector3( 0.0f, 0.0f, 0.0f);
		m_vPosition = Vector3( 0.0f, -4.0f, -4.0f);

		m_vUp       = Vector3( 0.0f, 0.0f, 1.0f);
		m_vForward  = m_vLookAt - m_vPosition;
		m_vForward.Normalize();

		Matrix44ViewAtLH( m_matView, m_vPosition, m_vLookAt, m_vUp);					// 计算视图矩阵

		m_matUpMatch.SetIdentity();

		//RecalcMatrices();
	}

	void CCamera::SetViewParams( Vector3* vPos, Vector3* vLookAt, Vector3* vUp)
	{
		if ( vPos)	  m_vPosition = *vPos;
		if ( vLookAt) m_vLookAt = *vLookAt;
		if ( vUp)     m_vUp = *vUp;

		UpdateViewMatrix();
	}

	// 设置上方向
	void  CCamera::SetUpDir( float x, float y, float z)
	{
		m_vUp.x = x;
		m_vUp.y = y;
		m_vUp.z = z;

		m_vUp.Normalize();

		//float angle = acos( Vector3Dot( m_vUp, Vector3( 0, 0, 1)));
		float angle = acos( m_vUp.z);

		Vector3 axisUp;
		axisUp = Vector3Cross( Vector3( 0, 0, 1), m_vUp);

		Matrix44RotationAxisAngle( m_matUpMatch, axisUp, angle);

		UpdateViewMatrix();
	}

	// 设置投影矩阵参数
	void CCamera::SetProjParams( float fovH, int screenWidth, int screenHeight, float fNearPlane, float fFarPlane, RenderDeviceType type)
	{
		m_fovH			= fovH;
		m_screenWidth	= static_cast<float>(screenWidth);
		m_screenHeight	= static_cast<float>(screenHeight);
		m_fAspect		= m_screenWidth/m_screenHeight;
		m_fNearPlane	= fNearPlane;
		m_fFarPlane		= fFarPlane;
		m_deviceType	= type;

		RefreshProjOrthoMatrix();
	}

	// 设置近截面
	void  CCamera::SetNear( float _near)
	{
		m_fNearPlane = _near;

		RefreshProjOrthoMatrix();
	}

	// 设置远截面
	void  CCamera::SetFar( float _far)
	{
		m_fFarPlane  = _far;

		RefreshProjOrthoMatrix();
	}

	// 设置开角
	void  CCamera::SetFov( float fov)
	{
		m_fovH = fov;

		RefreshProjOrthoMatrix();
	}

	// 设置摄像机位置
	void CCamera::SetPos(Vector3* vPos)
	{
		m_vPosition = *vPos;
		UpdateViewMatrix();
	}

	void CCamera::SetLookAt( const Vector3& lookAt)
	{
		m_vLookAt = lookAt;
	}

	void CCamera::UpdateViewMatrix()
	{
		Matrix44ViewAtLH( m_matView, m_vPosition, m_vLookAt, m_vUp);	

		RecalcMatrices();
	}

	void CCamera::RefreshProjOrthoMatrix()
	{
		// 正交矩阵
		FLOAT  w = m_screenWidth;
		FLOAT  h = w / m_fAspect;
		Matrix44OrthoLH( m_matOrtho, w, h, m_fNearPlane, m_fFarPlane);

		//m_matOrtho._41 = ( 2.f / (m_fFarPlane * tanf( m_fFov))) - (2.f / w);
		//m_matOrtho._42 = m_matOrtho._41 / m_fAspect;

		// 投影矩阵
		if( m_deviceType == D3D11)
		{
			Matrix44PerspectiveFovLH_D3DX( m_matProj, m_fovH, m_fAspect, m_fNearPlane, m_fFarPlane);

			// Create a unit-space matrix for sky box geometry. the ensures that the near and
			// far plane enclose the unit space around the camera
			Matrix44PerspectiveFovLH_D3DX( m_matUnitProj, m_fovH, m_fAspect, 0.1f, 10.0f);
		}
		else
		{
			Matrix44PerspectiveFovLH_OpenGL( m_matProj, m_fovH, m_fAspect, m_fNearPlane, m_fFarPlane);

			// Create a unit-space matrix for sky box geometry. the ensures that the near and
			// far plane enclose the unit space around the camera
			Matrix44PerspectiveFovLH_OpenGL( m_matUnitProj, m_fovH, m_fAspect, 0.1f, 10.0f);
		}

		m_frustem.SetPerspective( m_fovH, m_fAspect, m_fNearPlane, m_fFarPlane);
	}

	void CCamera::RecalcMatrices()
	{
		// 天空盒矩阵
		m_matSkyView		= m_matView;
		m_matSkyView._41 = 0.0f;
		m_matSkyView._42 = 0.0f;
		m_matSkyView._43 = 0.0f;

		// sky boxes use the inverse world matrix of the camera(the camera view matrix)
		// without any translation information
		m_matSkyViewProj = m_matSkyView * m_matUnitProj;

		// 平截头体
		m_matViewProj = m_matView * m_matProj;
		m_frustem.Build( m_vPosition, m_vForward, m_vUp);
	}

	// 更新
	BOOL CCamera::FrameMove(float fTime)
	{ 			
		//UpdatePosOrLookAt();
	
		UpdateViewMatrix();

		return TRUE;
	}

	// 获取viewProjMatrix
	Matrix44& CCamera::GetViewProj()
	{
		if (true /*需要更新标记*/)
		{
			m_matViewProj = m_matView * m_matProj;
		}

		return m_matViewProj;
	}

	// 正交矩阵
	const Matrix44& CCamera::GetOrtho()
	{
		return m_matOrtho;
	}

	// 观察正交矩阵
	const Matrix44& CCamera::GetViewOrtho()
	{
		m_matViewOrtho = m_matView * m_matOrtho;

		return m_matViewOrtho;
	}

	// 右方向
	Vector3 CCamera::GetRight() const 
	{ 
		Vector3 right( cos( m_horizonAngle + HALF_PI), sin( m_horizonAngle + HALF_PI), 0.f);

		right = m_matUpMatch.TransformVector3( right);

		return right; 
	}
}

