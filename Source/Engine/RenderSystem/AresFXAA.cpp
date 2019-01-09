#include <Engine/RenderSystem/AresFXAA.h>
#include <boost/lexical_cast.hpp>
#include <Engine/AresRoot.h>

namespace Ares
{
	// 构造函数
	FXAA::FXAA(  Scene& scene)
		: FrameFilter( scene)
	{
		// 设置Renderable布局
		m_renderable.SetRenderlayout( FrameFilter::GetScreenQuadRenderLayout());

		m_srcTexture = MakeShaderParam();
	}

	// 设置参数
	void FXAA::Set( FXAALevel level)
	{
		// 初始化材质
		ShaderMacros macros;
		macros.push_back( ShaderMacro( "FXAA_PRESET", boost::lexical_cast<string>(level).c_str()));
		RenderMethodPtr rMethod = AResSystem.LoadMethod("FXAA.hlsl");
		m_renderable.GetMaterial()->SetMethod( rMethod);

		m_renderable.GetMaterial()->SetParamRef( "G_WindowWH", m_scene.GetRenderSystem().GetSPABWindowWH());
		m_renderable.GetMaterial()->SetParamRef( "G_DiffuseTex", m_srcTexture);
	}

	// 设置源纹理
	bool FXAA::SetSrcTexture( TexturePtr& src)
	{
		*m_srcTexture = src;

		return true;
	}
}