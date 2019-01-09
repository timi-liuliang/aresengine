#pragma once

#include <Core/Graphics/AresTexture.h>
#include "OGLPlatform.h"
#include "OGLMapping.h"

namespace Ares
{
	//--------------------------------------
	// OGLTexture 2013-2-19 帝林
	//--------------------------------------
	class OGLTexture : public Texture
	{
	public:
		OGLTexture( TextureType type, UINT arraySize, UINT sampleCount, UINT sampleQuality, UINT accessHint);
		~OGLTexture();

		// 获取深度
		virtual UINT GetDepth( UINT level) const;

	protected:
		// 格式转换
		ElementFormat SRGBToRGB( ElementFormat pf);

	protected:
		GLuint					m_texture;		// 纹理
		GLenum					m_targetType;	// 目标类型
		vector<GLuint>			m_pBos;
		vector<vector<UINT8>>	m_texData;
	};

	//--------------------------------------
	// OGLTexture2D 2013-2-19 帝林
	//--------------------------------------
	class OGLTexture2D : public OGLTexture
	{
	public:
		OGLTexture2D( UINT width, UINT height, UINT numMipMaps, UINT arraySize, ElementFormat format, UINT sampleCount, UINT sampleQuality, UINT accessHint, const ElementInitData* initData);
	
		// returns the width of the texture
		virtual UINT GetWidth( UINT levle) const;

		// returns the height of the texture
		virtual UINT GetHeight( UINT levle) const;

		// Copies (and maybe scales to fit) the contents of this texture to another texture
		virtual void CopyToTexure( Texture& target);

		// copy texture
		virtual void CopyToSubTexture2D( Texture& target,UINT dstArrayIdx, UINT dstLevel, UINT dstXOffset, UINT dstYOffset, UINT dstWidth, UINT dstHeight, UINT srcArrayIdx, UINT srcLevel, UINT srcXOffset, UINT srcYOffset, UINT srcWidth, UINT srcHeight);

		// 生成Mipmaps
		virtual void GenerateMips();

	private:
		// Map2D texture
		virtual void Map2D( UINT arrayIndex, UINT level, TextureMapAccess access, UINT xOffset, UINT yOffset, UINT width, UINT height, void*&data, UINT& rowPitch);

		// Unmaap2D
		virtual void Unmap2D( UINT arrayIndex, UINT level);

	private:
		vector<UINT>			m_widthes;
		vector<UINT>			m_heights;
	};
}