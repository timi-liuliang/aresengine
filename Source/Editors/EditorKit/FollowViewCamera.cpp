#include "FollowViewerCamera.h"

namespace Ares
{
	// 构造函数
	CFollowViewerCamera::CFollowViewerCamera() : LiberityViewCamera()
	{
		m_bindLookAt = NULL;
	}

	// 更改摄像机位置 
	void CFollowViewerCamera::UpdateCameraDir(float relativeZAngle, float relativeYAngle)
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

	BOOL CFollowViewerCamera::FrameMove(float fTime)
	{
		if(m_bindLookAt != NULL &&  *m_bindLookAt != m_vLookAt)		// 绑定观察点移动
		{
			m_vLookAt = *m_bindLookAt;

			m_needUpdateViewMatrix = TRUE;	
		}

		if(m_fCurRadius != m_fDefaultRadius)
		{
			m_fCurRadius += (m_fDefaultRadius - m_fCurRadius) * fTime * 0.5f;

			if( abs( m_fCurRadius - m_fDefaultRadius) < 1e-4f)
			{
				m_fCurRadius = m_fDefaultRadius;
			}

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

	void CFollowViewerCamera::SetBindPos(Vector3 *bindPos)
	{
		m_bindLookAt = bindPos;
	}

	void CFollowViewerCamera::Release()
	{
		m_bindLookAt = NULL;
	}

	// 设置观察点
	void CFollowViewerCamera::SetLookAt(Vector3 *v_pLookAt)
	{
		m_vLookAt = *v_pLookAt;

		UpdatePosOrLookAt();
		UpdateViewMatrix();
	}

	// 设置位置点
	void CFollowViewerCamera::SetPos(Vector3* vPos)
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
	void CFollowViewerCamera::SetDefaultRadius(float relativeRadius)
	{
		m_fDefaultRadius = relativeRadius;

		m_fDefaultRadius = std::min<float>(m_fDefaultRadius, m_fMaxRadius);
		m_fDefaultRadius = std::max<float>(m_fDefaultRadius, m_fMinRadius);
	}

	// 更新位置
	void CFollowViewerCamera::UpdatePosOrLookAt()
	{ 
		m_vPosition = m_vLookAt - m_fCurRadius * m_vForward; 
	}
}