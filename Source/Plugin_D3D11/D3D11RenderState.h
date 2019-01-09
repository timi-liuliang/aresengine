#pragma once

#include "D3D11Typedefs.h"
#include <Core/Graphics/AresRenderState.h>

namespace Ares
{
	//----------------------------------------
	// D3D11光栅化状态 2012-3-30 帝林
	//----------------------------------------
	class D3D11RasterizerState : public RasterizerState
	{
	public:
		// 构造函数
		D3D11RasterizerState( const RasterizerStateDesc& desc);

		// 激活
		virtual void SetActive();

	private:
		ID3D11RasterizerStatePtr	m_d3d11RasterizerState;
	};

	//----------------------------------------
	// D3D11深度模板状态 2012-3-30 帝林
	//----------------------------------------
	class D3D11DepthStencilState : public DepthStencilState
	{
	public:
		// 构造函数
		D3D11DepthStencilState( const DepthStencilStateDesc& desc);

		// 激活
		virtual void SetActive( UINT frontStencilRef, UINT backStencilRef);

	private:
		ID3D11DepthStencilStatePtr	m_d3d11DepthStencilState;
	};

	//----------------------------------------
	// D3D11混合状态 2012-3-31 帝林
	//----------------------------------------
	class D3D11BlendState : public BlendState
	{
	public:
		// 构造函数
		D3D11BlendState( const BlendStateDesc& desc);

		// 激活
		virtual void SetActive( const ColorRGB& blendFactor, UINT sampleMask);

	private:
		ID3D11BlendStatePtr	m_d3d11BlendState;
	};

	//----------------------------------------
	// D3D11纹理采样状态 2012-3-31 帝林
	//----------------------------------------
	class D3D11SamplerState : public SamplerState
	{
	public:
		// 构造函数
		explicit D3D11SamplerState( const SamplerStateDesc& desc);

		// 获取D3D11SamplerState
		ID3D11SamplerStatePtr& GetD3D11SamperState() { return m_d3d11SamplerState; }

	private:
		ID3D11SamplerStatePtr		m_d3d11SamplerState;
	};
}