#include <Core/Graphics/AresElementFormat.h>
#include "Engine/AresRoot.h"
#include <Physics/ContBox3.h>
#include <Engine/RenderSystem/AresDirectionalLight.h>

namespace Ares
{
	// 构造函数
	DirectionalLight::DirectionalLight() 
		: Light( ED_LightDirection),
		  m_direction( Vector3::XAxis),
		  m_resolution( 0),
		  m_distance( 50.f)
	{
		//m_spab[SP_ViewCrop]	     = MakeShaderParam( Matrix44::Identity);
		//m_spab[SP_ViewCropRemap] = MakeShaderParam( Matrix44::Identity);
		m_spab[SP_Direction]	 = MakeShaderParam( m_direction);
		m_spab[SP_Color]		 = MakeShaderParam( GetColor());
		//m_spab[SP_DepthMap]		 = MakeShaderParam( m_depthTexture);

		m_csmMgr = CSMMgrPtr( new_ CSMMgr);
	}

	// 设置朝向
	void DirectionalLight::SetDirection( const Vector3& dir)
	{
		// 记录朝向
		m_direction = dir;
		m_direction.Normalize();

		UpdateSPAB();

		UpdateShaftPos();

		if( m_csmMgr)
			m_csmMgr->SetLightDirection( m_direction);
	}

	// 更新SPAB
	void DirectionalLight::UpdateSPAB()
	{
		//*m_spab[SP_ViewCrop]      = m_view * m_crop;
		//*m_spab[SP_ViewCropRemap] = m_view * m_crop * m_remap;
		*m_spab[SP_Direction]	  = m_direction;
		*m_spab[SP_Color]		  = GetColor();
		//*m_spab[SP_DepthMap]	  = m_depthTextureRT;
	}

	// 设置使用LightShaft
	void DirectionalLight::SetLightShaft( const char* fxPath)
	{
		m_shaftPath = fxPath;

		FxEntity* shaftEntity = NULL;

		Serialization::UnSerializeFromBinaryFile( fxPath, shaftEntity);

		m_shaft = FxEntityPtr( shaftEntity);

		UpdateShaftPos();

		m_shaft->OnAddToScene( GetScene());
	}

	// 获取Shaft中心坐标
	float2 DirectionalLight::GetShaftCenterCoord()
	{
		float2 coord( 0.5f, 0.5f);
		if( m_shaft)
		{
			Vector3 pos = -m_direction * 9.9f;
			Vector4 xpos( pos.x, pos.y, pos.z, 1.f);

			Matrix44 skyViewProj = GetCameraSystem().GetParams().m_skyViewProj;

			xpos = skyViewProj.TransformVector4( xpos);
			xpos /= xpos.w;

			coord.x =  (xpos.x+1)/2.f;
			coord.y = -(xpos.y-1)/2.f;
		}

		return coord;
	}

	// 更新
	void  DirectionalLight::FrameMove( float fTime)
	{
		Light::FrameMove( fTime);

		if( m_shaft)
		{
			GetScene()->GetCameraSystem().SetActive( CT_Sky);

			m_shaft->FrameMove( fTime);

			GetScene()->GetCameraSystem().SetActive( CT_Normal);
		}
	}

	// 是否使用Shaft
	bool DirectionalLight::IsUseLightShaft()
	{
		return m_shaft ? true : false;
	}

	// 更新Shaft位置 
	void DirectionalLight::UpdateShaftPos()
	{
		if( m_shaft)
		{
			Vector3 pos = -m_direction * 9.9f;

			Transform transform = m_shaft->GetTransform();
			transform.SetTrans( pos);
			transform.SetScale( 3.f);
			m_shaft->SetTransform( transform);
		}
	}

	// 提交渲染元素
	void DirectionalLight::SubmitRenderElements( RenderElementMgr& renderElementMgr)
	{
		//if( m_shaft)
		//	m_shaft->SubmitRenderElements( renderElementMgr);
	}
}