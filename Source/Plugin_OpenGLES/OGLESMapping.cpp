#include "OGLESMapping.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

namespace Ares
{
	// 映射格式
	void OGLESMapping::MappingFormat( GLint& internalFormat, GLenum& glformat, GLenum& gltype, ElementFormat ef)
	{
		switch (ef)
		{
		case EF_A8:
			{
				internalFormat = GL_ALPHA;
				glformat	   = GL_ALPHA;
				gltype		   = GL_UNSIGNED_BYTE;
			}
			break;

		case EF_R8:
			{
				internalFormat = GL_LUMINANCE;
				glformat	   = GL_LUMINANCE;
				gltype		   = GL_UNSIGNED_BYTE;
			}
			break;

		case EF_SIGNED_R8:
			{
				internalFormat = GL_LUMINANCE;
				glformat	   = GL_LUMINANCE;
				gltype		   = GL_BYTE;
			}	
			break;

		case EF_BGR8:
			{
				internalFormat = GL_RGB;
				glformat	   = GL_RGB;
				gltype		   = GL_UNSIGNED_BYTE;
			}		
			break;

		case EF_ARGB8:
			{
				internalFormat = GL_RGBA;
				glformat	   = GL_RGBA;
				gltype		   = GL_UNSIGNED_BYTE;
			}
			break;

		case EF_ABGR8:
			{
				internalFormat = GL_RGBA;
				glformat	   = GL_RGBA;
				gltype		   = GL_UNSIGNED_BYTE;
			}
			break;

		case EF_R16F:
			{
				internalFormat = GL_LUMINANCE;
				glformat	   = GL_LUMINANCE;
				gltype	       = GL_SHORT;
			}
			break;

		case EF_GR16F:
			{
				internalFormat = GL_LUMINANCE_ALPHA;
				glformat	   = GL_LUMINANCE_ALPHA;
				gltype		   = GL_SHORT;
			}
			break;

		case EF_BGR16F:
			{
				internalFormat = GL_RGB;
				glformat	   = GL_RGB;
				gltype		   = GL_SHORT;
			}

			break;

		case EF_ABGR16F:
			{
				internalFormat = GL_RGBA;
				glformat	   = GL_RGBA;
				gltype		   = GL_SHORT;
			}
			break;

		case EF_R32F:
			{
				internalFormat = GL_LUMINANCE;
				glformat	   = GL_LUMINANCE;
				gltype		   = GL_FLOAT;
			}
			break;

		case EF_GR32F:
			{
				internalFormat = GL_LUMINANCE_ALPHA;
				glformat	   = GL_LUMINANCE_ALPHA;
				gltype		   = GL_FLOAT;
			}
			break;

		case EF_BGR32F:
			{
				internalFormat = GL_RGB;
				glformat	   = GL_RGB;
				gltype		   = GL_FLOAT;
			}
			break;

		case EF_ABGR32F:
			{
				internalFormat = GL_RGBA;
				glformat	   = GL_RGBA;
				gltype		   = GL_FLOAT;
			}
			break;

		case EF_BC1:
			{
				//internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
				//glformat	   = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
				gltype		   = GL_UNSIGNED_BYTE;
			}
			break;

		case EF_BC2:
			{
				//internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
				//glformat	   = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
				gltype		   = GL_UNSIGNED_BYTE;
			}		
			break;

		case EF_BC3:
			{
				//internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				//glformat	   = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				gltype		   = GL_UNSIGNED_BYTE;
			}	
			break;

		case EF_BC4:
			{
				//internalFormat = GL_COMPRESSED_LUMINANCE_LATC1_EXT;
				//glformat	   = GL_COMPRESSED_LUMINANCE_LATC1_EXT;
				gltype		   = GL_UNSIGNED_BYTE;
			}	
			break;

		case EF_BC5:
			{
				//internalFormat = GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT;
				//glformat	   = GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT;
				gltype		   = GL_UNSIGNED_BYTE;
			}		
			break;

		case EF_SIGNED_BC4:
			{
				//internalFormat = GL_COMPRESSED_SIGNED_LUMINANCE_LATC1_EXT;
				//glformat	   = GL_COMPRESSED_SIGNED_LUMINANCE_LATC1_EXT;
				gltype		   = GL_UNSIGNED_BYTE;
			}	
			break;

		case EF_SIGNED_BC5:
			{
				//internalFormat = GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2_EXT;
				//glformat	   = GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2_EXT;
				gltype		   = GL_UNSIGNED_BYTE;
			}	
			break;

		case EF_D16:
			{
				internalFormat = GL_DEPTH_COMPONENT16;
				glformat	   = GL_DEPTH_COMPONENT;
				gltype		   = GL_UNSIGNED_SHORT;
			}
			break;

		case EF_D32F:
			{
				internalFormat = GL_DEPTH_COMPONENT;
				glformat	   = GL_DEPTH_COMPONENT;
				gltype		   = GL_FLOAT;			
			}
			break;

		case EF_D24S8:
			{
				internalFormat = GL_DEPTH24_STENCIL8_OES;
				glformat	   = GL_DEPTH_STENCIL_OES;
				gltype		   = GL_UNSIGNED_INT_24_8_OES;
			}
			break;

		default:
			A_ASSERT( FALSE);
		}
	}

	// 映射数据类型大小
	int OGLESMapping::MappingDataTypeSize( GLenum type)
	{
		switch( type)
		{
		case GL_FLOAT:
			return sizeof( GLfloat);

		case GL_FLOAT_VEC2:
			return sizeof( GLfloat) * 2;

		case GL_FLOAT_VEC3:
			return sizeof( GLfloat) * 3;

		case GL_FLOAT_VEC4:
			return sizeof( GLfloat) * 4;

		case GL_FLOAT_MAT2:
			return sizeof( GLfloat) * 4;

		case GL_FLOAT_MAT3:
			return sizeof( GLfloat) * 9;

		case GL_FLOAT_MAT4:
			return sizeof( GLfloat) * 16;

		default:
			A_ASSERT( FALSE);
			return 0;
		}
	}

	// 映射顶点格式
	void OGLESMapping::MappingVertexFormat( GLenum& gltype, GLboolean& normalized, ElementFormat ef)
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
				gltype     = GL_HALF_FLOAT_OES;
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

	// 映射
	void OGLESMapping::Mapping( GLenum& primType, UINT& primCount, const RenderLayout& layout)
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

	// 映射Attribute
	void OGLESMapping::Mapping( VertexElementUsage& usage, BYTE& usageIdx, const char* attributeName)
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