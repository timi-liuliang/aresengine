#include "Core/Graphics/AresTexture.h"

namespace Ares
{
	// 构造函数
	Texture::Texture( TextureType type, UINT sampleCount, UINT sampleQuality, UINT accessHint)
		: m_type( type),
		  m_sampleCount( sampleCount),
		  m_sampleQuality( sampleQuality),
		  m_accessHint( accessHint)
	{

	}

	// 析构函数
	Texture::~Texture()
	{

	}

	namespace DDS
	{
		enum
		{	
			DDSPF_ALPHAPIXELS = 0x00000001,		// The surface has alpha channel information in the pixel format.
			DDSPF_ALPHA		  = 0x00000002,		// The pixel format contains alpha only information
			DDSPF_FOURCC	  = 0x00000004,		// The FourCC code is valid.
			DDSPF_RGB		  = 0x00000040,		// The RGB data in the pixel format structure is valid.
			DDSPF_LUMINANCE   = 0x00020000,		// Luminance data in the pixel format is valid.Use this flag for luminance-only or luminance+alpha surfaces,the bit depth is then ddpf.dwLuminanceBitCount.			
			DDSPF_BUMPDUDV    = 0x00080000		// Bump map dUdV data in the pixel format is valid.
		};

		enum
		{
			// Indicates a complex surface structure is being described.  A
			// complex surface structure results in the creation of more than
			// one surface.  The additional surfaces are attached to the root
			// surface.  The complex structure can only be destroyed by
			// destroying the root.
			DDSCAPS_COMPLEX		= 0x00000008,

			// Indicates that this surface can be used as a 3D texture.  It does not
			// indicate whether or not the surface is being used for that purpose.
			DDSCAPS_TEXTURE		= 0x00001000,

			// Indicates surface is one level of a mip-map. This surface will
			// be attached to other DDSCAPS_MIPMAP surfaces to form the mip-map.
			// This can be done explicitly, by creating a number of surfaces and
			// attaching them with AddAttachedSurface or by implicitly by CreateSurface.
			// If this bit is set then DDSCAPS_TEXTURE must also be set.
			DDSCAPS_MIPMAP		= 0x00400000,
		};

		enum
		{
			// This flag is used at CreateSurface time to indicate that this set of
			// surfaces is a cubic environment map
			DDSCAPS2_CUBEMAP			= 0x00000200,

			// These flags preform two functions:
			// - At CreateSurface time, they define which of the six cube faces are
			//   required by the application.
			// - After creation, each face in the cubemap will have exactly one of these
			//   bits set.
			DDSCAPS2_CUBEMAP_POSITIVEX	= 0x00000400,
			DDSCAPS2_CUBEMAP_NEGATIVEX	= 0x00000800,
			DDSCAPS2_CUBEMAP_POSITIVEY	= 0x00001000,
			DDSCAPS2_CUBEMAP_NEGATIVEY	= 0x00002000,
			DDSCAPS2_CUBEMAP_POSITIVEZ	= 0x00004000,
			DDSCAPS2_CUBEMAP_NEGATIVEZ	= 0x00008000,

			// Indicates that the surface is a volume.
			// Can be combined with DDSCAPS_MIPMAP to indicate a multi-level volume
			DDSCAPS2_VOLUME				= 0x00200000,
		};

		// 产生FourCC常量
		template <BYTE ch0, BYTE ch1, BYTE ch2, BYTE ch3>
		struct MakeFourCC
		{
			enum { value = (ch0 << 0) + (ch1 << 8) + (ch2 << 16) + (ch3 << 24) };
		};

		// dds format
		struct DDSPIXELFORMAT
		{
			UINT	m_size;				// size of structure
			UINT	m_flags;			// pixel format flags
			UINT	m_fourCC;			// fourcc code
			UINT	m_rgbBitCount;		// how many bits per pixel
			UINT	m_rBitMask;			// mask for red bits
			UINT	m_gBitMask;			// mask for green bits
			UINT	m_bBitMask;			// mask for blue bits
			UINT	m_rgbAlphaBitMask;	// mask for alpha channels
		};

		struct DDSCAPS2
		{
			UINT	m_caps1;			// capabilities of surface wanted
			UINT	m_caps2;
			UINT	m_reserved[2];
		};

		enum
		{
			DDSD_CAPS			= 0x00000001,	// default, dds_caps field is valid.
			DDSD_HEIGHT			= 0x00000002,	// height field is valid.
			DDSD_WIDTH			= 0x00000004,	// width field is valid.
			DDSD_PITCH			= 0x00000008,	// pitch is valid.
			DDSD_PIXELFORMAT	= 0x00001000,	// pixel_format is valid.
			DDSD_MIPMAPCOUNT	= 0x00020000,	// mip_map_count is valid.
			DDSD_LINEARSIZE		= 0x00080000,	// linear_size is valid
			DDSD_DEPTH			= 0x00800000,	// depth is valid
		};

		struct DDS_HEADER
		{
			UINT			m_size;			// size of the DDSURFACEDESC structure
			UINT			m_flags;		// determines what fields ara valid
			UINT			m_height;		// height of surface to be created
			UINT			m_width;		// width of input surface

			union
			{
				INT			m_pitch;		// distance to start of next line (return value only)
				UINT		m_linearSize;	// formless late-allocated optimized surface size
			};

			UINT			m_depth;		// the depth if this is a volume texture
			UINT			m_mipMapCount;	// number of mip-map levels requestde
			UINT			m_reserved1[11];// reserved
			DDSPIXELFORMAT	m_pixelFormat;	// pixel format description of the surface
			DDSCAPS2		m_ddsCaps;		// direct draw surface capabilities
			UINT			m_reserved2;
		};

		// dx10 资源
		enum D3D10_RESOURCE_DIMENSION
		{
			D3D10_RESOURCE_DIMENSION_UNKNOWN   = 0,
			D3D10_RESOURCE_DIMENSION_BUFFER    = 1,
			D3D10_RESOURCE_DIMENSION_TEXTURE1D = 2,
			D3D10_RESOURCE_DIMENSION_TEXTURE2D = 3,
			D3D10_RESOURCE_DIMENSION_TEXTURE3D = 4,
		};

		enum D3D10_RESOURCE_MISC_FLAG
		{
			D3D10_RESOURCE_MISC_GENERATE_MIPS	  = 0x1L,
			D3D10_RESOURCE_MISC_SHARED			  = 0x2L,
			D3D10_RESOURCE_MISC_TEXTURECUBE		  = 0x4L,
			D3D10_RESOURCE_MISC_SHARED_KEYEDMUTEX = 0x10L,
			D3D10_RESOURCE_MISC_GDI_COMPATIBLE	  = 0x20L
		};

