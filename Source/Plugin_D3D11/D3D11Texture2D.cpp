#include <d3d11.h>
#include <d3dx11.h>
#include "D3D11Texture.h"
#include "D3D11Mapping.h"
#include <Core/AresException.h>
#include <Core/AresPtr.h>

namespace Ares
{
	// 构造函数
	D3D11Texture2D::D3D11Texture2D( ID3D11Texture2DPtr d3d11Tex, UINT sampleCount, UINT smapleQuality, UINT accessHint)
		: D3D11Texture( TT_2D, sampleCount, smapleQuality, accessHint)
	{
		m_d3d11Texture2D = d3d11Tex;

		UpdateParams();
	}

	// 构造函数
	D3D11Texture2D::D3D11Texture2D(UINT width, UINT height, UINT numMipMaps, UINT arraySize, ElementFormat format, UINT sampleCount, UINT smapleQuality, UINT accessHint, const ElementInitData* initData)
		: D3D11Texture( TT_2D, sampleCount, smapleQuality, accessHint)
	{
		// mip map num
		if( numMipMaps == 0)
		{
			m_numMipMaps = 1;
			
			UINT tWidth  = width;
			UINT tHeight = height;
			while( tWidth!=1 || tHeight!=1)
			{
				m_numMipMaps++;

				tWidth = std::max<UINT>( 1u, tWidth/2);
				tHeight= std::max<UINT>( 1u, tHeight/2);
			}
		}
		else
			m_numMipMaps = numMipMaps;

		D3D11RenderDevice&  renderFactory = D3D11RenderDevice::GetSingleton();
		if(  renderFactory.GetDeviceFeatureLevel() < D3D_FEATURE_LEVEL_9_3)
		{
			// heigh or width is not a power of 2 and multiple mip levels are specified. this is not supported at feature levels below 10.0
			if( m_numMipMaps>1 && (((width &(width-1)) !=0) || ((height &(height-1))!=0)))
				m_numMipMaps = 1;
		}

		m_arraySize = arraySize;
		m_format    = format;
		m_widthes.assign( 1, width);
		m_heights.assign( 1, height);

		m_desc.Width     = width;
		m_desc.Height    = height;
		m_desc.MipLevels = m_numMipMaps;
		m_desc.ArraySize = m_arraySize;

		switch( m_format)
		{
		case EF_D16:
			m_desc.Format = DXGI_FORMAT_R16_TYPELESS;
			break;

		case EF_D24S8:
			m_desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
			break;

		case EF_D32F:
			m_desc.Format = DXGI_FORMAT_R32_TYPELESS;
			break;

		default:
			m_desc.Format = D3D11Mapping::MappingFormat( m_format);
			break;
		}

		m_desc.SampleDesc.Count   = sampleCount;
		m_desc.SampleDesc.Quality = smapleQuality;

		GetD3DFlags( m_desc.Usage, m_desc.BindFlags, m_desc.CPUAccessFlags, m_desc.MiscFlags);

		vector<D3D11_SUBRESOURCE_DATA> subresData( m_arraySize * numMipMaps);
		if( initData != NULL)
		{
			for ( UINT j=0; j<arraySize; j++)
			{
				for ( UINT i=0; i<numMipMaps; i++)
				{
					subresData[j*numMipMaps+i].pSysMem			= initData[j*numMipMaps+i].m_data;
					subresData[j*numMipMaps+i].SysMemPitch		= initData[j*numMipMaps+i].m_rowPitch;
					subresData[j*numMipMaps+i].SysMemSlicePitch = initData[j*numMipMaps+i].m_slicePitch;
				}
			}
		}

		ID3D11Texture2D* d3dTex=NULL;
		TIF( m_d3dDevice->CreateTexture2D( &m_desc, (initData!=NULL) ? subresData.data() : NULL, &d3dTex));
		m_d3d11Texture2D = MakeCOMPtr( d3dTex);

		UpdateParams();
	}

	// returns the width of the texture
	UINT D3D11Texture2D::GetWidth( UINT levle) const
	{
		A_ASSERT( levle < m_numMipMaps);

		return m_widthes[levle];
	}

	// returns the height of the texture
	UINT D3D11Texture2D::GetHeight( UINT levle) const
	{
		A_ASSERT( levle < m_numMipMaps);

		return m_heights[levle];
	}

