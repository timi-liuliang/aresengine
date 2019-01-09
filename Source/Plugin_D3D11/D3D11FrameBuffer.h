#pragma once

#include "D3D11Typedefs.h"
#include <Core/Graphics/AresFrameBuffer.h>

namespace Ares
{
	//---------------------------------------------
	// D3D11FrameBuffer	2012-3-3 帝林
	//---------------------------------------------
	class D3D11FrameBuffer : public FrameBuffer
	{
	public:
		D3D11FrameBuffer( RenderDevice* device);
		virtual ~D3D11FrameBuffer();

		// 获取RenderTargetView
		ID3D11RenderTargetViewPtr GetD3D11RenderTargetView( UINT i) const;

		// 获取深度模板View
		ID3D11DepthStencilViewPtr GetD3D11DepthStencilView() const;

		// Clear
		virtual void Clear( const ColorRGB& color, float depth, UINT stencil, UINT flags=CBM_Depth|CBM_Stencil|CBM_Color);

		// Clear
		virtual void ClearMRTColor( const ColorRGB& color, Attachment attachment);

		// 绑定
		virtual void Bind();

		// 解绑
		virtual void Unbind();

	private:
		D3D11_VIEWPORT	m_d3dViewPort;			// d3d11 ViewPort
	};
}