#pragma once

#include "ThirdViewCamera.h"

namespace Ares
{
	//-----------------------------------------------------------------
	//  尾随视角摄像机：2009-12-28 18：02 -- 帝林
	//-----------------------------------------------------------------
	class CFollowViewerCamera : public LiberityViewCamera
	{
	public:
		CFollowViewerCamera();
		virtual BOOL  FrameMove(float fTime);
		virtual void  UpdateCameraDir(float relativeZAngle, float relativeYAngle);
		virtual void  SetBindPos(Vector3* bindPos);	

		// 设置观察点
		virtual void SetLookAt(Vector3 *v_pLookAt);

		// 设置位置点
		virtual void SetPos(Vector3* vPos);

		// 设置默认半径
		void SetDefaultRadius(float relativeRadius);

		virtual void  Release();

	protected:
		virtual void UpdatePosOrLookAt();

	private:
		Vector3* m_bindLookAt;	// 目标点
	};
}