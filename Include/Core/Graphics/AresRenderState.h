#pragma once

#include <Core/AresMath.h>

using namespace boost;

namespace Ares
{
	enum FillMode
	{
		FM_Point,		
		FM_WireFrame,	
		FM_Solid,			// default
	};

	enum ShadeMode
	{
		SM_Flat,
		SM_Gouraud,			// default
	};

	enum CullMode
	{
		CM_None,
		CM_Front,
		CM_Back,// default
	};

	//------------------------------------
	// 光栅化状态 2012-3-30 帝林
	//------------------------------------
	struct RasterizerStateDesc
	{
		FillMode		m_fillMode;			// 填充模式
		ShadeMode		m_shadeMode;		// 着色模式
		CullMode		m_cullMode;			// 隐藏面消除
		bool			m_frontFaceCCW;		
		float			m_depthBias;
		float			m_slopeScaledDepthBias;
		bool		    m_depthClipEnable;
		bool			m_scissorEnable;
		bool			m_multiSampleEnable;

		// 初始化
		RasterizerStateDesc()
		{
			m_fillMode				= FM_Solid;
			m_shadeMode				= SM_Gouraud;
			m_cullMode				= CM_None;
			m_frontFaceCCW			= false;
			m_depthBias				= 0.f;
			m_slopeScaledDepthBias	= 0.f;
			m_depthClipEnable		= true;
			m_scissorEnable			= false;
			m_multiSampleEnable		= false;
		}
	};

	// 重载运算符 "<"
	FORCEINLINE bool operator<(const RasterizerStateDesc& lhs, const RasterizerStateDesc& rhs)
	{
		return std::memcmp( &lhs, &rhs, sizeof(lhs)) < 0;
	}

	class CORE_EXPORT RasterizerState
	{
	public:
		// constructor
		explicit RasterizerState( const RasterizerStateDesc& desc)
			: m_desc( desc)
		{ }

		// 激活
		virtual void SetActive()=0;

	protected:
		RasterizerStateDesc m_desc;
	};

	enum CompareFunction
	{
		CF_Never,
		CF_Less,
		CF_Equal,
		CF_LessEqual,
		CF_Greater,
		CF_NotEqual,
		CF_GreateEqual,
		CF_Always,
	};

	enum StencilOperation
	{
		SOP_Keep,
		SOP_Zero,
		SOP_Replace,
		SOP_IncrSat,
		SOP_DecrSat,
		SOP_Invert,
		SOP_IncrWrap,
		SOP_DecrWrap
	};

	//------------------------------------
	// 深度模板状态 2012-3-30 帝林
	//------------------------------------
	struct CORE_EXPORT DepthStencilStateDesc
	{
		bool				m_depthEnable;			// 是否进行深度检测
		bool				m_depthWriteMask;
		CompareFunction		m_depthFun;				// 深度检测模式

		bool				m_frontStencilEnable;
		CompareFunction		m_frontStencilFun;
		WORD				m_frontStencilReadMask;
		WORD				m_frontStencilWriteMask;
		StencilOperation	m_frontStencilFail;
		StencilOperation	m_frontStencilDepthFail;
		StencilOperation	m_frontStencilPass;

		bool				m_backStencilEnable;
		CompareFunction		m_backStencilFun;
		WORD				m_backStencilReadMask;
		WORD				m_backStencilWriteMask;
		StencilOperation	m_backStencilFail;
		StencilOperation	m_backStencilDepthFail;
		StencilOperation	m_backStencilPass;

		// 初始化
		DepthStencilStateDesc()
		{
			m_depthEnable			= true;
			m_depthWriteMask		= true;	
			m_depthFun				= CF_LessEqual;
			m_frontStencilEnable	= false;
			m_frontStencilFun		= CF_Always;
			m_frontStencilReadMask	= 0xFFFF;
			m_frontStencilWriteMask = 0xFFFF;
			m_frontStencilFail		= SOP_Keep;
			m_frontStencilDepthFail	= SOP_Keep;
			m_frontStencilPass		= SOP_Keep;
			m_backStencilEnable		= false;
			m_backStencilFun		= CF_Always;
			m_backStencilReadMask	= 0xFFFF;
			m_backStencilWriteMask	= 0xFFFF;
			m_backStencilFail		= SOP_Keep;
			m_backStencilDepthFail	= SOP_Keep;
			m_backStencilPass		= SOP_Keep;
		}

