#pragma once

#include "AresElementFormat.h"
#include "AresRenderAPITypedefs.h"

using namespace std;
using namespace boost;

namespace Ares
{
	// 读写权限
	enum TextureMapAccess
	{
		TMA_ReadOnly  = 0,	// 只读
		TMA_WriteOnly,		// 只写
		TMA_ReadWrite,		// 读写
	};

	//------------------------------------
	// Texture 2012/3/2 帝林
	//------------------------------------
	class Texture
	{
	public:
		// Enum identifying the texture type
		enum TextureType
		{		
			TT_1D,	// 1D texture, used in combination with 1D texture coordinates
			TT_2D,	// 2D texture, used in combination with 2D texture coordinates
			TT_3D,  // 3D texture, used in combination with 3D texture coordinates
			TT_Cube,// cube map,   used in combination with 3D texture coordinates
		};

		// cube faces
		enum CubeFaces
		{
			CF_Positive_X = 0,
			CF_Negative_X = 1,
			CF_Positive_Y = 2,
			CF_Negative_Y = 3,
			CF_Positive_Z = 4,
			CF_Negative_Z = 5,
		};

	public:
		// GraphicsBuffer Modifier
		class Modifier : noncopyable
		{
			friend class Texture;
		public:
			// Mapper 2d texture
			Modifier( Texture& tex, TextureMapAccess tma, UINT arrayIndex=0, UINT level=0, UINT xOffset=0, UINT yOffset=0, UINT width=-1, UINT height=-1)
				: m_tex( tex), m_mappedArrayIndex( arrayIndex), m_mappedLevel( level)
			{
				width  = width==-1 ? tex.GetWidth(0) : width;
				height = height==-1? tex.GetHeight(0) : height;

				m_tex.Map2D( arrayIndex, level, tma, xOffset, yOffset, width, height, m_data, m_rowPitch);
				m_slicePitch = m_rowPitch * height;
			}

			// destructor
			~Modifier()
			{
				switch( m_tex.GetType())
				{
				case TT_2D:
					m_tex.Unmap2D( m_mappedArrayIndex, m_mappedLevel);
					break;
				}
			}

			// returns the pointer of the data
			template<typename T>
			T* GetPointer()
			{
				return static_cast<T*>(m_data);
			}

			// returns the pointer of the data
			template<typename T>
			const T* GetPointer() const
			{
				return static_cast<T*>(m_data);
			}

			// set data
			void Set( void* src, size_t sizeInByte, size_t baseByte=0)
			{
				std::memcpy( m_data, src, sizeInByte+baseByte);
			}

			// 获取RowPitch
			UINT GetRowPitch() const { return m_rowPitch; }

			// 获取SlicePitch
			UINT GetSlicePitch() const { return m_slicePitch; }

		private:
			Texture&	m_tex;
			void*		m_data;
			UINT		m_rowPitch;
			UINT		m_slicePitch;
			UINT		m_mappedArrayIndex;
			CubeFaces	m_mappedFace;
			UINT		m_mappedLevel;
		};

		// 构造函数
		explicit Texture( TextureType type, UINT sampleCount, UINT sampleQuality, UINT accessHint);

		// 析构函数
		virtual ~Texture();

		// Gets the number of mipmaps to be used for this texture
		UINT GetNumMipMaps() const { return m_numMipMaps; }

		// Gets the size of texture array
		UINT GetArraySize() const { return m_arraySize; }

		// Returns the pixel format for the texture surface
		ElementFormat GetFormat() const { return m_format; }

		// returns the sample count
		UINT GetSampleCount() const { return m_sampleCount; }

		// returns the sample quality
		UINT GetSampleQuality() const { return m_sampleQuality; }

		// returns the access hint
		UINT GetAccessHint() const { return m_accessHint; }

		// returns the width of the texture
		virtual UINT GetWidth( UINT levle) const=0;

		// returns the height of the texture
		virtual UINT GetHeight( UINT levle) const=0;

		// returns the depth of the texture( only for 3d texture)
		virtual UINT GetDepth( UINT level) const=0;

		// returns the texture type of the texture
		TextureType GetType() const { return m_type; }

		// generate mipmap levels
		virtual void GenerateMips()=0;

		// Copies (and maybe scales to fit) the contents of this texture to another texture
		virtual void CopyToTexure( Texture& target)=0;

		// copy texture
		virtual void CopyToSubTexture2D( Texture& target,UINT dstArrayIdx, UINT dstLevel, UINT dstXOffset, UINT dstYOffset, UINT dstWidth, UINT dstHeight, UINT srcArrayIdx, UINT srcLevel, UINT srcXOffset, UINT srcYOffset, UINT srcWidth, UINT srcHeight)=0;

		// Map2D texture
		virtual void Map2D( UINT arrayIndex, UINT level, TextureMapAccess access, UINT xOffset, UINT yOffset, UINT width, UINT height, void*&data, UINT& rowPitch)=0;

		// Unmaap2D
		virtual void Unmap2D( UINT arrayIndex, UINT level)=0;

		// set the respath
		void SetResPath( const char* resPath) { m_resPath = resPath; }

	public:
		// 从硬盘中加载纹理数据 only *.dds
		static bool LoadTexture( const char* texPath, TextureType& type, UINT& width, UINT& height, UINT& depth, UINT& numMipMaps, UINT& arraySize, ElementFormat& format, vector<ElementInitData>& initData, vector<BYTE>& dataBlock);

		// 向硬盘中写入纹理数据
		static bool SaveTexture( const char* texPath, TextureType type, UINT width, UINT height, UINT depth, UINT numMipMaps, UINT arraySize, ElementFormat format, const vector<ElementInitData>& initData);

	protected:
		string			m_resPath;		// 资源路径
		UINT			m_numMipMaps;	// num mip maps
		UINT			m_arraySize;	// 未知

		ElementFormat	m_format;		// format
		TextureType		m_type;			// texture type
		UINT			m_sampleCount;	// 采样数量
		UINT			m_sampleQuality;// 采样品质
		UINT			m_accessHint;	// 未知	
	};
}