#include "FxEditCamera.h"

namespace Ares
{
	// 构造函数
	FxEditCamera::FxEditCamera() 
		: LiberityViewCamera()
	{
		m_bindLookAt = NULL;
	}

	// 更改摄像机位置 
	void FxEditCamera::UpdateCameraDir(float relativeZAngle, float relativeYAngle)
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

	BOOL FxEditCamera::FrameMove(float fTime)
	{
		if(m_bindLookAt != NULL &&  *m_bindLookAt != m_vLookAt)		// 绑定观察点移动
		{
			m_vLookAt = *m_bindLookAt;

			m_needUpdateViewMatrix = TRUE;	
		}

		m_fCurRadius = m_fDefaultRadius;

		UpdatePosOrLookAt();
		UpdateViewMatrix();

		return TRUE;
	}

	void FxEditCamera::SetBindPos(Vector3 *bindPos)
	{
		m_bindLookAt = bindPos;
	}

	void FxEditCamera::Release()
	{
		m_bindLookAt = NULL;
	}

	// 设置观察点
	void FxEditCamera::SetLookAt(Vector3 *v_pLookAt)
	{
		m_vLookAt = *v_pLookAt;

		UpdatePosOrLookAt();
		UpdateViewMatrix();
	}

	// 设置位置点
	void FxEditCamera::SetPos(Vector3* vPos)
	{
		m_vForward = m_vLookAt - *vPos;
		SetDefaultRadius( m_vForward.Length());
		m_vForward.Normalize();

		m_vForward = m_matUpMatch.TransformVector3( m_vForward);

		m_verticleAngle = acos( m_vForward.z);
		float tScale = abs( sin( m_verticleAngle));

		m_horizonAngle = asin( m_vForward.y/ tScale);

		UpdatePosOrLookAt();
		UpdateViewMatrix();
	}

	// 设置默认半径
	void FxEditCamera::SetDefaultRadius(float relativeRadius)
	{
		m_fCurRadius	 = relativeRadius;
		m_fDefaultRadius = relativeRadius;

		m_fDefaultRadius = std::min<float>(m_fDefaultRadius, m_fMaxRadius);
		m_fDefaultRadius = std::max<float>(m_fDefaultRadius, m_fMinRadius);
	}

	// 更新位置
	void FxEditCamera::UpdatePosOrLookAt()
	{ 
		m_vPosition = m_vLookAt - m_fCurRadius * m_vForward; 
	}

	// viewProjMatrix
	Matrix44&  FxEditCamera::GetViewProj()
	{
		return m_matViewProj;
	}
}