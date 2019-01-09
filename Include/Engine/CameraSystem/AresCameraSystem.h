//--------------------------------------
// http://www.quyougame.com AresRender
//--------------------------------------
#pragma once

#include <stdio.h>
#include <Physics/Frustum3.h>
#include <Core/Graphics/AresRenderMethod.h>

namespace Ares
{
	enum CameraType
	{
		CT_Normal = 0,	// 常规摄像机
		CT_Sky,			// 天空盒摄像机
		CT_Total,
	};

	//-------------------------------------------------------
	// 摄像机接口  2010-10-11         帝林
	//-------------------------------------------------------
	class CameraSystem
	{
	public:
		struct Params
		{
			Matrix44	m_skyViewProj;
		};

	public:
		// shader param type
		enum SPType
		{
			SP_View		= 0,			// 观察矩阵
			SP_ViewProj,				// 摄像机观察投影矩阵
			SP_SkyViewProj,				// 天空盒观察投影矩阵
			SP_Pos,						// 摄像机位置
			SP_NearFar,					// 摄像机近远截面
			SP_Right,
			SP_Up,
			SP_Front,
			SP_Direction,				// 摄像机朝向
			SP_Total,
		};

		CameraSystem();
		~CameraSystem();

		// 获取近截面
		virtual float GetNear()=0;

		// 获取远截面
		virtual float GetFar()=0;

		// 获取朝向
		virtual Vector3& GetFacing()=0;

		// 获取观察矩阵
		virtual Matrix44& GetView()=0;

		// 获取观察矩阵旋转的逆
		Matrix44 GetViewRotationInverse();

		// 获取投影矩阵
		virtual Matrix44& GetProj()=0;

		// 获取观察投影矩阵
		virtual Matrix44& GetViewProj()=0;

		// 正交矩阵
		virtual const Matrix44& GetOrtho()=0;

		// 观察正交矩阵
		virtual const Matrix44& GetViewOrtho()=0;

		// 获取摄像机位置
		virtual Vector3& GetPosition()=0;

		// 获取活动摄像机搜寻包围盒
		virtual const Rect3& GetSearchRect()=0;

		// 获取活动摄像机平截头体
		virtual Frustum3* GetFrustum()=0;

		// 设置活跃摄像机
		void SetActive( CameraType type);

		// 获取参数
		const Params& GetParams() { return m_params; }

	public:
		// 获取正交投影
		virtual ShaderParamPtr& GetVpOrthoProj()=0;

		// 更新Shader参数
		virtual void UpdateSPAB()=0;
        
        // 每帧更新
		virtual void FrameMove(float fTime)=0;

		// 获取shader绑定参数
		ShaderParamPtr GetSPAB( SPType type) const;

	protected:
		Params									m_params;	// 参数
		CameraType								m_active;	// 当前活动摄像机
		boost::array<ShaderParamPtr, SP_Total>	m_spab;		// ShaderParamAutoBind
	};
}
