#include "MOLightmapUVRender.h"
#include <Engine/AresRoot.h>
#include <Core/AresMath.h>

namespace Ares
{
	// 构造函数
	LightmapUVRender::LightmapUVRender()
	{
		// 初始化材质
		m_material->SetMethod( AResSystem.LoadMethod("uvatlas.hlsl"));
	}

	// 析构函数
	LightmapUVRender::~LightmapUVRender()
	{
	}

	// 设置显示数据
	void LightmapUVRender::Set(RenderLayout::TopologyType topologyType, const void* positions, int vertexSizeInByte, const void* indices, int indicesSizeInByte, ElementFormat indiceFormat/*=EF_R16UI*/)
	{
		// define elementType
		VertexElementType elementTypePos;
		elementTypePos.push_back( VertexElement(VEU_Position, 0, EF_GR32F));

		if( !m_layout)
		{
			// 1.设置拓扑类型
			m_layout = ARenderDevice->CreateRenderLayout();
			m_layout->SetTopologyType( topologyType);

			// 2.设置顶点
			ElementInitData initVertexData;
			initVertexData.m_rowPitch   = vertexSizeInByte;
			initVertexData.m_slicePitch = 0;
			initVertexData.m_data		= positions;
			GraphicBufferPtr vertexPosBuffer = ARenderDevice->CreateVertexBuffer( BU_Dynamic, EAH_CPUWrite | EAH_GPURead, &initVertexData);
			m_layout->BindVertexStream( vertexPosBuffer, elementTypePos);

			// 索引
			if( indices)
			{
				ElementInitData initIndexData;
				initIndexData.m_rowPitch   = indicesSizeInByte;
				initIndexData.m_slicePitch = 0;
				initIndexData.m_data	   = indices;
				GraphicBufferPtr indexBuffer = ARenderDevice->CreateIndexBuffer( BU_Dynamic, EAH_CPUWrite | EAH_GPURead, &initIndexData);
				m_layout->BindIndexStream( indexBuffer, indiceFormat);
			}
		}
		else
		{
			m_layout->SetTopologyType( topologyType);

			// 2.更新顶点数据
			GraphicBufferPtr vertexPosBuffer = m_layout->GetVertexStream( elementTypePos);
			if( vertexPosBuffer)
			{
				vertexPosBuffer->Resize( vertexSizeInByte);
				GraphicsBuffer::Modifier tModifier( vertexPosBuffer, BA_WriteOnly);

				tModifier.Set( positions, vertexSizeInByte);
			}

			// 索引
			if( indices)
			{
				GraphicBufferPtr indexBuffer = m_layout->GetIndicesStream();
				if( indexBuffer)
				{
					indexBuffer->Resize( indicesSizeInByte);
					GraphicsBuffer::Modifier tModifier( indexBuffer, BA_WriteOnly);

					tModifier.Set( indices, indicesSizeInByte);
				}
			}
		}
	}
}