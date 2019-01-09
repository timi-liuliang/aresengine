#include "Core/Graphics/AresRenderLayout.h"

namespace Ares
{
	// 是否顶点格式相同
	bool RenderLayout::StreamUnit::IsElementTypeEqueal( const VertexElementType& type)
	{
		if( m_elementType.size() != type.size())
			return false;

		for( size_t i=0; i<m_elementType.size(); i++)
		{
			if( !(m_elementType[i] == type[i]))
				return false;
		}

		return true;
	}

	// 构造函数
	RenderLayout::RenderLayout()
		: m_topologyType( TT_PointList),
		  m_indexFormat( EF_UnKnown),
		  m_forceNumInstances( 0),
		  m_startVertexLocation( 0),
		  m_startIndexLocation( 0),
		  m_baseVertexLocation( 0),
		  m_startInstanceLocation( 0)
		 ,m_forceNumIndices(INVALID)
		 ,m_forceNumVertices(INVALID)
	{
		m_vertexStreams.reserve(1);
	}

	// 析构函数
	RenderLayout::~RenderLayout()
	{
		int a = 10;
	}

	// 获取索引数量
	UINT RenderLayout::GetNumIndices() const 
	{ 
		if( m_indexStream)
		{
			if( INVALID==m_forceNumIndices)
				return m_indexStream->GetSize() / NumFormatBytes( m_indexFormat);
			else
				return m_forceNumIndices;
		}

		return 0; 
	}

	// 获取顶点数量
	UINT RenderLayout::GetNumVertices() const 
	{ 
		if( m_forceNumVertices==INVALID)
			return m_vertexStreams[0].m_stream->GetSize() / m_vertexStreams[0].m_stride;
			
		return m_forceNumVertices;
	}

	// 绑定索引流
	void RenderLayout::BindIndexStream( const GraphicBufferPtr& IndexStream, ElementFormat format)
	{
		BOOST_ASSERT( format==EF_R16UI || format==EF_R32UI);

		m_indexFormat = format;
		m_indexStream = IndexStream;
	}

	// 绑定顶点流
	void RenderLayout::BindVertexStream( const GraphicBufferPtr& vertexStream, const VertexElementType& vertexElements, StreamType stype/*=ST_Geometry*/, UINT frequency/*=1*/)
	{
		BOOST_ASSERT( vertexStream);

		UINT stride = 0;
		for ( size_t i=0; i<vertexElements.size(); i++)
		{
			stride += vertexElements[i].GetSize();
		}

		if( ST_Geometry == stype)
		{
			for ( size_t i=0; i<m_vertexStreams.size(); i++)
			{
				if( m_vertexStreams[i].IsElementTypeEqueal( vertexElements))
				{
					m_vertexStreams[i].m_stream		= vertexStream;
					m_vertexStreams[i].m_stride		= stride;
					m_vertexStreams[i].m_type		= stype;
					m_vertexStreams[i].m_frequency	= frequency;

					return;
				}
			}

			StreamUnit vs;
			vs.m_stream		= vertexStream;
			vs.m_elementType= vertexElements;
			vs.m_stride		= stride;
			vs.m_type		= stype;
			vs.m_frequency	= frequency;
			m_vertexStreams.push_back( vs);
		}
		else
		{
			m_instanceStream.m_stream	  = vertexStream;
			m_instanceStream.m_elementType= vertexElements;
			m_instanceStream.m_stride	  = stride;
			m_instanceStream.m_type		  = stype;
			m_instanceStream.m_frequency  = frequency;
		}
	}

	// 获取顶点流
	const GraphicBufferPtr RenderLayout::GetVertexStream( const VertexElementType& elementType)
	{
		// 获取顶点流数量
		for( size_t i=0; i<m_vertexStreams.size(); i++)
		{
			if( m_vertexStreams[i].IsElementTypeEqueal( elementType))
				return m_vertexStreams[i].m_stream;
		}

		return GraphicBufferPtr();
	}
}