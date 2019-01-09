#pragma once

#include "ThirdViewCamera.h"

namespace Ares
{
	//----------------------------------------------
	// 特效编辑器专用 2012-8-20 帝林
	//----------------------------------------------
	class FxEditCamera : public LiberityViewCamera
	{
	public:
		FxEditCamera();
		virtual BOOL  FrameMove(float fTime);
		virtual void  UpdateCameraDir(float relativeZAngle, float relativeYAngle);
		virtual void  SetBindPos(Vector3* bindPos);	

		// 设置观察点
		virtual void SetLookAt(Vector3 *v_pLookAt);

		// 设置位置点
		virtual void SetPos(Vector3* vPos);

		// 设置默认半径
		void SetDefaultRadius(float relativeRadius);

		// viewProjMatrix
		virtual Matrix44&  GetViewProj();

		// 释放资源
		virtual void  Release();

	protected:
		virtual void UpdatePosOrLookAt();

	private:
		Vector3*	m_bindLookAt;	// 目标点
	};
}