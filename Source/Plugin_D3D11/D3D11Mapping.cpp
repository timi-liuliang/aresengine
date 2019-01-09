#include <string>
#include "D3D11Mapping.h"
#include <Core/AresException.h>

using namespace std;

namespace Ares
{
	// 映射颜色Mask
	BYTE D3D11Mapping::MappingColorMask( BYTE mask)
	{
		BYTE ret = 0;
		if( mask & ColorRGB::MASK_Red)
		{
			ret |= D3D11_COLOR_WRITE_ENABLE_RED;
		}
		if (mask & ColorRGB::MASK_Green)
		{
			ret |= D3D11_COLOR_WRITE_ENABLE_GREEN;
		}
		if (mask & ColorRGB::MASK_Blue)
		{
			ret |= D3D11_COLOR_WRITE_ENABLE_BLUE;
		}
		if (mask & ColorRGB::MASK_Alpha)
		{
			ret |= D3D11_COLOR_WRITE_ENABLE_ALPHA;
		}

		return ret;
	}

	// 映射清除Mask
	UINT D3D11Mapping::MappingDepthStencilClearMask( UINT mask)
	{
		UINT clearFlag = 0;
		if( mask & CBM_Depth)
		{
			clearFlag |= D3D11_CLEAR_DEPTH;
		}
		if( mask & CBM_Stencil)
		{
			clearFlag |= D3D11_CLEAR_STENCIL;
		}

		return clearFlag;
	}

	// 映射填充模式
	D3D11_FILL_MODE D3D11Mapping::Mapping( FillMode mode)
	{
		switch( mode)
		{
		case FM_Point:
		case FM_WireFrame:
			return D3D11_FILL_WIREFRAME;

		case FM_Solid:
			return D3D11_FILL_SOLID;

		default:
			BOOST_ASSERT(false);
		}

		return D3D11_FILL_SOLID;
	}

	// 映射纹理FilterOperation
	D3D11_FILTER D3D11Mapping::Mapping( TextureFilterOp filter)
	{
		switch (filter)
		{
		case TFO_MinMagMipPoint:
			return D3D11_FILTER_MIN_MAG_MIP_POINT;

		case TFO_MinMagPointMipLinear:
			return D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;

		case TFO_MinPointMagLinearMipPoint:
			return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;

		case TFO_MinPointMagMipLinear:
			return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;

		case TFO_MinMagLinearMipPoint:
			return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;

		case TFO_MinMagMipLinear:
			return D3D11_FILTER_MIN_MAG_MIP_LINEAR;

		case TFO_Anisotropic:
			return D3D11_FILTER_ANISOTROPIC;

		case TFO_CmpMinMagMipPoint:
			return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;

		case TFO_CmpMinMagPointMipLinear:
			return D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR;

		case TFO_CmpMinPointMagLinearMipPoint:
			return D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;

		case TFO_CmpMinPointMagMipLinear:
			return D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR;

		case TFO_CmpMinMagLinearMipPoint:
			return D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;

		case TFO_CmpMinMagMipLinear:
			return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;

		case TFO_CmpAnisotropic:
			return D3D11_FILTER_COMPARISON_ANISOTROPIC;

		default:
			BOOST_ASSERT( false);
			return D3D11_FILTER_MIN_MAG_MIP_POINT;
		}
	}

	// 纹理数据映射
	D3D11_MAP D3D11Mapping::Mapping( TextureMapAccess tma, Texture::TextureType type, UINT accessHint, UINT numMipmaps)
	{
		switch( tma)
		{
		case TMA_ReadOnly:
			return D3D11_MAP_READ;

		case TMA_WriteOnly:
			if( (EAH_CPUWrite|EAH_GPURead)==accessHint || EAH_CPUWrite==accessHint && 1==numMipmaps && type!=Texture::TT_Cube)
				return D3D11_MAP_WRITE_DISCARD;
			else
				return D3D11_MAP_WRITE;
			
		case TMA_ReadWrite:
			return D3D11_MAP_READ_WRITE;

		default:
			A_ASSERT( false);
			return D3D11_MAP_READ;
		}
	}

