#pragma once

#include "Core/AresPlatform.h"

namespace Ares
{
	// ElementChannel
	enum ElementChannel
	{
		EC_R = 0UL,
		EC_G = 1UL,
		EC_B = 2UL,
		EC_A = 3UL,
		EC_D = 4UL,
		EC_S = 5UL,
		EC_BC = 6UL,
		EC_E = 7UL
	};

	// ElementChannelType
	enum ElementChannelType
	{
		ECT_UNorm = 0UL,
		ECT_SNorm = 1UL,
		ECT_UInt = 2UL,
		ECT_SInt = 3UL,
		ECT_Float = 4UL,
		ECT_UNorm_SRGB = 5UL,
		ECT_Typeless = 6UL,
		ECT_SharedExp = 7UL
	};

	// element format is a 64-bit value:
	// 00000000 T3[4] T2[4] T1[4] T0[4] S3[6] S2[6] S1[6] S0[6] C3[4] C2[4] C1[4] C0[4]
	template <QWORD ch0, QWORD ch1, QWORD ch2, QWORD ch3, QWORD ch0Size, QWORD ch1Size, QWORD ch2Size, QWORD ch3Size, QWORD ch0Type, QWORD ch1Type, QWORD ch2Type, QWORD ch3Type>
	struct MakeElementFormat4
	{
		static QWORD const value = (ch0 << 0) | (ch1 << 4) | (ch2 << 8) | (ch3 << 12)
								 | (ch0Size << 16) | (ch1Size << 22) | (ch2Size << 28) | (ch3Size << 34)
								 | (ch0Type << 40) | (ch1Type << 44) | (ch2Type << 48) | (ch3Type << 52);
	};

	template <QWORD ch0, QWORD ch1, QWORD ch2, QWORD ch0Size, QWORD ch1Size, QWORD ch2Size, QWORD ch0Type, QWORD ch1Type, QWORD ch2Type>
	struct MakeElementFormat3
	{
		static QWORD const value = MakeElementFormat4<ch0, ch1, ch2, 0, ch0Size, ch1Size, ch2Size, 0, ch0Type, ch1Type, ch2Type, 0>::value;
	};

	template <QWORD ch0, QWORD ch1, QWORD ch0Size, QWORD ch1Size, QWORD ch0Type, QWORD ch1Type>
	struct MakeElementFormat2
	{
		static QWORD const value = MakeElementFormat3<ch0, ch1, 0, ch0Size, ch1Size, 0, ch0Type, ch1Type, 0>::value;
	};

	template<QWORD ch0, QWORD ch0Size, QWORD ch0Type>
	struct MakeElementFormat1
	{
		static QWORD const value = MakeElementFormat2<ch0, 0, ch0Size, 0, ch0Type, 0>::value;
	};

#ifdef ARES_STRONGLY_TYPED_ENUMS_SUPPORT
	enum ElementFormat : UINT64
	{
		// Unknown element format
		EF_UnKnown = 0,

		// 8-bit element format, all bits alpha
		EF_A8 = MakeElementFormat1<EC_A, 8, ECT_UNorm>::value,

		// 16-bit element format, 4 bits for alpha, red, green and blue.
		EF_ARGB4 = MakeElementFormat4<EC_A, EC_R, EC_G, EC_B, 4, 4, 4, 4, ECT_UNorm, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value,

		// 8-bit element format, 8 bits for red.
		EF_R8 = MakeElementFormat1<EC_R, 8, ECT_UNorm>::value,

		// 8-bit element format, 8 bits for signed red.
		EF_SIGNED_R8 = MakeElementFormat1<EC_R, 8, ECT_SNorm>::value,

		// 16-bit element format, 8 bits for red, green.
		EF_GR8 = MakeElementFormat2<EC_G, EC_R, 8, 8, ECT_UNorm, ECT_UNorm>::value,

		// 16-bit element format, 8 bits for signed red, green.
		EF_SIGNED_GR8 = MakeElementFormat2<EC_G, EC_R, 8, 8, ECT_SNorm, ECT_SNorm>::value,

		// 24-bit element format, 8 bits for red, green and blue.
		EF_BGR8 = MakeElementFormat3<EC_B, EC_G, EC_R, 8, 8, 8, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value,

		// 24-bit element format, 8 bits for signed red, green and blue.
		EF_SIGNED_BGR8 = MakeElementFormat3<EC_B, EC_G, EC_R, 8, 8, 8, ECT_SNorm, ECT_SNorm, ECT_SNorm>::value,

		// 32-bit element format, 8 bits for alpha, red, green and blue.
		EF_ARGB8 = MakeElementFormat4<EC_A, EC_R, EC_G, EC_B, 8, 8, 8, 8, ECT_UNorm, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value,

		// 32-bit element format, 8 bits for alpha, red, green and blue.
		EF_ABGR8 = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 8, 8, 8, 8, ECT_UNorm, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value,

		// 32-bit element format, 8 bits for signed alpha, red, green and blue.
		EF_SIGNED_ABGR8 = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 8, 8, 8, 8, ECT_SNorm, ECT_SNorm, ECT_SNorm, ECT_SNorm>::value,

		// 32-bit element format, 2 bits for alpha, 10 bits for red, green and blue.
		EF_A2BGR10 = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 2, 10, 10, 10, ECT_UNorm, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value,

		// 32-bit element format, 2 bits for alpha, 10 bits for signed red, green and blue.
		EF_SIGNED_A2BGR10 = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 2, 10, 10, 10, ECT_SNorm, ECT_SNorm, ECT_SNorm, ECT_SNorm>::value,

