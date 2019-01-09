#pragma  once

#include <Physics/Shapes.h>
#include <Core/Graphics/AresRenderDevice.h>
#include <string>

using namespace std;

namespace Ares
{
	//-----------------------------------------------------------------
	//  基础摄像机：摄像机必备属性 + 较少的操作 -- 帝林
	//-----------------------------------------------------------------
	class CCamera
	{
	public:
		// 枚举
		enum CameraType
		{
			CAMERA_TYPE_BASE, 
			CAMERA_TYPE_FOLLOW_VIEWER, 
			CAMERA_TYPE_LIBERITY_VIEW,
			CAMERA_TYPE_FXEDIT,
			EM_CAMERA_REFLECTION,
		};

		// 构造函数
		CCamera();
		
		// 设置名字
		virtual void  SetName( const char* name) { m_name = name; }

		// 获取名字
		virtual const char* GetName() { return m_name.c_str(); }

		// 设置常用参数
		virtual void  SetViewParams( Vector3* vPos, Vector3* vLookAt, Vector3* vUp);

		// 设置投影矩阵参数
		virtual void  SetProjParams(float fovH, int screenWidth, int screenHeight, float fNearPlane, float fFarPlane, RenderDeviceType type);

		// 设置近截面
		virtual void  SetNear( float _near);

		// 设置远截面
		virtual void  SetFar( float _far);

		// 设置开角
		virtual void  SetFov( float fov);
		
		// 设置EyePoint
		virtual void  SetPos(Vector3* vPos);
		
		// 设置观察点
		virtual void  SetLookAt( const Vector3& lookAt);	

		// 设置上方向
		virtual void  SetUpDir( float x, float y, float z);

		// 获取近截面
		virtual float GetNear() { return m_fNearPlane; }

		// 获取远截面
		virtual float GetFar() { return m_fFarPlane; }

		// 获取当前半径
		virtual float GetCurRadius() { return 0.f;};

		// 获取默认半径
		virtual float GetDefaultRadius() { return 0.f;}

		// 获取摄像机位置
		virtual const Vector3& GetPosition() const { return m_vPosition; }

		// 获取摄像机平截头体
		virtual Frustum3& GetFrustem() { return m_frustem; }

		// 右方向
		virtual Vector3 GetRight() const;

		// 被观察点
		virtual const Vector3& GetLookAt() const { return m_vLookAt; }

		// 获取上方向
		virtual const Vector3& GetUp() const { return m_vUp; }

		// 视图矩阵
		virtual Matrix44&  GetView(){ return m_matView; }

		// 投影矩阵
		virtual Matrix44&  GetProj(){ return m_matProj; }

		// viewProjMatrix
		virtual Matrix44&  GetViewProj();

		// 正交矩阵
		virtual const Matrix44& GetOrtho();

		// 观察正交矩阵
		virtual const Matrix44& GetViewOrtho();

		// 获取摄像机天空View
		virtual Matrix44& GetSkyView() { return m_matSkyView; }

		// 获取摄像机天空盒矩阵
		virtual Matrix44&  GetSkyViewProj() { return m_matSkyViewProj; }

		// 获取朝向
		virtual Vector3&  GetDirection(){ return m_vForward; }

		// 获取水平朝向角度
		virtual float GetHorizonAngle() { return m_horizonAngle; }

		// 更新
		virtual BOOL  FrameMove(float fTime);

	protected:
		// 更新ViewMatrix	
		void  UpdateViewMatrix();

		// 更新ProjMatrix
		void  RefreshProjOrthoMatrix();

	private:
		// 更新SearchRect
		void RecalcMatrices();

	protected:
		string			m_name;				// 摄像机名	
		Vector3			m_vPosition;		// 摄像机位置
		Vector3			m_vLookAt;			// 视点
		Vector3			m_vUp;				// 上方向

		Vector3			m_vForward;			// 朝向
		float			m_horizonAngle;		// 水平旋转角度(顺时针)
		float			m_verticleAngle;	// 垂直旋转角度(顺时针)

		float			m_fovH;				// FOV夹角
		float			m_fAspect;			// 纵横比
		float			m_fNearPlane;		// 近平面
		float			m_fFarPlane;		// 远平面
		float			m_screenWidth;		// 屏幕宽(像素)
		float			m_screenHeight;		// 屏幕高(像素)

		Matrix44		m_matView;			// 视图变换矩阵
		Matrix44		m_matProj;			// 投影变换矩阵
		Matrix44		m_matViewProj;		// 观察投影矩阵
		Matrix44		m_matUnitProj;		// 
		Matrix44		m_matSkyView;		// 天空盒观察矩阵
		Matrix44		m_matSkyViewProj;	// 天空盒观察投影矩阵
		Matrix44		m_inverseProj;		// 投影矩阵的逆矩阵

		Matrix44		m_matOrtho;
		Matrix44		m_matViewOrtho;

		Matrix44		m_matUpMatch;		// 设置上方向辅助

		Frustum3		m_frustem;			// 摄像机平截头体
		RenderDeviceType m_deviceType;			// 渲染设备类型
	};
}