	// 映射混合模式
	D3D11_BLEND D3D11Mapping::Mapping( AlphaBlendFactor factor)
	{
		switch (factor)
		{
		case ABF_Zero:
			return D3D11_BLEND_ZERO;

		case ABF_One:
			return D3D11_BLEND_ONE;

		case ABF_SrcAlpha:
			return D3D11_BLEND_SRC_ALPHA;

		case ABF_DstAlpha:
			return D3D11_BLEND_DEST_ALPHA;

		case ABF_InvSrcAlpha:
			return D3D11_BLEND_INV_SRC_ALPHA;

		case ABF_InvDstAlpha:
			return D3D11_BLEND_INV_DEST_ALPHA;

		case ABF_SrcColor:
			return D3D11_BLEND_SRC_COLOR;

		case ABF_DstColor:
			return D3D11_BLEND_DEST_COLOR;

		case ABF_InvSrcColor:
			return D3D11_BLEND_INV_SRC_COLOR;

		case ABF_InvDstColor:
			return D3D11_BLEND_INV_DEST_COLOR;

		case ABF_SrcAlphaSat:
			return D3D11_BLEND_SRC_ALPHA_SAT;

		default:
			throw ares_exception() << err_str( "Mapping D3D11_BLEND failed!");			
		}

		return D3D11_BLEND_ZERO;
	}

	// 映射混合函数
	D3D11_BLEND_OP D3D11Mapping::Mapping( BlendOperation bop)
	{
		switch( bop)
		{
		case BOP_Add:
			return D3D11_BLEND_OP_ADD;

		case BOP_Sub:
			return D3D11_BLEND_OP_SUBTRACT;

		case BOP_RevSub:
			return D3D11_BLEND_OP_REV_SUBTRACT;

		case BOP_Min:
			return D3D11_BLEND_OP_MIN;

		case BOP_Max:
			return D3D11_BLEND_OP_MAX;

		default:
			assert(false);
			return D3D11_BLEND_OP_ADD;
		}
	}

	// 映射剪切模式
	D3D11_CULL_MODE D3D11Mapping::Mapping( CullMode mode)
	{
		switch( mode)
		{
		case CM_None:
			return D3D11_CULL_NONE;

		case CM_Front:
			return D3D11_CULL_FRONT;

		case CM_Back:
			return D3D11_CULL_BACK;
		}

		return D3D11_CULL_NONE;
	}

	// 映射模板操作
	D3D11_STENCIL_OP D3D11Mapping::Mapping( StencilOperation op)
	{
		switch( op)
		{
		case SOP_Keep:
			return D3D11_STENCIL_OP_KEEP;

		case SOP_Zero:
			return D3D11_STENCIL_OP_ZERO;

		case SOP_Replace:
			return D3D11_STENCIL_OP_REPLACE;

		case SOP_IncrSat:
			return D3D11_STENCIL_OP_INCR_SAT;

		case SOP_DecrSat:
			return D3D11_STENCIL_OP_DECR_SAT;

		case SOP_Invert:
			return D3D11_STENCIL_OP_INVERT;

		case SOP_IncrWrap:
			return D3D11_STENCIL_OP_INCR;

		case SOP_DecrWrap:
			return D3D11_STENCIL_OP_DECR;

		default:
			throw ares_exception() << err_str( "Mapping D3D11_STENCIL_OP failed!");
		}
	}

	// 映射深度WriteMask
	D3D11_DEPTH_WRITE_MASK D3D11Mapping::Mapping( bool depthWriteMask)
	{
		if( depthWriteMask)
		{
			// Turn on writes to the depth-stencil buffer.
			return D3D11_DEPTH_WRITE_MASK_ALL;
		}
		else
		{
			// Turn off writes to the depth-stencil buffer.
			return D3D11_DEPTH_WRITE_MASK_ZERO;
		}
	}

	// 映射深度比较函数
	D3D11_COMPARISON_FUNC D3D11Mapping::Mapping( CompareFunction func)
	{
		switch( func)
		{
		case CF_Never:
			return D3D11_COMPARISON_NEVER;

		case CF_Less:
			return D3D11_COMPARISON_LESS;

		case CF_Equal:
			return D3D11_COMPARISON_EQUAL;

		case CF_LessEqual:
			return D3D11_COMPARISON_LESS_EQUAL;
				
		case CF_Greater:
			return D3D11_COMPARISON_GREATER;

		case CF_GreateEqual:
			return D3D11_COMPARISON_GREATER_EQUAL;

		case CF_Always:
			return D3D11_COMPARISON_ALWAYS;

		default:
			throw ares_exception() << err_str( "Mapping D3D11_COMPARISON failed!");
		}
	}