		// 32-bit element format, 8 bits for alpha, red, green and blue.
		EF_R8UI = MakeElementFormat1<EC_R, 8, ECT_UInt>::value,

		// 32-bit element format, 8 bits for alpha, red, green and blue.
		EF_R8I = MakeElementFormat1<EC_R, 8, ECT_SInt>::value,

		// 16-bit element format, 8 bits for red, green.
		EF_GR8UI = MakeElementFormat2<EC_G, EC_R, 8, 8, ECT_UInt, ECT_UInt>::value,

		// 16-bit element format, 8 bits for red, green.
		EF_GR8I = MakeElementFormat2<EC_G, EC_R, 8, 8, ECT_SInt, ECT_SInt>::value,

		// 24-bit element format, 8 bits for red, green and blue.
		EF_BGR8UI = MakeElementFormat3<EC_B, EC_G, EC_R, 8, 8, 8, ECT_UInt, ECT_UInt, ECT_UInt>::value,

		// 24-bit element format, 8 bits for red, green and blue.
		EF_BGR8I = MakeElementFormat3<EC_B, EC_G, EC_R, 8, 8, 8, ECT_SInt, ECT_SInt, ECT_SInt>::value,

		// 32-bit element format, 8 bits for alpha, red, green and blue.
		EF_ABGR8UI = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 8, 8, 8, 8, ECT_UInt, ECT_UInt, ECT_UInt, ECT_UInt>::value,

		// 32-bit element format, 8 bits for signed alpha, red, green and blue.
		EF_ABGR8I = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 8, 8, 8, 8, ECT_SInt, ECT_SInt, ECT_SInt, ECT_SInt>::value,

		// 32-bit element format, 2 bits for alpha, 10 bits for red, green and blue.
		EF_A2BGR10UI = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 2, 10, 10, 10, ECT_UInt, ECT_UInt, ECT_UInt, ECT_UInt>::value,

		// 32-bit element format, 2 bits for alpha, 10 bits for red, green and blue.
		EF_A2BGR10I = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 2, 10, 10, 10, ECT_SInt, ECT_SInt, ECT_SInt, ECT_SInt>::value,

		// 16-bit element format, 16 bits for red.
		EF_R16 = MakeElementFormat1<EC_R, 16, ECT_UNorm>::value,

		// 16-bit element format, 16 bits for signed red.
		EF_SIGNED_R16 = MakeElementFormat1<EC_R, 16, ECT_SNorm>::value,

		// 32-bit element format, 16 bits for red and green.
		EF_GR16 = MakeElementFormat2<EC_G, EC_R, 16, 16, ECT_UNorm, ECT_UNorm>::value,

		// 32-bit element format, 16 bits for signed red and green.
		EF_SIGNED_GR16 = MakeElementFormat2<EC_G, EC_R, 16, 16, ECT_SNorm, ECT_SNorm>::value,

		// 48-bit element format, 16 bits for alpha, blue, green and red.
		EF_BGR16 = MakeElementFormat3<EC_B, EC_G, EC_R, 16, 16, 16, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value,

		// 48-bit element format, 16 bits for signed alpha, blue, green and red.
		EF_SIGNED_BGR16 = MakeElementFormat3<EC_B, EC_G, EC_R, 16, 16, 16, ECT_SNorm, ECT_SNorm, ECT_SNorm>::value,

		// 64-bit element format, 16 bits for alpha, blue, green and red.
		EF_ABGR16 = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 16, 16, 16, 16, ECT_UNorm, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value,

		// 64-bit element format, 16 bits for signed alpha, blue, green and red.
		EF_SIGNED_ABGR16 = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 16, 16, 16, 16, ECT_SNorm, ECT_SNorm, ECT_SNorm, ECT_SNorm>::value,

		// 32-bit element format, 32 bits for red.
		EF_R32 = MakeElementFormat1<EC_R, 32, ECT_UNorm>::value,

		// 32-bit element format, 32 bits for signed red.
		EF_SIGNED_R32 = MakeElementFormat1<EC_R, 32, ECT_SNorm>::value,

		// 64-bit element format, 16 bits for red and green.
		EF_GR32 = MakeElementFormat2<EC_G, EC_R, 32, 32, ECT_UNorm, ECT_UNorm>::value,

		// 64-bit element format, 16 bits for signed red and green.
		EF_SIGNED_GR32 = MakeElementFormat2<EC_G, EC_R, 32, 32, ECT_SNorm, ECT_SNorm>::value,

		// 96-bit element format, 16 bits for alpha, blue, green and red.
		EF_BGR32 = MakeElementFormat3<EC_B, EC_G, EC_R, 32, 32, 32, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value,

		// 96-bit element format, 16 bits for signed_alpha, blue, green and red.
		EF_SIGNED_BGR32 = MakeElementFormat3<EC_B, EC_G, EC_R, 32, 32, 32, ECT_SNorm, ECT_SNorm, ECT_SNorm>::value,

		// 128-bit element format, 16 bits for alpha, blue, green and red.
		EF_ABGR32 = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 32, 32, 32, 32, ECT_UNorm, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value,

		// 128-bit element format, 16 bits for signed alpha, blue, green and red.
		EF_SIGNED_ABGR32 = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 32, 32, 32, 32, ECT_SNorm, ECT_SNorm, ECT_SNorm, ECT_SNorm>::value,

		// 16-bit element format, 16 bits for red.
		EF_R16UI = MakeElementFormat1<EC_R, 16, ECT_UInt>::value,

		// 16-bit element format, 16 bits for signed red.
		EF_R16I = MakeElementFormat1<EC_R, 16, ECT_SInt>::value,

