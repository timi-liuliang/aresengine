#include "OGLESTexture.h"
#include "OGLESRenderDevice.h"

namespace Ares
{
	OGLESTexture2D::OGLESTexture2D( UINT width, UINT height, UINT numMipMaps, UINT arraySize, ElementFormat format, UINT sampleCount, UINT sampleQuality, UINT accessHint, const ElementInitData* initData)
		: OGLESTexture( TT_2D, arraySize, sampleCount, sampleQuality, accessHint)
	{
		if( IsSRGB( format))
			format = SRGBToRGB( format);
		
		m_format = format;

		if( 0==numMipMaps)
		{
			m_numMipMaps = 1;
			UINT32 w = width;
			UINT32 h = height;
			while( (w!=1)||(h!=1))
			{
				++m_numMipMaps;

				w = std::max<UINT32>( 1U, w/2);
				h = std::max<UINT32>( 1U, h/2);
			}
		}
		else
		{
			m_numMipMaps = numMipMaps;
		}

		m_widthes.resize( m_numMipMaps);
		m_heights.resize( m_numMipMaps);
		{
			UINT32 w = width;
			UINT32 h = height;
			for( UINT32 level=0; level<numMipMaps; level++)
			{
				m_widthes[level] = w;
				m_heights[level] = h;

				w = std::max<UINT32>( 1U, w/2);
				h = std::max<UINT32>( 1U, h/2);
			}
		}

		arraySize = 1;

		UINT32 texelSize = NumFormatBytes( m_format);

		GLint  glinternalFormat;
		GLenum glFormat;
		GLenum glType;
		OGLESMapping::MappingFormat( glinternalFormat, glFormat, glType, m_format);

		OGLESDebug( glGenTextures( 1, &m_texture));
		OGLESDebug( glBindTexture( m_targetType, m_texture));
		OGLESDebug( glTexParameteri( m_targetType, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		OGLESDebug( glTexParameteri( m_targetType, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		//glTexParameteri( m_targetType, GL_TEXTURE_MAX_LEVEL,  m_numMipMaps-1);

		m_texData.resize( m_numMipMaps);

		for( UINT32 level=0; level<m_numMipMaps; level++)
		{
			UINT32 const w = m_widthes[level];
			UINT32 const h = m_heights[level];

			if( IsCompressedFormat( m_format))
			{
				int blockSize;
				if( (EF_BC1==format) || (EF_SIGNED_BC1==format) || (EF_BC1_SRGB==format || ( EF_BC4==format) || ( EF_SIGNED_BC4==format) || (EF_BC4_SRGB==format)))
					blockSize = 8;
				else
					blockSize = 16;

				GLsizei const imageSize = ( (w+3)/4) * ( (h+3)/4) * blockSize;
				if( initData)
				{
					m_texData[level].resize( imageSize, 0);
				}
				else
				{
					m_texData[level].resize( imageSize);
					memcpy( &m_texData[level][0], initData[level].m_data, imageSize);
				}

				OGLESDebug( glCompressedTexImage2D( m_targetType, level, glinternalFormat, w, h, 0, imageSize, &m_texData[level][0]));
			}
			else
			{
				GLsizei const imageSize = w*h*texelSize;
				if( !initData)
				{
					m_texData[level].resize( imageSize, 0);
				}
				else
				{
					m_texData[level].resize( imageSize);
					memcpy( &m_texData[level][0], initData[level].m_data, imageSize);
				}
					
				OGLESDebug( glTexImage2D( m_targetType, level, glinternalFormat, w, h, 0, glFormat, glType, &m_texData[level][0]));
			}
		}
	}

	// returns the width of the texture
	UINT OGLESTexture2D::GetWidth( UINT levle) const
	{
		A_ASSERT( levle < m_numMipMaps);

		return m_widthes[levle];
	}

	// returns the height of the texture
	UINT OGLESTexture2D::GetHeight( UINT levle) const
	{
		A_ASSERT( levle < m_numMipMaps);

		return m_heights[levle];
	}

	// Copies (and maybe scales to fit) the contents of this texture to another texture
	void OGLESTexture2D::CopyToTexure( Texture& target)
	{
		A_ASSERT( m_type == target.GetType());

		for( UINT32 arrayIndex=0; arrayIndex<m_arraySize; arrayIndex++)
		{
			for( UINT32 level=0; level<m_numMipMaps; level++)
			{
				this->CopyToSubTexture2D( target, arrayIndex, level, 0, 0, target.GetWidth( level), target.GetHeight( level), arrayIndex, level, 0, 0, GetWidth( level), GetHeight( level));
			}
		}
	}

	// copy texture
	void OGLESTexture2D::CopyToSubTexture2D( Texture& target,UINT dstArrayIdx, UINT dstLevel, UINT dstXOffset, UINT dstYOffset, UINT dstWidth, UINT dstHeight, UINT srcArrayIdx, UINT srcLevel, UINT srcXOffset, UINT srcYOffset, UINT srcWidth, UINT srcHeight)
	{
		A_ASSERT( m_type==target.GetType());

		//if( !re)
	}

	// Map2D texture
	void OGLESTexture2D::Map2D( UINT arrayIndex, UINT level, TextureMapAccess access, UINT xOffset, UINT yOffset, UINT width, UINT height, void*&data, UINT& rowPitch)
	{

	}

	// Unmaap2D
	void OGLESTexture2D::Unmap2D( UINT arrayIndex, UINT level)
	{

	}

	// Éú³ÉMipmaps
	void OGLESTexture2D::GenerateMips()
	{

	}
}