#include "OGLTexture.h"
#include "OGLRenderDevice.h"

namespace Ares
{
	OGLTexture2D::OGLTexture2D( UINT width, UINT height, UINT numMipMaps, UINT arraySize, ElementFormat format, UINT sampleCount, UINT sampleQuality, UINT accessHint, const ElementInitData* initData)
		: OGLTexture( TT_2D, arraySize, sampleCount, sampleQuality, accessHint)
	{
		if( !glloader_GL_EXT_texture_sRGB())
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

		UINT32 texelSize = NumFormatBytes( m_format);

		GLint  glinternalFormat;
		GLenum glFormat;
		GLenum glType;
		OGLMapping::MappingFormat( glinternalFormat, glFormat, glType, m_format);

		if( glloader_GL_ARB_pixel_buffer_object())
		{
			m_pBos.resize( arraySize * m_numMipMaps);
			glGenBuffers( static_cast<GLsizei>(m_pBos.size()), m_pBos.data());
		}
		else
		{
			m_texData.resize( arraySize * m_numMipMaps);
		}

		if( sampleCount<=1)
		{
			glGenTextures( 1, &m_texture);
			glBindTexture( m_targetType, m_texture);
			glTexParameteri( m_targetType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri( m_targetType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri( m_targetType, GL_TEXTURE_MAX_LEVEL,  m_numMipMaps-1);

			for( UINT32 arrayIndex=0; arrayIndex<arraySize; arrayIndex++)
			{
				for( UINT32 level=0; level<m_numMipMaps; level++)
				{
					UINT32 const w = m_widthes[level];
					UINT32 const h = m_heights[level];

					if( !m_pBos.empty())
					{
						PDRenderDevice.BindBuffer( GL_PIXEL_UNPACK_BUFFER_ARB, m_pBos[arrayIndex*m_numMipMaps + level]);
					}

					if( IsCompressedFormat( m_format))
					{
						int blockSize;
						if( (EF_BC1==format) || (EF_SIGNED_BC1==format) || (EF_BC1_SRGB==format || ( EF_BC4==format) || ( EF_SIGNED_BC4==format) || (EF_BC4_SRGB==format)))
							blockSize = 8;
						else
							blockSize = 16;

						GLsizei const imageSize = ( (w+3)/4) * ( (h+3)/4) * blockSize;
						if( !m_pBos.empty())
						{
							glBufferData( GL_PIXEL_UNPACK_BUFFER_ARB, imageSize, NULL, GL_STREAM_DRAW);
							PDRenderDevice.BindBuffer( GL_PIXEL_UNPACK_BUFFER_ARB, 0);
						}
						else
						{
							m_texData[arrayIndex*m_numMipMaps+level].resize( imageSize);
						}

						if( arraySize>1)
						{
							if( arrayIndex==0)
								glCompressedTexImage3D( m_targetType, level, glinternalFormat, w, h, arraySize, 0, imageSize, NULL);
							
							glCompressedTexSubImage3D( m_targetType, level, 0, 0, arrayIndex, w, h, 1, glFormat, glType, initData ? initData[arrayIndex*m_numMipMaps+level].m_data : NULL);
						}
						else
						{
							glCompressedTexImage2D( m_targetType, level, glinternalFormat, w, h, 0, imageSize, initData ? initData[arrayIndex*numMipMaps+level].m_data : NULL);
						}
					}
					else
					{
						GLsizei const imageSize = w*h*texelSize;
						if( !m_pBos.empty())
						{
							glBufferData( GL_PIXEL_UNPACK_BUFFER_ARB, imageSize, NULL, GL_STREAM_DRAW);
							PDRenderDevice.BindBuffer( GL_PIXEL_UNPACK_BUFFER_ARB, 0);
						}
						else
						{
							m_texData[arrayIndex*m_numMipMaps+level].resize( imageSize);
						}

						if( arraySize>1)
						{
							if( arraySize)
								glTexImage3D( m_targetType, level, glinternalFormat, w, h, arraySize, 0, glFormat, glType, NULL);

							glTexSubImage3D( m_targetType, level, 0, 0, arrayIndex, w, h, 1, glFormat, glType, initData ? initData[arrayIndex*m_numMipMaps+level].m_data : NULL);
						}
						else
						{
							glTexImage2D( m_targetType, level, glinternalFormat, w, h, 0, glFormat, glType, initData ? initData[arrayIndex*m_numMipMaps+level].m_data : NULL);
						}
					}
				}
			}
		}
		else
		{
			glGenRenderbuffersEXT( 1, &m_texture);
			glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, m_texture);
			glRenderbufferStorageMultisampleEXT( GL_RENDERBUFFER_EXT, sampleCount, glinternalFormat, width, height);
		}
	}

	// returns the width of the texture
	UINT OGLTexture2D::GetWidth( UINT levle) const
	{
		A_ASSERT( levle < m_numMipMaps);

		return m_widthes[levle];
	}

	// returns the height of the texture
	UINT OGLTexture2D::GetHeight( UINT levle) const
	{
		A_ASSERT( levle < m_numMipMaps);

		return m_heights[levle];
	}

	// Copies (and maybe scales to fit) the contents of this texture to another texture
	void OGLTexture2D::CopyToTexure( Texture& target)
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
	void OGLTexture2D::CopyToSubTexture2D( Texture& target,UINT dstArrayIdx, UINT dstLevel, UINT dstXOffset, UINT dstYOffset, UINT dstWidth, UINT dstHeight, UINT srcArrayIdx, UINT srcLevel, UINT srcXOffset, UINT srcYOffset, UINT srcWidth, UINT srcHeight)
	{
		A_ASSERT( m_type==target.GetType());

		//if( !re)
	}

	// Map2D texture
	void OGLTexture2D::Map2D( UINT arrayIndex, UINT level, TextureMapAccess access, UINT xOffset, UINT yOffset, UINT width, UINT height, void*&data, UINT& rowPitch)
	{

	}

	// Unmaap2D
	void OGLTexture2D::Unmap2D( UINT arrayIndex, UINT level)
	{

	}

	// Éú³ÉMipmaps
	void OGLTexture2D::GenerateMips()
	{

	}
}