		// 32-bit element format, 16 bits for red and green.
		EF_GR16UI = MakeElementFormat2<EC_G, EC_R, 16, 16, ECT_UInt, ECT_UInt>::value,

		// 32-bit element format, 16 bits for signed red and green.
		EF_GR16I = MakeElementFormat2<EC_G, EC_R, 16, 16, ECT_SInt, ECT_SInt>::value,

		// 48-bit element format, 16 bits for alpha, blue, green and red.
		EF_BGR16UI = MakeElementFormat3<EC_B, EC_G, EC_R, 16, 16, 16, ECT_UInt, ECT_UInt, ECT_UInt>::value,

		// 48-bit element format, 16 bits for signed alpha, blue, green and red.
		EF_BGR16I = MakeElementFormat3<EC_B, EC_G, EC_R, 16, 16, 16, ECT_SInt, ECT_SInt, ECT_SInt>::value,

		// 64-bit element format, 16 bits for alpha, blue, green and red.
		EF_ABGR16UI = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 16, 16, 16, 16, ECT_UInt, ECT_UInt, ECT_UInt, ECT_UInt>::value,

		// 64-bit element format, 16 bits for signed alpha, blue, green and red.
		EF_ABGR16I = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 16, 16, 16, 16, ECT_SInt, ECT_SInt, ECT_SInt, ECT_SInt>::value,

		// 32-bit element format, 32 bits for red.
		EF_R32UI = MakeElementFormat1<EC_R, 32, ECT_UInt>::value,

		// 32-bit element format, 32 bits for signed red.
		EF_R32I = MakeElementFormat1<EC_R, 32, ECT_SInt>::value,

		// 64-bit element format, 16 bits for red and green.
		EF_GR32UI = MakeElementFormat2<EC_G, EC_R, 32, 32, ECT_UInt, ECT_UInt>::value,

		// 64-bit element format, 16 bits for signed red and green.
		EF_GR32I = MakeElementFormat2<EC_G, EC_R, 32, 32, ECT_SInt, ECT_SInt>::value,

		// 96-bit element format, 16 bits for alpha, blue, green and red.
		EF_BGR32UI = MakeElementFormat3<EC_B, EC_G, EC_R, 32, 32, 32, ECT_UInt, ECT_UInt, ECT_UInt>::value,

		// 96-bit element format, 16 bits for signed_alpha, blue, green and red.
		EF_BGR32I = MakeElementFormat3<EC_B, EC_G, EC_R, 32, 32, 32, ECT_SInt, ECT_SInt, ECT_SInt>::value,

		// 128-bit element format, 16 bits for alpha, blue, green and red.
		EF_ABGR32UI = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 32, 32, 32, 32, ECT_UInt, ECT_UInt, ECT_UInt, ECT_UInt>::value,

		// 128-bit element format, 16 bits for signed alpha, blue, green and red.
		EF_ABGR32I = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 32, 32, 32, 32, ECT_SInt, ECT_SInt, ECT_SInt, ECT_SInt>::value,

		// 16-bit element format, 16 bits floating-point for red.
		EF_R16F = MakeElementFormat1<EC_R, 16, ECT_Float>::value,

		// 32-bit element format, 16 bits floating-point for green and red.
		EF_GR16F = MakeElementFormat2<EC_G, EC_R, 16, 16, ECT_Float, ECT_Float>::value,

		// 32-bit element format, 11 bits floating-point for green and red, 10 bits floating-point for blue.
		EF_B10G11R11F = MakeElementFormat3<EC_B, EC_G, EC_R, 10, 11, 11, ECT_Float, ECT_Float, ECT_Float>::value,

		// 48-bit element format, 16 bits floating-point for blue, green and red.
		EF_BGR16F = MakeElementFormat3<EC_B, EC_G, EC_R, 16, 16, 16, ECT_Float, ECT_Float, ECT_Float>::value,

		// 64-bit element format, 16 bits floating-point for alpha, blue, green and red.
		EF_ABGR16F = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 16, 16, 16, 16, ECT_Float, ECT_Float, ECT_Float, ECT_Float>::value,

		// 32-bit element format, 32 bits floating-point for red.
		EF_R32F = MakeElementFormat1<EC_R, 32, ECT_Float>::value,

		// 64-bit element format, 32 bits floating-point for green and red.
		EF_GR32F = MakeElementFormat2<EC_G, EC_R, 32, 32, ECT_Float, ECT_Float>::value,

		// 96-bit element format, 32 bits floating-point for blue, green and red.
		EF_BGR32F = MakeElementFormat3<EC_B, EC_G, EC_R, 32, 32, 32, ECT_Float, ECT_Float, ECT_Float>::value,

		// 128-bit element format, 32 bits floating-point for alpha, blue, green and red.
		EF_ABGR32F = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 32, 32, 32, 32, ECT_Float, ECT_Float, ECT_Float, ECT_Float>::value,

		// BC1 compression element format, DXT1
		EF_BC1 = MakeElementFormat1<EC_BC, 1, ECT_UNorm>::value,

		// BC1 compression element format, signed DXT1
		EF_SIGNED_BC1 = MakeElementFormat1<EC_BC, 1, ECT_SNorm>::value,

		// BC2 compression element format, DXT3
		EF_BC2 = MakeElementFormat1<EC_BC, 2, ECT_UNorm>::value,

		// BC2 compression element format, signed DXT3
		EF_SIGNED_BC2 = MakeElementFormat1<EC_BC, 2, ECT_SNorm>::value,

