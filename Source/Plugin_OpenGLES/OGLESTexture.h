#pragma once

#include <Core/Graphics/AresTexture.h>
#include "OGLESPlatform.h"
#include "OGLESMapping.h"

namespace Ares
{
	//--------------------------------------
	// OGLESTexture 2013-2-19 帝林
	//--------------------------------------
	class OGLESTexture : public Texture
	{
	public:
		OGLESTexture( TextureType type, UINT arraySize, UINT sampleCount, UINT sampleQuality, UINT accessHint);
		~OGLESTexture();

		// 获取深度
		virtual UINT GetDepth( UINT level) const;

	protected:
		// 格式转换
		ElementFormat SRGBToRGB( ElementFormat pf);

	protected:
		GLuint					m_texture;		// 纹理
		GLenum					m_targetType;	// 目标类型
		vector<vector<UINT8> >	m_texData;
	};

	//--------------------------------------
	// OGLESTexture2D 2013-2-19 帝林
	//--------------------------------------
	class OGLESTexture2D : public OGLESTexture
	{
	public:
		OGLESTexture2D( UINT width, UINT height, UINT numMipMaps, UINT arraySize, ElementFormat format, UINT sampleCount, UINT sampleQuality, UINT accessHint, const ElementInitData* initData);
	
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