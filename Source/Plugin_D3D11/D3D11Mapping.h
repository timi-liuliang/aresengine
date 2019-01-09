#pragma once

#include <d3d11.h>
#include <Core/Graphics/AresRenderLayout.h>
#include <Core/Graphics/AresElementFormat.h>
#include <Core/Graphics/AresRenderState.h>
#include <Core/Graphics/AresRenderLayout.h>
#include <Core/Graphics/AresTexture.h>
#include <Core/Graphics/AresFrameBuffer.h>

using namespace std;

namespace Ares
{
	//-----------------------------------
	// D3D11 映射 2012-3-8<妇女节> 帝林
	//-----------------------------------
	class D3D11Mapping
	{
	public:
		// 映射颜色Mask
		static BYTE MappingColorMask( BYTE mask);

		// 映射清除Mask
		static UINT MappingDepthStencilClearMask( UINT mask);

		// 映射混合模式
		static D3D11_BLEND Mapping( AlphaBlendFactor factor);

		// 映射混合函数
		static D3D11_BLEND_OP Mapping( BlendOperation bop);

		// 纹理数据映射
		static D3D11_MAP Mapping( TextureMapAccess tma, Texture::TextureType type, UINT accessHint, UINT numMipmaps);

		// 映射填充模式
		static D3D11_FILL_MODE Mapping( FillMode mode);

		// 映射纹理FilterOperation
		static D3D11_FILTER Mapping( TextureFilterOp filter);

		// 映射剪切模式
		static D3D11_CULL_MODE Mapping( CullMode mode);

		// 映射模板操作
		static D3D11_STENCIL_OP Mapping( StencilOperation op);

		// 映射深度比较函数
		static D3D11_COMPARISON_FUNC Mapping( CompareFunction func);

		// 映射深度WriteMask
		static D3D11_DEPTH_WRITE_MASK Mapping( bool depthWriteMask);

		// 映射纹理地址循环模式
		static D3D11_TEXTURE_ADDRESS_MODE Mapping( TextureAddressingMode mode);

		// 映射格式
		static DXGI_FORMAT MappingFormat( ElementFormat format);

		// 格式映射
		static ElementFormat MappingFormat( DXGI_FORMAT d3dformat);

		// 映射拓扑类型
		static D3D11_PRIMITIVE_TOPOLOGY Mapping( RenderLayout::TopologyType type);

		// 映射InputElementDesc
		static void MappingInputElement( vector<D3D11_INPUT_ELEMENT_DESC>& oElements, size_t stream, const VertexElementType& vertexElementType, RenderLayout::StreamType type, UINT frequency);
	};
}