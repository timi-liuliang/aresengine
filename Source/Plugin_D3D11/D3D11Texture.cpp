#include <d3dx11.h>
#include "D3D11Texture.h"
#include <Core/AresException.h>

namespace Ares
{
	// constructor
	D3D11Texture::D3D11Texture( TextureType type, UINT sampleCount, UINT sampleQuality, UINT accessHint)
		: Texture( type, sampleCount, sampleQuality, accessHint)
	{
		if( m_accessHint & EAH_GPUWrite)
		{
			A_ASSERT( !(accessHint & EAH_CPURead));
			A_ASSERT( !(accessHint & EAH_CPUWrite));
		}

		m_d3dDevice	= PDRenderDevice.GetD3DDevice();
		m_d3dImmCtx = PDRenderDevice.GetD3DDeviceImmContext();
	}

	// destructor
	D3D11Texture::~D3D11Texture()
	{

	}

	// 获取宽
	UINT D3D11Texture::GetWidth( UINT levle) const
	{
		A_ASSERT( levle < m_numMipMaps);

		return 1;
	}

	// 获取高
	UINT D3D11Texture::GetHeight( UINT levle) const
	{
		A_ASSERT( levle < m_numMipMaps);

		return 1;
	}

	// 获取深
	UINT D3D11Texture::GetDepth( UINT level) const
	{
		A_ASSERT( level < m_numMipMaps);

		return 1;
	}

	// 检索ShaderResourceView
	const ID3D11ShaderResourceViewPtr& D3D11Texture::RetriveD3DShaderResourceView( UINT mostDetailedMip, UINT mipLevels)
	{
		A_ASSERT(false);
		static ID3D11ShaderResourceViewPtr ret;
		return ret;
	}

	// 检索RenderTargetView
	const ID3D11RenderTargetViewPtr& D3D11Texture::RetriveD3DRenderTargetView( UINT firstArrayIdx, UINT arraySize, UINT level)
	{
		A_ASSERT( false);
		static ID3D11RenderTargetViewPtr ret;
		return ret;
	}

	// 获取D3DFlags
	void D3D11Texture::GetD3DFlags( D3D11_USAGE& usage, UINT& bindFlags, UINT& cpuAccessFlags, UINT& miscFlags)
	{
		if( m_accessHint & EAH_Immutable)
			usage = D3D11_USAGE_IMMUTABLE;
		else
		{
			if( ( EAH_CPUWrite | EAH_GPURead) == m_accessHint)
				usage = D3D11_USAGE_DYNAMIC;
			else
			{
				if( EAH_CPUWrite == m_accessHint)
				{
					if( (m_numMipMaps!=1)||(TT_Cube==m_type))
						usage = D3D11_USAGE_STAGING;
					else
						usage = D3D11_USAGE_DYNAMIC;
				}
				else
				{
					if( !(m_accessHint&EAH_CPURead) && !(m_accessHint&EAH_CPUWrite))
						usage = D3D11_USAGE_DEFAULT;
					else
						usage = D3D11_USAGE_STAGING;
				}
			}
		}
		
		bindFlags = 0;
		if( (m_accessHint&EAH_GPURead) || (D3D11_USAGE_DYNAMIC==usage))
			bindFlags |= D3D11_BIND_SHADER_RESOURCE;

		if( m_accessHint & EAH_GPUWrite)
		{
			if( IsDepthFormat( m_format))
				bindFlags |= D3D11_BIND_DEPTH_STENCIL;
			else
				bindFlags |= D3D11_BIND_RENDER_TARGET;
		}

		if( m_accessHint & EAH_GPUUnordered)
			bindFlags |= D3D11_BIND_UNORDERED_ACCESS;

		cpuAccessFlags = 0;
		if( m_accessHint & EAH_CPURead)
			cpuAccessFlags |= D3D11_CPU_ACCESS_READ;

		if( m_accessHint & EAH_CPUWrite)
			cpuAccessFlags |= D3D11_CPU_ACCESS_WRITE;

		miscFlags = 0;
		if( m_accessHint & EAH_GenerateMips)
			miscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
	}

	// RetriveD3DSRV
	const ID3D11ShaderResourceViewPtr& D3D11Texture::RetriveD3DSRV( const D3D11_SHADER_RESOURCE_VIEW_DESC& desc)
	{
		for( size_t i=0; i<m_d3dSRViews.size(); i++)
		{
			if( std::memcmp( &desc, &m_d3dSRViews[i].first, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC))==0)
				return m_d3dSRViews[i].second;
		}

