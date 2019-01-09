#include "OGLESTexture.h"

namespace Ares
{
	OGLESTexture::OGLESTexture( TextureType type, UINT arraySize, UINT sampleCount, UINT sampleQuality, UINT accessHint)
		: Texture( type, sampleCount, sampleQuality,  accessHint)
	{
		A_ASSERT( arraySize==1);

		switch( m_type)
		{
		case TT_1D:
		case TT_2D:		m_targetType = GL_TEXTURE_2D;		break;
		case TT_Cube:	m_targetType = GL_TEXTURE_CUBE_MAP; break;
		default:		m_targetType = GL_TEXTURE_2D;		break;
		}
	}

	OGLESTexture::~OGLESTexture()
	{

	}

	// 获取深
	UINT OGLESTexture::GetDepth( UINT level) const
	{
		A_ASSERT( level < m_numMipMaps);

		return 1;
	}

	// 格式转换
	ElementFormat OGLESTexture::SRGBToRGB( ElementFormat pf)
	{
		switch( pf)
		{
		case EF_ARGB8_SRGB:
			return EF_ARGB8;

		case EF_BC1_SRGB:
			return EF_BC1;

		case EF_BC2_SRGB:
			return EF_BC2;

		case EF_BC3_SRGB:
			return EF_BC3;

		default:
			return pf;
		}
	}
}