	// 映射纹理地址循环模式
	D3D11_TEXTURE_ADDRESS_MODE D3D11Mapping::Mapping( TextureAddressingMode mode)
	{
		switch (mode)
		{
		case TAM_Clamp:
			return D3D11_TEXTURE_ADDRESS_CLAMP;

		case TAM_Wrap:
			return D3D11_TEXTURE_ADDRESS_WRAP;

		case TAM_Mirror:
			return D3D11_TEXTURE_ADDRESS_MIRROR;

		case TAM_Border:
			return D3D11_TEXTURE_ADDRESS_BORDER;

		default:
			BOOST_ASSERT( false);	
		}

		return D3D11_TEXTURE_ADDRESS_CLAMP; 
	}

	// 映射格式
	DXGI_FORMAT D3D11Mapping::MappingFormat( ElementFormat format)
	{
		switch (format)
		{
		case EF_A8:
			return DXGI_FORMAT_A8_UNORM;

		case EF_R8:
			return DXGI_FORMAT_R8_UNORM;

		case EF_SIGNED_R8:
			return DXGI_FORMAT_R8_SNORM;

		case EF_GR8:
			return DXGI_FORMAT_R8G8_UNORM;

		case EF_SIGNED_GR8:
			return DXGI_FORMAT_R8G8_SNORM;

		case EF_ARGB8:
			return DXGI_FORMAT_B8G8R8A8_UNORM;

		case EF_ABGR8:
			return DXGI_FORMAT_R8G8B8A8_UNORM;

		case EF_SIGNED_ABGR8:
			return DXGI_FORMAT_R8G8B8A8_SNORM;

		case EF_A2BGR10:
			return DXGI_FORMAT_R10G10B10A2_UNORM;

		case EF_R8UI:
			return DXGI_FORMAT_R8_UINT;

		case EF_R8I:
			return DXGI_FORMAT_R8_SINT;

		case EF_GR8UI:
			return DXGI_FORMAT_R8G8_UINT;

		case EF_GR8I:
			return DXGI_FORMAT_R8G8_SINT;

		case EF_ABGR8UI:
			return DXGI_FORMAT_R8G8B8A8_UINT;

		case EF_ABGR8I:
			return DXGI_FORMAT_R8G8B8A8_SINT;

		case EF_A2BGR10UI:
			return DXGI_FORMAT_R10G10B10A2_UINT;

		case EF_R16:
			return DXGI_FORMAT_R16_UNORM;

		case EF_SIGNED_R16:
			return DXGI_FORMAT_R16_SNORM;

		case EF_GR16:
			return DXGI_FORMAT_R16G16_UNORM;

		case EF_SIGNED_GR16:
			return DXGI_FORMAT_R16G16_SNORM;

		case EF_ABGR16:
			return DXGI_FORMAT_R16G16B16A16_UNORM;

		case EF_SIGNED_ABGR16:
			return DXGI_FORMAT_R16G16B16A16_SNORM;

		case EF_R16UI:
			return DXGI_FORMAT_R16_UINT;

		case EF_R16I:
			return DXGI_FORMAT_R16_SINT;

		case EF_GR16UI:
			return DXGI_FORMAT_R16G16_UINT;

		case EF_GR16I:
			return DXGI_FORMAT_R16G16_SINT;

		case EF_ABGR16UI:
			return DXGI_FORMAT_R16G16B16A16_UINT;

		case EF_ABGR16I:
			return DXGI_FORMAT_R16G16B16A16_SINT;

		case EF_R32UI:
			return DXGI_FORMAT_R32_UINT;

		case EF_R32I:
			return DXGI_FORMAT_R32_SINT;

		case EF_GR32UI:
			return DXGI_FORMAT_R32G32_UINT;

		case EF_GR32I:
			return DXGI_FORMAT_R32G32_SINT;

		case EF_BGR32UI:
			return DXGI_FORMAT_R32G32B32_UINT;

		case EF_BGR32I:
			return DXGI_FORMAT_R32G32B32_SINT;

		case EF_ABGR32UI:
			return DXGI_FORMAT_R32G32B32A32_UINT;

		case EF_ABGR32I:
			return DXGI_FORMAT_R32G32B32A32_SINT;

		case EF_R16F:
			return DXGI_FORMAT_R16_FLOAT;

		case EF_GR16F:
			return DXGI_FORMAT_R16G16_FLOAT;

		case EF_B10G11R11F:
			return DXGI_FORMAT_R11G11B10_FLOAT;

		case EF_ABGR16F:
			return DXGI_FORMAT_R16G16B16A16_FLOAT;

		case EF_R32F:
			return DXGI_FORMAT_R32_FLOAT;

		case EF_GR32F:
			return DXGI_FORMAT_R32G32_FLOAT;

		case EF_BGR32F:
			return DXGI_FORMAT_R32G32B32_FLOAT;

		case EF_ABGR32F:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;

		case EF_BC1:
			return DXGI_FORMAT_BC1_UNORM;

		case EF_BC2:
			return DXGI_FORMAT_BC2_UNORM;

		case EF_BC3:
			return DXGI_FORMAT_BC3_UNORM;

		case EF_BC4:
			return DXGI_FORMAT_BC4_UNORM;

		case EF_SIGNED_BC4:
			return DXGI_FORMAT_BC4_SNORM;

		case EF_BC5:
			return DXGI_FORMAT_BC5_UNORM;

		case EF_SIGNED_BC5:
			return DXGI_FORMAT_BC5_SNORM;

		case EF_BC6:
			return DXGI_FORMAT_BC6H_UF16;

		case EF_SIGNED_BC6:
			return DXGI_FORMAT_BC6H_SF16;

		case EF_BC7:
			return DXGI_FORMAT_BC7_UNORM;

		case EF_D16:
			return DXGI_FORMAT_D16_UNORM;

		case EF_D24S8:
			return DXGI_FORMAT_D24_UNORM_S8_UINT;

		case EF_D32F:
			return DXGI_FORMAT_D32_FLOAT;

		case EF_ARGB8_SRGB:
			return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;

		case EF_ABGR8_SRGB:
			return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

		case EF_BC1_SRGB:
			return DXGI_FORMAT_BC1_UNORM_SRGB;

		case EF_BC2_SRGB:
			return DXGI_FORMAT_BC2_UNORM_SRGB;

		case EF_BC3_SRGB:
			return DXGI_FORMAT_BC3_UNORM_SRGB;

		case EF_BC7_SRGB:
			return DXGI_FORMAT_BC7_UNORM_SRGB;

			// 抛出异常
		default:
			throw ares_exception() << err_str( "Element Mapping D3D11Format failed!");
		}
	}

