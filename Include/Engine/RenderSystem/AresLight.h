#pragma once

#include "Engine/SceneSystem/AresEntity.h"
#include <Engine/FxSystem/AresFxEntity.h>

namespace Ares
{
	// 光源类型
	enum LightType
	{
		LT_Ambient   = 0,
		LT_Direction,
		LT_Point,
		LT_Spot,
	};

	//------------------------------------------
	//  光源：2010-08-02 14:37 	    帝林
	//------------------------------------------
	class Light : public Entity
	{
	public:
		// 获取光源类型
		LightType GetLightType() const { return m_lightType; }

		// 获取颜色
		const ColorRGB& GetColor() const { return m_color; }

		// 设置颜色
		void  SetColor( const ColorRGB& color);

		// 判断某点是否处于光源的影响
		bool IsPointIn( const Vector3& pos);

		// 设置使用LightShaft
		virtual void SetLightShaft( const char* fxPath) {}

		// 获取Shaft
		virtual FxEntityPtr GetLightShaft() { return FxEntityPtr(); }	

		// 获取Shaft中心坐标
		virtual float2 GetShaftCenterCoord() { return float2( 0.5f, 0.5f);}

		// 是否使用Shaft
		virtual bool IsUseLightShaft() { return false; }

	public:
		// 获取朝向 LT_Direction
		virtual const Vector3& GetDirection() const;

		// 设置朝向 LT_Direction
		virtual void SetDirection( const Vector3& dir);

		// 获取颜色值
		const ShaderParamPtr& GetSPABColor() const { return m_spabColor; }

		// 设置纹理分辨率
		//virtual void SetShadowMapResolution( size_t resolution=0);

	protected:
		Light( EntityTypeDetail typeDetail);
		~Light();

	protected:
		LightType		m_lightType;	// 光源类型
		ColorRGB		m_color;		// 环境光颜色
		ShaderParamPtr  m_spabColor;	// 颜色值
	};
	typedef s_ptr<Light>  LightPtr;

	//---------------------------------------------------
	// 环境光 2010-08-25  11:31  帝林
	//---------------------------------------------------
	class AmbientLight : public Light
	{
	public:
		// 构造函数
		AmbientLight() : Light( ED_LightAmbient) {}

		// 提交渲染元素
		virtual void SubmitRenderElements( RenderElementMgr& renderElementMgr){}
	};
	typedef s_ptr<AmbientLight> AmbientLightPtr;

	//---------------------------------------------------
	// 聚光灯  2010-08-25    帝林
	//---------------------------------------------------
	class ILightSpot : public Light
	{
	public:
		// 构造函数
		ILightSpot() : Light( ED_LightSpot) {}

		// 获取衰减公式( a, b*d, c*d*d)系数a, b ,c，其中 d为光源位置到顶点的距离
		virtual const Vector3& GetAttenuate()=0;

		// 获取角度的cos值
		virtual const float& GetCosine()=0;

		// 获取光源方向
		virtual const Vector3& GetDirection()=0;

		// 获取位置
		virtual const Vector3& GetPosition()=0;

		// 获取聚光灯的开角
		virtual float& GetRadian()=0;

		// 设置衰减度
		virtual void SetAttenuate( float a, float b, float c)=0;

		// 设置光源方向
		virtual void SetDirection( const Vector3& dir)=0;

		// 设置位置
		virtual void SetPosition( float x, float y, float z)=0;

		// 设置聚光灯的开角
		virtual void SetRadian( float radian)=0;
	};

	//---------------------------------------------------
	// 光源管理接口 ILightManager 2010-08-13  帝林
	//---------------------------------------------------
	class ILightManager
	{
	public:
		// 创建光源
		virtual ILightSpot* CreateSpotLight()=0;

	public:
		// 获取灯光总数
		virtual UINT GetActiveLightNum()=0;

		// 根据索引获取灯光
		virtual Light* GetActiveLight( size_t index)=0;

		// 获取灯源
		virtual Light*  GetLight( size_t index)=0;

		// 获取光源总数
		virtual UINT GetLightNum()=0;

	public:
		// 添加光源
		virtual void  AddActiveLight( Light* pLight ,int len = 0)=0;

		// 清除
		virtual void  ClearActiveLightList()=0;
	};
}