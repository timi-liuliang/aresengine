#pragma once

#include "BaseCamera.h"

namespace Ares
{
	//-----------------------------------------------------------------
	//  第三人称摄像机(自由视角摄像机)：2009-12-28 18：02 -- 帝林
	//-----------------------------------------------------------------
	class LiberityViewCamera : public Camera
	{
	public:
		LiberityViewCamera();
		virtual BOOL  FrameMove(float fTime);
		virtual void  UpdateCameraDir(float relativeZAngle, float relativeYAngle);
		virtual void  SetYawAngle(float yAngle){ m_horizonAngle = yAngle; UpdateCameraDir(0.0f, 0.0f); UpdatePosOrLookAt();};
		virtual void  SetRollAngle(float zAngle){ m_verticleAngle = zAngle; UpdateCameraDir( 0.0f, 0.0f); UpdatePosOrLookAt();};
		virtual float GetYawAngle(){  return m_horizonAngle; };
		virtual float GetRollAngle(){ return m_verticleAngle; };
		virtual void  SetCurRadius(float fRadius);
		virtual void  ChangeDefaultRadius(float relativeRadius);
		virtual void  SetDefaultRadius(float fRadius);
		virtual void  SetViewParams(Vector3* vPos, Vector3* vLookAt);
		virtual void  SetPos(Vector3* vPos);
		virtual void  SetLookAt(Vector3* v_pLookAt);
		virtual void  Release();

		// Zoom
		virtual void Zoom( float radius);

		// 获取当前半径
		virtual float GetCurRadius(){ return m_fCurRadius; }

		// 获取默认半径
		virtual float GetDefaultRadius() { 
			return m_fDefaultRadius; }

	protected:
		virtual void UpdatePosOrLookAt();

	protected:
		float m_fCurRadius;			// 当前半径 (摄像机LookAt点当中心)
		float m_fDefaultRadius;		// 默认半径
		float m_fMinRadius;			// 所允许的最小半径
		float m_fMaxRadius;			// 所允许的最大半径

		BOOL  m_needUpdateViewMatrix;
	};
}

