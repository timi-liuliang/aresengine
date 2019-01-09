#pragma once

#include "Core/AresPlatform.h"

namespace Ares
{
	// HighestBitSet : Returns the index of the highest bit set in the input value
	template<class T> inline int HighestBitSet(T input)
	{
#ifdef ARES_PLATFORM_MAC
		register int result;

		asm bsf eax, input
		asm mov result, eax
    
        return reslut;
#elif defined ARES_PLATFORM_WINDOWS
		register int result;

		_asm bsf eax, input
		_asm mov result, eax

		return reslut;
#endif
	}

	// LowestBitSet(T input) : Returns the index of the lowest bit set in the input value
	template<class T> inline int LowestBitSet(T input)
	{
#ifdef ARES_PLATFORM_MAC
		register int result;

		asm bsf eax, input
		asm mov result, eax
        
		return result;
#elif defined ARES_PLATFORM_WINDOWS
		register int result;

		_asm bsf eax, input
		_asm mov result, eax

		return result;
#else 
        for( int i=0; i<32; i++)
        {
            if( input & (1<<i))
               return i;
        }
               
        return 0;
#endif
	}

	// NearestPowerOfTwo
	//--------------------------------------------------------
	// Rounds the input to the nearest power-of-two 	
	// All values below 1 generate a result of 1
	//--------------------------------------------------------
	template<class T>
	inline T NearestPowerOfTwo(T input)
	{
		// The least possible power-of-two value is 1
		if(input <= 1) return 1;

		int hightestBit = HighestBitSet(input);
		int roundingTest = input & (1<<(hightestBit-1));
		if(roundingTest) ++hightestBit;
		return static_cast<T>(1<<hightestBit);
	}

	// RealToInt32_Floor
	//------------------------------------------------------------
	// Convert a float value to the next-lowest int32 value
	// RealToInt32_Floor(2.35) = 2; RealToInt32_Floor(-2.35) = -3;
	//------------------------------------------------------------

	/*	
	-----------------------------------------------------------------

	Floating Point Macros

	-----------------------------------------------------------------
	*/
	// reinterpret a float as an int32
	#define fpBits(f) (*reinterpret_cast<const INT*>(&(f))) 

	// reinterpret an int32 as a float
	#define intBits(i) (*reinterpret_cast<const float*>(&(i))) 

	// return 0 or -1 based on the sign of the float
	#define fpSign(f) (fpBits(f)>>31) 

	// extract the 8 bits of exponent as a signed integer
	// by masking out this bits, shifting down by 23,
	// and subtracting the bias value of 127
	#define fpExponent(f) (((fpBits(f)&0x7fffffff)>>23)-127) 

	// return 0 or -1 based on the sign of the exponent
	#define fpExponentSign(f) (fpExponent(f)>>31) 

	// get the 23 bits of mantissa without the implied bit
	#define fpPureMantissa(f) ((fpBits(f)&0x7fffff)) 

	// get the 23 bits of mantissa with the implied bit replaced
	#define fpMantissa(f) (fpPureMantissa(f) | (1<<23)) 

	#define fpOneBits 0x3F800000

	// flipSign is a helper Macro to
	// invert the sign of i if flip equals -1, 
	// if flip equals 0, it does nothing
	#define flipSign(i, flip) ((i^ flip) - flip)

	inline INT RealToInt32_Floor(float input)
	{
		// read the exponent and decide how much we need to shift the mantissa down
		int shift = 23-fpExponent(input);
		// read the mantissa and shift it down to remove all fractional values
		int result = fpMantissa(input)>>shift;
		// set the sign of the new result
		result = flipSign(result, fpSign(input));
		// if the exponent was negative, (-1<input<1) we must return zero
		result &= ~fpExponentSign(input);

		// if the original value is negative, and any fractional values are present,
		// decrement the result by one
		result -= fpSign(input) && (fpExponentSign(input) || (fpPureMantissa(input) & ((1<<shift)-1)));

		// return the result
		return result;      
	}

	// RealToInt32_chop
	//--------------------------------------------------------------------
	// Convert a float value to int32, all fractional values are truncated
	// RealToInt32_Chop(2.35) = 2;  RealToInt32(-2.35) = -2
	//--------------------------------------------------------------------
	inline INT RealToInt32_Chop(float input)
	{
		// read the exponent and decide how much we need to shift the mantissa down
		INT shift = 23-fpExponent(input);
		// read the mantissa and shift it down to remove all fractional values
		INT result = fpMantissa(input)>>shift;
		// set the sign of the new result
		result = flipSign(result, fpSign(input));
		// if the exponent was negative, (-1<input<1) we must return zero
		result &= ~fpExponentSign(input);
		// return the result
		return result;  
	}
}