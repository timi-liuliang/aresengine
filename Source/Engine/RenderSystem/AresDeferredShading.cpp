#include <Engine/AresRoot.h>
#include <Engine/RenderSystem/AresDeferredShading.h>

namespace Ares
{
	// 构造函数
	DeferredShading::DeferredShading( Scene& scene)
		: FrameFilter( scene)
	{
		// 设置Renderable布局
		m_renderable.SetRenderlayout( FrameFilter::GetScreenQuadRenderLayout());

		// 初始化材质
		ShaderMacros macros;
		//macros.push_back( ShaderMacro( "LightBlur", ""));
		RenderMethodPtr rMethod = AResSystem.LoadMethod("DeferredShading.hlsl", macros);
        m_renderable.GetMaterial()->SetMethod( rMethod);

		m_depthTexture	  = MakeShaderParam();
		m_diffuseTexture  = MakeShaderParam();
		m_normalTexture	  = MakeShaderParam();
		m_lightTexture	  = MakeShaderParam();
		m_renderable.GetMaterial()->SetParamRef( "TexOffset",		m_blurTexOffseter.m_texOffset);
		m_renderable.GetMaterial()->SetParamRef( "DiffuseTexture",  m_diffuseTexture);
		m_renderable.GetMaterial()->SetParamRef( "G_NormalTexture", m_normalTexture);
		m_renderable.GetMaterial()->SetParamRef( "DepthTexture",	m_depthTexture);
		m_renderable.GetMaterial()->SetParamRef( "LightTexture",	m_lightTexture);

		DepthStencilStateDesc dDesc;
		dDesc.m_depthEnable = false;
		dDesc.m_depthWriteMask = false;
		DepthStencilStatePtr depthStencilState = ARenderDevice->CreateDepthStencilState( dDesc);
		m_renderable.GetMaterial()->SetDepthStencilState( depthStencilState);
	}

	// 析构函数
	DeferredShading::~DeferredShading()
	{

	}

	// 设置输入
	void DeferredShading::SetInput( const TexturePtr& diffuse, const TexturePtr& normal, const TexturePtr& light, const TexturePtr& depth)
	{
		m_blurTexOffseter.SetInput( light, BlurTexOffseter::WeightTable3x3, 9);
		*m_diffuseTexture = diffuse;
		*m_normalTexture  = normal;
		*m_depthTexture	  = depth;
		*m_lightTexture	  = light;
	}
}