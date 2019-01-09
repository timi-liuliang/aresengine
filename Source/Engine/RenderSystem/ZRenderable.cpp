#include <Engine/RenderSystem/ZRenderable.h>
#include <Engine/AresRoot.h>

namespace Ares
{
	// 构造函数
	ZRenderable::ZRenderable( bool isZTest)
	{
		ShaderMacros macros;
		if( isZTest)
			macros.push_back( ShaderMacro("ZTESTSUPPORT", ""));

        RenderMethodPtr rMethod = AResSystem.LoadMethod("ZPrestatic.hlsl", macros);
		m_material.SetMethod( rMethod);

		m_worldViewProj = MakeShaderParam();
		m_material.SetParamRef( "WorldViewProj", m_worldViewProj);

		if( isZTest)
		{
			m_maskTexture   = MakeShaderParam();
			m_material.SetParamRef( "G_DiffuseTex", m_maskTexture);
		}
	}

	// 析构函数
	ZRenderable::~ZRenderable()
	{

	}

	// 设置纹理
	void ZRenderable::SetMaskTexture( ShaderParamPtr mask)
	{
		*m_maskTexture = *(mask->m_texture);
	}

	// 渲染
	void ZRenderable::Render( const Matrix44& worldViewProj)
	{
		*m_worldViewProj = worldViewProj;

		if( m_layout && m_material.GetMethod())
		{
			ARenderDevice->Render( m_layout.get(), &m_material);
		}
	}
}