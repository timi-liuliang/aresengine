#include <Engine/RenderSystem/AresFloodColor.h>
#include <Engine/AresRoot.h>

namespace Ares
{
	FloodColor::FloodColor( Scene& scene)
		: FrameFilter( scene)
		, m_bufferWidth( INVALID)
		, m_bufferHeight( INVALID)
	{
		// 设置Renderable布局
		m_renderable.SetRenderlayout( FrameFilter::GetScreenQuadRenderLayout());

		// 初始化材?
        RenderMethodPtr rMethod = AResSystem.LoadMethod("FloodColor.hlsl");
		m_renderable.GetMaterial()->SetMethod( rMethod);

		m_bufferTexture = MakeShaderParam();
		m_renderable.GetMaterial()->SetParamRef( "BufferTexture", m_bufferTexture);

		// 混合状态 
		BlendStateDesc bDesc;
		bDesc.m_blendEnable.assign( true);
		bDesc.m_blendOp.assign( BOP_Add);
		bDesc.m_srcBlend.assign( ABF_One);
		bDesc.m_destBlend.assign( ABF_One);
		bDesc.m_blendOpAlpha.assign( BOP_Add);
		bDesc.m_srcBlendAlpha.assign( ABF_One);
		bDesc.m_destBlendAlpha.assign( ABF_Zero);
		bDesc.m_colorWriteMask.assign( ColorRGB::MASK_All);
		BlendStatePtr blendState = ARenderDevice->CreateBlendState( bDesc);
		m_renderable.GetMaterial()->SetBlendState( blendState, ColorRGB(0.f,0.f,0.f), 0xffffffff);

		DepthStencilStateDesc dDesc;
		dDesc.m_depthEnable = false;
		dDesc.m_depthWriteMask = false;
		DepthStencilStatePtr depthStencilState = ARenderDevice->CreateDepthStencilState( dDesc);
		m_renderable.GetMaterial()->SetDepthStencilState( depthStencilState);

		m_gaussianBlur = MakeSharePtr( new_ GaussianBlur( scene));
	}

	FloodColor::~FloodColor()
	{

	}

	// 更改窗口大小
	void FloodColor::Resize( size_t width, size_t height)
	{
		if( m_bufferWidth!=width || m_bufferHeight!=height)
		{
			m_bufferWidth  = static_cast<UINT>( static_cast<float>(width)/* / 4.f*/);
			m_bufferHeight = static_cast<UINT>( static_cast<float>(height)/* / 4.f*/);

			m_frameBuffer			= ARenderDevice->CreateFrameBuffer();
			m_frameTexture			= ARenderDevice->CreateTexture2D( m_bufferWidth,   m_bufferHeight, 1, 1, EF_ARGB8,  1, 0, EAH_GPURead | EAH_GPUWrite, NULL);
			m_frameBuffer->AttachRenderView( FrameBuffer::ATT_Color0, ARenderDevice->Create2DRenderView( m_frameTexture, 0, 1, 0));

			m_bufferWidth = width;
			m_bufferHeight= height;
		}
	}

	// 执行滤镜
	void FloodColor::Process( vector<EntityPtr>& entitys, RenderViewPtr& iDepth, FrameBufferPtr& output)
	{
		m_frameBuffer->AttachRenderView( FrameBuffer::ATT_DepthStencil, iDepth);
		m_frameBuffer->Clear( ColorRGB( 0.f, 0.f, 0.f, 0.f), 1, 0, CBM_Color);

		// 1 输出
		ARenderDevice->BindFrameBuffer( m_frameBuffer);

		// 2.渲染
		m_floodRenderMgr.Write( entitys);

		*m_bufferTexture	= m_frameTexture;

		// 3.高斯模糊
		m_gaussianBlur->SetInput( m_frameTexture, m_frameTexture, GaussianBlur::WeightTable1x49, 49);
		m_gaussianBlur->Execute();

		// 4.绑定输出帧
		ARenderDevice->BindFrameBuffer( output);

		FrameFilter::Execute();
	}
}