		// BC3 compression element format, DXT5
		EF_BC3 = MakeElementFormat1<EC_BC, 3, ECT_UNorm>::value,

		// BC3 compression element format, signed DXT5
		EF_SIGNED_BC3 = MakeElementFormat1<EC_BC, 3, ECT_SNorm>::value,

		// BC4 compression element format, 1 channel
		EF_BC4 = MakeElementFormat1<EC_BC, 4, ECT_UNorm>::value,

		// BC4 compression element format, 1 channel signed
		EF_SIGNED_BC4 = MakeElementFormat1<EC_BC, 4, ECT_SNorm>::value,

		// BC5 compression element format, 2 channels
		EF_BC5 = MakeElementFormat1<EC_BC, 5, ECT_UNorm>::value,

		// BC5 compression element format, 2 channels signed
		EF_SIGNED_BC5 = MakeElementFormat1<EC_BC, 5, ECT_SNorm>::value,

		// BC6 compression element format, 3 channels
		EF_BC6 = MakeElementFormat1<EC_BC, 6, ECT_UNorm>::value,

		// BC6 compression element format, 3 channels
		EF_SIGNED_BC6 = MakeElementFormat1<EC_BC, 6, ECT_SNorm>::value,

		// BC7 compression element format, 3 channels
		EF_BC7 = MakeElementFormat1<EC_BC, 7, ECT_UNorm>::value,

		// 16-bit element format, 16 bits depth
		EF_D16 = MakeElementFormat1<EC_D, 16, ECT_UNorm>::value,

		// 32-bit element format, 24 bits depth and 8 bits stencil
		EF_D24S8 = MakeElementFormat2<EC_D, EC_S, 24, 8, ECT_UNorm, ECT_UInt>::value,

		// 32-bit element format, 32 bits depth
		EF_D32F = MakeElementFormat1<EC_D, 32, ECT_Float>::value,

		// 32-bit element format, 8 bits for alpha, red, green and blue. Standard RGB (gamma = 2.2).
		EF_ARGB8_SRGB = MakeElementFormat4<EC_A, EC_R, EC_G, EC_B, 8, 8, 8, 8, ECT_UNorm_SRGB, ECT_UNorm_SRGB, ECT_UNorm_SRGB, ECT_UNorm_SRGB>::value,

		// 32-bit element format, 8 bits for alpha, red, green and blue. Standard RGB (gamma = 2.2).
		EF_ABGR8_SRGB = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 8, 8, 8, 8, ECT_UNorm_SRGB, ECT_UNorm_SRGB, ECT_UNorm_SRGB, ECT_UNorm_SRGB>::value,

		// BC1 compression element format. Standard RGB (gamma = 2.2).
		EF_BC1_SRGB = MakeElementFormat1<EC_BC, 1, ECT_UNorm_SRGB>::value,

		// BC2 compression element format. Standard RGB (gamma = 2.2).
		EF_BC2_SRGB = MakeElementFormat1<EC_BC, 2, ECT_UNorm_SRGB>::value,

		// BC3 compression element format. Standard RGB (gamma = 2.2).
		EF_BC3_SRGB = MakeElementFormat1<EC_BC, 3, ECT_UNorm_SRGB>::value,

		// BC4 compression element format. Standard RGB (gamma = 2.2).
		EF_BC4_SRGB = MakeElementFormat1<EC_BC, 4, ECT_UNorm_SRGB>::value,

		// BC5 compression element format. Standard RGB (gamma = 2.2).
		EF_BC5_SRGB = MakeElementFormat1<EC_BC, 5, ECT_UNorm_SRGB>::value,

		// BC7 compression element format. Standard RGB (gamma = 2.2).
		EF_BC7_SRGB = MakeElementFormat1<EC_BC, 7, ECT_UNorm_SRGB>::value,
	};
#else
	typedef QWORD ElementFormat;

	// Unknown element format
	ElementFormat const EF_UnKnown = 0;

	// 8-bit element format, all bits alpha
	ElementFormat const EF_A8 = MakeElementFormat1<EC_A, 8, ECT_UNorm>::value;

	// 16-bit element format, 4 bits for alpha, red, green and blue.
	ElementFormat const EF_ARGB4 = MakeElementFormat4<EC_A, EC_R, EC_G, EC_B, 4, 4, 4, 4, ECT_UNorm, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value;

	// 8-bit element format, 8 bits for red.
	ElementFormat const EF_R8 = MakeElementFormat1<EC_R, 8, ECT_UNorm>::value;

	// 8-bit element format, 8 bits for signed red.
	ElementFormat const EF_SIGNED_R8 = MakeElementFormat1<EC_R, 8, ECT_SNorm>::value;

	// 16-bit element format, 8 bits for red, green.
	ElementFormat const EF_GR8 = MakeElementFormat2<EC_G, EC_R, 8, 8, ECT_UNorm, ECT_UNorm>::value;

	// 16-bit element format, 8 bits for signed red, green.
	ElementFormat const EF_SIGNED_GR8 = MakeElementFormat2<EC_G, EC_R, 8, 8, ECT_SNorm, ECT_SNorm>::value;

	// 24-bit element format, 8 bits for red, green and blue.
	ElementFormat const EF_BGR8 = MakeElementFormat3<EC_B, EC_G, EC_R, 8, 8, 8, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value;

	// 24-bit element format, 8 bits for signed red, green and blue.
	ElementFormat const EF_SIGNED_BGR8 = MakeElementFormat3<EC_B, EC_G, EC_R, 8, 8, 8, ECT_SNorm, ECT_SNorm, ECT_SNorm>::value;

