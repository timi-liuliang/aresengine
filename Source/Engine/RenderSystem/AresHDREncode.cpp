#include <algorithm>
#include <Engine/RenderSystem/AresHDREncode.h>

namespace Ares
{
	HDREncode										 g_hdrEncode;		// 全局变量, 用于powofTwo初始化
	array<DOUBLE, HDREncode::LenEncode+1> HDREncode::m_powsOfTwo;

	// 构造函数
	HDREncode::HDREncode()
	{
		for( int i = 0; i <= LenEncode; i++ )
		{
			m_powsOfTwo[i] = powf( 2.0f, ( float )( i - HalfEncode));
		}
	}

	int HDREncode::Log2Ceiling( float val )
	{
		int iMax = LenEncode;
		int iMin = 0;

		while( iMax - iMin > 1 )
		{
			int iMiddle = ( iMax + iMin ) / 2;

			if( val > m_powsOfTwo[iMiddle] )
				iMin = iMiddle;
			else
				iMax = iMiddle;
		}

		return iMax - HalfEncode;
	}

	// 编码RGBE8
	ColorRGB HDREncode::EncodeRGBE8( const ColorRGB& rgb)
	{
		ColorRGB vEncoded = rgb;

		// Determine the largest color component
		float maxComponent = std::max<float>( std::max<float>( vEncoded.r, vEncoded.g), vEncoded.b);

		// Round to the nearest integer exponent
		int nExp = Log2Ceiling(maxComponent);

		// Divide the components by the shared exponent
		FLOAT fDivisor = (float)(m_powsOfTwo[nExp+HalfEncode]);

		// Divide the components by the shared exponent
		vEncoded.r /= fDivisor;
		vEncoded.g /= fDivisor;
		vEncoded.b /= fDivisor;

		vEncoded.r = std::max<float>( 0.f, std::min<float>( 1.f, vEncoded.r));
		vEncoded.g = std::max<float>( 0.f, std::min<float>( 1.f, vEncoded.g));
		vEncoded.b = std::max<float>( 0.f, std::min<float>( 1.f, vEncoded.b));
		vEncoded.a = (nExp + HalfEncode) / (float)MaxEncode;

		return vEncoded;
	}

	// 解码RGBE8
	ColorRGB HDREncode::DecodeRGBE8( const ColorRGB& rgbe)
	{
		ColorRGB vDecoded = rgbe;

		// Retrieve the shared exponent
		int     nExp = (int)(rgbe.a * MaxEncode);

		// Multiply through the color components
		vDecoded.r = (float)(vDecoded.r * m_powsOfTwo[nExp]);
		vDecoded.g = (float)(vDecoded.g * m_powsOfTwo[nExp]);
		vDecoded.b = (float)(vDecoded.b * m_powsOfTwo[nExp]);

		return vDecoded;
	}

	#define RGBMRange 8.f

	// 编码RGBM8
	ColorRGB HDREncode::EncodeRGBM8( const ColorRGB& rgb)
	{
		ColorRGB vEncoded = rgb;

		// Determine the largest color component
		float maxCeil = std::max<float>(std::max<float>( std::max<float>( vEncoded.r, vEncoded.g), vEncoded.b), 0.1f);

		vEncoded.a  = maxCeil / RGBMRange;
		vEncoded.r /= maxCeil;
		vEncoded.g /= maxCeil;
		vEncoded.b /= maxCeil;

		return vEncoded;
	}

	// 解码RGBM8
	ColorRGB HDREncode::DecodeRGBM8( const ColorRGB& rgbe)
	{
		ColorRGB vDecoded = rgbe;

		vDecoded.r *= vDecoded.a * RGBMRange;
		vDecoded.g *= vDecoded.a * RGBMRange;
		vDecoded.b *= vDecoded.a * RGBMRange;

		return vDecoded;
	}
}