	// 格式映射
	ElementFormat D3D11Mapping::MappingFormat( DXGI_FORMAT d3dformat)
	{
		switch (d3dformat)
		{
		case DXGI_FORMAT_A8_UNORM:
			return EF_A8;

		case DXGI_FORMAT_R8_UNORM:
			return EF_R8;

		case DXGI_FORMAT_R8_SNORM:
			return EF_SIGNED_R8;

		case DXGI_FORMAT_R8G8_UNORM:
			return EF_GR8;

		case DXGI_FORMAT_R8G8_SNORM:
			return EF_SIGNED_GR8;

		case DXGI_FORMAT_B8G8R8A8_UNORM:
			return EF_ARGB8;

		case DXGI_FORMAT_R8G8B8A8_UNORM:
			return EF_ABGR8;

		case DXGI_FORMAT_R8G8B8A8_SNORM:
			return EF_SIGNED_ABGR8;

		case DXGI_FORMAT_R10G10B10A2_UNORM:
			return EF_A2BGR10;

		case DXGI_FORMAT_R8_UINT:
			return EF_R8UI;

		case DXGI_FORMAT_R8_SINT:
			return EF_R8I;

		case DXGI_FORMAT_R8G8_UINT:
			return EF_GR8UI;

		case DXGI_FORMAT_R8G8_SINT:
			return EF_GR8I;

		case DXGI_FORMAT_R8G8B8A8_UINT:
			return EF_ABGR8UI;

		case DXGI_FORMAT_R8G8B8A8_SINT:
			return EF_ABGR8I;

		case DXGI_FORMAT_R10G10B10A2_UINT:
			return EF_A2BGR10UI;

		case DXGI_FORMAT_R16_UNORM:
			return EF_R16;

		case DXGI_FORMAT_R16_SNORM:
			return EF_SIGNED_R16;

		case DXGI_FORMAT_R16G16_UNORM:
			return EF_GR16;

		case DXGI_FORMAT_R16G16_SNORM:
			return EF_SIGNED_GR16;

		case DXGI_FORMAT_R16G16B16A16_UNORM:
			return EF_ABGR16;

		case DXGI_FORMAT_R16G16B16A16_SNORM:
			return EF_SIGNED_ABGR16;

		case DXGI_FORMAT_R16_UINT:
			return EF_R16UI;

		case DXGI_FORMAT_R16_SINT:
			return EF_R16I;

		case DXGI_FORMAT_R16G16_UINT:
			return EF_GR16UI;

		case DXGI_FORMAT_R16G16_SINT:
			return EF_GR16I;

		case DXGI_FORMAT_R16G16B16A16_UINT:
			return EF_ABGR16UI;

		case DXGI_FORMAT_R16G16B16A16_SINT:
			return EF_ABGR16I;

		case DXGI_FORMAT_R32_UINT:
			return EF_R32UI;

		case DXGI_FORMAT_R32_SINT:
			return EF_R32I;

		case DXGI_FORMAT_R32G32_UINT:
			return EF_GR32UI;

		case DXGI_FORMAT_R32G32_SINT:
			return EF_GR32I;

		case DXGI_FORMAT_R32G32B32_UINT:
			return EF_BGR32UI;

		case DXGI_FORMAT_R32G32B32_SINT:
			return EF_BGR32I;

		case DXGI_FORMAT_R32G32B32A32_UINT:
			return EF_ABGR32UI;

		case DXGI_FORMAT_R32G32B32A32_SINT:
			return EF_ABGR32I;

		case DXGI_FORMAT_R16_FLOAT:
			return EF_R16F;

		case DXGI_FORMAT_R16G16_FLOAT:
			return EF_GR16F;

		case DXGI_FORMAT_R11G11B10_FLOAT:
			return EF_B10G11R11F;

		case DXGI_FORMAT_R16G16B16A16_FLOAT:
			return EF_ABGR16F;

		case DXGI_FORMAT_R32_FLOAT:
			return EF_R32F;

		case DXGI_FORMAT_R32G32_FLOAT:
			return EF_GR32F;

		case DXGI_FORMAT_R32G32B32_FLOAT:
			return EF_BGR32F;

		case DXGI_FORMAT_R32G32B32A32_FLOAT:
			return EF_ABGR32F;

		case DXGI_FORMAT_BC1_UNORM:
			return EF_BC1;

		case DXGI_FORMAT_BC2_UNORM:
			return EF_BC2;

		case DXGI_FORMAT_BC3_UNORM:
			return EF_BC3;

		case DXGI_FORMAT_BC4_UNORM:
			return EF_BC4;

		case DXGI_FORMAT_BC4_SNORM:
			return EF_SIGNED_BC4;

		case DXGI_FORMAT_BC5_UNORM:
			return EF_BC5;

		case DXGI_FORMAT_BC5_SNORM:
			return EF_SIGNED_BC5;

		case DXGI_FORMAT_BC6H_UF16:
			return EF_BC6;

		case DXGI_FORMAT_BC6H_SF16:
			return EF_SIGNED_BC6;

		case DXGI_FORMAT_BC7_UNORM:
			return EF_BC7;

		case DXGI_FORMAT_D16_UNORM:
			return EF_D16;

		case DXGI_FORMAT_D24_UNORM_S8_UINT:
			return EF_D24S8;

		case DXGI_FORMAT_D32_FLOAT:
			return EF_D32F;

		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
			return EF_ARGB8_SRGB;

		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
			return EF_ABGR8_SRGB;

		case DXGI_FORMAT_BC1_UNORM_SRGB:
			return EF_BC1_SRGB;

		case DXGI_FORMAT_BC2_UNORM_SRGB:
			return EF_BC2_SRGB;

		case DXGI_FORMAT_BC3_UNORM_SRGB:
			return EF_BC3_SRGB;

		case DXGI_FORMAT_BC7_UNORM_SRGB:
			return EF_BC7_SRGB;

		default:
			throw ares_exception() << err_str( "D3D11 Mapping ElementFormat failed!");
		}
	}