	// 32-bit element format, 8 bits for alpha, red, green and blue.
	ElementFormat const EF_ARGB8 = MakeElementFormat4<EC_A, EC_R, EC_G, EC_B, 8, 8, 8, 8, ECT_UNorm, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value;

	// 32-bit element format, 8 bits for alpha, red, green and blue.
	ElementFormat const EF_ABGR8 = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 8, 8, 8, 8, ECT_UNorm, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value;

	// 32-bit element format, 8 bits for signed alpha, red, green and blue.
	ElementFormat const EF_SIGNED_ABGR8 = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 8, 8, 8, 8, ECT_SNorm, ECT_SNorm, ECT_SNorm, ECT_SNorm>::value;

	// 32-bit element format, 2 bits for alpha, 10 bits for red, green and blue.
	ElementFormat const EF_A2BGR10 = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 2, 10, 10, 10, ECT_UNorm, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value;

	// 32-bit element format, 2 bits for alpha, 10 bits for signed red, green and blue.
	ElementFormat const EF_SIGNED_A2BGR10 = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 2, 10, 10, 10, ECT_SNorm, ECT_SNorm, ECT_SNorm, ECT_SNorm>::value;


	// 32-bit element format, 8 bits for alpha, red, green and blue.
	ElementFormat const EF_R8UI = MakeElementFormat1<EC_R, 8, ECT_UInt>::value;

	// 32-bit element format, 8 bits for alpha, red, green and blue.
	ElementFormat const EF_R8I = MakeElementFormat1<EC_R, 8, ECT_SInt>::value;

	// 16-bit element format, 8 bits for red, green.
	ElementFormat const EF_GR8UI = MakeElementFormat2<EC_G, EC_R, 8, 8, ECT_UInt, ECT_UInt>::value;

	// 16-bit element format, 8 bits for red, green.
	ElementFormat const EF_GR8I = MakeElementFormat2<EC_G, EC_R, 8, 8, ECT_SInt, ECT_SInt>::value;

	// 24-bit element format, 8 bits for red, green and blue.
	ElementFormat const EF_BGR8UI = MakeElementFormat3<EC_B, EC_G, EC_R, 8, 8, 8, ECT_UInt, ECT_UInt, ECT_UInt>::value;

	// 24-bit element format, 8 bits for red, green and blue.
	ElementFormat const EF_BGR8I = MakeElementFormat3<EC_B, EC_G, EC_R, 8, 8, 8, ECT_SInt, ECT_SInt, ECT_SInt>::value;

	// 32-bit element format, 8 bits for alpha, red, green and blue.
	ElementFormat const EF_ABGR8UI = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 8, 8, 8, 8, ECT_UInt, ECT_UInt, ECT_UInt, ECT_UInt>::value;

	// 32-bit element format, 8 bits for signed alpha, red, green and blue.
	ElementFormat const EF_ABGR8I = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 8, 8, 8, 8, ECT_SInt, ECT_SInt, ECT_SInt, ECT_SInt>::value;

	// 32-bit element format, 2 bits for alpha, 10 bits for red, green and blue.
	ElementFormat const EF_A2BGR10UI = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 2, 10, 10, 10, ECT_UInt, ECT_UInt, ECT_UInt, ECT_UInt>::value;

	// 32-bit element format, 2 bits for alpha, 10 bits for red, green and blue.
	ElementFormat const EF_A2BGR10I = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 2, 10, 10, 10, ECT_SInt, ECT_SInt, ECT_SInt, ECT_SInt>::value;

	// 16-bit element format, 16 bits for red.
	ElementFormat const EF_R16 = MakeElementFormat1<EC_R, 16, ECT_UNorm>::value;

	// 16-bit element format, 16 bits for signed red.
	ElementFormat const EF_SIGNED_R16 = MakeElementFormat1<EC_R, 16, ECT_SNorm>::value;

	// 32-bit element format, 16 bits for red and green.
	ElementFormat const EF_GR16 = MakeElementFormat2<EC_G, EC_R, 16, 16, ECT_UNorm, ECT_UNorm>::value;

	// 32-bit element format, 16 bits for signed red and green.
	ElementFormat const EF_SIGNED_GR16 = MakeElementFormat2<EC_G, EC_R, 16, 16, ECT_SNorm, ECT_SNorm>::value;

	// 48-bit element format, 16 bits for alpha, blue, green and red.
	ElementFormat const EF_BGR16 = MakeElementFormat3<EC_B, EC_G, EC_R, 16, 16, 16, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value;

	// 48-bit element format, 16 bits for signed alpha, blue, green and red.
	ElementFormat const EF_SIGNED_BGR16 = MakeElementFormat3<EC_B, EC_G, EC_R, 16, 16, 16, ECT_SNorm, ECT_SNorm, ECT_SNorm>::value;

	// 64-bit element format, 16 bits for alpha, blue, green and red.
	ElementFormat const EF_ABGR16 = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 16, 16, 16, 16, ECT_UNorm, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value;

	// 64-bit element format, 16 bits for signed alpha, blue, green and red.
	ElementFormat const EF_SIGNED_ABGR16 = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 16, 16, 16, 16, ECT_SNorm, ECT_SNorm, ECT_SNorm, ECT_SNorm>::value;

	// 32-bit element format, 32 bits for red.
	ElementFormat const EF_R32 = MakeElementFormat1<EC_R, 32, ECT_UNorm>::value;

	// 32-bit element format, 32 bits for signed red.
	ElementFormat const EF_SIGNED_R32 = MakeElementFormat1<EC_R, 32, ECT_SNorm>::value;

