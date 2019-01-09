#include <Engine/RenderSystem/AresDepthOfField.h>
#include <Engine/AresRoot.h>

namespace Ares
{
	// 构造函数
	DepthOfField::DepthOfField(  Scene& scene)
		: FrameFilter( scene)
	{
		// 设置Renderable布局
		m_renderable.SetRenderlayout( FrameFilter::GetScreenQuadRenderLayout());

		// 初始化材?
        RenderMethodPtr rMethod = AResSystem.LoadMethod("DepthOfField.hlsl");
		m_renderable.GetMaterial()->SetMethod( rMethod);

		// 参数
		m_zInv		   = MakeShaderParam();
		m_depthOfField = MakeShaderParam();
		m_bufferTexture= MakeShaderParam();
		m_depthTexture = MakeShaderParam();
		m_renderable.GetMaterial()->SetParamRef( "DepthOfField",m_depthOfField);
		m_renderable.GetMaterial()->SetParamRef( "ZInv",		  m_zInv);
		m_renderable.GetMaterial()->SetParamRef( "TexOffset",	  m_blurTexOffseter.m_texOffset);
		m_renderable.GetMaterial()->SetParamRef( "BufferTexture", m_bufferTexture);
		m_renderable.GetMaterial()->SetParamRef( "DepthTexture",  m_depthTexture);

		DepthStencilStateDesc dDesc;
		dDesc.m_depthEnable = false;
		dDesc.m_depthWriteMask = false;
		DepthStencilStatePtr depthStencilState = ARenderDevice->CreateDepthStencilState( dDesc);
		m_renderable.GetMaterial()->SetDepthStencilState( depthStencilState);
	}

	// 析构函数
	DepthOfField::~DepthOfField()
	{

	}

	// 更改窗口大小
	void DepthOfField::Resize( size_t width, size_t height)
	{
		if( !m_frameTexture || m_frameTexture->GetWidth(0)!=width || m_frameTexture->GetHeight(0)!=height)
		{
			m_frameBuffer		= ARenderDevice->CreateFrameBuffer();
			m_frameTexture		= ARenderDevice->CreateTexture2D( width,   height, 1, 1, EF_ARGB8,  1, 0, EAH_GPURead | EAH_GPUWrite, NULL);
			m_frameDepth		= ARenderDevice->CreateTexture2D( width, height, 1, 1, EF_D32F,  1, 0, EAH_GPURead | EAH_GPUWrite, NULL);
			m_frameBuffer->AttachRenderView( FrameBuffer::ATT_Color0,       ARenderDevice->Create2DRenderView( m_frameTexture, 0, 1, 0));
			m_frameBuffer->AttachRenderView( FrameBuffer::ATT_DepthStencil, ARenderDevice->Create2DDepthStencilRenderView( m_frameDepth, 0, 1, 0));

			m_blurTexOffseter.SetInput( m_frameTexture, BlurTexOffseter::WeightTable7x7, 49);
		}
	}

	// 执行滤镜
	void DepthOfField::Process( FrameBufferPtr output)
	{
		CameraSystem& cameraSystem = m_scene.GetCameraSystem();

		ARenderDevice->BindFrameBuffer( output);

		// zInv
		Matrix44 invProj;
		Matrix44Inverse( invProj, cameraSystem.GetProj());
		float4   zInv = float4( invProj._14, invProj._24, invProj._34, invProj._44);
		*m_zInv       = zInv;

		*m_depthOfField  = float4( 20.f, 100.f, 20.f, 100.f);
		*m_bufferTexture = m_frameTexture;
		*m_depthTexture  = m_frameDepth;

		Execute();
	}
}