		// DDS_HEADER_DXT10
		struct DDS_HEADER_DXT10
		{
			UINT					 m_dxgiFormat;
			D3D10_RESOURCE_DIMENSION m_resDimension;
			UINT					 m_miscFlag;
			UINT					 m_arraySize;
			UINT					 m_reserved;
		};

		struct DDSSURFACEDESC2
		{
			UINT			m_size;					// size o the DDSSURFACEDESC structure
			UINT			m_flags;				// determines what fields are valid
			UINT			m_height;				// height of surface to be created
			UINT			m_width;				// width of input surface
			union
			{
				INT			m_pitch;				// distance to start of next line (return value only)
				UINT		m_linearSize;			// Formless late-allocated optimized surface size
			};
			UINT			m_depth;				// the dept if this is a volume texture
			UINT			m_mipMapCount;			// number of mip-map levels requestde
			UINT			m_reserved1[11];		// reserved
			DDSPIXELFORMAT	m_pixelFormat;			// pixel format description of the surface
			DDSCAPS2		m_ddsCaps;				// direct draw surface capabilities
			UINT			m_reserved2;
		};

	#ifndef DXGI_FORMAT_DEFINED
		enum DXGI_FORMAT
		{
			DXGI_FORMAT_UNKNOWN	                    = 0,
			DXGI_FORMAT_R32G32B32A32_TYPELESS       = 1,
			DXGI_FORMAT_R32G32B32A32_FLOAT          = 2,
			DXGI_FORMAT_R32G32B32A32_UINT           = 3,
			DXGI_FORMAT_R32G32B32A32_SINT           = 4,
			DXGI_FORMAT_R32G32B32_TYPELESS          = 5,
			DXGI_FORMAT_R32G32B32_FLOAT             = 6,
			DXGI_FORMAT_R32G32B32_UINT              = 7,
			DXGI_FORMAT_R32G32B32_SINT              = 8,
			DXGI_FORMAT_R16G16B16A16_TYPELESS       = 9,
			DXGI_FORMAT_R16G16B16A16_FLOAT          = 10,
			DXGI_FORMAT_R16G16B16A16_UNORM          = 11,
			DXGI_FORMAT_R16G16B16A16_UINT           = 12,
			DXGI_FORMAT_R16G16B16A16_SNORM          = 13,
			DXGI_FORMAT_R16G16B16A16_SINT           = 14,
			DXGI_FORMAT_R32G32_TYPELESS             = 15,
			DXGI_FORMAT_R32G32_FLOAT                = 16,
			DXGI_FORMAT_R32G32_UINT                 = 17,
			DXGI_FORMAT_R32G32_SINT                 = 18,
			DXGI_FORMAT_R32G8X24_TYPELESS           = 19,
			DXGI_FORMAT_D32_FLOAT_S8X24_UINT        = 20,
			DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS    = 21,
			DXGI_FORMAT_X32_TYPELESS_G8X24_UINT     = 22,
			DXGI_FORMAT_R10G10B10A2_TYPELESS        = 23,
			DXGI_FORMAT_R10G10B10A2_UNORM           = 24,
			DXGI_FORMAT_R10G10B10A2_UINT            = 25,
			DXGI_FORMAT_R11G11B10_FLOAT             = 26,
			DXGI_FORMAT_R8G8B8A8_TYPELESS           = 27,
			DXGI_FORMAT_R8G8B8A8_UNORM              = 28,
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB         = 29,
			DXGI_FORMAT_R8G8B8A8_UINT               = 30,
			DXGI_FORMAT_R8G8B8A8_SNORM              = 31,
			DXGI_FORMAT_R8G8B8A8_SINT               = 32,
			DXGI_FORMAT_R16G16_TYPELESS             = 33,
			DXGI_FORMAT_R16G16_FLOAT                = 34,
			DXGI_FORMAT_R16G16_UNORM                = 35,
			DXGI_FORMAT_R16G16_UINT                 = 36,
			DXGI_FORMAT_R16G16_SNORM                = 37,
			DXGI_FORMAT_R16G16_SINT                 = 38,
			DXGI_FORMAT_R32_TYPELESS                = 39,
			DXGI_FORMAT_D32_FLOAT                   = 40,
			DXGI_FORMAT_R32_FLOAT                   = 41,
			DXGI_FORMAT_R32_UINT                    = 42,
			DXGI_FORMAT_R32_SINT                    = 43,
			DXGI_FORMAT_R24G8_TYPELESS              = 44,
			DXGI_FORMAT_D24_UNORM_S8_UINT           = 45,
			DXGI_FORMAT_R24_UNORM_X8_TYPELESS       = 46,
			DXGI_FORMAT_X24_TYPELESS_G8_UINT        = 47,
			DXGI_FORMAT_R8G8_TYPELESS               = 48,
			DXGI_FORMAT_R8G8_UNORM                  = 49,
			DXGI_FORMAT_R8G8_UINT                   = 50,
			DXGI_FORMAT_R8G8_SNORM                  = 51,
			DXGI_FORMAT_R8G8_SINT                   = 52,
			DXGI_FORMAT_R16_TYPELESS                = 53,
			DXGI_FORMAT_R16_FLOAT                   = 54,
			DXGI_FORMAT_D16_UNORM                   = 55,
			DXGI_FORMAT_R16_UNORM                   = 56,
			DXGI_FORMAT_R16_UINT                    = 57,
			DXGI_FORMAT_R16_SNORM                   = 58,
			DXGI_FORMAT_R16_SINT                    = 59,
			DXGI_FORMAT_R8_TYPELESS                 = 60,
			DXGI_FORMAT_R8_UNORM                    = 61,
			DXGI_FORMAT_R8_UINT                     = 62,
			DXGI_FORMAT_R8_SNORM                    = 63,
			DXGI_FORMAT_R8_SINT                     = 64,
			DXGI_FORMAT_A8_UNORM                    = 65,
			DXGI_FORMAT_R1_UNORM                    = 66,
			DXGI_FORMAT_R9G9B9E5_SHAREDEXP          = 67,
			DXGI_FORMAT_R8G8_B8G8_UNORM             = 68,
			DXGI_FORMAT_G8R8_G8B8_UNORM             = 69,
			DXGI_FORMAT_BC1_TYPELESS                = 70,
			DXGI_FORMAT_BC1_UNORM                   = 71,
			DXGI_FORMAT_BC1_UNORM_SRGB              = 72,
			DXGI_FORMAT_BC2_TYPELESS                = 73,
			DXGI_FORMAT_BC2_UNORM                   = 74,
			DXGI_FORMAT_BC2_UNORM_SRGB              = 75,
			DXGI_FORMAT_BC3_TYPELESS                = 76,
			DXGI_FORMAT_BC3_UNORM                   = 77,
			DXGI_FORMAT_BC3_UNORM_SRGB              = 78,
			DXGI_FORMAT_BC4_TYPELESS                = 79,
			DXGI_FORMAT_BC4_UNORM                   = 80,
			DXGI_FORMAT_BC4_SNORM                   = 81,
			DXGI_FORMAT_BC5_TYPELESS                = 82,
			DXGI_FORMAT_BC5_UNORM                   = 83,
			DXGI_FORMAT_BC5_SNORM                   = 84,
			DXGI_FORMAT_B5G6R5_UNORM                = 85,
			DXGI_FORMAT_B5G5R5A1_UNORM              = 86,
			DXGI_FORMAT_B8G8R8A8_UNORM              = 87,
			DXGI_FORMAT_B8G8R8X8_UNORM              = 88,
			DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM  = 89,
			DXGI_FORMAT_B8G8R8A8_TYPELESS           = 90,
			DXGI_FORMAT_B8G8R8A8_UNORM_SRGB         = 91,
			DXGI_FORMAT_B8G8R8X8_TYPELESS           = 92,
			DXGI_FORMAT_B8G8R8X8_UNORM_SRGB         = 93,
			DXGI_FORMAT_BC6H_TYPELESS               = 94,
			DXGI_FORMAT_BC6H_UF16                   = 95,
			DXGI_FORMAT_BC6H_SF16                   = 96,
			DXGI_FORMAT_BC7_TYPELESS                = 97,
			DXGI_FORMAT_BC7_UNORM                   = 98,
			DXGI_FORMAT_BC7_UNORM_SRGB              = 99,
			DXGI_FORMAT_FORCE_UINT                  = 0xffffffff,
		};
	#endif

