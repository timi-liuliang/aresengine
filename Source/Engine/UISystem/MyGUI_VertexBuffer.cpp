#include "Engine/UISystem/MyGUI_VertexBuffer.h"
#include "Engine/AresRoot.h"

namespace MyGUI
{
	// 构造函数
	MyGUIBuffer::MyGUIBuffer()
		: m_vertexNum( 0)
	{
		
	}

	// 析构函数
	MyGUIBuffer::~MyGUIBuffer()
	{

	}

	// 设置顶点数量
	void MyGUIBuffer::setVertexCount( size_t count)
	{
		if( count)
		{
			m_layout = ARenderDevice->CreateRenderLayout();
			m_layout->SetTopologyType( RenderLayout::TT_TriangleList);

			// 顶点类型
			VertexElementType elementType;
			elementType.push_back( VertexElement(VEU_Position,     0, EF_BGR32F));
			elementType.push_back( VertexElement(VEU_TextureCoord, 0, EF_GR32F));
			elementType.push_back( VertexElement(VEU_Color,      0, EF_ARGB8));

			// 设置顶点
			vector<Vertex> data; 
			data.resize(count);
			ElementInitData initVertexData;
			initVertexData.m_rowPitch   = sizeof(Vertex) * data.size();
			initVertexData.m_slicePitch = 0;
			initVertexData.m_data		= data.data();
			m_buffer = ARenderDevice->CreateVertexBuffer( BU_Dynamic, EAH_CPUWrite | EAH_GPURead, &initVertexData);

			m_layout->BindVertexStream( m_buffer, elementType);
		}
	}

	size_t MyGUIBuffer::getVertexCount()
	{
		return m_vertexNum;
	}
}