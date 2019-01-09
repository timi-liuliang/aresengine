#include "D3D11GraphicsBuffer.h"
#include "D3D11RenderDevice.h"
#include "D3D11Mapping.h"
#include <Core/AresCore.h>

namespace Ares
{
	// 构造函数
	D3D11GraphicsBuffer::D3D11GraphicsBuffer( BufferUsage usage, UINT accessHint, UINT bindFlags, const ElementInitData* initData, ElementFormat format)
		: GraphicsBuffer( usage, accessHint),
		  m_bindFlags( bindFlags),
		  m_hwBufSize( 0),
		  m_formatAsShaderRes( format)
	{
		// 修改绑定标记
		if( accessHint & EAH_GPUUnordered && m_formatAsShaderRes != EF_UnKnown)
			m_bindFlags = 0;

		D3D11RenderDevice* renderFactory = D3D11RenderDevice::GetSingletonPtr();
		m_d3d11Device = renderFactory->GetD3DDevice();
		m_d3d11ImmCtx = renderFactory->GetD3DDeviceImmContext();
		m_sizeInByte  = 0;

		if( initData && (initData->m_rowPitch || initData->m_slicePitch))
		{
			// Init Buffer description
			D3D11_BUFFER_DESC desc;
			GetD3DFlags( desc.Usage, desc.CPUAccessFlags, desc.BindFlags, desc.MiscFlags);
			desc.ByteWidth           = initData->m_rowPitch;
			desc.StructureByteStride = NumFormatBits( m_formatAsShaderRes);

			m_sizeInByte = initData->m_rowPitch;

			D3D11_SUBRESOURCE_DATA subresInit;
			subresInit.pSysMem			= initData->m_data;
			subresInit.SysMemPitch		= initData->m_rowPitch;
			subresInit.SysMemSlicePitch = initData->m_slicePitch;

			ID3D11Buffer* buffer;
			TIF( m_d3d11Device->CreateBuffer( &desc, &subresInit, &buffer));
			m_d3d11Buffer = MakeCOMPtr( buffer);
			m_hwBufSize   = GetSize();

			if( (accessHint & EAH_GPURead) && (m_formatAsShaderRes!=EF_UnKnown))
			{
				D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceDesc;
				shaderResourceDesc.Format				= m_accessHint & EAH_GPUStructured ? DXGI_FORMAT_UNKNOWN : D3D11Mapping::MappingFormat( m_formatAsShaderRes);
				shaderResourceDesc.ViewDimension		= D3D11_SRV_DIMENSION_BUFFER;
				shaderResourceDesc.Buffer.ElementOffset = 0;
				shaderResourceDesc.Buffer.ElementWidth  = GetSize() / NumFormatBytes( m_formatAsShaderRes);

				ID3D11ShaderResourceView* d3d11SrView	= NULL;
				TIF( m_d3d11Device->CreateShaderResourceView(  m_d3d11Buffer.get(), &shaderResourceDesc, &d3d11SrView));
				m_d3d11SrView = MakeCOMPtr( d3d11SrView);
			}

			if( (accessHint & EAH_GPUUnordered) && (m_formatAsShaderRes!=EF_UnKnown))
			{
				D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
				uavDesc.Format				= DXGI_FORMAT_UNKNOWN;
				uavDesc.ViewDimension		= D3D11_UAV_DIMENSION_BUFFER;
				uavDesc.Buffer.FirstElement = 0;
				uavDesc.Buffer.NumElements	= GetSize() / desc.StructureByteStride;
				uavDesc.Buffer.Flags		= 0;

				ID3D11UnorderedAccessView* d3dUaView;
				TIF( m_d3d11Device->CreateUnorderedAccessView( m_d3d11Buffer.get(), &uavDesc, &d3dUaView));
				m_d3d11Uaview = MakeCOMPtr( d3dUaView);
			}
		}
	}

	// 获取D3D标记
	void D3D11GraphicsBuffer::GetD3DFlags( D3D11_USAGE& usage, UINT& cpuAccessFlags, UINT& bindFlags, UINT& miscFlags)
	{
		// usage
		if( m_accessHint & EAH_Immutable)
			usage = D3D11_USAGE_IMMUTABLE;
		else
		{
			if( (EAH_CPUWrite == m_accessHint) || (EAH_CPUWrite | EAH_GPURead)== m_accessHint)
				usage = D3D11_USAGE_DYNAMIC;
			else
			{
				if( !( m_accessHint & EAH_CPURead) && ( m_accessHint & EAH_CPUWrite))
					usage = D3D11_USAGE_DEFAULT;
				else
					usage = D3D11_USAGE_STAGING;
			}
		}

		// access flags
		cpuAccessFlags = 0;
		if( m_accessHint & EAH_CPURead)
			cpuAccessFlags |= D3D11_CPU_ACCESS_READ;

		if( m_accessHint & EAH_CPUWrite)
			cpuAccessFlags |= D3D11_CPU_ACCESS_WRITE;


		// bind flags
		if( D3D10_USAGE_STAGING == usage)
			bindFlags = 0;
		else
			bindFlags = m_bindFlags;

		D3D11RenderDevice* renderFactory = D3D11RenderDevice::GetSingletonPtr();
		if( renderFactory->GetDeviceFeatureLevel() > D3D_FEATURE_LEVEL_9_3)
		{
			if( m_accessHint & EAH_GPURead)
				m_bindFlags |= D3D11_BIND_SHADER_RESOURCE;
		
			if( m_accessHint & EAH_GPUWrite)
				m_bindFlags |= D3D11_BIND_STREAM_OUTPUT;

			if( m_accessHint & EAH_GPUUnordered)
				m_bindFlags |= D3D11_BIND_UNORDERED_ACCESS;
		}

		// misc flags
		miscFlags = 0;
		if( m_accessHint & EAH_GPUUnordered)
		{
			miscFlags = ( m_accessHint & EAH_GPUStructured) ? D3D11_RESOURCE_MISC_BUFFER_STRUCTURED : D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
		}
	}

