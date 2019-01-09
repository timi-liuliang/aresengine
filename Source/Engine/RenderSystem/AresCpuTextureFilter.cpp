#include <Engine/AresRoot.h>
#include <Engine/RenderSystem/AresCpuTextureFilter.h>

namespace Ares
{
	// 索引器
	struct IndexerHelper
	{
	public:
		// 构造函数
		IndexerHelper( UINT width, UINT height, UINT wIdx, UINT hIdx)
			: m_width(width), m_height(height), m_wIdx(wIdx), m_hIdx(hIdx)
		{}

		// 获取相对索引
		UINT GetRelativeIdx( int rw, int rh) const
		{
			return ( m_hIdx+rh)*m_width + m_wIdx + rw;
		}

		// 判断索引是否有效
		bool IsValidIdx( UINT idx) const
		{
			return idx<m_width*m_height;
		}

	private:
		UINT	m_width;
		UINT	m_height;
		UINT	m_wIdx;
		UINT	m_hIdx;
	};

	// 边缘填充：主要用于解决光照图color bleeding问题
	bool CpuTextureFilter::EdgePadding( TexturePtr& source, ColorRGB backgroundColor, int extendPixel, bool isRGBE)
	{
		if( !source)
			return false;

		ElementFormat format	 = source->GetFormat();
		UINT		  numMipmaps = 1;
		UINT		  arraySize  = 1;
		int			  width		 = static_cast<int>(source->GetWidth( 0));
		int			  height	 = static_cast<int>(source->GetHeight( 0));

		// 断言纹理格式
		A_ASSERT( format == EF_ARGB8);
		A_ASSERT( source->GetType() == Texture::TT_2D);

		TexturePtr textureSysMem = ARenderDevice->CreateTexture2D( width, height, numMipmaps, arraySize, format, 1, 0, EAH_CPURead | EAH_CPUWrite, NULL);

		// 拷贝纹理
		source->CopyToTexure( *textureSysMem);

		// 纹理处理()
		{
			Texture::Modifier modifie( *textureSysMem, TMA_ReadWrite, 0, 0, 0, 0, width, height);
			DWORD* out  = modifie.GetPointer<DWORD>();

			// 临时数据
			vector<DWORD> colors(width*height);
			std::memcpy( colors.data(), out, sizeof(DWORD)*colors.size());

			// 执行处理
			#pragma omp parallel for
			for ( int w=0; w<width; w++)
			{
				for ( int h=0; h<height; h++)
				{
					// 获取当前像素颜色
					IndexerHelper tIHelper( width, height, w, h);
					DWORD		  bColor	  = backgroundColor.ToDWORD();
					DWORD&		  curPixel    = out[tIHelper.GetRelativeIdx(0, 0)];
					float		  tTotalPer   = 0.f;
					ColorRGB	  tTotalColor = ColorRGB::Black;
					int			  curLen	  = 0xFFFFF;
					if( curPixel == bColor)
					{
						// 搜寻周围像素点,并记录距离与颜色值
						for ( int ew=-extendPixel; ew<=extendPixel; ew++)
						{
							for ( int eh=-extendPixel; eh<=extendPixel; eh++)
							{
								int sIdx = tIHelper.GetRelativeIdx( ew, eh);								
								if( tIHelper.IsValidIdx( sIdx) && colors[sIdx]!=bColor)
								{
									int sqrLen = ew*ew+eh*eh;
									if( curLen > sqrLen)
									{
										curLen = sqrLen;
										tTotalPer	= 1.f;
										tTotalColor = HDREncode::DecodeRGBM8(colors[sIdx]);
									}
									else if( curLen==sqrLen)
									{
										tTotalPer	+= 1.f;
										tTotalColor += HDREncode::DecodeRGBM8(colors[sIdx]);
									}
								}
							}
						}

						// 修改当前背影点的颜色值
						curPixel = HDREncode::EncodeRGBM8( tTotalColor/tTotalPer).ToDWORD();
					}
				}
			}
		}

		// 拷贝回去
		textureSysMem->CopyToTexure( *source);
		
		return true;
	}