	// 64-bit element format, 16 bits for red and green.
	ElementFormat const EF_GR32 = MakeElementFormat2<EC_G, EC_R, 32, 32, ECT_UNorm, ECT_UNorm>::value;

	// 64-bit element format, 16 bits for signed red and green.
	ElementFormat const EF_SIGNED_GR32 = MakeElementFormat2<EC_G, EC_R, 32, 32, ECT_SNorm, ECT_SNorm>::value;

	// 96-bit element format, 16 bits for alpha, blue, green and red.
	ElementFormat const EF_BGR32 = MakeElementFormat3<EC_B, EC_G, EC_R, 32, 32, 32, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value;

	// 96-bit element format, 16 bits for signed_alpha, blue, green and red.
	ElementFormat const EF_SIGNED_BGR32 = MakeElementFormat3<EC_B, EC_G, EC_R, 32, 32, 32, ECT_SNorm, ECT_SNorm, ECT_SNorm>::value;

	// 128-bit element format, 16 bits for alpha, blue, green and red.
	ElementFormat const EF_ABGR32 = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 32, 32, 32, 32, ECT_UNorm, ECT_UNorm, ECT_UNorm, ECT_UNorm>::value;

	// 128-bit element format, 16 bits for signed alpha, blue, green and red.
	ElementFormat const EF_SIGNED_ABGR32 = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 32, 32, 32, 32, ECT_SNorm, ECT_SNorm, ECT_SNorm, ECT_SNorm>::value;

	// 16-bit element format, 16 bits for red.
	ElementFormat const EF_R16UI = MakeElementFormat1<EC_R, 16, ECT_UInt>::value;

	// 16-bit element format, 16 bits for signed red.
	ElementFormat const EF_R16I = MakeElementFormat1<EC_R, 16, ECT_SInt>::value;

	// 32-bit element format, 16 bits for red and green.
	ElementFormat const EF_GR16UI = MakeElementFormat2<EC_G, EC_R, 16, 16, ECT_UInt, ECT_UInt>::value;

	// 32-bit element format, 16 bits for signed red and green.
	ElementFormat const EF_GR16I = MakeElementFormat2<EC_G, EC_R, 16, 16, ECT_SInt, ECT_SInt>::value;

	// 48-bit element format, 16 bits for alpha, blue, green and red.
	ElementFormat const EF_BGR16UI = MakeElementFormat3<EC_B, EC_G, EC_R, 16, 16, 16, ECT_UInt, ECT_UInt, ECT_UInt>::value;

	// 48-bit element format, 16 bits for signed alpha, blue, green and red.
	ElementFormat const EF_BGR16I = MakeElementFormat3<EC_B, EC_G, EC_R, 16, 16, 16, ECT_SInt, ECT_SInt, ECT_SInt>::value;

	// 64-bit element format, 16 bits for alpha, blue, green and red.
	ElementFormat const EF_ABGR16UI = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 16, 16, 16, 16, ECT_UInt, ECT_UInt, ECT_UInt, ECT_UInt>::value;

	// 64-bit element format, 16 bits for signed alpha, blue, green and red.
	ElementFormat const EF_ABGR16I = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 16, 16, 16, 16, ECT_SInt, ECT_SInt, ECT_SInt, ECT_SInt>::value;

	// 32-bit element format, 32 bits for red.
	ElementFormat const EF_R32UI = MakeElementFormat1<EC_R, 32, ECT_UInt>::value;

	// 32-bit element format, 32 bits for signed red.
	ElementFormat const EF_R32I = MakeElementFormat1<EC_R, 32, ECT_SInt>::value;

	// 64-bit element format, 16 bits for red and green.
	ElementFormat const EF_GR32UI = MakeElementFormat2<EC_G, EC_R, 32, 32, ECT_UInt, ECT_UInt>::value;

	// 64-bit element format, 16 bits for signed red and green.
	ElementFormat const EF_GR32I = MakeElementFormat2<EC_G, EC_R, 32, 32, ECT_SInt, ECT_SInt>::value;

	// 96-bit element format, 16 bits for alpha, blue, green and red.
	ElementFormat const EF_BGR32UI = MakeElementFormat3<EC_B, EC_G, EC_R, 32, 32, 32, ECT_UInt, ECT_UInt, ECT_UInt>::value;

	// 96-bit element format, 16 bits for signed_alpha, blue, green and red.
	ElementFormat const EF_BGR32I = MakeElementFormat3<EC_B, EC_G, EC_R, 32, 32, 32, ECT_SInt, ECT_SInt, ECT_SInt>::value;

	// 128-bit element format, 16 bits for alpha, blue, green and red.
	ElementFormat const EF_ABGR32UI = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 32, 32, 32, 32, ECT_UInt, ECT_UInt, ECT_UInt, ECT_UInt>::value;

	// 128-bit element format, 16 bits for signed alpha, blue, green and red.
	ElementFormat const EF_ABGR32I = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 32, 32, 32, 32, ECT_SInt, ECT_SInt, ECT_SInt, ECT_SInt>::value;

	// 16-bit element format, 16 bits floating-point for red.
	ElementFormat const EF_R16F = MakeElementFormat1<EC_R, 16, ECT_Float>::value;

	// 32-bit element format, 16 bits floating-point for green and red.
	ElementFormat const EF_GR16F = MakeElementFormat2<EC_G, EC_R, 16, 16, ECT_Float, ECT_Float>::value;

	// 32-bit element format, 11 bits floating-point for green and red, 10 bits floating-point for blue.
	ElementFormat const EF_B10G11R11F = MakeElementFormat3<EC_B, EC_G, EC_R, 10, 11, 11, ECT_Float, ECT_Float, ECT_Float>::value;

