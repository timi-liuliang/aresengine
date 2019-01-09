#include "D3D11RenderState.h"
#include "D3D11Mapping.h"
#include "D3D11RenderDevice.h"
#include <Core/AresException.h>
#include <Core/AresPtr.h>

namespace Ares
{
	// 构造函数
	D3D11RasterizerState::D3D11RasterizerState( const RasterizerStateDesc& desc)
		: RasterizerState( desc)
	{
		D3D11RenderDevice& renderDevice = D3D11RenderDevice::GetSingleton();

		D3D11_RASTERIZER_DESC d3d11RasterizerDesc;
		d3d11RasterizerDesc.FillMode = D3D11Mapping::Mapping( m_desc.m_fillMode);
		d3d11RasterizerDesc.CullMode = D3D11Mapping::Mapping( m_desc.m_cullMode);
		d3d11RasterizerDesc.FrontCounterClockwise = static_cast<BOOL>( m_desc.m_frontFaceCCW);
		d3d11RasterizerDesc.DepthBias			  = static_cast<int>(m_desc.m_depthBias);
		d3d11RasterizerDesc.DepthBiasClamp		  = m_desc.m_depthBias;
		d3d11RasterizerDesc.SlopeScaledDepthBias  = m_desc.m_slopeScaledDepthBias;
		d3d11RasterizerDesc.DepthClipEnable		  = m_desc.m_depthClipEnable;
		d3d11RasterizerDesc.ScissorEnable		  = m_desc.m_scissorEnable;
		d3d11RasterizerDesc.MultisampleEnable	  = m_desc.m_multiSampleEnable;
		d3d11RasterizerDesc.AntialiasedLineEnable = m_desc.m_multiSampleEnable;

		ID3D11RasterizerState* rasterizerState;
		const ID3D11DevicePtr& d3d11Device = renderDevice.GetD3DDevice();
		TIF( d3d11Device->CreateRasterizerState( &d3d11RasterizerDesc, &rasterizerState));

		m_d3d11RasterizerState = MakeCOMPtr( rasterizerState);
	}

	// 激活
	void D3D11RasterizerState::SetActive()
	{
		D3D11RenderDevice& renderDevice = D3D11RenderDevice::GetSingleton();

		renderDevice.RSSetState( m_d3d11RasterizerState);
	}

	// 构造函数
	D3D11DepthStencilState::D3D11DepthStencilState( const DepthStencilStateDesc& desc)
		: DepthStencilState( desc)
	{
		D3D11RenderDevice& renderDevice = D3D11RenderDevice::GetSingleton();

		D3D11_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
		d3dDepthStencilDesc.DepthEnable					= m_desc.m_depthEnable;
		d3dDepthStencilDesc.DepthWriteMask				= D3D11Mapping::Mapping( m_desc.m_depthWriteMask);
		d3dDepthStencilDesc.DepthFunc					= D3D11Mapping::Mapping( m_desc.m_depthFun);
		d3dDepthStencilDesc.StencilEnable				= m_desc.m_frontStencilEnable;
		d3dDepthStencilDesc.StencilReadMask				= static_cast<UINT8>( m_desc.m_frontStencilReadMask);
		d3dDepthStencilDesc.StencilWriteMask			= static_cast<UINT8>( m_desc.m_frontStencilWriteMask);
		d3dDepthStencilDesc.FrontFace.StencilFailOp		= D3D11Mapping::Mapping( m_desc.m_frontStencilFail);
		d3dDepthStencilDesc.FrontFace.StencilDepthFailOp= D3D11Mapping::Mapping( m_desc.m_frontStencilDepthFail);
		d3dDepthStencilDesc.FrontFace.StencilPassOp		= D3D11Mapping::Mapping( m_desc.m_frontStencilPass);
		d3dDepthStencilDesc.FrontFace.StencilFunc		= D3D11Mapping::Mapping( m_desc.m_frontStencilFun);
		d3dDepthStencilDesc.BackFace.StencilFailOp		= D3D11Mapping::Mapping( m_desc.m_backStencilFail);
		d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D11Mapping::Mapping( m_desc.m_backStencilDepthFail);
		d3dDepthStencilDesc.BackFace.StencilPassOp		= D3D11Mapping::Mapping( m_desc.m_backStencilPass);
		d3dDepthStencilDesc.BackFace.StencilFunc		= D3D11Mapping::Mapping( m_desc.m_backStencilFun);

		ID3D11DepthStencilState* depthStencilState;
		const ID3D11DevicePtr& d3d11Device = renderDevice.GetD3DDevice();
		TIF( d3d11Device->CreateDepthStencilState( &d3dDepthStencilDesc, &depthStencilState));

		m_d3d11DepthStencilState = MakeCOMPtr( depthStencilState);
	}