	static string D3D11_INPUT_ELEMENT_SEMANTICS[] =
	{
		"POSITION",
		"NORMAL",
		"COLOR",
		"BLENDWEIGHT",
		"BLENDINDICES",
		"TEXCOORD",
		"TANGENT",
		"BINORMAL"
	};

	// 映射InputElementDesc
	void D3D11Mapping::MappingInputElement( vector<D3D11_INPUT_ELEMENT_DESC>& oElements, size_t stream, const VertexElementType& vertexElementType, RenderLayout::StreamType type, UINT frequency)
	{
		WORD elemOffset = 0;
		oElements.resize( vertexElementType.size());
		for ( size_t i=0; i<oElements.size(); i++)
		{
			// Get Reference
			const VertexElement& iElement      = vertexElementType[i];
			D3D11_INPUT_ELEMENT_DESC& oElement = oElements[i];

			oElement.SemanticIndex		= iElement.m_usageIdx;
			oElement.Format				= D3D11Mapping::MappingFormat( iElement.m_format);
			oElement.InputSlot			= static_cast<WORD>(stream);
			oElement.AlignedByteOffset	= elemOffset;

			if( type==RenderLayout::ST_Geometry)
			{
				oElement.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
				oElement.InstanceDataStepRate = 0;
			}
			else
			{
				BOOST_ASSERT( type==RenderLayout::ST_Instance);

				oElement.InputSlotClass       = D3D11_INPUT_PER_INSTANCE_DATA;
				oElement.InstanceDataStepRate = frequency;
			}

			// semantic
			oElement.SemanticName = D3D11_INPUT_ELEMENT_SEMANTICS[iElement.m_usage].c_str();

			elemOffset = static_cast<WORD>( elemOffset + iElement.GetSize());
		}
	}