		// 序列化
		template <typename Archive> void serialize( Archive& ar, const unsigned int version)
		{
			ar & m_depthEnable;
			ar & m_depthWriteMask;
			ar & m_depthFun;

			ar & m_frontStencilEnable;
			ar & m_frontStencilFun;
			ar & m_frontStencilReadMask;
			ar & m_frontStencilWriteMask;
			ar & m_frontStencilFail;
			ar & m_frontStencilDepthFail;
			ar & m_frontStencilPass;

			ar & m_backStencilEnable;
			ar & m_backStencilFun;
			ar & m_backStencilReadMask;
			ar & m_backStencilWriteMask;
			ar & m_backStencilFail;
			ar & m_backStencilDepthFail;
			ar & m_backStencilPass;
		}
	};

	// 重载运算符 "<"
	FORCEINLINE bool operator<(const DepthStencilStateDesc& lhs, const DepthStencilStateDesc& rhs)
	{
		return std::memcmp( &lhs, &rhs, sizeof(lhs)) < 0;
	}

	// 重载运算符 "!="
	FORCEINLINE bool operator!=(const DepthStencilStateDesc& lhs, const DepthStencilStateDesc& rhs)
	{
		return std::memcmp( &lhs, &rhs, sizeof(lhs)) != 0;
	}

	class CORE_EXPORT DepthStencilState
	{
	public:
		// constructor
		explicit DepthStencilState( const DepthStencilStateDesc& desc)
			: m_desc( desc)
		{ }

		// 激活
		virtual void SetActive( UINT frontStencilRef, UINT backStencilRef)=0;

	protected:
		DepthStencilStateDesc m_desc;
	};

	enum BlendOperation
	{
		BOP_Add		= 1,
		BOP_Sub		= 2,
		BOP_RevSub	= 3,
		BOP_Min		= 4,
		BOP_Max		= 5,
	};

	enum AlphaBlendFactor
	{
		ABF_Zero,
		ABF_One,
		ABF_SrcAlpha,
		ABF_DstAlpha,
		ABF_InvSrcAlpha,
		ABF_InvDstAlpha,
		ABF_SrcColor,
		ABF_DstColor,
		ABF_InvSrcColor,
		ABF_InvDstColor,
		ABF_SrcAlphaSat,
	};

	//------------------------------------
	// Alpha混合状态 2012-3-30 帝林
	//------------------------------------
	struct CORE_EXPORT BlendStateDesc
	{
		bool			m_alphaToCoverageEnable;
		bool			m_independentBlendEnable;

		boost::array<bool,				8>	m_blendEnable;
		boost::array<BlendOperation,	8>	m_blendOp;
		boost::array<AlphaBlendFactor, 8>	m_srcBlend;
		boost::array<AlphaBlendFactor, 8>	m_destBlend;
		boost::array<BlendOperation,	8>  m_blendOpAlpha;
		boost::array<AlphaBlendFactor, 8>	m_srcBlendAlpha;
		boost::array<AlphaBlendFactor, 8>	m_destBlendAlpha;
		boost::array<BYTE,				8>	m_colorWriteMask;

		// 初始化
		BlendStateDesc()
			: m_alphaToCoverageEnable( false),
			  m_independentBlendEnable( false)
		{
			m_blendEnable.assign( false);
			m_blendOp.assign( BOP_Add);
			m_srcBlend.assign( ABF_One);
			m_destBlend.assign( ABF_Zero);
			m_blendOpAlpha.assign( BOP_Add);
			m_srcBlendAlpha.assign( ABF_One);
			m_destBlendAlpha.assign( ABF_Zero);
			m_colorWriteMask.assign( ColorRGB::MASK_All);
		}
	};

	// 重载运算符 "<"
	FORCEINLINE bool operator<(const BlendStateDesc& lhs, const BlendStateDesc& rhs)
	{
		return std::memcmp( &lhs, &rhs, sizeof(lhs)) < 0;
	}

	class BlendState
	{
	public:
		// constructor
		explicit BlendState( const BlendStateDesc& desc)
			: m_desc( desc)
		{ }

		// 激活
		virtual void SetActive( const ColorRGB& blendFactor=ColorRGB::White, UINT sampleMask=0xffffffff)=0;

	protected:
		BlendStateDesc m_desc;
	};

	enum TextureAddressingMode
	{
		TAM_Wrap,			// texture wraps at values over 1.0
		TAM_Mirror,			// texture mirrors (flips) at joins over 1.0
		TAM_Clamp,			// texture clamps at 1.0
		TAM_Border			// texture coordinates outside the range[0.0 1.0] are set the border color
	};