	// Copies (and maybe scales to fit) the contents of this texture to another texture
	void D3D11Texture2D::CopyToTexure( Texture& target)
	{
		A_ASSERT( m_type == target.GetType());

		D3D11Texture2D* other = dynamic_cast<D3D11Texture2D*>(&target);
		if( GetWidth(0) == target.GetWidth(0) && GetHeight(0)==target.GetHeight(0) && GetFormat()==target.GetFormat() && GetNumMipMaps()==target.GetNumMipMaps())
		{
			if( GetSampleCount()>1 && 1==target.GetSampleCount())
			{
				for ( UINT l=0; l<GetNumMipMaps(); l++)
					m_d3dImmCtx->ResolveSubresource( other->GetD3DTexture().get(), D3D11CalcSubresource( l, 0, other->GetNumMipMaps()), 
													  m_d3d11Texture2D.get(), D3D11CalcSubresource( l, 0, GetNumMipMaps()), D3D11Mapping::MappingFormat( target.GetFormat()));
			}
			else
			{
				m_d3dImmCtx->CopyResource( other->GetD3DTexture().get(), m_d3d11Texture2D.get());
			}
		}
		else
		{
			D3DX11_TEXTURE_LOAD_INFO info;
			info.pSrcBox	 = NULL;
			info.pDstBox	 = NULL;
			info.SrcFirstMip = D3D11CalcSubresource( 0, 0, GetNumMipMaps());
			info.DstFirstMip = D3D11CalcSubresource( 0, 0, other->GetNumMipMaps());
			info.NumMips	 = std::min<UINT>( GetNumMipMaps(), target.GetNumMipMaps());
			info.SrcFirstElement = 0;
			info.DstFirstElement = 0;
			info.NumElements	 = 0;
			info.Filter		 = D3DX11_FILTER_LINEAR;
			info.MipFilter	 = D3DX11_FILTER_LINEAR;
			if( IsSRGB( m_format))
			{
				info.Filter    |= D3DX11_FILTER_SRGB_IN;
				info.MipFilter |= D3DX11_FILTER_SRGB_IN;
			}
			else if( IsSRGB( target.GetFormat()))
			{
				info.Filter	   |= D3DX11_FILTER_SRGB_OUT;
				info.MipFilter |= D3DX11_FILTER_SRGB_OUT;
			}
			else
			{
				info.Filter		 = D3DX11_DEFAULT;
				info.MipFilter	 = D3DX11_DEFAULT;
			}

			HRESULT hr = ( D3DX11LoadTextureFromTexture( m_d3dImmCtx.get(), m_d3d11Texture2D.get(), &info, other->GetD3DTexture().get()));

			int a = 10;
		}
	}

	// copy texture
	void D3D11Texture2D::CopyToSubTexture2D( Texture& target,UINT dstArrayIdx, UINT dstLevel, UINT dstXOffset, UINT dstYOffset, UINT dstWidth, UINT dstHeight, UINT srcArrayIdx, UINT srcLevel, UINT srcXOffset, UINT srcYOffset, UINT srcWidth, UINT srcHeight)
	{
		A_ASSERT( m_type==target.GetType());

		CopyToSubTexture( target,
			D3D11CalcSubresource( dstLevel, dstArrayIdx, target.GetNumMipMaps()), dstXOffset, dstYOffset, 0, dstWidth, dstHeight, 1,
			D3D11CalcSubresource( srcLevel, srcArrayIdx,  this->GetNumMipMaps()), srcXOffset, srcYOffset, 0, srcWidth, srcHeight, 1);
	}

	// 生成Mipmaps
	void D3D11Texture2D::GenerateMips()
	{
		if( !m_d3dSRViews.empty())
		{
			A_ASSERT( m_accessHint & EAH_GenerateMips);

			m_d3dImmCtx->GenerateMips( m_d3dSRViews[0].second.get());
		}
		else
		{
			TIF( D3DX11FilterTexture( m_d3dImmCtx.get(), m_d3d11Texture2D.get(), 0, D3DX11_FILTER_LINEAR));
		}

		UpdateParams();
	}

	// 更新参数
	void D3D11Texture2D::UpdateParams()
	{
		m_d3d11Texture2D->GetDesc( &m_desc);

		m_numMipMaps	= m_desc.MipLevels;
		m_arraySize		= m_desc.ArraySize;
		m_sampleCount   = m_desc.SampleDesc.Count;
		m_sampleQuality = m_desc.SampleDesc.Quality;

		A_ASSERT( m_numMipMaps != 0);

		m_widthes.resize( m_numMipMaps);
		m_heights.resize( m_numMipMaps);

		m_widthes[0] = m_desc.Width;
		m_heights[0] = m_desc.Height;

		for ( UINT level=1; level<m_numMipMaps; level++)
		{
			m_widthes[level] = std::max<UINT>(1U, m_widthes[level-1] / 2);
			m_heights[level] = std::max<UINT>(1U, m_heights[level-1] / 2);
		}

		switch( m_desc.Format)
		{
		case DXGI_FORMAT_R16_TYPELESS:
			m_format = EF_D16;
			break;

		case DXGI_FORMAT_R24G8_TYPELESS:
			m_format = EF_D24S8;
			break;

		case DXGI_FORMAT_R32_TYPELESS:
			m_format = EF_D32F;
			break;

		default:
			m_format = D3D11Mapping::MappingFormat( m_desc.Format);
			break;
		}
	}