	// 映射拓扑类型
	D3D11_PRIMITIVE_TOPOLOGY D3D11Mapping::Mapping( RenderLayout::TopologyType type)
	{
		switch( type)
		{
		case RenderLayout::TT_PointList:
			return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

		case RenderLayout::TT_LineList:
			return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

		case RenderLayout::TT_LineStrip:
			return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;

		case RenderLayout::TT_TriangleList:
			return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		case RenderLayout::TT_TriangleStrip:
			return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

		case RenderLayout::TT_LineListAdj:
			return D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;

		case RenderLayout::TT_LineStripAdj:
			return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;

		case RenderLayout::TT_TriangleListAdj:
			return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;

		case RenderLayout::TT_TriangleStripAdj:
			return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ;

		case RenderLayout::TT_1_CtrlPtPatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST;

		case RenderLayout::TT_2_CtrlPtPatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST;

		case RenderLayout::TT_3_CtrlPtPatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;

		case RenderLayout::TT_4_CtrlPtPatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;

		case RenderLayout::TT_5_CtrlPtPatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST;

		case RenderLayout::TT_6_CtrlPtPatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST;

		case RenderLayout::TT_7_CtrlPtPatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST;

		case RenderLayout::TT_8_CtrlPtPatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST;

		case RenderLayout::TT_9_CtrlPtPatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST;

		case RenderLayout::TT_10_CtrlPtPatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST;

		case RenderLayout::TT_11_CtrlPtPatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST;

		case RenderLayout::TT_12_CtrlPtPatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST;

		case RenderLayout::TT_13_CtrlPtPatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST;

		case RenderLayout::TT_14_CtrlPtPatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST;

		case RenderLayout::TT_15_CtrlPtPatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST;

		case RenderLayout::TT_16_CtrlPtPatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST;

		case RenderLayout::TT_17_CtrlPtPatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST;

		case RenderLayout::TT_18_CtrlPtPatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST;

		case RenderLayout::TT_19_CtrlPtPatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST;

		case RenderLayout::TT_20_CtrlPtPatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST;

		case RenderLayout::TT_21_CtrlPtPatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST;

		case RenderLayout::TT_22_CtrlPtPatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST;

		case RenderLayout::TT_23_CtrlPtPatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST;

		case RenderLayout::TT_24_CtrlPtPatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST;

		case RenderLayout::TT_25_CtrlPtPatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST;

		case RenderLayout::TT_26_CtrlPtPatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST;

		case RenderLayout::TT_27_CtrlPtPatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST;

		case RenderLayout::TT_28_CtrlPtPatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST;

		case RenderLayout::TT_29_CtrlPtPatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST;

		case RenderLayout::TT_30_CtrlPtPatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST;

		case RenderLayout::TT_31_CtrlPtPatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST;

		case RenderLayout::TT_32_CtrlPtPatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST;

		default:
			BOOST_ASSERT(false);
			return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		}
	}
}