		// DDS转Ares(图形格式)
		ElementFormat DXGIFormatToElementFormat( UINT format)
		{
			switch( format)
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
				A_ASSERT( false);
			}

			return EF_UnKnown;
		}

		DXGI_FORMAT ElementFormatToDXGIFormat( ElementFormat format)
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
			case EF_ARGB8_SRGB:
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

			case EF_D16:
				return DXGI_FORMAT_D16_UNORM;

			case EF_D24S8:
				return DXGI_FORMAT_D24_UNORM_S8_UINT;

			case EF_D32F:
				return DXGI_FORMAT_D32_FLOAT;

			case EF_ABGR8_SRGB:
				return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

			case EF_BC1_SRGB:
				return DXGI_FORMAT_BC1_UNORM_SRGB;

			case EF_BC2_SRGB:
				return DXGI_FORMAT_BC2_UNORM_SRGB;

			case EF_BC3_SRGB:
				return DXGI_FORMAT_BC3_UNORM_SRGB;

			default:
				A_ASSERT(false);
				return DXGI_FORMAT_UNKNOWN;
			}
		}
	}

	// 从硬盘中加载纹理数据 only *.dds
	bool Texture::LoadTexture( const char* texPath, TextureType& type, UINT& width, UINT& height, UINT& depth, UINT& numMipMaps, UINT& arraySize, ElementFormat& format, vector<ElementInitData>& initData, vector<BYTE>& dataBlock)
	{
		using namespace DDS;

		FILE* fileHandle = fopen( texPath, "rb");
		if( !fileHandle)
			return false;

		// 检测文件大小,一次性读进内存
		fseek( fileHandle, 0, SEEK_END);
		size_t iEnd = ftell( fileHandle);
		fseek( fileHandle, 0, SEEK_SET);
		size_t iStart = ftell( fileHandle);
		size_t fileSize = iEnd - iStart;

		BYTE* bufPtr    = NULL;

		// Allocate memory for whole file
		vector<BYTE> texBuffer( fileSize);
		bufPtr     = &texBuffer[0];

		// load file into buffer
		size_t size = fread( bufPtr, 1, fileSize, fileHandle);

		// close the file , we don't need it any more
		fflush( fileHandle);
		fclose( fileHandle);

		if( size != fileSize)
			return false;

		// read flag
		UINT ddsFlag;
		memcpy( &ddsFlag, bufPtr, sizeof(UINT));
		bufPtr += sizeof( UINT);

		if( ddsFlag != MakeFourCC<'D', 'D', 'S', ' '>::value)
			return false;

		// read desc
		DDS_HEADER desc;
		memcpy( &desc, bufPtr, sizeof(desc));
		bufPtr += sizeof(desc);

		DDS_HEADER_DXT10 desc10;
		if( desc.m_pixelFormat.m_fourCC == MakeFourCC<'D', 'D', 'S', '0'>::value)
		{
			std::memcpy( &desc10, bufPtr, sizeof(desc10));
			arraySize = desc10.m_arraySize;
		}
		else
		{
			std::memset( &desc10, 0, sizeof(desc10));
			arraySize = 1;

			BOOST_ASSERT( (desc.m_flags & DDSD_CAPS)!=0);
			BOOST_ASSERT( (desc.m_flags & DDSD_PIXELFORMAT)!=0);
		}

		BOOST_ASSERT((desc.m_flags & DDSD_CAPS)!=0);
		BOOST_ASSERT((desc.m_flags & DDSD_HEIGHT)!=0);

		if( (desc.m_flags & DDSD_MIPMAPCOUNT)==0)
			desc.m_mipMapCount = 1;

		format = EF_ARGB8;
		if( (desc.m_pixelFormat.m_flags & DDSPF_FOURCC) != 0)
		{
			// "Programming Guide for DDS"
			// http://msdn.microsoft.com/en-us/library/bb943991.aspx
			switch( desc.m_pixelFormat.m_fourCC)
			{
			case 36:
				format = EF_ABGR16;
				break;

			case 110:
				format = EF_SIGNED_ABGR16;
				break;

			case 111:
				format = EF_R16F;
				break;

			case 112:
				format = EF_GR16F;
				break;

			case 113:
				format = EF_ABGR16F;
				break;

			case 114:
				format = EF_R32F;
				break;

			case 116:
				format = EF_ABGR32F;
				break;

			case MakeFourCC<'D', 'X', 'T', '1'>::value:
				format = EF_BC1;
				break;

			case MakeFourCC<'D', 'X', 'T', '3'>::value:
				format = EF_BC2;
				break;

			case MakeFourCC<'D', 'X', 'T', '5'>::value:
				format = EF_BC3;
				break;

			case MakeFourCC<'B', 'C', '4', 'U'>::value:
			case MakeFourCC<'A', 'T', 'I', '1'>::value:
				format = EF_BC4;
				break;

			case MakeFourCC<'B', 'C', '4', 'S'>::value:
				format = EF_SIGNED_BC4;
				break;

			case MakeFourCC<'A', 'T', 'I', '2'>::value:
				format = EF_BC5;
				break;;

			case MakeFourCC<'B', 'C', '5', 'S'>::value:
				format = EF_SIGNED_BC5;
				break;

			case MakeFourCC<'D', 'X', '1', '0'>::value:
				format = DXGIFormatToElementFormat( desc10.m_dxgiFormat);
				break;
			}
		}
		else
		{
			if( (desc.m_pixelFormat.m_flags & DDSPF_RGB) != 0)
			{
				// 象素位数
				switch( desc.m_pixelFormat.m_rgbBitCount)
				{
				case 16:
					{
						if( 0xF000 == desc.m_pixelFormat.m_rgbAlphaBitMask && 0x0F00 == desc.m_pixelFormat.m_rBitMask &&0x0F00 == desc.m_pixelFormat.m_rBitMask && 0x0F00 == desc.m_pixelFormat.m_rBitMask)
							format = EF_ARGB4;
						else
							BOOST_ASSERT( false);
					}
					break;

				case 32:
					{
						if( 0x00FF0000==desc.m_pixelFormat.m_rBitMask && 0x0000FF00==desc.m_pixelFormat.m_gBitMask && 0x000000FF==desc.m_pixelFormat.m_bBitMask)
						{
							if( (desc.m_pixelFormat.m_flags & DDSPF_ALPHAPIXELS) != 0)
								format = EF_ARGB8;
							else
								BOOST_ASSERT( false);
						}
						else
						{
							if ( 0xC0000000 == desc.m_pixelFormat.m_rgbAlphaBitMask && 0x3FF00000 == desc.m_pixelFormat.m_rBitMask && 0x000FFC00 == desc.m_pixelFormat.m_gBitMask && 0x000003FF == desc.m_pixelFormat.m_bBitMask)
								format = EF_A2BGR10;
							else
							{
								if ((0xFF000000 == desc.m_pixelFormat.m_rgbAlphaBitMask) && (0x000000FF == desc.m_pixelFormat.m_rBitMask) && (0x0000FF00 == desc.m_pixelFormat.m_gBitMask) && (0x00FF0000 == desc.m_pixelFormat.m_bBitMask))
									format = EF_ABGR8;
								else
								{
									if ((0x00000000 == desc.m_pixelFormat.m_rgbAlphaBitMask) && (0x0000FFFF == desc.m_pixelFormat.m_rBitMask) && (0xFFFF0000 == desc.m_pixelFormat.m_gBitMask) && (0x00000000 == desc.m_pixelFormat.m_bBitMask))
										format = EF_GR16;
									else
										A_ASSERT(false);
								}
							}
						}
					}
					break;

				default:
					{
						A_ASSERT( false);
					}
					break;
				}
			}
			else
			{
				if( (desc.m_pixelFormat.m_flags & DDSPF_LUMINANCE) != 0)
				{
					switch( desc.m_pixelFormat.m_rgbBitCount)
					{
					case 8:
						{
							if( 0 == (desc.m_pixelFormat.m_flags & DDSPF_ALPHAPIXELS))
								format = EF_R8;
							else
								A_ASSERT( false);
						}
						break;

					case 16:
						{
							if( 0 == (desc.m_pixelFormat.m_flags & DDSPF_ALPHAPIXELS))
								format = EF_R16;
							else
								A_ASSERT( false);								
						}
						break;

					default:
						{
							A_ASSERT( false);
						}
						break;
					}
				}
				else
				{
					if( (desc.m_pixelFormat.m_flags & DDSPF_BUMPDUDV) != 0)
					{
						switch( desc.m_pixelFormat.m_rgbBitCount)
						{
						case 16:
							{
								if( 0x000000FF == desc.m_pixelFormat.m_rBitMask && 0x0000FF00==desc.m_pixelFormat.m_gBitMask)
									format = EF_SIGNED_GR8;
								else
								{
									if( 0x0000FFFF == desc.m_pixelFormat.m_rBitMask)
										format = EF_SIGNED_R16;
									else
										A_ASSERT( false);
								}
							}
							break;

						case 32:
							{
								if(0x000000FF == desc.m_pixelFormat.m_rBitMask && 0x0000FF00 == desc.m_pixelFormat.m_gBitMask && 0x00FF0000==desc.m_pixelFormat.m_bBitMask)
									format = EF_SIGNED_ABGR8;
								else
								{
									if( (0xC0000000 == desc.m_pixelFormat.m_rgbAlphaBitMask) && (0x3FF00000 == desc.m_pixelFormat.m_rBitMask) && (0x000FFC00 == desc.m_pixelFormat.m_gBitMask) && (0x000003FF == desc.m_pixelFormat.m_bBitMask))
										format = EF_SIGNED_A2BGR10;
									else
									{
										if ((0x00000000 == desc.m_pixelFormat.m_rgbAlphaBitMask) && (0x0000FFFF == desc.m_pixelFormat.m_rBitMask) && (0xFFFF0000 == desc.m_pixelFormat.m_gBitMask) && (0x00000000 == desc.m_pixelFormat.m_bBitMask))
											format = EF_SIGNED_GR16;
										else
											A_ASSERT(false);
									}
								}
							}
							break;

						default:
							{
								A_ASSERT( false);
							}
							break;
						}
					}
					else
					{
						if( (desc.m_pixelFormat.m_flags & DDSPF_ALPHA) != 0)
							format = EF_A8;
						else
							A_ASSERT( false);
					}
				}
			}
		}

		// 图形尺寸
		UINT  mainImageSize;
		if( (desc.m_flags & DDSD_LINEARSIZE) != 0)
			mainImageSize = desc.m_linearSize;
		else
		{
			if( (desc.m_flags & DDSD_PITCH) != 0)
				mainImageSize = desc.m_pitch * desc.m_height;
			else
			{
				if( (desc.m_flags & desc.m_pixelFormat.m_flags & 0x00000040) != 0)
					mainImageSize = desc.m_width * desc.m_height * desc.m_pixelFormat.m_rgbBitCount / 8;
				else
					mainImageSize = desc.m_width * desc.m_height * NumFormatBytes( format);
			}
		}

		if( desc.m_reserved1[0] != 0)
			format = MakeSRGB( format);

		// 宽高
		width	   = desc.m_width;
		numMipMaps = desc.m_mipMapCount;
		if( MakeFourCC<'D', 'X', '1', '0'>::value == desc.m_pixelFormat.m_fourCC)
		{
			// 立方体贴图
			if( D3D10_RESOURCE_MISC_TEXTURECUBE == desc10.m_miscFlag)
			{
				type   = TT_Cube;
				arraySize /= 6;
				height = desc.m_width;
				depth  = 1;
			}
			else
			{
				switch( desc10.m_resDimension)
				{
				case D3D10_RESOURCE_DIMENSION_TEXTURE1D:
					{
						type   = TT_1D;
						height = 1;
						depth  = 1;
					}
					break;

				case D3D10_RESOURCE_DIMENSION_TEXTURE2D:
					{
						type   = TT_2D;
						height = desc.m_height;
						depth  = 1;
					}
					break;

				case D3D10_RESOURCE_DIMENSION_TEXTURE3D:
					{
						type   = TT_3D;
						height = desc.m_height;
						depth  = desc.m_depth;
					}
					break;

				default:
					{
						A_ASSERT(false);
					}
					break;
				}
			}
		}
		else
		{
			if( (desc.m_ddsCaps.m_caps2 & DDSCAPS2_CUBEMAP) != 0)
			{
				type   = TT_Cube;
				height = desc.m_width;
				depth  = 1;
			}
			else
			{
				if( (desc.m_ddsCaps.m_caps2 & DDSCAPS2_VOLUME) != 0)
				{
					type   = TT_3D;
					height = desc.m_width;
					depth  = desc.m_depth;
				}
				else
				{
					type   = TT_2D;
					height = desc.m_height;
					depth  = 1;
				}
			}
		}

		vector<size_t> base;
		UINT formatSize = NumFormatBytes( format);
		switch( type)
		{
		case TT_1D:
			{
				initData.resize( arraySize * numMipMaps);
				base.resize( arraySize * numMipMaps);
				for( UINT arrayIndex=0; arrayIndex<arraySize; arrayIndex++)
				{
					UINT theWidth = width;
					for (UINT level = 0; level < numMipMaps; ++ level)
					{
						size_t const index = arrayIndex * numMipMaps + level;
						UINT imageSize;
						if (IsCompressedFormat(format))
						{
							int block_size;
							if ((EF_BC1 == format) || (EF_SIGNED_BC1 == format) || (EF_BC1_SRGB == format) || (EF_BC4 == format) || (EF_SIGNED_BC4 == format) || (EF_BC4_SRGB == format))
								block_size = 8;
							else
								block_size = 16;

							imageSize = ((theWidth + 3) / 4) * block_size;
						}
						else
						{
							imageSize = mainImageSize / (1UL << (level * 2));
						}

						base[index] = dataBlock.size();
						dataBlock.resize(base[index] + imageSize);
						initData[index].m_rowPitch   = imageSize;
						initData[index].m_slicePitch = imageSize;

						memcpy( &dataBlock[base[index]], bufPtr, imageSize);
						bufPtr += imageSize;

						theWidth = std::max<UINT>( theWidth / 2, 1);
					}
				}
			}
			break;

		case TT_2D:
			{
				initData.resize(arraySize * numMipMaps);
				base.resize(arraySize * numMipMaps);
				for ( UINT arrayIndex = 0; arrayIndex < arraySize; ++ arrayIndex)
				{
					UINT theWidth = width;
					UINT theHeight = height;
					for ( UINT level = 0; level < numMipMaps; ++level)
					{
						size_t const index = arrayIndex * numMipMaps + level;
						if (IsCompressedFormat(format))
						{
							int blockSize;
							if ((EF_BC1 == format) || (EF_SIGNED_BC1 == format) || (EF_BC1_SRGB == format) || (EF_BC4 == format) || (EF_SIGNED_BC4 == format) || (EF_BC4_SRGB == format))
								blockSize = 8;
							else
								blockSize = 16;

							UINT imageSize = ((theWidth + 3) / 4) * ((theHeight + 3) / 4) * blockSize;

							base[index] = dataBlock.size();
							dataBlock.resize(base[index] + imageSize);
							initData[index].m_rowPitch = (theWidth + 3) / 4 * blockSize;
							initData[index].m_slicePitch = imageSize;

							memcpy( &dataBlock[base[index]], bufPtr, imageSize);
							bufPtr += imageSize;
						}
						else
						{
							if (desc.m_flags & DDSD_PITCH)
								initData[index].m_rowPitch = static_cast<UINT>(((desc.m_pitch >> level) + 3) / 4 * 4);
							else
								initData[index].m_rowPitch = theWidth * formatSize;
	
							initData[index].m_slicePitch = initData[index].m_rowPitch * theHeight;
							base[index] = dataBlock.size();
							dataBlock.resize(base[index] + initData[index].m_slicePitch);

							memcpy( &dataBlock[base[index]], bufPtr, initData[index].m_slicePitch);
							bufPtr += initData[index].m_slicePitch;
						}

						theWidth = std::max<UINT>(theWidth / 2, 1);
						theHeight = std::max<UINT>(theHeight / 2, 1);
					}
				}
			}
			break;

		case TT_3D:
			{
				initData.resize( arraySize * numMipMaps);
				base.resize( arraySize * numMipMaps);
				for ( UINT arrayIndex = 0; arrayIndex < arraySize; arrayIndex++)
				{
					UINT theWidth  = width;
					UINT theHeight = height;
					UINT theDepth  = depth;
					for (UINT level = 0; level < numMipMaps; ++ level)
					{
						size_t const index = arrayIndex * numMipMaps + level;
						if ( IsCompressedFormat(format))
						{
							int blockSize;
							if ((EF_BC1 == format) || (EF_SIGNED_BC1 == format) || (EF_BC1_SRGB == format) || (EF_BC4 == format) || (EF_SIGNED_BC4 == format) || (EF_BC4_SRGB == format))
								blockSize = 8;
							else
								blockSize = 16;

							UINT imageSize = ((theWidth + 3) / 4) * ((theHeight + 3) / 4) * theDepth * blockSize;

							base[index] = dataBlock.size();
							dataBlock.resize( base[index] + imageSize);
							initData[index].m_rowPitch = (theWidth + 3) / 4 * blockSize;
							initData[index].m_slicePitch = ((theWidth + 3) / 4) * ((theHeight + 3) / 4) * blockSize;

							memcpy( &dataBlock[base[index]], bufPtr, imageSize);
							bufPtr += imageSize;
						}
						else
						{
							if (desc.m_flags & DDSD_PITCH)
							{
								initData[index].m_rowPitch = static_cast<UINT>(((desc.m_pitch >> level) + 3) / 4 * 4);
								initData[index].m_slicePitch = initData[index].m_rowPitch * (theHeight + 3) / 4 * 4;
							}
							else
							{
								initData[index].m_rowPitch = theWidth * formatSize;
								initData[index].m_slicePitch = initData[index].m_rowPitch * theHeight;
							}
							base[index] = dataBlock.size();
							dataBlock.resize(base[index] + initData[index].m_slicePitch * theDepth);

							memcpy( &dataBlock[base[index]], bufPtr, initData[index].m_slicePitch * theDepth);
							bufPtr += initData[index].m_slicePitch * theDepth;
						}

						theWidth  = std::max<UINT>(theWidth / 2, 1);
						theHeight = std::max<UINT>(theHeight / 2, 1);
						theDepth  = std::max<UINT>(theDepth / 2, 1);
					}
				}
			}
			break;

		case TT_Cube:
			{
				initData.resize( arraySize * 6 * numMipMaps);
				base.resize( arraySize * 6 * numMipMaps);
				for (UINT array_index = 0; array_index < arraySize; ++ array_index)
				{
					for (UINT face = Texture::CF_Positive_X; face <= Texture::CF_Negative_Z; ++ face)
					{
						UINT theWidth = width;
						UINT theHeight = height;
						for (UINT level = 0; level < numMipMaps; ++ level)
						{
							size_t const index = (array_index * 6 + face - Texture::CF_Positive_X) * numMipMaps + level;
							if (IsCompressedFormat(format))
							{
								int blockSize;
								if ((EF_BC1 == format) || (EF_SIGNED_BC1 == format) || (EF_BC1_SRGB == format) || (EF_BC4 == format) || (EF_SIGNED_BC4 == format) || (EF_BC4_SRGB == format))
								{
									blockSize = 8;
								}
								else
								{
									blockSize = 16;
								}

								UINT imageSize = ((theWidth + 3) / 4) * ((theHeight + 3) / 4) * blockSize;

								base[index] = dataBlock.size();
								dataBlock.resize(base[index] + imageSize);
								initData[index].m_rowPitch = (theWidth + 3) / 4 * blockSize;
								initData[index].m_slicePitch = imageSize;

								memcpy( &dataBlock[base[index]], bufPtr, imageSize);
								bufPtr += imageSize;
							}
							else
							{
								if (desc.m_flags & DDSD_PITCH)
								{
									initData[index].m_rowPitch = static_cast<UINT>(((desc.m_pitch >> level) + 3) / 4 * 4);
								}
								else
								{
									initData[index].m_rowPitch = theWidth * formatSize;
								}
								initData[index].m_slicePitch = initData[index].m_rowPitch * theWidth;
								base[index] = dataBlock.size();
								dataBlock.resize(base[index] + initData[index].m_slicePitch);

								memcpy( &dataBlock[base[index]], bufPtr, initData[index].m_slicePitch);
								bufPtr += initData[index].m_slicePitch;
							}

							theWidth = std::max<UINT>(theWidth / 2, 1);
							theHeight = std::max<UINT>(theHeight / 2, 1);
						}
					}
				}
			}
			break;
		}

		for ( size_t i=0; i<base.size(); i++)
		{
			initData[i].m_data = &dataBlock[base[i]];
		}

		return true;
	}

	// 向硬盘中写入纹理数据
	bool Texture::SaveTexture( const char* texPath, TextureType type, UINT width, UINT height, UINT depth, UINT numMipMaps, UINT arraySize, ElementFormat format, const vector<ElementInitData>& initData)
	{
		using namespace DDS;

		FILE* fileHandle = fopen( texPath, "wb");
		if( !fileHandle)
			return false;

		// 保存文件标识
		UINT magic = MakeFourCC<'D', 'D', 'S', ' '>::value;
		fwrite( &magic, sizeof(magic), 1, fileHandle);

		DDSSURFACEDESC2 desc;
		std::memset( &desc, 0, sizeof(desc));

		desc.m_size   = sizeof( desc);
		desc.m_flags |= DDSD_WIDTH;
		desc.m_flags |= DDSD_HEIGHT;
		desc.m_width  = width;
		desc.m_height = height;

		if( numMipMaps != 0)
		{
			desc.m_flags	   |= DDSD_MIPMAPCOUNT;
			desc.m_mipMapCount  = numMipMaps;
		}

		desc.m_pixelFormat.m_size = sizeof( desc.m_pixelFormat);

		if( IsSRGB( format))
			desc.m_reserved1[0] = 1;

		if( arraySize != 1)
		{
			desc.m_pixelFormat.m_flags  |= DDSPF_FOURCC;
			desc.m_pixelFormat.m_fourCC |= MakeFourCC<'D', 'X', '1', '0'>::value;
		}
		else
		{
			if( EF_ABGR16==format || IsFloatFormat(format) || IsCompressedFormat(format))
			{
				desc.m_pixelFormat.m_flags |= DDSPF_FOURCC;

				switch( format)
				{
				case EF_ABGR16:
					desc.m_pixelFormat.m_fourCC = 36;
					break;

				case EF_SIGNED_ABGR16:
					desc.m_pixelFormat.m_fourCC = 110;
					break;

				case EF_R16F:
					desc.m_pixelFormat.m_fourCC = 111;
					break;

				case EF_GR16F:
					desc.m_pixelFormat.m_fourCC = 112;
					break;

				case EF_ABGR16F:
					desc.m_pixelFormat.m_fourCC = 113;
					break;

				case EF_R32F:
					desc.m_pixelFormat.m_fourCC = 114;
					break;

				case EF_GR32F:
					desc.m_pixelFormat.m_fourCC = 115;
					break;

				case EF_ABGR32F:
					desc.m_pixelFormat.m_fourCC = 116;
					break;

				case EF_BC1:
				case EF_BC1_SRGB:
					desc.m_pixelFormat.m_fourCC = MakeFourCC<'D', 'X', 'T', '1'>::value;
					break;

				case EF_BC2:
				case EF_BC2_SRGB:
					desc.m_pixelFormat.m_fourCC = MakeFourCC<'D', 'X', 'T', '3'>::value;
					break;

				case EF_BC3:
				case EF_BC3_SRGB:
					desc.m_pixelFormat.m_fourCC = MakeFourCC<'D', 'X', 'T', '5'>::value;
					break;

				case EF_BC4:
				case EF_BC4_SRGB:
					desc.m_pixelFormat.m_fourCC = MakeFourCC<'B', 'C', '4', 'U'>::value;
					break;

				case EF_SIGNED_BC4:
					desc.m_pixelFormat.m_fourCC = MakeFourCC<'B', 'C', '4', 'S'>::value;
					break;

				case EF_BC5:
				case EF_BC5_SRGB:
					desc.m_pixelFormat.m_fourCC = MakeFourCC<'A', 'T', 'I', '2'>::value;
					break;

				case EF_SIGNED_BC5:
					desc.m_pixelFormat.m_fourCC = MakeFourCC<'B', 'C', '5', 'S'>::value;
					break;

				case EF_B10G11R11F:
					desc.m_pixelFormat.m_fourCC = MakeFourCC<'D', 'X', '1', '0'>::value;
					break;

				default:
					A_ASSERT(false);
					break;
				}
			}
			else
			{
				switch (format)
				{
				case EF_ARGB4:
					{
						desc.m_pixelFormat.m_flags |= DDSPF_RGB;
						desc.m_pixelFormat.m_flags |= DDSPF_ALPHAPIXELS;
						desc.m_pixelFormat.m_rgbBitCount = 16;

						desc.m_pixelFormat.m_rgbAlphaBitMask = 0x0000F000;
						desc.m_pixelFormat.m_rBitMask = 0x00000F00;
						desc.m_pixelFormat.m_gBitMask = 0x000000F0;
						desc.m_pixelFormat.m_bBitMask = 0x0000000F;
					}
					break;

				case EF_SIGNED_GR8:
					{
						desc.m_pixelFormat.m_flags |= DDSPF_BUMPDUDV;
						desc.m_pixelFormat.m_rgbBitCount = 16;

						desc.m_pixelFormat.m_rgbAlphaBitMask = 0x00000000;
						desc.m_pixelFormat.m_rBitMask = 0x000000FF;
						desc.m_pixelFormat.m_gBitMask = 0x0000FF00;
						desc.m_pixelFormat.m_bBitMask = 0x00000000;
					}
					break;

				case EF_SIGNED_R16:
					{
						desc.m_pixelFormat.m_flags |= DDSPF_BUMPDUDV;
						desc.m_pixelFormat.m_rgbBitCount = 16;

						desc.m_pixelFormat.m_rgbAlphaBitMask = 0x00000000;
						desc.m_pixelFormat.m_rBitMask = 0x0000FFFF;
						desc.m_pixelFormat.m_gBitMask = 0x00000000;
						desc.m_pixelFormat.m_bBitMask = 0x00000000;
					}
					break;

				case EF_ARGB8:
				case EF_ARGB8_SRGB:
					{
						desc.m_pixelFormat.m_flags |= DDSPF_RGB;
						desc.m_pixelFormat.m_flags |= DDSPF_ALPHAPIXELS;
						desc.m_pixelFormat.m_rgbBitCount = 32;

						desc.m_pixelFormat.m_rgbAlphaBitMask = 0xFF000000;
						desc.m_pixelFormat.m_rBitMask = 0x00FF0000;
						desc.m_pixelFormat.m_gBitMask = 0x0000FF00;
						desc.m_pixelFormat.m_bBitMask = 0x000000FF;
					}
					break;

				case EF_ABGR8:
				case EF_ABGR8_SRGB:
					{
						desc.m_pixelFormat.m_flags |= DDSPF_RGB;
						desc.m_pixelFormat.m_flags |= DDSPF_ALPHAPIXELS;
						desc.m_pixelFormat.m_rgbBitCount = 32;

						desc.m_pixelFormat.m_rgbAlphaBitMask = 0xFF000000;
						desc.m_pixelFormat.m_rBitMask = 0x000000FF;
						desc.m_pixelFormat.m_gBitMask = 0x0000FF00;
						desc.m_pixelFormat.m_bBitMask = 0x00FF0000;
					}
					break;

				case EF_SIGNED_ABGR8:
					{
						desc.m_pixelFormat.m_flags |= DDSPF_BUMPDUDV;
						desc.m_pixelFormat.m_rgbBitCount = 32;

						desc.m_pixelFormat.m_rgbAlphaBitMask = 0xFF000000;
						desc.m_pixelFormat.m_rBitMask = 0x000000FF;
						desc.m_pixelFormat.m_gBitMask = 0x0000FF00;
						desc.m_pixelFormat.m_bBitMask = 0x00FF0000;
					}
					break;

				case EF_A2BGR10:
					{
						desc.m_pixelFormat.m_flags |= DDSPF_RGB;
						desc.m_pixelFormat.m_flags |= DDSPF_ALPHAPIXELS;
						desc.m_pixelFormat.m_rgbBitCount = 32;

						desc.m_pixelFormat.m_rgbAlphaBitMask = 0xC0000000;
						desc.m_pixelFormat.m_rBitMask = 0x000003FF;
						desc.m_pixelFormat.m_gBitMask = 0x000FFC00;
						desc.m_pixelFormat.m_bBitMask = 0x3FF00000;
					}
					break;

				case EF_SIGNED_A2BGR10:
					{
						desc.m_pixelFormat.m_flags |= DDSPF_BUMPDUDV;
						desc.m_pixelFormat.m_rgbBitCount = 32;

						desc.m_pixelFormat.m_rgbAlphaBitMask = 0xC0000000;
						desc.m_pixelFormat.m_rBitMask = 0x000003FF;
						desc.m_pixelFormat.m_gBitMask = 0x000FFC00;
						desc.m_pixelFormat.m_bBitMask = 0x3FF00000;
					}
					break;

				case EF_GR16:
					{
						desc.m_pixelFormat.m_flags |= DDSPF_RGB;
						desc.m_pixelFormat.m_rgbBitCount = 32;

						desc.m_pixelFormat.m_rgbAlphaBitMask = 0x00000000;
						desc.m_pixelFormat.m_rBitMask = 0x0000FFFF;
						desc.m_pixelFormat.m_gBitMask = 0xFFFF0000;
						desc.m_pixelFormat.m_bBitMask = 0x00000000;
					}
					break;

				case EF_SIGNED_GR16:
					{
						desc.m_pixelFormat.m_flags |= DDSPF_BUMPDUDV;
						desc.m_pixelFormat.m_rgbBitCount = 32;

						desc.m_pixelFormat.m_rgbAlphaBitMask = 0x00000000;
						desc.m_pixelFormat.m_rBitMask = 0x0000FFFF;
						desc.m_pixelFormat.m_gBitMask = 0xFFFF0000;
						desc.m_pixelFormat.m_bBitMask = 0x00000000;
					}
					break;

				case EF_R8:
					{
						desc.m_pixelFormat.m_flags |= DDSPF_LUMINANCE;
						desc.m_pixelFormat.m_rgbBitCount = 8;

						desc.m_pixelFormat.m_rgbAlphaBitMask = 0x00000000;
						desc.m_pixelFormat.m_rBitMask = 0x000000FF;
						desc.m_pixelFormat.m_gBitMask = 0x00000000;
						desc.m_pixelFormat.m_bBitMask = 0x00000000;
					}
					break;

				case EF_R16:
					{
						desc.m_pixelFormat.m_flags |= DDSPF_LUMINANCE;
						desc.m_pixelFormat.m_rgbBitCount = 16;

						desc.m_pixelFormat.m_rgbAlphaBitMask = 0x00000000;
						desc.m_pixelFormat.m_rBitMask = 0x0000FFFF;
						desc.m_pixelFormat.m_gBitMask = 0x00000000;
						desc.m_pixelFormat.m_bBitMask = 0x00000000;
					}
					break;

				case EF_A8:
					{
						desc.m_pixelFormat.m_flags |= DDSPF_ALPHA;
						desc.m_pixelFormat.m_rgbBitCount = 8;

						desc.m_pixelFormat.m_rgbAlphaBitMask = 0x000000FF;
						desc.m_pixelFormat.m_rBitMask = 0x00000000;
						desc.m_pixelFormat.m_gBitMask = 0x00000000;
						desc.m_pixelFormat.m_bBitMask = 0x00000000;
					}
					break;

				default:
					BOOST_ASSERT(false);
					break;
				}
			}
		}

		if( desc.m_pixelFormat.m_fourCC != MakeFourCC<'D', 'X', '1', '0'>::value)
		{
			desc.m_flags |= DDSD_CAPS;
			desc.m_flags |= DDSD_PIXELFORMAT;
			
			desc.m_ddsCaps.m_caps1 = DDSCAPS_TEXTURE;
			if (numMipMaps != 1)
			{
				desc.m_ddsCaps.m_caps1 |= DDSCAPS_MIPMAP;
				desc.m_ddsCaps.m_caps1 |= DDSCAPS_COMPLEX;
			}
			if (Texture::TT_3D == type)
			{
				desc.m_ddsCaps.m_caps1 |= DDSCAPS_COMPLEX;
				desc.m_ddsCaps.m_caps2 |= DDSCAPS2_VOLUME;
				desc.m_flags |= DDSD_DEPTH;
				desc.m_depth  = depth;
			}
			if (Texture::TT_Cube == type)
			{
				desc.m_ddsCaps.m_caps1 |= DDSCAPS_COMPLEX;
				desc.m_ddsCaps.m_caps2 |= DDSCAPS2_CUBEMAP;
				desc.m_ddsCaps.m_caps2 |= DDSCAPS2_CUBEMAP_POSITIVEX;
				desc.m_ddsCaps.m_caps2 |= DDSCAPS2_CUBEMAP_NEGATIVEX;
				desc.m_ddsCaps.m_caps2 |= DDSCAPS2_CUBEMAP_POSITIVEY;
				desc.m_ddsCaps.m_caps2 |= DDSCAPS2_CUBEMAP_NEGATIVEY;
				desc.m_ddsCaps.m_caps2 |= DDSCAPS2_CUBEMAP_POSITIVEZ;
				desc.m_ddsCaps.m_caps2 |= DDSCAPS2_CUBEMAP_NEGATIVEZ;
			}
		}

		UINT formatSize    = NumFormatBytes( format);
		UINT mainImageSize = width * height * formatSize;
		if( IsCompressedFormat( format))
		{
			if ( EF_BC1==format || EF_SIGNED_BC1==format || EF_BC1_SRGB==format || EF_BC4==format || EF_SIGNED_BC4==format || EF_BC4_SRGB==format)
				mainImageSize = width * height / 2;
			else
				mainImageSize = width * height;

			desc.m_flags |= DDSD_LINEARSIZE;
			desc.m_linearSize = mainImageSize;
		}

		DDSSURFACEDESC2 descLe = desc;
		fwrite( reinterpret_cast<char*>(&descLe), sizeof(descLe), 1, fileHandle);

		if( MakeFourCC<'D', 'X', '1', '0'>::value == desc.m_pixelFormat.m_fourCC)
		{
			DDS_HEADER_DXT10 desc10;
			desc10.m_dxgiFormat = ElementFormatToDXGIFormat( format);
			desc10.m_miscFlag	= 0;
			switch( type)
			{
			case Texture::TT_1D:
				desc10.m_resDimension = D3D10_RESOURCE_DIMENSION_TEXTURE1D;
				break;

			case Texture::TT_2D:
				desc10.m_resDimension = D3D10_RESOURCE_DIMENSION_TEXTURE2D;
				break;

			case Texture::TT_3D:
				desc10.m_resDimension = D3D10_RESOURCE_DIMENSION_TEXTURE3D;
				break;

			case Texture::TT_Cube:
				desc10.m_resDimension = D3D10_RESOURCE_DIMENSION_TEXTURE2D;
				desc10.m_miscFlag = D3D10_RESOURCE_MISC_TEXTURECUBE;
				break;

			default:
				A_ASSERT(false);
				desc10.m_resDimension = D3D10_RESOURCE_DIMENSION_TEXTURE2D;
				break;
			}
			desc10.m_arraySize = arraySize;
			desc10.m_reserved  = 0;

			fwrite( reinterpret_cast<char*>(&desc10), sizeof( desc10), 1, fileHandle);
		}

		switch( type)
		{
		case Texture::TT_2D:
			{
				for ( UINT arrayIndex = 0; arrayIndex < arraySize; ++arrayIndex)
				{
					UINT theWidth  = width;
					UINT theHeight = height;
					for ( UINT level = 0; level<desc.m_mipMapCount; ++level)
					{
						size_t const index = arrayIndex * desc.m_mipMapCount + level;
						if ( IsCompressedFormat(format))
						{
							int blockSize;
							if ((EF_BC1 == format) || (EF_SIGNED_BC1 == format) || (EF_BC1_SRGB == format) || (EF_BC4 == format) || (EF_SIGNED_BC4 == format) || (EF_BC4_SRGB == format))
								blockSize = 8;
							else
								blockSize = 16;

							UINT imageSize = ((theWidth + 3) / 4) * ((theHeight + 3) / 4) * blockSize;

							fwrite( reinterpret_cast<char const*>(initData[index].m_data), imageSize, 1, fileHandle);
						}
						else
						{
							fwrite( reinterpret_cast<char const*>(initData[index].m_data), theWidth * theHeight * formatSize, 1, fileHandle);
						}

						theWidth  = std::max<UINT>( theWidth / 2,  1);
						theHeight = std::max<UINT>( theHeight / 2, 1);
					}
				}
			}
			break;
		}

		// close the file , we don't need it any more
		fflush( fileHandle);
		fclose( fileHandle);

		return true;
	}
}