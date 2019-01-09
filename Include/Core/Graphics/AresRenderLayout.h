#pragma once

#include <vector>
#include "AresElementFormat.h"
#include "AresGraphicsBuffer.h"
#include "AresRenderAPITypedefs.h"

using namespace std;

namespace Ares
{
	enum VertexElementUsage
	{
		VEU_Position = 0,
		VEU_Normal,
		VEU_Color,
		VEU_BlendWeight,
		VEU_BlendIndex,
		VEU_TextureCoord,
		VEU_Tangent,
		VEU_Binormal
	};

	//-------------------------------------
	// 顶点格式 2012-4-1(愚人节) 帝林
	//-------------------------------------
	struct VertexElement
	{
		ElementFormat		m_format;
		VertexElementUsage	m_usage;
		BYTE				m_usageIdx;		// 索引

		// 构造函数
		VertexElement() {}

		// 构造函数
		VertexElement( VertexElementUsage usage, BYTE usageIdx, ElementFormat format)
			: m_usage( usage), m_usageIdx( usageIdx), m_format( format)
		{}

		// 大小(Bytes)
		BYTE GetSize() const
		{
			return NumFormatBytes( m_format);
		}

		// 运算符重载 "=="
		friend bool operator == ( const VertexElement& lve, const VertexElement& rve)
		{
			return std::memcmp( &lve, &rve, sizeof(VertexElement))==0;
		}		   
	};
	typedef vector<VertexElement> VertexElementType;

	//-------------------------------------
	// 渲染布局 2012-3-31 帝林
	//-------------------------------------
	class CORE_EXPORT RenderLayout
	{
	public:
		// 拓扑类型
		enum TopologyType
		{
			TT_PointList,
			TT_LineList,
			TT_LineStrip,
			TT_TriangleList,
			TT_TriangleStrip,
			TT_LineListAdj,
			TT_LineStripAdj,
			TT_TriangleListAdj,
			TT_TriangleStripAdj,
			TT_1_CtrlPtPatchList,
			TT_2_CtrlPtPatchList,
			TT_3_CtrlPtPatchList,
			TT_4_CtrlPtPatchList,
			TT_5_CtrlPtPatchList,
			TT_6_CtrlPtPatchList,
			TT_7_CtrlPtPatchList,
			TT_8_CtrlPtPatchList,
			TT_9_CtrlPtPatchList,
			TT_10_CtrlPtPatchList,
			TT_11_CtrlPtPatchList,
			TT_12_CtrlPtPatchList,
			TT_13_CtrlPtPatchList,
			TT_14_CtrlPtPatchList,
			TT_15_CtrlPtPatchList,
			TT_16_CtrlPtPatchList,
			TT_17_CtrlPtPatchList,
			TT_18_CtrlPtPatchList,
			TT_19_CtrlPtPatchList,
			TT_20_CtrlPtPatchList,
			TT_21_CtrlPtPatchList,
			TT_22_CtrlPtPatchList,
			TT_23_CtrlPtPatchList,
			TT_24_CtrlPtPatchList,
			TT_25_CtrlPtPatchList,
			TT_26_CtrlPtPatchList,
			TT_27_CtrlPtPatchList,
			TT_28_CtrlPtPatchList,
			TT_29_CtrlPtPatchList,
			TT_30_CtrlPtPatchList,
			TT_31_CtrlPtPatchList,
			TT_32_CtrlPtPatchList
		};

		enum StreamType
		{
			ST_Geometry,
			ST_Instance,
		};

		// 顶点流
		struct StreamUnit
		{
			UINT				m_stride;		// 点大小
			UINT				m_frequency;	// 频次
			StreamType			m_type;			// 实例化否
			VertexElementType	m_elementType;	// 顶点格式(顶点格式)
			GraphicBufferPtr	m_stream;		// 顶点数据

			// 是否顶点格式相同
			bool IsElementTypeEqueal( const VertexElementType& type);
		};

	public:
		// 构造函数
		RenderLayout();

		// 析构函数
		virtual ~RenderLayout();

		// 获取拓扑类型
		TopologyType GetTopologyType() const { return m_topologyType; }

		// 设置拓扑类型
		virtual void SetTopologyType( TopologyType type) { m_topologyType = type; }

		// 绑定索引流
		virtual void BindIndexStream( const GraphicBufferPtr& IndexStream, ElementFormat format);

		// 绑定顶点流
		virtual void BindVertexStream( const GraphicBufferPtr& vertexStream, const VertexElementType& vertexElements, StreamType stype=ST_Geometry, UINT frequency=1);

	public:
		// 获取顶点数量
		UINT GetNumVertices() const;

		// 获取顶点流数量
		UINT GetNumVertexStreams() const { return static_cast<UINT>(m_vertexStreams.size()); }

		// 获取顶点流
		const GraphicBufferPtr GetVertexStream( const VertexElementType& elementType);

		// 获取顶点流
		const GraphicBufferPtr& GetVertexStream( UINT index) { return m_vertexStreams[index].m_stream; }

		// 获取顶点流格式
		const VertexElementType& GetVertexStreamFormat( UINT index) const { return m_vertexStreams[index].m_elementType; }

		// 获取顶点Stride
		UINT GetVertexStreamStride( UINT index) const { return m_vertexStreams[index].m_stride; }

		// 是否使用索引
		bool IsUseIndices() const { return GetNumIndices()!=0; }

		// 获取索引数量
		UINT GetNumIndices() const;

		// 获取索引
		const GraphicBufferPtr GetIndicesStream() { return m_indexStream; }

		// 设置索引数据
		void SetNumIndices( UINT numIndices) { m_forceNumIndices = numIndices; }

		// 获取开始索引位置
		UINT GetStartIndexLocation() const { return m_startIndexLocation; }

		// 设置开始索引位置
		void SetStartIndexLocation( UINT startIndexLocation) { m_startIndexLocation = startIndexLocation; }

		// 获取顶点索引位置
		UINT GetStartVertexLocation() const { return m_startVertexLocation; }

	protected:
		TopologyType		m_topologyType;		// 拓扑类型

		vector<StreamUnit>	m_vertexStreams;	// 顶点流
		StreamUnit			m_instanceStream;	// 实例化流

		GraphicBufferPtr	m_indexStream;		// 索引流
		ElementFormat		m_indexFormat;		// 索引格式(UINT16 or UINT32)

		UINT				m_forceNumVertices;	// 外部设置
		UINT				m_forceNumIndices;	// 外部设置
		UINT				m_forceNumInstances;

		UINT				m_startVertexLocation;
		UINT				m_startIndexLocation;
		UINT				m_baseVertexLocation;
		UINT				m_startInstanceLocation;
	};
}