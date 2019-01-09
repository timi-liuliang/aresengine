#pragma once

#include <Engine/InputSystem/OIS.h>
#include <Engine/CameraSystem/AresCameraSystem.h>
#include <Core/Graphics/AresRenderMethod.h>
#include <EditorKit/BaseCamera.h>
#include <EditorKit/FxEditCamera.h>

namespace Ares
{
	//------------------------------------------------
	// 摄像机逻辑 2012-8-20 帝林
	//------------------------------------------------
	class MOCameraLogic : public CameraSystem , public OIS::MouseListener
	{
	public:
		MOCameraLogic();
		~MOCameraLogic();

		// 每帧更新
		void FrameMove(float fTime);

		// 设置投影参数
		void  SetProjParams( float fovW,  int screenWidth, int screenHeight, float fNearPlane, float fFarPlane);

		// 设置上方向
		void  SetUpDir( float x, float y, float z);

		// 设置远近截面
		void  SetNearFar( float fNear, float fFar);

		// 设置开角
		void  SetFov( float fov);

		// 设置观察点
		void SetLookAt( const Vector3& lookAt);

		// 设置默认半径
		void SetDefaultRadius( float radius);

		// 添加ScreenOffset
		void AddScreenOffset( float xOffset, float yOffset);

		// 使观察者位于屏幕中间
		void Zoom( const Rect3 bounds);

		// 更新Shader参数
		virtual void UpdateSPAB();

	public:
		// 是否可用
		virtual bool IsValid();

		// 鼠标移动消息
		bool mouseMoved( const OIS::MouseEvent &arg);

		// 鼠标移动
		void OnMouseMove();

		// RenderIO
	public:
		// 获取近截面
		virtual float GetNear();

		// 获取远截面
		virtual float GetFar();

		// 获取右朝向
		Vector3	GetRight();

		// 获取朝向
		virtual Vector3& GetFacing();

		// 获取观察矩阵
		virtual Matrix44& GetView();

		// 获取投影矩阵
		virtual Matrix44& GetProj();

		// 获取屏幕偏移
		Matrix44& GetOffset() { return m_screenOffset; }

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

		// 设置速度
		void SetSpeed( float speed) { m_fSpeed = speed; }

	private:
		// 获取摄像机
		FxEditCamera* GetNormalCamera();

	public:
		// 获取正交投影
		ShaderParamPtr& GetVpOrthoProj();

	public:
		Vector3					m_vPos;
		Vector3					m_bindLookAt;
		float					m_fSpeed;

		ShaderParamPtr			m_vpViewProj;
		ShaderParamPtr			m_vpOrthoProj;

		float					m_xOffset;
		float					m_yOffset;
		Matrix44				m_screenOffset;

		boost::array<Camera*, CT_Total> m_cameras;
	};
}