	// 48-bit element format, 16 bits floating-point for blue, green and red.
	ElementFormat const EF_BGR16F = MakeElementFormat3<EC_B, EC_G, EC_R, 16, 16, 16, ECT_Float, ECT_Float, ECT_Float>::value;

	// 64-bit element format, 16 bits floating-point for alpha, blue, green and red.
	ElementFormat const EF_ABGR16F = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 16, 16, 16, 16, ECT_Float, ECT_Float, ECT_Float, ECT_Float>::value;

	// 32-bit element format, 32 bits floating-point for red.
	ElementFormat const EF_R32F = MakeElementFormat1<EC_R, 32, ECT_Float>::value;

	// 64-bit element format, 32 bits floating-point for green and red.
	ElementFormat const EF_GR32F = MakeElementFormat2<EC_G, EC_R, 32, 32, ECT_Float, ECT_Float>::value;

	// 96-bit element format, 32 bits floating-point for blue, green and red.
	ElementFormat const EF_BGR32F = MakeElementFormat3<EC_B, EC_G, EC_R, 32, 32, 32, ECT_Float, ECT_Float, ECT_Float>::value;

	// 128-bit element format, 32 bits floating-point for alpha, blue, green and red.
	ElementFormat const EF_ABGR32F = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 32, 32, 32, 32, ECT_Float, ECT_Float, ECT_Float, ECT_Float>::value;

	// BC1 compression element format, DXT1
	ElementFormat const EF_BC1 = MakeElementFormat1<EC_BC, 1, ECT_UNorm>::value;

	// BC1 compression element format, signed DXT1
	ElementFormat const EF_SIGNED_BC1 = MakeElementFormat1<EC_BC, 1, ECT_SNorm>::value;

	// BC2 compression element format, DXT3
	ElementFormat const EF_BC2 = MakeElementFormat1<EC_BC, 2, ECT_UNorm>::value;

	// BC2 compression element format, signed DXT3
	ElementFormat const EF_SIGNED_BC2 = MakeElementFormat1<EC_BC, 2, ECT_SNorm>::value;

	// BC3 compression element format, DXT5
	ElementFormat const EF_BC3 = MakeElementFormat1<EC_BC, 3, ECT_UNorm>::value;

	// BC3 compression element format, signed DXT5
	ElementFormat const EF_SIGNED_BC3 = MakeElementFormat1<EC_BC, 3, ECT_SNorm>::value;

	// BC4 compression element format, 1 channel
	ElementFormat const EF_BC4 = MakeElementFormat1<EC_BC, 4, ECT_UNorm>::value;

	// BC4 compression element format, 1 channel signed
	ElementFormat const EF_SIGNED_BC4 = MakeElementFormat1<EC_BC, 4, ECT_SNorm>::value;

	// BC5 compression element format, 2 channels
	ElementFormat const EF_BC5 = MakeElementFormat1<EC_BC, 5, ECT_UNorm>::value;

	// BC5 compression element format, 2 channels signed
	ElementFormat const EF_SIGNED_BC5 = MakeElementFormat1<EC_BC, 5, ECT_SNorm>::value;

	// BC6 compression element format, 3 channels
	ElementFormat const EF_BC6 = MakeElementFormat1<EC_BC, 6, ECT_UNorm>::value;

	// BC6 compression element format, 3 channels
	ElementFormat const EF_SIGNED_BC6 = MakeElementFormat1<EC_BC, 6, ECT_SNorm>::value;

	// BC7 compression element format, 3 channels
	ElementFormat const EF_BC7 = MakeElementFormat1<EC_BC, 7, ECT_UNorm>::value;

	// 16-bit element format, 16 bits depth
	ElementFormat const EF_D16 = MakeElementFormat1<EC_D, 16, ECT_UNorm>::value;

	// 32-bit element format, 24 bits depth and 8 bits stencil
	ElementFormat const EF_D24S8 = MakeElementFormat2<EC_D, EC_S, 24, 8, ECT_UNorm, ECT_UInt>::value;

	// 32-bit element format, 32 bits depth
	ElementFormat const EF_D32F = MakeElementFormat1<EC_D, 32, ECT_Float>::value;

	// 32-bit element format, 8 bits for alpha, red, green and blue. Standard RGB (gamma = 2.2).
	ElementFormat const EF_ARGB8_SRGB = MakeElementFormat4<EC_A, EC_R, EC_G, EC_B, 8, 8, 8, 8, ECT_UNorm_SRGB, ECT_UNorm_SRGB, ECT_UNorm_SRGB, ECT_UNorm_SRGB>::value;

	// 32-bit element format, 8 bits for alpha, red, green and blue. Standard RGB (gamma = 2.2).
	ElementFormat const EF_ABGR8_SRGB = MakeElementFormat4<EC_A, EC_B, EC_G, EC_R, 8, 8, 8, 8, ECT_UNorm_SRGB, ECT_UNorm_SRGB, ECT_UNorm_SRGB, ECT_UNorm_SRGB>::value;

	// BC1 compression element format. Standard RGB (gamma = 2.2).
	ElementFormat const EF_BC1_SRGB = MakeElementFormat1<EC_BC, 1, ECT_UNorm_SRGB>::value;

	// BC2 compression element format. Standard RGB (gamma = 2.2).
	ElementFormat const EF_BC2_SRGB = MakeElementFormat1<EC_BC, 2, ECT_UNorm_SRGB>::value;

