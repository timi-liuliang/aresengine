#include <Engine/AresRoot.h>
#include <Engine/RenderSystem/AresGrayscale.h>

namespace Ares
{
	// 构造函数
	GrayscaleFilter::GrayscaleFilter( Scene& scene)
		: FrameFilter( scene)
	{
		// 设置Renderable布局
		m_renderable.SetRenderlayout( FrameFilter::GetScreenQuadRenderLayout());

		// 初始化材?
        RenderMethodPtr rMethod = AResSystem.LoadMethod("Grayscale.hlsl");
		m_renderable.GetMaterial()->SetMethod( rMethod);
	}

	// 设置源纹理
	bool GrayscaleFilter::SetSrcTexture( TexturePtr& src)
	{
		ShaderParamPtr tParam = MakeShaderParam( src);
		//tParam->SetSamplerState( ARenderDevice->CreateSamplerState( SamplerStateDesc()));
		m_renderable.GetMaterial()->SetParamRef( "DiffuseTex", tParam);

		return true;
	}
}