#include "OGLMapping.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

namespace Ares
{
	// ”≥…‰∏Ò Ω
	void OGLMapping::MappingFormat( GLint& internalFormat, GLenum& glformat, GLenum& gltype, ElementFormat ef)
	{
		switch (ef)
		{
		case EF_A8:
			{
				internalFormat = GL_ALPHA8;
				glformat	   = GL_ALPHA;
				gltype		   = GL_UNSIGNED_BYTE;
			}
			break;

		case EF_ARGB4:
			{
				internalFormat = GL_RGBA4;
				glformat	   = GL_BGRA;
				gltype		   = GL_UNSIGNED_SHORT_4_4_4_4_REV;
			}
			break;

		case EF_R8:
			{
				if (glloader_GL_VERSION_3_0() || glloader_GL_ARB_texture_rg())
				{
					internalFormat = GL_R8;
					glformat       = GL_RED;
					gltype         = GL_UNSIGNED_BYTE;
				}
				else
				{
					internalFormat = GL_LUMINANCE8;
					glformat	   = GL_LUMINANCE;
					gltype		   = GL_UNSIGNED_BYTE;
				}
			}
			break;

		case EF_SIGNED_R8:
			{
				if (glloader_GL_VERSION_3_0() || glloader_GL_ARB_texture_rg())
				{
					internalFormat = GL_R8;
					glformat       = GL_RED;
					gltype         = GL_BYTE;
				}
				else
				{
					internalFormat = GL_LUMINANCE8;
					glformat	   = GL_LUMINANCE;
					gltype		   = GL_BYTE;
				}
			}	
			break;

		case EF_GR8:
			{
				if (glloader_GL_VERSION_3_0() || glloader_GL_ARB_texture_rg())
				{
					internalFormat = GL_RG8;
					glformat	   = GL_RG;
					gltype		   = GL_UNSIGNED_BYTE;
				}
				else
				{
					A_ASSERT( FALSE);
				}
			}		
			break;

		case EF_SIGNED_GR8:
			{
				if (glloader_GL_VERSION_3_0() || glloader_GL_ARB_texture_rg())
				{
					internalFormat = GL_RG8;
					glformat	   = GL_RG;
					gltype		   = GL_BYTE;
				}
				else
				{
					A_ASSERT( FALSE);
				}
			}
			break;

		case EF_BGR8:
			{
				internalFormat = GL_RGB8;
				glformat = GL_RGB;
				gltype = GL_UNSIGNED_BYTE;
			}		
			break;

		case EF_SIGNED_BGR8:
			if (glloader_GL_NV_texture_shader())
			{
				internalFormat = GL_SIGNED_RGB8_NV;
				glformat	   = GL_RGB;
				gltype		   = GL_BYTE;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_ARGB8:
			{
				internalFormat = GL_RGBA8;
				glformat	   = GL_BGRA;
				gltype		   = GL_UNSIGNED_INT_8_8_8_8_REV;
			}
			break;

		case EF_ABGR8:
			{
				internalFormat = GL_RGBA8;
				glformat	   = GL_RGBA;
				gltype		   = GL_UNSIGNED_BYTE;
			}
			break;

		case EF_SIGNED_ABGR8:
			{
				if (glloader_GL_NV_texture_shader())
				{
					internalFormat = GL_SIGNED_RGBA8_NV;
					glformat	   = GL_RGBA;
					gltype		   = GL_BYTE;
				}
				else
				{
					A_ASSERT( FALSE);
				}
			}
			break;

		case EF_A2BGR10:
			{
				internalFormat = GL_RGB10_A2;
				glformat	   = GL_RGBA;
				gltype		   = GL_UNSIGNED_INT_2_10_10_10_REV;
			}
			break;

		case EF_SIGNED_A2BGR10:
			{
				internalFormat = GL_RGB10_A2;
				glformat	   = GL_RGBA;
				if (glloader_GL_VERSION_3_3() || glloader_GL_ARB_vertex_type_2_10_10_10_rev())
				{
					gltype = GL_INT_2_10_10_10_REV;
				}
				else
				{
					gltype = GL_UNSIGNED_INT_2_10_10_10_REV;
				}
			}		
			break;

		case EF_R8UI:
			{
				if (glloader_GL_VERSION_3_0() || glloader_GL_EXT_texture_integer())
				{
					internalFormat = GL_R8UI;
					glformat	   = GL_RED_INTEGER_EXT;
					gltype		   = GL_UNSIGNED_BYTE;
				}
				else
				{
					A_ASSERT( FALSE);
				}
			}		
			break;

		case EF_R8I:
			{
				if (glloader_GL_VERSION_3_0() || glloader_GL_EXT_texture_integer())
				{
					internalFormat = GL_R8I;
					glformat	   = GL_RED_INTEGER_EXT;
					gltype		   = GL_BYTE;
				}
				else
				{
					A_ASSERT( FALSE);
				}
			}	
			break;

		case EF_GR8UI:
			if (glloader_GL_VERSION_3_0() || glloader_GL_EXT_texture_integer())
			{
				internalFormat = GL_RG8UI;
				glformat = GL_RG_INTEGER;
				gltype = GL_UNSIGNED_BYTE;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_GR8I:
			if (glloader_GL_VERSION_3_0() || glloader_GL_EXT_texture_integer())
			{
				internalFormat = GL_RG8I;
				glformat = GL_RG_INTEGER;
				gltype = GL_BYTE;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_BGR8UI:
			if (glloader_GL_VERSION_3_0() || glloader_GL_EXT_texture_integer())
			{
				internalFormat = GL_RGB8UI;
				glformat = GL_RGB_INTEGER;
				gltype = GL_UNSIGNED_BYTE;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_BGR8I:
			if (glloader_GL_VERSION_3_0() || glloader_GL_EXT_texture_integer())
			{
				internalFormat = GL_RGB8I;
				glformat = GL_RGB_INTEGER;
				gltype = GL_BYTE;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_ABGR8UI:
			if (glloader_GL_VERSION_3_0() || glloader_GL_EXT_texture_integer())
			{
				internalFormat = GL_RGBA8UI;
				glformat = GL_RGBA_INTEGER;
				gltype = GL_UNSIGNED_INT_8_8_8_8;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_ABGR8I:
			if (glloader_GL_VERSION_3_0() || glloader_GL_EXT_texture_integer())
			{
				internalFormat = GL_RGBA8I;
				glformat = GL_RGBA_INTEGER;
				gltype = GL_UNSIGNED_INT_8_8_8_8;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_R16:
			if (glloader_GL_VERSION_3_0() || glloader_GL_ARB_texture_rg())
			{
				internalFormat = GL_R16;
				glformat = GL_RED;
				gltype = GL_UNSIGNED_SHORT;
			}
			else
			{
				internalFormat = GL_LUMINANCE16;
				glformat = GL_LUMINANCE;
				gltype = GL_UNSIGNED_SHORT;
			}
			break;

		case EF_SIGNED_R16:
			if (glloader_GL_VERSION_3_0() || glloader_GL_ARB_texture_rg())
			{
				internalFormat = GL_R16;
				glformat = GL_RED;
				gltype = GL_SHORT;
			}
			else
			{
				internalFormat = GL_LUMINANCE16;
				glformat = GL_LUMINANCE;
				gltype = GL_SHORT;
			}
			break;

		case EF_GR16:
			if (glloader_GL_VERSION_3_0() || glloader_GL_ARB_texture_rg())
			{
				internalFormat = GL_RG16;
				glformat = GL_RG;
				gltype = GL_UNSIGNED_SHORT;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_SIGNED_GR16:
			if (glloader_GL_VERSION_3_0() || glloader_GL_ARB_texture_rg())
			{
				internalFormat = GL_RG16;
				glformat = GL_RG;
				gltype = GL_SHORT;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_BGR16:
			internalFormat = GL_RGB16;
			glformat = GL_RGB;
			gltype = GL_UNSIGNED_SHORT;
			break;

		case EF_SIGNED_BGR16:
			internalFormat = GL_RGB16;
			glformat = GL_RGB;
			gltype = GL_SHORT;
			break;

		case EF_ABGR16:
			internalFormat = GL_RGBA16;
			glformat = GL_RGBA;
			gltype = GL_UNSIGNED_SHORT;
			break;

		case EF_SIGNED_ABGR16:
			internalFormat = GL_RGBA16;
			glformat = GL_RGBA;
			gltype = GL_SHORT;
			break;

		case EF_R16UI:
			if (glloader_GL_VERSION_3_0() || glloader_GL_EXT_texture_integer())
			{
				internalFormat = GL_R16UI;
				glformat = GL_RED_INTEGER_EXT;
				gltype = GL_UNSIGNED_SHORT;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_R16I:
			if (glloader_GL_VERSION_3_0() || glloader_GL_EXT_texture_integer())
			{
				internalFormat = GL_R16I;
				glformat = GL_RED_INTEGER_EXT;
				gltype = GL_SHORT;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_GR16UI:
			if (glloader_GL_VERSION_3_0() || glloader_GL_EXT_texture_integer())
			{
				internalFormat = GL_RG16UI;
				glformat = GL_RG_INTEGER;
				gltype = GL_UNSIGNED_SHORT;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_GR16I:
			if (glloader_GL_VERSION_3_0() || glloader_GL_EXT_texture_integer())
			{
				internalFormat = GL_RG16I;
				glformat = GL_RG_INTEGER;
				gltype = GL_SHORT;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_BGR16UI:
			if (glloader_GL_VERSION_3_0() || glloader_GL_EXT_texture_integer())
			{
				internalFormat = GL_RGB16UI;
				glformat = GL_RGB_INTEGER;
				gltype = GL_UNSIGNED_SHORT;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_BGR16I:
			if (glloader_GL_VERSION_3_0() || glloader_GL_EXT_texture_integer())
			{
				internalFormat = GL_RGB16I;
				glformat = GL_RGB_INTEGER;
				gltype = GL_SHORT;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_ABGR16UI:
			if (glloader_GL_VERSION_3_0() || glloader_GL_EXT_texture_integer())
			{
				internalFormat = GL_RGBA16UI;
				glformat = GL_RGBA_INTEGER;
				gltype = GL_UNSIGNED_SHORT;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_ABGR16I:
			if (glloader_GL_VERSION_3_0() || glloader_GL_EXT_texture_integer())
			{
				internalFormat = GL_RGBA16I;
				glformat = GL_RGBA_INTEGER;
				gltype = GL_SHORT;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_R32UI:
			if (glloader_GL_VERSION_3_0() || glloader_GL_EXT_texture_integer())
			{
				internalFormat = GL_R32UI;
				glformat = GL_RED_INTEGER_EXT;
				gltype = GL_UNSIGNED_INT;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_R32I:
			if (glloader_GL_VERSION_3_0() || glloader_GL_EXT_texture_integer())
			{
				internalFormat = GL_R32I;
				glformat = GL_RED_INTEGER_EXT;
				gltype = GL_INT;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_GR32UI:
			if (glloader_GL_VERSION_3_0() || glloader_GL_EXT_texture_integer())
			{
				internalFormat = GL_RG32UI;
				glformat = GL_RG_INTEGER;
				gltype = GL_UNSIGNED_INT;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_GR32I:
			if (glloader_GL_VERSION_3_0() || glloader_GL_EXT_texture_integer())
			{
				internalFormat = GL_RG32I;
				glformat = GL_RG_INTEGER;
				gltype = GL_INT;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_BGR32UI:
			if (glloader_GL_VERSION_3_0() || glloader_GL_EXT_texture_integer())
			{
				internalFormat = GL_RGB32UI;
				glformat = GL_RGB_INTEGER;
				gltype = GL_UNSIGNED_INT;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_BGR32I:
			if (glloader_GL_VERSION_3_0() || glloader_GL_EXT_texture_integer())
			{
				internalFormat = GL_RGB32I;
				glformat = GL_RGB_INTEGER;
				gltype = GL_INT;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_ABGR32UI:
			if (glloader_GL_VERSION_3_0() || glloader_GL_EXT_texture_integer())
			{
				internalFormat = GL_RGBA32UI;
				glformat = GL_RGBA_INTEGER;
				gltype = GL_UNSIGNED_SHORT;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_ABGR32I:
			if (glloader_GL_VERSION_3_0() || glloader_GL_EXT_texture_integer())
			{
				internalFormat = GL_RGBA32I;
				glformat = GL_RGBA_INTEGER;
				gltype = GL_SHORT;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_R16F:
			if (glloader_GL_VERSION_3_0() || glloader_GL_ARB_texture_rg())
			{
				internalFormat = GL_R16F;
				glformat = GL_RED;
				gltype = GL_HALF_FLOAT_ARB;
			}
			else
			{
				internalFormat = GL_LUMINANCE16F_ARB;
				glformat = GL_LUMINANCE;
				gltype = GL_HALF_FLOAT_ARB;
			}
			break;

		case EF_GR16F:
			if (glloader_GL_VERSION_3_0() || glloader_GL_ARB_texture_rg())
			{
				internalFormat = GL_RG16F;
				glformat = GL_RG;
				gltype = GL_HALF_FLOAT_ARB;
			}
			else
			{
				internalFormat = GL_LUMINANCE_ALPHA16F_ARB;
				glformat = GL_LUMINANCE_ALPHA;
				gltype = GL_FLOAT;
			}
			break;

		case EF_B10G11R11F:
			if (glloader_GL_VERSION_3_0() || glloader_GL_EXT_packed_float())
			{
				internalFormat = GL_R11F_G11F_B10F;
				glformat = GL_RGB;
				gltype = GL_UNSIGNED_INT_10F_11F_11F_REV;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_BGR16F:
			internalFormat = GL_RGB16F_ARB;
			glformat = GL_RGB;
			gltype = GL_HALF_FLOAT_ARB;
			break;

		case EF_ABGR16F:
			internalFormat = GL_RGBA16F_ARB;
			glformat = GL_RGBA;
			gltype = GL_HALF_FLOAT_ARB;
			break;

		case EF_R32F:
			if (glloader_GL_VERSION_3_0() || glloader_GL_ARB_texture_rg())
			{
				internalFormat = GL_R32F;
				glformat = GL_RED;
				gltype = GL_FLOAT;
			}
			else
			{
				internalFormat = GL_LUMINANCE32F_ARB;
				glformat = GL_LUMINANCE;
				gltype = GL_FLOAT;
			}
			break;

		case EF_GR32F:
			if (glloader_GL_VERSION_3_0() || glloader_GL_ARB_texture_rg())
			{
				internalFormat = GL_RG32F;
				glformat = GL_RG;
				gltype = GL_FLOAT;
			}
			else
			{
				internalFormat = GL_LUMINANCE_ALPHA32F_ARB;
				glformat = GL_LUMINANCE_ALPHA;
				gltype = GL_FLOAT;
			}
			break;

		case EF_BGR32F:
			internalFormat = GL_RGB32F_ARB;
			glformat = GL_RGB;
			gltype = GL_FLOAT;
			break;

		case EF_ABGR32F:
			internalFormat = GL_RGBA32F_ARB;
			glformat = GL_RGBA;
			gltype = GL_FLOAT;
			break;

		case EF_BC1:
			if (glloader_GL_EXT_texture_compression_s3tc())
			{
				internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
				glformat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
				gltype = GL_UNSIGNED_BYTE;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_BC2:
			if (glloader_GL_EXT_texture_compression_s3tc())
			{
				internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
				glformat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
				gltype = GL_UNSIGNED_BYTE;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_BC3:
			if (glloader_GL_EXT_texture_compression_s3tc())
			{
				internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				glformat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				gltype = GL_UNSIGNED_BYTE;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_BC4:
			if (glloader_GL_EXT_texture_compression_latc())
			{
				internalFormat = GL_COMPRESSED_LUMINANCE_LATC1_EXT;
				glformat = GL_COMPRESSED_LUMINANCE;
				gltype = GL_UNSIGNED_BYTE;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_BC5:
			if (glloader_GL_EXT_texture_compression_latc())
			{
				internalFormat = GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT;
				glformat = GL_COMPRESSED_LUMINANCE_ALPHA;
				gltype = GL_UNSIGNED_BYTE;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_SIGNED_BC4:
			if (glloader_GL_EXT_texture_compression_latc())
			{
				internalFormat = GL_COMPRESSED_SIGNED_LUMINANCE_LATC1_EXT;
				glformat = GL_COMPRESSED_LUMINANCE;
				gltype = GL_UNSIGNED_BYTE;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_SIGNED_BC5:
			if (glloader_GL_EXT_texture_compression_latc())
			{
				internalFormat = GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2_EXT;
				glformat = GL_COMPRESSED_LUMINANCE_ALPHA;
				gltype = GL_UNSIGNED_BYTE;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_D16:
			internalFormat = GL_DEPTH_COMPONENT16;
			glformat = GL_DEPTH_COMPONENT;
			gltype = GL_UNSIGNED_SHORT;
			break;

		case EF_D24S8:
			if (glloader_GL_EXT_packed_depth_stencil())
			{
				internalFormat = GL_DEPTH24_STENCIL8_EXT;
				glformat = GL_DEPTH_STENCIL_EXT;
				gltype = GL_UNSIGNED_INT_24_8_EXT;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_D32F:
			{
				internalFormat = GL_DEPTH_COMPONENT32F;
				glformat	   = GL_DEPTH_COMPONENT;
				gltype		   = GL_FLOAT;
			}
			break;

		case EF_ARGB8_SRGB:
			if (glloader_GL_EXT_texture_sRGB())
			{
				internalFormat = GL_SRGB8_ALPHA8_EXT;
				glformat = GL_BGRA;
				gltype = GL_UNSIGNED_INT_8_8_8_8_REV;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_ABGR8_SRGB:
			if (glloader_GL_EXT_texture_sRGB())
			{
				internalFormat = GL_SRGB8_ALPHA8_EXT;
				glformat = GL_RGBA;
				gltype = GL_UNSIGNED_BYTE;
			}
			else
			{
				A_ASSERT( FALSE);
			}
			break;

		case EF_BC1_SRGB:
			{
				if (glloader_GL_EXT_texture_sRGB())
				{
					internalFormat = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT;
					glformat = GL_BGRA;
					gltype = GL_UNSIGNED_BYTE;
				}
				else
				{
					A_ASSERT( FALSE);
				}
			}
			break;

		case EF_BC2_SRGB:
			{
				if (glloader_GL_EXT_texture_sRGB())
				{
					internalFormat = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT;
					glformat	   = GL_BGRA;
					gltype		   = GL_UNSIGNED_BYTE;
				}
				else
				{
					A_ASSERT( FALSE);
				}
			}
			break;

		case EF_BC3_SRGB:
			{
				if (glloader_GL_EXT_texture_sRGB())
				{
					internalFormat = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT;
					glformat	   = GL_BGRA;
					gltype		   = GL_UNSIGNED_BYTE;
				}
				else
				{
					A_ASSERT( FALSE);
				}
			}
			break;

		case EF_BC4_SRGB:
			{
				if (glloader_GL_EXT_texture_sRGB())
				{
					internalFormat = GL_COMPRESSED_SLUMINANCE_EXT;
					glformat	   = GL_LUMINANCE;
					gltype		   = GL_UNSIGNED_BYTE;
				}
				else
				{
					A_ASSERT( FALSE);
				}
			}
			break;

		case EF_BC5_SRGB:
			{
				if (glloader_GL_EXT_texture_sRGB())
				{
					internalFormat = GL_COMPRESSED_SLUMINANCE_ALPHA_EXT;
					glformat	   = GL_LUMINANCE_ALPHA;
					gltype		   = GL_UNSIGNED_BYTE;
				}
				else
				{
					A_ASSERT( FALSE);
				}
			}
			break;

		default:
			A_ASSERT( FALSE);
		}
	}

	// ”≥…‰∂•µ„∏Ò Ω
	void OGLMapping::MappingVertexFormat( GLenum& gltype, GLboolean& normalized, ElementFormat ef)
	{
		switch (ef)
		{
		case EF_A8:
		case EF_R8:
		case EF_GR8:
		case EF_BGR8:
		case EF_ARGB8:
		case EF_ABGR8:
			{
				gltype	   = GL_UNSIGNED_BYTE;
				normalized = GL_TRUE;
			}
			break;

		case EF_R8UI:
		case EF_GR8UI:
		case EF_BGR8UI:
		case EF_ABGR8UI:
			{
				gltype	   = GL_UNSIGNED_BYTE;
				normalized = GL_FALSE;
			}
			break;

		case EF_SIGNED_R8:
		case EF_SIGNED_GR8:
		case EF_SIGNED_BGR8:
		case EF_SIGNED_ABGR8:
			{
				gltype     = GL_BYTE;
				normalized = GL_TRUE;
			}
			break;

		case EF_R8I:
		case EF_GR8I:
		case EF_BGR8I:
		case EF_ABGR8I:
			{
				gltype     = GL_BYTE;
				normalized = GL_FALSE;
			}
			break;

		case EF_R16:
		case EF_GR16:
		case EF_BGR16:
		case EF_ABGR16:
			{
				gltype     = GL_UNSIGNED_SHORT;
				normalized = GL_TRUE;
			}
			break;

		case EF_R16UI:
		case EF_GR16UI:
		case EF_BGR16UI:
		case EF_ABGR16UI:
			{
				gltype     = GL_UNSIGNED_SHORT;
				normalized = GL_FALSE;
			}
			break;

		case EF_SIGNED_R16:
		case EF_SIGNED_GR16:
		case EF_SIGNED_BGR16:
		case EF_SIGNED_ABGR16:
			{
				gltype	   = GL_SHORT;
				normalized = GL_TRUE;
			}
			break;

		case EF_R16I:
		case EF_GR16I:
		case EF_BGR16I:
		case EF_ABGR16I:
			{
				gltype     = GL_SHORT;
				normalized = GL_FALSE;
			}
			break;

		case EF_R32UI:
		case EF_GR32UI:
		case EF_BGR32UI:
		case EF_ABGR32UI:
			{
				gltype     = GL_UNSIGNED_INT;
				normalized = GL_FALSE;
			}
			break;

		case EF_R32I:
		case EF_GR32I:
		case EF_BGR32I:
		case EF_ABGR32I:
			{
				gltype	   = GL_INT;
				normalized = GL_FALSE;
			}
			break;

		case EF_R16F:
		case EF_GR16F:
		case EF_BGR16F:
		case EF_ABGR16F:
			{
				gltype     = GL_HALF_FLOAT;
				normalized = GL_FALSE;
			}
			break;

		case EF_R32F:
		case EF_GR32F:
		case EF_BGR32F:
		case EF_ABGR32F:
			{
				gltype     = GL_FLOAT;
				normalized = GL_FALSE;
			}
			break;

		default:
			A_ASSERT( FALSE);
			break;
		}
	}

	// ”≥…‰
	void OGLMapping::Mapping( GLenum& primType, UINT& primCount, const RenderLayout& layout)
	{
		UINT vertexCount = static_cast<UINT>( layout.IsUseIndices() ? layout.GetNumIndices() : layout.GetNumVertices());

		primType  = GL_POINTS;
		primCount = vertexCount;

		switch( layout.GetTopologyType())
		{
		case RenderLayout::TT_PointList:
			{
				primType  = GL_POINTS;
				primCount = vertexCount;
			}
			break;

		case RenderLayout::TT_LineList:
			{
				primType  = GL_LINES;
				primCount = vertexCount / 2;
			}
			break;

		case RenderLayout::TT_LineStrip:
			{
				primType  = GL_LINE_STRIP;
				primCount = vertexCount - 1;
			}
			break;

		case RenderLayout::TT_TriangleList:
			{
				primType  = GL_TRIANGLES;
				primCount = vertexCount / 3;
			}
			break;

		case RenderLayout::TT_TriangleStrip:
			{
				primType  = GL_TRIANGLE_STRIP;
				primCount = vertexCount - 2;
			}
			break;

		default:
			A_ASSERT( FALSE);
			break;
		}
	}

	#define GLSL_SUPPORT_USAGES_NUM  1
	static string GLSL_SUPPORT_USAGES[GLSL_SUPPORT_USAGES_NUM] =
	{
		"GLPosition",
	};

	// ”≥…‰Attribute
	void OGLMapping::Mapping( VertexElementUsage& usage, BYTE& usageIdx, const char* attributeName)
	{
		A_ASSERT( attributeName);
		A_ASSERT( boost::starts_with( attributeName, "GL"));

		vector<string> result;
		boost::split( result, attributeName, boost::is_any_of("_"));

		string strUsage = result[0];
		int	   strIndex = boost::lexical_cast<int>(result[1]);

		// Usage
		for( size_t i=0; i<GLSL_SUPPORT_USAGES_NUM; i++)
		{
			if( GLSL_SUPPORT_USAGES[i]==strUsage)
			{
				usage = VertexElementUsage(i);

				break;
			}
		}

		// UsageIndex
		usageIdx = static_cast<BYTE>( strIndex);
	}
}