	// BC3 compression element format. Standard RGB (gamma = 2.2).
	ElementFormat const EF_BC3_SRGB = MakeElementFormat1<EC_BC, 3, ECT_UNorm_SRGB>::value;

	// BC4 compression element format. Standard RGB (gamma = 2.2).
	ElementFormat const EF_BC4_SRGB = MakeElementFormat1<EC_BC, 4, ECT_UNorm_SRGB>::value;

	// BC5 compression element format. Standard RGB (gamma = 2.2).
	ElementFormat const EF_BC5_SRGB = MakeElementFormat1<EC_BC, 5, ECT_UNorm_SRGB>::value;

	// BC7 compression element format. Standard RGB (gamma = 2.2).
	ElementFormat const EF_BC7_SRGB = MakeElementFormat1<EC_BC, 7, ECT_UNorm_SRGB>::value;
#endif

	template<int C>
	INLINE ElementChannel Channel(ElementFormat format)
	{
		return static_cast<ElementChannel>((format>>(4*C)) & 0xF);
	}

	template<int C>
	INLINE BYTE ChannelBits( ElementFormat format)
	{
		return ( format>>(16+6*C)) & 0x3F;
	}

	template<int C>
	INLINE ElementChannelType ChannelType( ElementFormat ef)
	{
		return static_cast<ElementChannelType>( (ef>>(40+4*C)) & 0xF);
	}

	template<int C>
	INLINE ElementFormat ChannelType( ElementFormat ef, ElementChannelType ct)
	{
		QWORD ef64 = static_cast<QWORD>( ef);
		ef64 &= ~(0xFULL << (40 + 4 * C));
		ef64 |= (static_cast<QWORD>(ct) << (40 + 4 * C));

		return static_cast<ElementFormat>( ef64);
	}

	// Is FloatFormat
	INLINE bool IsFloatFormat( ElementFormat format)
	{
		return ( ECT_Float == ChannelType<0>(format));
	}

	// Is compressedFormat
	INLINE bool IsCompressedFormat( ElementFormat format)
	{
		return ( EC_BC == Channel<0>(format));
	}

	// Is depth format
	INLINE bool IsDepthFormat( ElementFormat format)
	{
		return (EC_D == Channel<0>(format));
	}

	// Is Stencil format
	INLINE bool IsStencilFormat( ElementFormat format)
	{
		return (EC_S == Channel<1>(format));
	}

	// Is SRGB
	INLINE bool IsSRGB( ElementFormat format)
	{
		return ( ECT_UNorm_SRGB == Channel<0>(format));
	}

	// NumComponents
	INLINE UINT NumComponents( ElementFormat format)
	{
		if( IsCompressedFormat( format))
		{
			switch( ChannelBits<0>(format))
			{
			case 1:
			case 2:
			case 3:
				return 4;

			case 4:
				return 1;

			case 5:
				return 2;

			default:
				A_ASSERT( FALSE);
				return 0;
			}
		}
		else
		{
			return (ChannelBits<0>(format)!=0) + (ChannelBits<1>(format)!=0) + (ChannelBits<2>(format)!=0) + (ChannelBits<3>(format)!=0);
		}
	}

	// NumDepthBits
	INLINE BYTE NumDepthBits( ElementFormat format)
	{
		if( EC_D == Channel<0>(format))
		{
			return ChannelBits<0>(format);
		}

		return 0;
	}

	// NumStencilBits
	INLINE BYTE NumStencilBits( ElementFormat format)
	{
		if( EC_S == Channel<1>(format))
		{
			return ChannelBits<1>(format);
		}
		
		return 0;
	}

	// 格式位数
	INLINE BYTE NumFormatBits( ElementFormat format)
	{
		if (IsCompressedFormat(format))
		{
			switch ( ChannelBits<0>(format))
			{
			case 1:
			case 4:
				return 16;

			case 2:
			case 3:
			case 5:
				return 32;

			default:
				assert(false);
				return 0;
			}
		}
		else
		{
			return ChannelBits<0>(format) + ChannelBits<1>(format) + ChannelBits<2>(format) + ChannelBits<3>(format);
		}
	}

	// 格式字节数
	INLINE BYTE NumFormatBytes( ElementFormat format)
	{
		return NumFormatBits( format) / 8;
	}

	// MakeSRGB
	INLINE ElementFormat MakeSRGB( ElementFormat format)
	{
		if( ECT_UNorm == ChannelType<0>(format))
			format = ChannelType<0>( format, ECT_UNorm_SRGB);
		
		if( !IsCompressedFormat( format))
		{
			if( ECT_UNorm == ChannelType<1>(format))
				format = ChannelType<1>( format, ECT_UNorm_SRGB);

			if( ECT_UNorm == ChannelType<2>(format))
				format = ChannelType<2>( format, ECT_UNorm_SRGB);

			if( ECT_UNorm == ChannelType<3>(format))
				format = ChannelType<3>( format, ECT_UNorm_SRGB);
		}

		return format;
	}

	// InitData
	struct ElementInitData
	{
		const void*  m_data;
		UINT		 m_rowPitch;
		UINT		 m_slicePitch;
	};

	// element access hint
	enum ElementAccessHint
	{
		EAH_CPURead			= 1UL << 0,
		EAH_CPUWrite		= 1UL << 1,
		EAH_GPURead			= 1UL << 2,
		EAH_GPUWrite		= 1UL << 3,
		EAH_GPUUnordered	= 1UL << 4,
		EAH_GPUStructured	= 1UL << 5,
		EAH_GenerateMips	= 1UL << 6,
		EAH_Immutable		= 1UL << 7
	};
}