	// 激活
	void D3D11DepthStencilState::SetActive( UINT frontStencilRef, UINT backStencilRef)
	{
		D3D11RenderDevice& renderDevice = D3D11RenderDevice::GetSingleton();

		renderDevice.OMSetDepthStencilState( m_d3d11DepthStencilState, frontStencilRef);
	}


	// 构造函数
	D3D11BlendState::D3D11BlendState( const BlendStateDesc& desc)
		: BlendState( desc)
	{
		D3D11RenderDevice& renderDevice = D3D11RenderDevice::GetSingleton();

		D3D11_BLEND_DESC  d3d11BlendDesc;
		d3d11BlendDesc.AlphaToCoverageEnable  = m_desc.m_alphaToCoverageEnable;
		d3d11BlendDesc.IndependentBlendEnable = m_desc.m_independentBlendEnable;

		for ( int i=0; i<8; i++)
		{
			d3d11BlendDesc.RenderTarget[i].BlendEnable		= m_desc.m_blendEnable[i];
			d3d11BlendDesc.RenderTarget[i].RenderTargetWriteMask = static_cast<BYTE>( D3D11Mapping::MappingColorMask( m_desc.m_colorWriteMask[i]));
			d3d11BlendDesc.RenderTarget[i].SrcBlend			= D3D11Mapping::Mapping( m_desc.m_srcBlend[i]);
			d3d11BlendDesc.RenderTarget[i].DestBlend		= D3D11Mapping::Mapping( m_desc.m_destBlend[i]);
			d3d11BlendDesc.RenderTarget[i].BlendOp			= D3D11Mapping::Mapping( m_desc.m_blendOp[i]);
			d3d11BlendDesc.RenderTarget[i].SrcBlendAlpha	= D3D11Mapping::Mapping( m_desc.m_srcBlendAlpha[i]);
			d3d11BlendDesc.RenderTarget[i].DestBlendAlpha	= D3D11Mapping::Mapping( m_desc.m_destBlendAlpha[i]);
			d3d11BlendDesc.RenderTarget[i].BlendOpAlpha		= D3D11Mapping::Mapping( m_desc.m_blendOpAlpha[i]);
		}

		ID3D11BlendState* blendState;
		const ID3D11DevicePtr& d3d11Device = renderDevice.GetD3DDevice();
		TIF( d3d11Device->CreateBlendState( &d3d11BlendDesc, &blendState));
		m_d3d11BlendState = MakeCOMPtr( blendState);
	}

	// 激活
	void D3D11BlendState::SetActive( const ColorRGB& blendFactor, UINT sampleMask)
	{
		D3D11RenderDevice& renderDevice = D3D11RenderDevice::GetSingleton();

		renderDevice.OMSetBlendState( m_d3d11BlendState, blendFactor, sampleMask);
	}

	// 构造函数
	D3D11SamplerState::D3D11SamplerState( const SamplerStateDesc& desc)
		: SamplerState( desc)
	{
		D3D11RenderDevice& renderDevice = D3D11RenderDevice::GetSingleton();

		const ID3D11DevicePtr& d3d11Device = renderDevice.GetD3DDevice();
		D3D_FEATURE_LEVEL feature_level = renderDevice.GetDeviceFeatureLevel();

		D3D11_SAMPLER_DESC d3d_desc;
		d3d_desc.Filter		= D3D11Mapping::Mapping( m_desc.m_filter);
		d3d_desc.AddressU	= D3D11Mapping::Mapping( m_desc.m_addrModeU);
		d3d_desc.AddressV	= D3D11Mapping::Mapping( m_desc.m_addrModeV);
		d3d_desc.AddressW	= D3D11Mapping::Mapping( m_desc.m_addrModeW);
		d3d_desc.MipLODBias     = m_desc.m_mipMapLodBias;
		d3d_desc.MaxAnisotropy  = m_desc.m_maxAnisotropy;
		d3d_desc.ComparisonFunc = D3D11Mapping::Mapping( m_desc.m_cmpFun);
		d3d_desc.BorderColor[0] = m_desc.m_borderColor.r;
		d3d_desc.BorderColor[1] = m_desc.m_borderColor.g;
		d3d_desc.BorderColor[2] = m_desc.m_borderColor.b;
		d3d_desc.BorderColor[3] = m_desc.m_borderColor.a;
		if (feature_level <= D3D_FEATURE_LEVEL_9_3)
		{
			d3d_desc.MinLOD = floor( m_desc.m_minLod);
			d3d_desc.MaxLOD = FLT_BIG;
		}
		else
		{
			d3d_desc.MinLOD = m_desc.m_minLod;
			d3d_desc.MaxLOD = m_desc.m_maxLod;
		}

		ID3D11SamplerState* samplerState;
		TIF( d3d11Device->CreateSamplerState(&d3d_desc, &samplerState));
		m_d3d11SamplerState = MakeCOMPtr( samplerState);
	}
}