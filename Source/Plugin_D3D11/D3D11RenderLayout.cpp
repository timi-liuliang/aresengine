#include "D3D11RenderLayout.h"
#include "D3D11Mapping.h"
#include "D3D11RenderDevice.h"
#include <Core/AresException.h>

namespace Ares
{
	// 构造函数
	D3D11RenderLayout::D3D11RenderLayout()
	{

	}

	// 析构函数
	D3D11RenderLayout::~D3D11RenderLayout()
	{

	}

	// 设置激活
	void D3D11RenderLayout::Render( D3D11RenderLayout* preLayout, D3D11RenderMethod* method)
	{
		static vector<size_t>		  strides(8);
		static vector<size_t>		  offsets(8);
		static vector<ID3D11Buffer*>  pVertexBuffers(8);

		const ID3D11DeviceContextPtr& devCtx    = D3D11RenderDevice::GetSingleton().GetD3DDeviceImmContext();
		size_t allNumVertexStream				= m_vertexStreams.size() + ( m_instanceStream.m_stream ? 1 : 0);

		// 1.TopologyType
		PDRenderDevice.SetTopologyType( m_d3d11TopologyType);

		// 2.InputLayout
		const ID3D11InputLayoutPtr& inputLayout = GetInputLayout( method->GetVSCode());
		devCtx->IASetInputLayout( inputLayout.get());
		A_ASSERT( inputLayout.get());

		// 3.Vertex Instance Streams
		UINT numIndices = GetNumIndices();
		if( allNumVertexStream)
		{
			strides.resize(allNumVertexStream);
			offsets.resize(allNumVertexStream);
			pVertexBuffers.resize(allNumVertexStream);
			for ( size_t i=0; i<GetNumVertexStreams(); i++)
			{
				s_ptr<D3D11GraphicsBuffer> vertexBuffer = dynamic_pointer_cast<D3D11GraphicsBuffer>( m_vertexStreams[i].m_stream);

				pVertexBuffers[i] = vertexBuffer->GetD3D11Buffer().get();
				strides[i] = m_vertexStreams[i].m_stride;
				offsets[i] = 0;
			}

			devCtx->IASetVertexBuffers( 0, allNumVertexStream, pVertexBuffers.data(), strides.data(), offsets.data());
			A_ASSERT( pVertexBuffers.data());

			if( !numIndices)
			{
				UINT numVertices = GetNumVertices();

				devCtx->Draw( numVertices, m_startVertexLocation);
			}
		}

		// 4.IndexBuffer and draw
		if( numIndices)
		{
			D3D11GraphicsBuffer* indexBuffer = dynamic_cast<D3D11GraphicsBuffer*>( m_indexStream.get());
			devCtx->IASetIndexBuffer( indexBuffer->GetD3D11Buffer().get(), D3D11Mapping::MappingFormat( m_indexFormat), 0);
			A_ASSERT( indexBuffer->GetD3D11Buffer().get());

			devCtx->DrawIndexed( numIndices, m_startIndexLocation, m_baseVertexLocation);
		}
	}

	// 获取InputLayout
	const ID3D11InputLayoutPtr& D3D11RenderLayout::GetInputLayout( ID3D10Blob* signature) const
	{
		A_ASSERT( signature);

		InputLayoutMaps::const_iterator it = m_inputLayouts.find( signature);
		if( it != m_inputLayouts.end())
			return it->second;
	
		vector<D3D11_INPUT_ELEMENT_DESC> elements;
		elements.reserve( m_vertexStreams.size());
		for ( size_t i=0; i<GetNumVertexStreams(); i++)
		{
			vector<D3D11_INPUT_ELEMENT_DESC> streamElements;
			D3D11Mapping::MappingInputElement( streamElements, i, GetVertexStreamFormat(i), m_vertexStreams[i].m_type, m_vertexStreams[i].m_frequency);
		
			elements.insert( elements.end(), streamElements.begin(), streamElements.end());
		}

		ID3D11InputLayout* inputLayout = NULL;
		ID3D11DevicePtr& d3d11Device = D3D11RenderDevice::GetSingleton().GetD3DDevice();

		if( d3d11Device)
		{
			TIF( d3d11Device->CreateInputLayout( elements.data(), elements.size(), signature->GetBufferPointer(), signature->GetBufferSize(), &inputLayout));
		}
	
		m_inputLayouts[signature] = MakeCOMPtr( inputLayout);

		return m_inputLayouts[signature];
	}

	// 设置拓扑类型
	void D3D11RenderLayout::SetTopologyType( TopologyType type)
	{
		RenderLayout::SetTopologyType( type);

		m_d3d11TopologyType = D3D11Mapping::Mapping( m_topologyType);
	}
}