	enum TextureFilterOp
	{
		// Dont' use these enum directly
		TFOE_MipPoint	= 0x0,
		TFOE_MipLinear = 0x1,
		TFOE_MagPoint	= 0x0,
		TFOE_MagLinear = 0x2,
		TFOE_MinPoint	= 0x0,
		TFOE_MinLinear = 0x4,
		TFOE_Anisotropic= 0x0F,
		TFOE_Comparison = 0x10,

		// Use these
		TFO_MinMagMipPoint					= TFOE_MinPoint  | TFOE_MagPoint  | TFOE_MipPoint,
		TFO_MinMagPointMipLinear			= TFOE_MinPoint  | TFOE_MagPoint  | TFOE_MipLinear,
		TFO_MinPointMagLinearMipPoint		= TFOE_MinPoint  | TFOE_MagLinear | TFOE_MipPoint,
		TFO_MinPointMagMipLinear			= TFOE_MinPoint  | TFOE_MagLinear | TFOE_MipLinear,
		TFO_MinLinearMagMipPoint			= TFOE_MinLinear | TFOE_MagPoint  | TFOE_MipPoint,
		TFO_MinLinearMagPointMipLinear		= TFOE_MinLinear | TFOE_MagPoint  | TFOE_MipLinear,
		TFO_MinMagLinearMipPoint			= TFOE_MinLinear | TFOE_MagLinear | TFOE_MipPoint,
		TFO_MinMagMipLinear					= TFOE_MinLinear | TFOE_MagLinear | TFOE_MipLinear,
		TFO_Anisotropic						= TFOE_Anisotropic,

		TFO_CmpMinMagMipPoint				= TFOE_Comparison | TFO_MinMagMipPoint,
		TFO_CmpMinMagPointMipLinear			= TFOE_Comparison | TFO_MinMagPointMipLinear,
		TFO_CmpMinPointMagLinearMipPoint	= TFOE_Comparison | TFO_MinPointMagLinearMipPoint,
		TFO_CmpMinPointMagMipLinear			= TFOE_Comparison | TFO_MinPointMagMipLinear,
		TFO_CmpMinLinearMagMipPoint			= TFOE_Comparison | TFO_MinLinearMagMipPoint,
		TFO_CmpMinLinearMagPointMipLinear	= TFOE_Comparison | TFO_MinLinearMagPointMipLinear,
		TFO_CmpMinMagLinearMipPoint			= TFOE_Comparison | TFO_MinMagLinearMipPoint,
		TFO_CmpMinMagMipLinear				= TFOE_Comparison | TFO_MinMagMipLinear,
		TFO_CmpAnisotropic					= TFOE_Comparison | TFO_Anisotropic
	};

	//------------------------------------
	// 纹理采样状态 2012-3-30 帝林
	//------------------------------------
	struct SamplerStateDesc
	{
		ColorRGB				m_borderColor;

		TextureAddressingMode	m_addrModeU;
		TextureAddressingMode	m_addrModeV;
		TextureAddressingMode	m_addrModeW;

		TextureFilterOp			m_filter;

		BYTE					m_maxAnisotropy;
		float					m_minLod;
		float					m_maxLod;
		float					m_mipMapLodBias;

		CompareFunction			m_cmpFun;

		// 初始化
		SamplerStateDesc()
			: m_borderColor( 0.f, 0.f, 0.f, 0.f)
		{
			m_addrModeU		= TAM_Wrap;
			m_addrModeV		= TAM_Wrap;
			m_addrModeW		= TAM_Wrap;
			m_filter		= TFO_MinMagMipLinear;
			m_maxAnisotropy = 16;
			m_minLod		= 0;
			m_maxLod		= FLT_BIG;
			m_mipMapLodBias = 0;
			m_cmpFun		= CF_Never;
		}
	};

	// 重载运算符 "<"
	FORCEINLINE bool operator<(const SamplerStateDesc& lhs, const SamplerStateDesc& rhs)
	{
		return std::memcmp( &lhs, &rhs, sizeof(lhs)) < 0;
	}

	class CORE_EXPORT SamplerState
	{
	public:
		// constructor
		explicit SamplerState( const SamplerStateDesc& desc)
			: m_desc( desc)
		{ }

		// destructo
		virtual ~SamplerState()
		{}

	protected:
		SamplerStateDesc m_desc;
	};
}