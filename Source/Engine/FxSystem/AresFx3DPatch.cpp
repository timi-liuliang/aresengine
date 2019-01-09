#include <Core/Graphics/AresRenderMethod.h>
#include <Core/Graphics/AresRenderState.h>
#include "Engine/RenderSystem/AresRenderable.h"
#include <Engine/FxSystem/AresFx3DPatch.h>
#include <Engine/FxSystem/AresFxEntity.h>
#include <Engine/AresRoot.h>

BOOST_CLASS_EXPORT( Ares::Fx3DPatch)

namespace Ares
{
	// 构造函数
	Fx3DPatch::Fx3DPatch()
		: FxObject( FOT_3DPatch)
		, m_color( ColorRGB::White)
	{
		m_renderable = MakeSharePtr( new FxRenderable());

		UpdateBuffer();
	}

	// 虚析构函数
	Fx3DPatch::~Fx3DPatch()
	{
		m_renderable.reset();
	}

	// 更新Buffer
	void Fx3DPatch::UpdateBuffer()
	{
		// 顶点缓冲
		vector<FxRenderable::VertexFormat> patch3d( 4);
		patch3d[0].m_position = float3( -0.5f, -0.5f, 0.f); patch3d[0].m_color = m_color; patch3d[0].m_texcoord = float2( 0.0f, 1.0f);
		patch3d[1].m_position = float3( -0.5f,  0.5f, 0.f); patch3d[1].m_color = m_color; patch3d[1].m_texcoord = float2( 0.0f, 0.0f);
		patch3d[2].m_position = float3(  0.5f, -0.5f, 0.f); patch3d[2].m_color = m_color; patch3d[2].m_texcoord = float2( 1.0f, 1.0f);
		patch3d[3].m_position = float3(  0.5f,  0.5f, 0.f); patch3d[3].m_color = m_color; patch3d[3].m_texcoord = float2( 1.0f, 0.0f);
		
		// 索引
		vector<WORD> indices;
		indices.push_back( 0); indices.push_back( 1); indices.push_back( 2); indices.push_back( 3);

		// 更新到渲染
		m_renderable->UpdateBuffer( RenderLayout::TT_TriangleStrip, patch3d.data(), sizeof(FxRenderable::VertexFormat)*patch3d.size(), indices.data(), sizeof(WORD)*indices.size());
	}

	// 设置颜色
	void Fx3DPatch::SetColor( const ColorRGB& color)
	{
		m_color = color;

		UpdateBuffer();
	}

	// 更新
	void Fx3DPatch::FrameMove( float elpasedTime, FxEntity* fxEntity)
	{
		m_renderable->FrameMove( elpasedTime, fxEntity, this);
	}

	// 执行
	void Fx3DPatch::SubmitRenderElements( RenderElementMgr& renderElementMgr)
	{
		m_renderable->SubmitRenderElements( renderElementMgr);
	}
}