	// 重置大小
	void D3D11GraphicsBuffer::Resize( UINT sizeInByte)
	{
		GraphicsBuffer::Resize( sizeInByte);

		A_ASSERT( m_sizeInByte != 0);

		// 若当前Buffer太小,重新创建Buffer
		if( GetSize()>m_hwBufSize)
		{
			D3D11_BUFFER_DESC bufDesc;
			GetD3DFlags( bufDesc.Usage, bufDesc.CPUAccessFlags, bufDesc.BindFlags, bufDesc.MiscFlags);
			bufDesc.ByteWidth			= GetSize();
			bufDesc.StructureByteStride = NumFormatBytes( m_formatAsShaderRes);

			ID3D11Buffer* buffer;
			TIF( m_d3d11Device->CreateBuffer( &bufDesc, NULL, &buffer));
			m_d3d11Buffer = MakeCOMPtr( buffer);
			m_hwBufSize   = GetSize();

			if( (m_accessHint&EAH_GPURead) && ( m_formatAsShaderRes!=EF_UnKnown))
			{
				D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
				srDesc.Format = ( m_accessHint & EAH_GPUStructured) ? DXGI_FORMAT_UNKNOWN : D3D11Mapping::MappingFormat( m_formatAsShaderRes);
				srDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
				srDesc.Buffer.ElementOffset = 0;
				srDesc.Buffer.ElementWidth = GetSize() / NumFormatBytes( m_formatAsShaderRes);

				ID3D11ShaderResourceView* d3d11Srview;
				TIF( m_d3d11Device->CreateShaderResourceView( m_d3d11Buffer.get(), &srDesc, &d3d11Srview));
				m_d3d11SrView = MakeCOMPtr( d3d11Srview);
			}

			if( (m_accessHint&EAH_GPUUnordered) && ( m_formatAsShaderRes!=EF_UnKnown))
			{
				D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
				uavDesc.Format				= DXGI_FORMAT_UNKNOWN;
				uavDesc.ViewDimension		= D3D11_UAV_DIMENSION_BUFFER;
				uavDesc.Buffer.FirstElement = 0;
				uavDesc.Buffer.NumElements	= GetSize() / bufDesc.StructureByteStride;
				uavDesc.Buffer.Flags		= 0;

				ID3D11UnorderedAccessView* d3d11Uaview;
				TIF( m_d3d11Device->CreateUnorderedAccessView( m_d3d11Buffer.get(), &uavDesc, &d3d11Uaview));
				m_d3d11Uaview = MakeCOMPtr( d3d11Uaview);
			}
		}
	}

	// Map
	void* D3D11GraphicsBuffer::Map( BufferAccess bufAccess)
	{
		A_ASSERT( m_d3d11Buffer);

		D3D11_MAP mapType;
		switch( bufAccess)
		{
		case BA_ReadOnly:
			{
				mapType = D3D11_MAP_READ;
			}
			break;

		case BA_WriteOnly:
			{
				if ((EAH_CPUWrite == m_accessHint) || ((EAH_CPUWrite | EAH_GPURead) == m_accessHint))
					mapType = D3D11_MAP_WRITE_DISCARD;
				else
					mapType = D3D11_MAP_WRITE;
			}
			break;

		case BA_ReadWrite:
			{
				mapType = D3D11_MAP_READ_WRITE;
			}	
			break;

		default:
			{
				A_ASSERT(false);
				mapType = D3D11_MAP_READ;
			}
			break;
		}

		D3D11_MAPPED_SUBRESOURCE mapped;
		mapped.pData = NULL;
		TIF( m_d3d11ImmCtx->Map( m_d3d11Buffer.get(), 0, mapType, 0, &mapped));

		return mapped.pData;
	}

	// Unmap
	void D3D11GraphicsBuffer::Unmap()
	{
		A_ASSERT( m_d3d11Buffer);

		m_d3d11ImmCtx->Unmap( m_d3d11Buffer.get(), 0);
	}
}