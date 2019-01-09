#include <Engine/AresRoot.h>
#include <Core/Graphics/AresRenderWindow.h>
#include <Engine/RenderSystem/AresFrameFilter.h>

namespace Ares
{
	// 构造函数
	FrameFilter::FrameFilter( Scene& scene)
		: m_scene( scene)
	{
	}

	// 构造函数
	FrameFilter::~FrameFilter()
	{ }

	// 执行滤镜
	void FrameFilter::Execute()
	{
		m_renderable.Render();
	}

	// 顶点格式
	struct VertexFormat
	{
		float  m_position[3];		// 顶点位置
		float  m_texcoord[2];		// 贴图坐标
	};

	// 获取ScreenQuadRenderLayout
	RenderLayoutPtr& FrameFilter::GetScreenQuadRenderLayout()
	{
		static RenderLayoutPtr screenQuadRenderLayout;
		if( !screenQuadRenderLayout)
		{
			screenQuadRenderLayout = ARenderDevice->CreateRenderLayout();
			screenQuadRenderLayout->SetTopologyType( RenderLayout::TT_TriangleStrip);

			// 创建辅助图象处理的模型(顶点缓冲)
			VertexFormat fullScreenQuad[4] =
			{
				{{-1.0f, -1.0f, 0.0f},{0.0f, 1.0f}},
				{{-1.0f,  1.0f, 0.0f},{0.0f, 0.0f}},
				{{ 1.0f, -1.0f, 0.0f},{1.0f, 1.0f}},
				{{ 1.0f,  1.0f, 0.0f},{1.0f, 0.0f}}
			};

			ElementInitData initVertexData;
			initVertexData.m_rowPitch   = sizeof( fullScreenQuad);
			initVertexData.m_slicePitch = 0;
			initVertexData.m_data		= fullScreenQuad;
			GraphicBufferPtr posUVBuffer = ARenderDevice->CreateVertexBuffer( BU_Static, EAH_GPURead | EAH_Immutable, &initVertexData);
		
			VertexElementType elementType;
			elementType.push_back( VertexElement( VEU_Position,		0, EF_BGR32F));
			elementType.push_back( VertexElement( VEU_TextureCoord, 0, EF_GR32F));
			
			screenQuadRenderLayout->BindVertexStream( posUVBuffer, elementType);
		}
		
		return screenQuadRenderLayout;
	}
}