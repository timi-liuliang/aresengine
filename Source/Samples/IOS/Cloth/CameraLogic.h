#pragma once

#include "BaseCamera.h"
#include <Engine/CameraSystem/AresCameraSystem.h>
#include <Core/Graphics/AresRenderMethod.h>

namespace Ares
{
	class CameraLogic : public CameraSystem
	{
	public:
		CameraLogic();
		~CameraLogic();

		// 每帧更新
		void FrameMove(float fTime);

		// 设置投影参数
		void  SetProjParams( float fovW, int screenWidth, int screenHeight, float fNearPlane, float fFarPlane, RenderDeviceType type);

		// 设置上方向
		void  SetUpDir( float x, float y, float z);

		// 设置远近截面
		void  SetNearFar( float fNear, float fFar);

		// 设置开角
		void  SetFov( float fov);

		// 更新Shader参数
		virtual void UpdateSPAB();

	public:
		// 是否可用
		virtual bool IsValid();

		// RenderIO
	public:
		// 获取近截面
		virtual float GetNear();

		// 获取远截面
		virtual float GetFar();

		// 获取朝向
		virtual Vector3& GetFacing();

		// 获取观察矩阵
		virtual Matrix44& GetView();

		// 获取投影矩阵
		virtual Matrix44& GetProj();

		// 正交矩阵
		virtual const Matrix44& GetOrtho();

		// 观察正交矩阵
		virtual const Matrix44& GetViewOrtho();

		// 获取观察投影矩阵
		virtual Matrix44& GetViewProj();

		// 获取摄像机位置
		virtual Vector3& GetPosition();

		// 获取活动摄像机搜寻包围盒
		virtual const Rect3& GetSearchRect();

		// 获取活动摄像机平截头体
		virtual Frustum3* GetFrustum();

		// 更新反射摄像机
		virtual void SwitchToReflectionCamera();

		// 设置速度
		void SetSpeed( float speed) { m_fSpeed = speed; }

	public:
		// 获取正交投影
		virtual ShaderParamPtr& GetVpOrthoProj();

	private:
		Vector3					m_vPos;
		float					m_fSpeed;

		ShaderParamPtr			m_vpOrthoProj;

		boost::array<CCamera*, CT_Total> m_cameras;
	};
}
