#include "ThirdViewCamera.h"

namespace Ares
{
	LiberityViewCamera::LiberityViewCamera() 
		: Camera()
	{
		m_fCurRadius = 8.0f;
		m_fDefaultRadius = 8.0f;
		m_fMinRadius = 0.02f;
		m_fMaxRadius = 1000.0f;

		m_horizonAngle  = 0.0f;
		m_verticleAngle = 0.75f * PI;

		m_needUpdateViewMatrix = FALSE;
	}

	// 更新摄像机朝向
	void LiberityViewCamera::UpdateCameraDir(float relativeZAngle, float relativeYAngle)
	{
		m_verticleAngle += relativeZAngle;	   m_horizonAngle += relativeYAngle;

		m_verticleAngle = min( m_verticleAngle, PI - 0.01f);
		m_verticleAngle = max( m_verticleAngle, 0.01f);

		float tScale = abs( sin( m_verticleAngle)); 
		m_vForward.x = tScale * cos( m_horizonAngle);
		m_vForward.y = tScale * sin( m_horizonAngle);
		m_vForward.z = cos( m_verticleAngle);

		m_vForward = m_matUpMatch.TransformVector3( m_vForward);

		UpdatePosOrLookAt();
		UpdateViewMatrix();	 // 更新视图矩阵
	}

	BOOL LiberityViewCamera::FrameMove(float fTime)
	{
		if(m_fCurRadius != m_fDefaultRadius)
		{
			float curRadius = m_fCurRadius + (m_fDefaultRadius - m_fCurRadius) * fTime * 5.0f;

			SetCurRadius( m_fCurRadius);

			m_needUpdateViewMatrix = TRUE;	
		}

		if (m_needUpdateViewMatrix)					// 不是每次都更新ViewMatrix
		{
			UpdatePosOrLookAt();
			UpdateViewMatrix();
			m_needUpdateViewMatrix = FALSE;

			return TRUE;
		}

		return FALSE;
	}

	void LiberityViewCamera::SetCurRadius(float fRadius)
	{
		m_fCurRadius = fRadius;
		m_fCurRadius = __min(m_fCurRadius, m_fDefaultRadius);
		m_fCurRadius = __max(m_fCurRadius, m_fMinRadius);

		UpdatePosOrLookAt();
		UpdateViewMatrix();
	}

	void LiberityViewCamera::ChangeDefaultRadius(float relativeRadius)
	{
		m_fDefaultRadius += relativeRadius;
		m_fDefaultRadius = __min(m_fDefaultRadius, m_fMaxRadius);
		m_fDefaultRadius = __max(m_fDefaultRadius, m_fMinRadius);

		//m_fCurRadius = __min(m_fCurRadius, m_fDefaultRadius);

		UpdatePosOrLookAt();
		UpdateViewMatrix();
	}

	void LiberityViewCamera::SetDefaultRadius(float fRadius)
	{
		m_fDefaultRadius = __min(fRadius, m_fMaxRadius);
		m_fDefaultRadius = __max(m_fDefaultRadius, m_fMinRadius);

		m_fCurRadius = __min(m_fCurRadius, m_fDefaultRadius);

		UpdatePosOrLookAt();
		UpdateViewMatrix();
	}

	void LiberityViewCamera::SetLookAt(Vector3 *v_pLookAt)
	{
		m_vForward = *v_pLookAt - m_vPosition;
		m_vForward.Normalize();

		m_vForward = m_matUpMatch.TransformVector3( m_vForward);

		m_verticleAngle = acos( m_vForward.z);
		float tScale = abs( sin( m_verticleAngle));

		m_horizonAngle = asin( m_vForward.y/ tScale);

		UpdatePosOrLookAt();
		UpdateViewMatrix();
	}

	void LiberityViewCamera::SetViewParams(Vector3* vPos, Vector3* vLookAt)
	{
		m_vPosition = *vPos;
		m_vLookAt = *vLookAt;

		UpdateViewMatrix();
	}

	void LiberityViewCamera::SetPos(Vector3* vPos)
	{
		m_vPosition = *vPos;

		UpdatePosOrLookAt();
		UpdateViewMatrix();
	}

	// 更新LookAt
	void LiberityViewCamera::UpdatePosOrLookAt()
	{ 
		m_vLookAt =  m_vPosition + m_fCurRadius * m_vForward; 
	}

	void LiberityViewCamera::Release()
	{

	}

	// Zoom
	void LiberityViewCamera::Zoom( float radius)
	{
		float fovW = m_fovH * m_fAspect;
		float fov  = std::max<float>( fovW, m_fovH);

		float cameraRadius = radius / std::sin( fov/2.f);

		SetDefaultRadius( cameraRadius);
	}
}