	// 检索ShaderResourceView
	const ID3D11ShaderResourceViewPtr& D3D11Texture2D::RetriveD3DShaderResourceView( UINT mostDetailedMip, UINT mipLevels, UINT firstArrayIndex, UINT arraySize)
	{
		A_ASSERT( GetAccessHint() & EAH_GPURead);

		if( arraySize == INVALID)
			arraySize = m_arraySize;

		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		std::memset( &desc, 0, sizeof( desc));
		switch( m_format)
		{
		case EF_D16:
			desc.Format = DXGI_FORMAT_R16_UNORM;
			break;

		case EF_D24S8:
			desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			break;

		case EF_D32F:
			desc.Format = DXGI_FORMAT_R32_FLOAT;
			break;

		default:
			desc.Format = m_desc.Format;
			break;
		}

		if( m_arraySize > 1)
		{
			if( m_sampleCount>1)
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
			else
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;

			desc.Texture2DArray.MostDetailedMip = mostDetailedMip;
			desc.Texture2DArray.MipLevels		= mipLevels;
			desc.Texture2DArray.ArraySize		= arraySize;
			desc.Texture2DArray.FirstArraySlice = firstArrayIndex;
		}
		else
		{
			if( m_sampleCount>1)
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
			else
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

			desc.Texture2D.MostDetailedMip = mostDetailedMip;
			desc.Texture2D.MipLevels	   = mipLevels;
		}
		
		return RetriveD3DSRV( desc);
	}

	// 检索RenderTargetView
	const ID3D11RenderTargetViewPtr& D3D11Texture2D::RetriveD3DRenderTargetView( UINT firstArrayIdx, UINT arraySize, UINT level)
	{
		A_ASSERT( GetAccessHint() & EAH_GPUWrite);
		A_ASSERT( firstArrayIdx < GetArraySize());
		A_ASSERT( firstArrayIdx + arraySize <= GetArraySize());

		RTVDSVCreation rtvCreation;
		rtvCreation.m_firstArrayIdx = firstArrayIdx;
		rtvCreation.m_arraySize		= arraySize;
		rtvCreation.m_level			= level;
		
		for ( size_t i=0; i<m_d3dRtViews.size(); i++)
		{
			if( 0 == memcmp( &m_d3dRtViews[i].first, &rtvCreation, sizeof(rtvCreation)))
				return m_d3dRtViews[i].second;
		}

		D3D11_RENDER_TARGET_VIEW_DESC desc;
		desc.Format = D3D11Mapping::MappingFormat( GetFormat());

		if( GetSampleCount() > 1)
		{
			if( GetArraySize() > 1)
				desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY;
			else
				desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
		}
		else
		{
			if( GetArraySize() > 1)
				desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			else
				desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		}

		if( GetArraySize() > 1)
		{
			desc.Texture2DArray.MipSlice		= level;
			desc.Texture2DArray.ArraySize		= arraySize;
			desc.Texture2DArray.FirstArraySlice = firstArrayIdx;
		}
		else
		{
			desc.Texture2D.MipSlice = level;
		}

		ID3D11RenderTargetView* rtView;
		TIF( m_d3dDevice->CreateRenderTargetView( GetD3DTexture().get(), &desc, &rtView));
		m_d3dRtViews.push_back( make_pair( rtvCreation, MakeCOMPtr( rtView)));

		return m_d3dRtViews.back().second;
	}

	// 检索DepthStencilView
	const ID3D11DepthStencilViewPtr& D3D11Texture2D::RetriveD3DDepthStencilView( UINT firstArrayIndex, UINT arraySize, UINT level)
	{
		A_ASSERT( GetAccessHint() & EAH_GPUWrite);
		A_ASSERT( firstArrayIndex<GetArraySize());
		A_ASSERT( firstArrayIndex+arraySize<=GetArraySize());

		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		std::memset( &desc, 0, sizeof( desc));
		desc.Format = D3D11Mapping::MappingFormat( GetFormat());
		desc.Flags  = 0;
		if( GetSampleCount()>1)
		{
			if( GetArraySize()>1)
				desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY;
			else
				desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		}	
		else
		{
			if( GetArraySize()>1)
				desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
			else 
				desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		}

		if( GetArraySize()>1)
		{
			desc.Texture2DArray.MipSlice = level;
			desc.Texture2DArray.ArraySize= arraySize;
			desc.Texture2DArray.FirstArraySlice = firstArrayIndex;
		}
		else
		{
			desc.Texture2D.MipSlice = level;
		}

		return RetriveD3DDSV( desc);
	}

	// Map2D texture
	void D3D11Texture2D::Map2D( UINT arrayIndex, UINT level, TextureMapAccess access, UINT xOffset, UINT yOffset, UINT width, UINT height, void*&data, UINT& rowPitch)
	{
		D3D11_MAPPED_SUBRESOURCE mapped;
		TIF( m_d3dImmCtx->Map( m_d3d11Texture2D.get(), D3D11CalcSubresource( level, arrayIndex, m_numMipMaps), D3D11Mapping::Mapping( access, m_type, m_accessHint, m_numMipMaps), 0, &mapped));

		BYTE* pointer = static_cast<BYTE*>( mapped.pData);
		data = pointer + yOffset * mapped.RowPitch + xOffset * NumFormatBytes( m_format);
		rowPitch = mapped.RowPitch;
	}

	// Unmaap2D
	void D3D11Texture2D::Unmap2D( UINT arrayIndex, UINT level)
	{
		m_d3dImmCtx->Unmap( m_d3d11Texture2D.get(), D3D11CalcSubresource( level, arrayIndex, m_numMipMaps));
	}
}