//---------------------------------------------------------------------------
//	AresXcodeApple.h : Ares definitions for Xcode running under Apple
//	Copyright 2012-2015	 µ€¡÷ All Rights Reserved
//---------------------------------------------------------------------------
#ifndef ARES_XCODE_APPLE
#define ARES_XCODE_APPLE

//#if !_WINDOWS_
//#error _WINDOWS not defined
//#endif
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <assert.h>
#include <stdlib.h>
#include <cmath>
#include <boost/cstdint.hpp>

// Function type macros
#define FORCEINLINE	inline          // Force code to be inline
#define INLINE		inline			// inline	

// Unsigned base types
typedef unsigned char	 BYTE;		// 8-bit unsigned
typedef unsigned char    UINT8;     // 8-bit unsigned
typedef boost::uint16_t  WORD;		// 16-bit unsigned
typedef boost::uint32_t  UINT;		// 32-bit unsigned
typedef boost::uint32_t  UINT32;    // 32-bit unsigned
typedef unsigned long    DWORD;		// 32-bit unsigned
typedef boost::uint64_t  QWORD;		// 64-bit unsigned

// Signed base types
typedef signed  char	 SBYTE;
typedef boost::int32_t	 INT;		// 32-bit signed
typedef long			 LONG;		// defined in windows.h

// Other base types
typedef signed char	BOOL;			// Boolean 0 (false) or 1(true)
typedef UINT        UBOOL;			// Boolean 0 (false) or 1(true)
typedef float       FLOAT;			// 32-bit IEEE floating point
typedef double      DOUBLE;			// 64-bit IEEE double.

// The SCALAR type abstracts floating point numbers, to easily
// swith between double and single floating point precision
#if defined(USE_DOUBLE_PRECISION)
typedef double  SCALAR;
#else
typedef float	SCALAR;
#endif

// Bitfield type
typedef unsigned long	BITFIELD;	// For bitfields

// Standard constants
#undef FALSE
#undef TRUE
#undef NULL

#define FALSE    0
#define TRUE     1
#define NULL     0
#define INVALID -1
#define FLT_MAX  3.4e38f

// Alignment
#define MS_ALIGN(n) __declspec(align(n))

#define ARES_PLATFORM_APPLE   1
#define ARES_PLATFORM_IOS     1
#define ARES_EDITOR_MODE      1

#define CORE_EXPORT

#if _MSC_VER || __INTEL_COMPILER
#define __HAS_SSE__ 1
#endif

// Enable vector intrinsics if possible
#ifndef ENABLE_VECTORINTRINSICS
#if ( PS3 || XBOX || ( __HAS_SSE__ && !__GNUC__ && !defined( __cplusplus_cli ) && !__INTEL_COMPILER ) )
#if !_WIN64		// @todo win64: Enable this after we've updated MMX code to SSE2 where appropriate
/** Turning this on enables the cross-platform intrinsic code path in the engine. */
#define ENABLE_VECTORINTRINSICS 0
#endif
#endif
#endif

#pragma warning(disable: 4251 4275 4819 4996)

#ifdef _DEBUG
#define ARES_DEBUG 1
#else
#define ARES_DEBUG 0
#endif

#if ARES_DEBUG
#define _CRTDBG_MAP_ALLOC
#define new_ new(_NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define new_ new
#endif

//------------------------
// ∫Í∂®“Â
//------------------------
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_VECTOR
#define SAFE_DELETE_VECTOR(v) {	for ( size_t i=0; i<v.size(); i++) SAFE_DELETE( v[i]); v.clear();}
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif
#ifndef SAFE_FREE
#define SAFE_FREE(p)      { if(p) { free(p); (p)=NULL; } }
#endif

// breakable statement, used to allow break call inside parenthesis.
#define	breakable switch(1) case 1: default:

#define A_ASSERT(expr)    assert(expr)
#define V_ASSERT(expr)   { assert(expr); }

#define GET_VARARGS_RESULT( result, len, fmt) { va_list args; va_start(args, fmt); _vsnprintf( result, len, fmt, args); va_end(args); }

#define s_ptr	boost::shared_ptr

#ifdef _DEBUG
#define ARES_DEBUG_MODE 1
#else
#define ARES_DEBUG_MODE 0
#endif

#define SUCCEEDED(hr)   (((HRESULT)(hr)) >= 0)
#define FAILED(hr)      (((HRESULT)(hr)) < 0)

#define S_OK			((HRESULT)0L)

// MathLibrary
FORCEINLINE FLOAT appTan( FLOAT value) { return tanf(value); }

//
// MSM: Fast float square root using SSE
// Accurate to within 1 LSB
//
FORCEINLINE FLOAT appSqrt( FLOAT f)
{
	// DB: crt's sqrt is ~60% faster than the below code and is defined for near-zero values.
	return sqrt( f);
}

//
// MSM: Fast float inverse square root using SSE
// Accurate to within 1 LSB
FORCEINLINE FLOAT appInvSqrt( FLOAT f)
{
	return 1.0f / sqrtf( f );
}

#endif

