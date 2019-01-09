#include <Engine/AresRoot.h>
#include <Engine/RenderSystem/AresSpaceDistorted.h>

namespace Ares
{
	// 构造函数
	SpaceDistorted::SpaceDistorted( Scene& scene)
		: FrameFilter( scene)
	{
		// 设置Renderable布局
		m_renderable.SetRenderlayout( FrameFilter::GetScreenQuadRenderLayout());

		// 初始化材?
        RenderMethodPtr rMethod = AResSystem.LoadMethod("SpaceDistorted.hlsl");
		m_renderable.GetMaterial()->SetMethod( rMethod);

		// 创建DistortedTexture DistortedFrameBuffer
		const RenderWindow* window = m_scene.GetRenderSystem().GetRenderWindow();
		m_distortedFrameBuffer	= ARenderDevice->CreateFrameBuffer();
		m_distortedTexture	= ARenderDevice->CreateTexture2D( window->GetWidth(), window->GetHeight(), 1, 1, EF_ARGB8, 1, 0, EAH_GPURead | EAH_GPUWrite, NULL);
		m_distortedFrameBuffer->AttachRenderView( FrameBuffer::ATT_Color0,	   ARenderDevice->Create2DRenderView( m_distortedTexture, 0, 1, 0));
	};

	// 设置源纹理
	bool SpaceDistorted::SetSrcTexture( TexturePtr& src)
	{
		ShaderParamPtr tParam = MakeShaderParam( src);
		m_renderable.GetMaterial()->SetParamRef( "DiffuseTex", tParam);

		ShaderParamPtr tDistorted = MakeShaderParam( m_distortedTexture);
		m_renderable.GetMaterial()->SetParamRef( "DistortedTex", tDistorted);

		return true;
	}
}