	// 索引器
	struct GaussianHelper
	{
	public:
		// 构造函数
		GaussianHelper( float radius)
		{
			radius = std::min<float>( radius, 50);

			m_rho				= radius / 3.f;
			m_halfDimension		= static_cast<int>(std::ceil( radius));
			m_dimension			= m_halfDimension * 2 + 1;

			// 预计算distribution
			InitDistribution( m_rho);
		}

		// 获取高斯模糊贡献率(辅助函数)
		float GetDistribution( int dx, int dy)
		{
			return m_distribution[dx+m_halfDimension][dy+m_halfDimension];
		}

		// 获取半径
		int GetHalfDimension() { return m_halfDimension; }

	private:
		// 初始化分布率
		void InitDistribution( float rho)
		{
			float g = 1.f / (2.f*PI*rho*rho);

			// 预计算distribution
			for ( int dx=-m_halfDimension; dx<=m_halfDimension; dx++)
				for ( int dy=-m_halfDimension; dy<=m_halfDimension; dy++)
				{
					m_distribution[dx+m_halfDimension][dy+m_halfDimension]  = g * std::exp( -(dx*dx+dy*dy) / (2*rho*rho));
				}
		}

	private:
		float			m_rho;
		int				m_dimension;				// 维数
		int				m_halfDimension;			// 0.5维数
		float			m_distribution[101][101];	// 分布率
	};

	// 高斯模糊
	bool CpuTextureFilter::GaussianBlur( TexturePtr& source, float radius)
	{
		if( !source)
		{
			ALogSystem.Warning( "高斯模糊,传入空纹理!");

			return false;
		}

		ElementFormat format	 = source->GetFormat();
		UINT		  numMipmaps = 1;
		UINT		  arraySize  = 1;
		int			  width		 = static_cast<int>(source->GetWidth( 0));
		int			  height	 = static_cast<int>(source->GetHeight( 0));

		// 断言纹理格式
		A_ASSERT( format == EF_ARGB8);
		A_ASSERT( source->GetType() == Texture::TT_2D);

		TexturePtr textureSysMem = ARenderDevice->CreateTexture2D( width, height, numMipmaps, arraySize, format, 1, 0, EAH_CPURead | EAH_CPUWrite, NULL);
		source->CopyToTexure( *textureSysMem);

		// 纹理处理()
		{
			Texture::Modifier modifie( *textureSysMem, TMA_ReadWrite, 0, 0, 0, 0, width, height);
			DWORD* out = modifie.GetPointer<DWORD>();

			// 临时数据
			vector<DWORD> colors(width*height);
			std::memcpy( colors.data(), out, sizeof(DWORD)*colors.size());

			GaussianHelper tGHelper( radius);

			// 执行处理
			#pragma omp parallel for
			for ( int x=0; x<width; x++)
			{
				for ( int y=0; y<height; y++)
				{
					float		  tTotalPer  = 0.f;
					ColorRGB	  tTotalColor= ColorRGB::Black;
					int			  halfDim	 = tGHelper.GetHalfDimension();
					IndexerHelper tIHelper( width, height, x, y);
					DWORD&		  curPixel   = out[tIHelper.GetRelativeIdx(0, 0)];

					for ( int dx=-halfDim; dx<=halfDim; dx++)
					{
						for ( int dy=-halfDim; dy<=halfDim; dy++)
						{
							int curIdx = tIHelper.GetRelativeIdx( dx, dy);
							if( tIHelper.IsValidIdx(curIdx))
							{
								float dsb   = tGHelper.GetDistribution( dx, dy);
								tTotalPer  += dsb;
								tTotalColor+= HDREncode::DecodeRGBM8( colors[curIdx]) * dsb;
							}
						}
					}

					curPixel = HDREncode::EncodeRGBM8( tTotalColor/tTotalPer).ToDWORD();
				}
			}
		}

		// 拷贝回去
		textureSysMem->CopyToTexure( *source);

		return true;
	}
}