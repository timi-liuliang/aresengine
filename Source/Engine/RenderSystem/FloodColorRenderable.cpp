#include <Engine/RenderSystem/FloodColorRenderable.h>
#include <Engine/AresRoot.h>

namespace Ares
{
	// 构造函数
	FloodColorRenderable::FloodColorRenderable()
	{
        RenderMethodPtr rMethod = AResSystem.LoadMethod( "FloodColorRender.hlsl");
		m_material.SetMethod( rMethod);

		m_worldViewProj = MakeShaderParam();
		m_floodColor	= MakeShaderParam();
		m_material.SetParamRef( "WorldViewProj", m_worldViewProj);
		m_material.SetParamRef( "FloodColor",	 m_floodColor);

		// 混合状态 
		BlendStateDesc bDesc;
		bDesc.m_blendEnable.assign( true);
		bDesc.m_blendOp.assign( BOP_Add);
		bDesc.m_srcBlend.assign( ABF_SrcAlpha);
		bDesc.m_destBlend.assign( ABF_InvSrcAlpha);
		bDesc.m_blendOpAlpha.assign( BOP_Add);
		bDesc.m_srcBlendAlpha.assign( ABF_One);
		bDesc.m_destBlendAlpha.assign( ABF_Zero);
		bDesc.m_colorWriteMask.assign( ColorRGB::MASK_All);
		BlendStatePtr blendState = ARenderDevice->CreateBlendState( bDesc);
		m_material.SetBlendState( blendState, ColorRGB(0.f,0.f,0.f), 0xffffffff);
	}

	// 析构函数
	FloodColorRenderable::~FloodColorRenderable()
	{

	}

	// 渲染
	void FloodColorRenderable::Render( const Matrix44& worldViewProj)
	{
		*m_worldViewProj = worldViewProj;
		*m_floodColor	 = ColorRGB( 0.f, 1.f, 0.f, .8f);

		if( m_layout && m_material.GetMethod())
		{
			ARenderDevice->Render( m_layout.get(), &m_material);
		}
	}
}