		ID3D11ShaderResourceView* d3dSrView;
		HRESULT hr = m_d3dDevice->CreateShaderResourceView( GetD3DResource().get(), &desc, &d3dSrView);

		m_d3dSRViews.push_back( make_pair( desc, MakeCOMPtr( d3dSrView)));

		return m_d3dSRViews.back().second;
	}

	// RetriveD3DDSV
	const ID3D11DepthStencilViewPtr& D3D11Texture::RetriveD3DDSV( const D3D11_DEPTH_STENCIL_VIEW_DESC& desc)
	{
		for( size_t i=0; i<m_d3dDsViews.size(); i++)
		{
			if( std::memcmp( &desc, &m_d3dDsViews[i].first, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC))==0)
				return m_d3dDsViews[i].second;
		}

		ID3D11DepthStencilView* d3dDSView;
		HRESULT hr = m_d3dDevice->CreateDepthStencilView( GetD3DResource().get(), &desc, &d3dDSView);

		m_d3dDsViews.push_back( make_pair( desc, MakeCOMPtr( d3dDSView)));

		return m_d3dDsViews.back().second;
	}

	// 拷贝纹理
	bool D3D11Texture::CopyToSubTexture( Texture& target, UINT dstSubRes, UINT dstXOffset, UINT dstYOffset, UINT dstZOffset, UINT dstWidth, UINT dstHeight, UINT dstDepth, UINT srcSubRes, UINT srcXOffset, UINT srcYOffset, UINT srcZOffset, UINT srcWidth, UINT srcHeight, UINT srcDepth)
	{
		D3D11Texture* other = dynamic_cast<D3D11Texture*>(&target);
		if( !other)
			return false;

		if( srcWidth==dstWidth && srcHeight==dstHeight && GetFormat()==target.GetFormat())
		{
			D3D11_BOX srcBox;
			srcBox.left   = srcXOffset;
			srcBox.top    = srcYOffset;
			srcBox.front  = srcZOffset;
			srcBox.right  = srcXOffset + srcWidth;
			srcBox.bottom = srcYOffset + srcHeight;
			srcBox.back   = srcZOffset + srcDepth;

			m_d3dImmCtx->CopySubresourceRegion( other->GetD3DResource().get(), dstSubRes, dstXOffset, dstYOffset, 0, GetD3DResource().get(), srcSubRes, &srcBox);
		}
		else
		{
			D3D11_BOX srcBox, dstBox;

			srcBox.left   = srcXOffset;
			srcBox.top    = srcYOffset;
			srcBox.front  = srcZOffset;
			srcBox.right  = srcXOffset + srcWidth;
			srcBox.bottom = srcYOffset + srcHeight;
			srcBox.back   = srcZOffset + srcDepth;

			dstBox.left   = dstXOffset;
			dstBox.top    = dstYOffset;
			dstBox.front  = dstZOffset;
			dstBox.right  = dstXOffset + dstWidth;
			dstBox.bottom = dstYOffset + dstHeight;
			dstBox.back   = dstZOffset + dstDepth;

			D3DX11_TEXTURE_LOAD_INFO info;
			info.pSrcBox		= &srcBox;
			info.pDstBox		= &dstBox;
			info.SrcFirstMip	= srcSubRes;
			info.DstFirstMip	= dstSubRes;
			info.NumMips		= 1;
			info.SrcFirstElement= 0;
			info.DstFirstElement= 0;
			info.NumElements	= 0;
			info.Filter			= D3DX11_FILTER_LINEAR;
			info.MipFilter		= D3DX11_FILTER_LINEAR;
			if (IsSRGB( m_format))
			{
				info.Filter	   |= D3DX11_FILTER_SRGB_IN;
				info.MipFilter |= D3DX11_FILTER_SRGB_IN;
			}
			if (IsSRGB(target.GetFormat()))
			{
				info.Filter    |= D3DX11_FILTER_SRGB_OUT;
				info.MipFilter |= D3DX11_FILTER_SRGB_OUT;
			}

			TIF( D3DX11LoadTextureFromTexture( m_d3dImmCtx.get(), GetD3DResource().get(), &info, other->GetD3DResource().get()));
		}

		return true;
	}

	// Map2D texture
	void D3D11Texture::Map2D( UINT arrayIndex, UINT level, TextureMapAccess access, UINT xOffset, UINT yOffset, UINT width, UINT height, void*&data, UINT& rowPitch)
	{
		A_ASSERT( false);
	}

	// Unmaap2D
	void D3D11Texture::Unmap2D( UINT arrayIndex, UINT level)
	{
		A_ASSERT( false);
	}
}