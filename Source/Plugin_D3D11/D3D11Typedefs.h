#pragma once

#include <d3d11.h>
#include <Core/AresCore.h>

namespace Ares
{
	typedef boost::shared_ptr<IDXGIFactory1>				IDXGIFactory1Ptr;
	typedef boost::shared_ptr<ID3D11Device>					ID3D11DevicePtr;
	typedef boost::shared_ptr<ID3D11DeviceContext>			ID3D11DeviceContextPtr;
	typedef boost::shared_ptr<IDXGISwapChain>				IDXGISwapChainPtr;
	typedef boost::shared_ptr<ID3D11Resource>				ID3D11ResourcePtr;
	typedef boost::shared_ptr<ID3D11Texture2D>				ID3D11Texture2DPtr;
	typedef boost::shared_ptr<ID3D11RenderTargetView>		ID3D11RenderTargetViewPtr;
	typedef boost::shared_ptr<ID3D11DepthStencilView>		ID3D11DepthStencilViewPtr;
	typedef boost::shared_ptr<ID3D11Buffer>					ID3D11BufferPtr;
	typedef boost::shared_ptr<ID3D11RasterizerState>		ID3D11RasterizerStatePtr;
	typedef boost::shared_ptr<ID3D11DepthStencilState>		ID3D11DepthStencilStatePtr;
	typedef boost::shared_ptr<ID3D11BlendState>				ID3D11BlendStatePtr;
	typedef boost::shared_ptr<ID3D11SamplerState>			ID3D11SamplerStatePtr;
	typedef boost::shared_ptr<ID3D11UnorderedAccessView>	ID3D11UnorderedAccessViewPtr;
	typedef boost::shared_ptr<ID3D11ShaderResourceView>		ID3D11ShaderResourceViewPtr;
	typedef boost::shared_ptr<ID3D11InputLayout>			ID3D11InputLayoutPtr;
	typedef boost::shared_ptr<ID3DBlob>						ID3DBlobPtr;
	typedef boost::shared_ptr<ID3D11VertexShader>			ID3D11VertexShaderPtr;
	typedef boost::shared_ptr<ID3D11PixelShader>			ID3D11PixelShaderPtr;
	typedef boost::shared_ptr<ID3D11GeometryShader>			ID3D11GeometryShaderPtr;
	typedef boost::shared_ptr<ID3D11ComputeShader>			ID3D11ComputeShaderPtr;
	typedef boost::shared_ptr<ID3D11HullShader>				ID3D11HullShaderPtr;
	typedef boost::shared_ptr<ID3D11DomainShader>			ID3D11DomainShaderPtr;
	class D3D11FrameBuffer;
	typedef boost::shared_ptr<D3D11FrameBuffer>				D3D11FrameBufferPtr;
	class D3D11Texture2D;
	typedef boost::shared_ptr<D3D11Texture2D>				D3D11Texture2DPtr;
}