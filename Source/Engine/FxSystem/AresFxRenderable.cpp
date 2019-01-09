#include <Engine/AresRoot.h>
#include <Engine/FxSystem/AresFxRenderable.h>
#include <Engine/FxSystem/AresFxParticleSystem.h>
#include <Engine/SceneSystem/AresScene.h>

namespace Ares
{
	FxRenderable::FxRenderable()
		: Renderable()
		, m_isBillBoard( false)
	{
		// 渲染队列
		m_material->SetRenderQueue( RQ_TransparentEffect);

		// 材质初始化
		m_spabWVP = MakeShaderParam( Matrix44::Identity);
        RenderMethodPtr renderMethod = AResSystem.LoadMethod("FxRender.hlsl");
		m_material->SetMethod( renderMethod);
		m_material->SetParamRef( "WorldViewProj", m_spabWVP);

		// 设置光栅化状态
		RasterizerStateDesc desc;
		desc.m_cullMode = CM_None;
		RasterizerStatePtr rasterizerState = ARenderDevice->CreateRasterizerState( desc);
		m_material->SetRasterizerState( rasterizerState);

		// 设置深度状态
		DepthStencilStateDesc dsDesc;
		dsDesc.m_depthWriteMask = false;
		DepthStencilStatePtr depthStencilState = ARenderDevice->CreateDepthStencilState( dsDesc);
		m_material->SetDepthStencilState( depthStencilState);

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
		m_material->SetBlendState( blendState, ColorRGB(0.f,0.f,0.f), 0xffffffff);

		// 初始化RenderLayout
		m_layout = ARenderDevice->CreateRenderLayout(); A_ASSERT( m_layout);
		m_layout->SetTopologyType( RenderLayout::TT_TriangleList);

		GraphicBufferPtr posUVBuffer = ARenderDevice->CreateVertexBuffer( BU_Static, EAH_CPUWrite | EAH_GPURead, NULL);

		m_elementType.push_back( VertexElement( VEU_Position,	  0, EF_BGR32F));
		m_elementType.push_back( VertexElement( VEU_TextureCoord, 0, EF_ABGR32F));
		m_elementType.push_back( VertexElement( VEU_TextureCoord, 1, EF_GR32F));

		m_layout->BindVertexStream( posUVBuffer, m_elementType);

		GraphicBufferPtr indexBuffer = ARenderDevice->CreateIndexBuffer( BU_Dynamic, EAH_CPUWrite | EAH_GPURead, NULL);
		m_layout->BindIndexStream( indexBuffer, EF_R16UI);
	}

	// 更新渲染缓冲
	void FxRenderable::UpdateBuffer( RenderLayout::TopologyType topologyType,const VertexFormat* vertices, int vertexSizeInByte, WORD* indices, int indicesSizeInByte)
	{
		A_ASSERT( m_layout);

		m_layout->SetTopologyType( topologyType);

		// 2.更新顶点数据
		GraphicBufferPtr vertexBuffer = m_layout->GetVertexStream( m_elementType);
		if( vertexBuffer && vertexSizeInByte)
		{
			vertexBuffer->Resize( vertexSizeInByte);
			GraphicsBuffer::Modifier tModifier( vertexBuffer, BA_WriteOnly);

			tModifier.Set( vertices, vertexSizeInByte);
		}

		// 索引
		GraphicBufferPtr indexBuffer = m_layout->GetIndicesStream();
		if( indexBuffer && indicesSizeInByte)
		{
			indexBuffer->Resize( indicesSizeInByte);
			GraphicsBuffer::Modifier tModifier( indexBuffer, BA_WriteOnly);

			tModifier.Set( indices, indicesSizeInByte);
		}
	}

	// 更新
	void FxRenderable::FrameMove( float elpasedTime, FxEntity* fxEntity, FxObject* fxObject)
	{
		CameraSystem& cameraSystem = fxEntity->GetScene()->GetCameraSystem();

		Matrix44 world = fxObject->GetTransform()->GetMatrix();
		if( fxEntity)
			world = fxEntity->GetTransform().GetMatrix() * world;

		if( m_isBillBoard)
			*m_spabWVP = cameraSystem.GetViewRotationInverse() * world * cameraSystem.GetViewProj();
		else
			*m_spabWVP = world * cameraSystem.GetViewProj();
	}

	// 设置为空间扭曲
	void FxRenderable::SetRenderQueue( BITFIELD renderQueue)
	{
		GetMaterial()->SetRenderQueue( renderQueue);
	}

	// 设置纹理
	void FxRenderable::SetTexture( const char* texturePath)
	{
		m_texture = texturePath;

		TexturePtr texture = AResSystem.SyncLoadDDS( texturePath, EAH_GPURead | EAH_Immutable);
		if( texture)
			m_material->SetParamRef( "g_diffuse", MakeShaderParam( texture));
	}
}