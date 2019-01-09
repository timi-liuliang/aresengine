#include "OGLTexture.h"

namespace Ares
{
	OGLTexture::OGLTexture( TextureType type, UINT arraySize, UINT sampleCount, UINT sampleQuality, UINT accessHint)
		: Texture( type, sampleCount, sampleQuality,  accessHint)
	{

	}

	OGLTexture::~OGLTexture()
	{

	}

	// 获取深
	UINT OGLTexture::GetDepth( UINT level) const
	{
		A_ASSERT( level < m_numMipMaps);

		return 1;
	}

	// 格式转换
	ElementFormat OGLTexture::SRGBToRGB( ElementFormat pf)
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