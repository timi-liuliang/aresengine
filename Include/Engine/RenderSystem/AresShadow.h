#pragma once

#include <Physics/Rect3.h>
#include <Physics/Frustum3.h>

namespace Ares
{
	//----------------------------------------------
	// 光源空间阴影 2010-12-08 帝林
	//----------------------------------------------
	class ILiSPSM
	{
	public:
		// 初始化
		virtual void Init()=0;

		// 准备渲染shadowmap
		virtual void Begin()=0;

		// 结束渲染
		virtual void End()=0;

		// 设置阴影投射者包围盒
		virtual void SetShadowCastBoundingBox( const Rect3& boundingBox)=0;

		// 获取光源searchRect
		virtual const Rect3& GetLightSearchRect()=0;

		// 获取光源朝向
		virtual const Vector3& GetLightDir()=0;

	public:
		typedef void( *RenderCallBack)( const Rect3&, const Frustum3*);

		// 更新ShadowMap
		virtual void  UpdateShadowMap()=0;

		// 获取光渗透值
		virtual float GetLBRAmount()=0;

		// 设置是否进行模糊
		virtual void SetBlur(bool blur)=0;

		// 设置采样宽度
		virtual void  SetSampleSize( int iSize)=0;

		// 设置Light bleeding reduction
		virtual void  SetLBR( float fLBR)=0;

		// 获取渲染shadowMap的特效文件
		virtual const Matrix44& GetLightMatrix()=0;

		// 获取ShadwMapRefMap
		virtual const Matrix44& GetLightReMapMatrix()=0;

		// 获取shadowMap
		//virtual SPtr<ITexture>& GetShadowTexture()=0;

		// 设置渲染回调函数
		virtual void SetRenderShadowMapFun( void( *pFun)( const Rect3&, const Frustum3* ))=0;
	};
}