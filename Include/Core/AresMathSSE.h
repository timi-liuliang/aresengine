//------------------------------------------------------------------
// AresMathSSE.h Intel SSE specific vector intrinsics
//												--2012/1/6
//------------------------------------------------------------------
#pragma once

#include <mmintrin.h>

namespace Ares
{
	// float4 vector register type, where the first float(X) is stored in the lowest 32 bits, and so on
	typedef __m128	VectorRegister;

	// Returns a vector based on 4 FLOATs.
	//
	// @param X		1st FLOAT component
	// @param Y		2nd FLOAT component
	// @param Z		3rd FLOAT component
	// @param W		4th FLOAT component
	// @return		Vector of the 4 FLOATs
	FORCEINLINE VectorRegister MakeVectorRegister( FLOAT X, FLOAT Y, FLOAT Z, FLOAT W )
	{
		return _mm_setr_ps( X, Y, Z, W );
	}

	// @param A0	Selects which element (0-3) from 'A' into 1st slot in the result
	// @param A1	Selects which element (0-3) from 'A' into 2nd slot in the result
	// @param B2	Selects which element (0-3) from 'B' into 3rd slot in the result
	// @param B3	Selects which element (0-3) from 'B' into 4th slot in the resul
	#define SHUFFLEMASK(A0,A1,B2,B3) ( (A0) | ((A1)<<2) | ((B2)<<4) | ((B3)<<6) )

	// Subtracts a vector from another (component-wise) and returns the result.
	//
	// @param Vec1	1st vector
	// @param Vec2	2nd vector
	// @return		VectorRegister( Vec1.x-Vec2.x, Vec1.y-Vec2.y, Vec1.z-Vec2.z, Vec1.w-Vec2.w )
	#define VectorSubtract( Vec1, Vec2 )	_mm_sub_ps( Vec1, Vec2 )

	// Multiplies two vectors (component-wise) and returns the result.
	//
	// @param Vec1	1st vector
	// @param Vec2	2nd vector
	// @return		VectorRegister( Vec1.x*Vec2.x, Vec1.y*Vec2.y, Vec1.z*Vec2.z, Vec1.w*Vec2.w )
	#define VectorMultiply( Vec1, Vec2 )	_mm_mul_ps( Vec1, Vec2 )

	// Multiplies two vectors (component-wise), adds in the third vector and returns the result.
	//
	// @param Vec1	1st vector
	// @param Vec2	2nd vector
	// @param Vec3	3rd vector
	// @return		VectorRegister( Vec1.x*Vec2.x + Vec3.x, Vec1.y*Vec2.y + Vec3.y, Vec1.z*Vec2.z + Vec3.z, Vec1.w*Vec2.w + Vec3.w )
	#define VectorMultiplyAdd( Vec1, Vec2, Vec3 )	_mm_add_ps( _mm_mul_ps(Vec1, Vec2), Vec3 )

	// Replicates one element into all four elements and returns the new vector.
	//
	// @param Vec			Source vector
	// @param ElementIndex	Index (0-3) of the element to replicate
	// @return				VectorRegister( Vec[ElementIndex], Vec[ElementIndex], Vec[ElementIndex], Vec[ElementIndex] )
	#define VectorReplicate( Vec, ElementIndex )	_mm_shuffle_ps( Vec, Vec, SHUFFLEMASK(ElementIndex,ElementIndex,ElementIndex,ElementIndex) )

	// Calculates the cross product of two vectors (XYZ components). W is set to 0.
	//
	// @param Vec1	1st vector
	// @param Vec2	2nd vector
	// @return cross(Vec1.xyz, Vec2.xyz). W is set to 0.
	FORCEINLINE VectorRegister VectorCross( const VectorRegister& Vec1, const VectorRegister& Vec2 )
	{
		VectorRegister A_YZXW = _mm_shuffle_ps( Vec1, Vec1, SHUFFLEMASK(1,2,0,3) );
		VectorRegister B_ZXYW = _mm_shuffle_ps( Vec2, Vec2, SHUFFLEMASK(2,0,1,3) );
		VectorRegister A_ZXYW = _mm_shuffle_ps( Vec1, Vec1, SHUFFLEMASK(2,0,1,3) );
		VectorRegister B_YZXW = _mm_shuffle_ps( Vec2, Vec2, SHUFFLEMASK(1,2,0,3) );
		return VectorSubtract( VectorMultiply(A_YZXW,B_ZXYW), VectorMultiply(A_ZXYW, B_YZXW) );
	}

	// Multiplies two 4x4 matrices
	//
	// @param Result	Pointer to where the result should be stored
	// @param Matrix1	Pointer to the first matrix
	// @param Matrix2	Pointer to the second matrix 
	FORCEINLINE void VectorMatrixMultiply( void* result, const void* matrix1, const void* matrix2)
	{
		const VectorRegister* A	= (const VectorRegister*) matrix1;
		const VectorRegister* B	= (const VectorRegister*) matrix2;
		VectorRegister*		  R	= (VectorRegister*) result;
		VectorRegister Temp, R0, R1, R2, R3;

		// First row of result (Matrix1[0] * Matrix2).
		Temp	= VectorMultiply( VectorReplicate( A[0], 0 ), B[0] );
		Temp	= VectorMultiplyAdd( VectorReplicate( A[0], 1 ), B[1], Temp );
		Temp	= VectorMultiplyAdd( VectorReplicate( A[0], 2 ), B[2], Temp );
		R0		= VectorMultiplyAdd( VectorReplicate( A[0], 3 ), B[3], Temp );

		// Second row of result (Matrix1[1] * Matrix2).
		Temp	= VectorMultiply( VectorReplicate( A[1], 0 ), B[0] );
		Temp	= VectorMultiplyAdd( VectorReplicate( A[1], 1 ), B[1], Temp );
		Temp	= VectorMultiplyAdd( VectorReplicate( A[1], 2 ), B[2], Temp );
		R1		= VectorMultiplyAdd( VectorReplicate( A[1], 3 ), B[3], Temp );

		// Third row of result (Matrix1[2] * Matrix2).
		Temp	= VectorMultiply( VectorReplicate( A[2], 0 ), B[0] );
		Temp	= VectorMultiplyAdd( VectorReplicate( A[2], 1 ), B[1], Temp );
		Temp	= VectorMultiplyAdd( VectorReplicate( A[2], 2 ), B[2], Temp );
		R2		= VectorMultiplyAdd( VectorReplicate( A[2], 3 ), B[3], Temp );

		// Fourth row of result (Matrix1[3] * Matrix2).
		Temp	= VectorMultiply( VectorReplicate( A[3], 0 ), B[0] );
		Temp	= VectorMultiplyAdd( VectorReplicate( A[3], 1 ), B[1], Temp );
		Temp	= VectorMultiplyAdd( VectorReplicate( A[3], 2 ), B[2], Temp );
		R3		= VectorMultiplyAdd( VectorReplicate( A[3], 3 ), B[3], Temp );

		// Store result
		R[0] = R0;
		R[1] = R1;
		R[2] = R2;
		R[3] = R3;
	}

	// Calculate the inverse of an Matrix
	FORCEINLINE void VectorMatrixInverse( void* result, const void* srcMatrix)
	{
		FLOAT *des           = (FLOAT*)result;
		FLOAT *src			 = (FLOAT*)srcMatrix;
		VectorRegister* vrSrc= (VectorRegister*)srcMatrix;

		VectorRegister minor0, minor1, minor2, minor3;
		VectorRegister det;

		// fool compiler only..
		VectorRegister tmp1 = *(vrSrc);
		VectorRegister row0 = *(vrSrc);
		VectorRegister row1 = *(vrSrc+1);
		VectorRegister row2 = *(vrSrc+2);
		VectorRegister row3 = *(vrSrc+3);

		tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(src)), (__m64*)(src+ 4));
		row1 = _mm_loadh_pi(_mm_loadl_pi(row1, (__m64*)(src+8)), (__m64*)(src+12));
		row0 = _mm_shuffle_ps(tmp1, row1, 0x88);
		row1 = _mm_shuffle_ps(row1, tmp1, 0xDD);
		tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(src+ 2)), (__m64*)(src+ 6));
		row3 = _mm_loadh_pi(_mm_loadl_pi(row3, (__m64*)(src+10)), (__m64*)(src+14));
		row2 = _mm_shuffle_ps(tmp1, row3, 0x88);
		row3 = _mm_shuffle_ps(row3, tmp1, 0xDD);

		// -----------------------------------------------
		tmp1 = _mm_mul_ps(row2, row3);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		minor0 = _mm_mul_ps(row1, tmp1);
		minor1 = _mm_mul_ps(row0, tmp1);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp1), minor0);
		minor1 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor1);
		minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);
		// -----------------------------------------------
		tmp1 = _mm_mul_ps(row1, row2);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		minor0 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor0);
		minor3 = _mm_mul_ps(row0, tmp1);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp1));
		minor3 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor3);
		minor3 = _mm_shuffle_ps(minor3, minor3, 0x4E);
		// -----------------------------------------------	
		tmp1 = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		row2 = _mm_shuffle_ps(row2, row2, 0x4E);
		minor0 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor0);
		minor2 = _mm_mul_ps(row0, tmp1);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp1));
		minor2 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor2);
		minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);
		// -----------------------------------------------
		tmp1 = _mm_mul_ps(row0, row1);	
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		minor2 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor2);
		minor3 = _mm_sub_ps(_mm_mul_ps(row2, tmp1), minor3);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor2 = _mm_sub_ps(_mm_mul_ps(row3, tmp1), minor2);
		minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp1));
		// -----------------------------------------------
		tmp1 = _mm_mul_ps(row0, row3);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp1));
		minor2 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor2);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor1 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor1);
		minor2 = _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp1));
		// -----------------------------------------------
		tmp1 = _mm_mul_ps(row0, row2);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		minor1 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor1);
		minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp1));
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp1));
		minor3 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor3);
		// -----------------------------------------------
		det = _mm_mul_ps(row0, minor0);
		det = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
		det = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);
		tmp1 = _mm_rcp_ss(det);
		det = _mm_sub_ss(_mm_add_ss(tmp1, tmp1), _mm_mul_ss(det, _mm_mul_ss(tmp1, tmp1)));
		det = _mm_shuffle_ps(det, det, 0x00);
		minor0 = _mm_mul_ps(det, minor0);
		_mm_storel_pi((__m64*)(des), minor0);
		_mm_storeh_pi((__m64*)(des+2), minor0);
		minor1 = _mm_mul_ps(det, minor1);
		_mm_storel_pi((__m64*)(des+4), minor1);
		_mm_storeh_pi((__m64*)(des+6), minor1);
		minor2 = _mm_mul_ps(det, minor2);
		_mm_storel_pi((__m64*)(des+ 8), minor2);
		_mm_storeh_pi((__m64*)(des+10), minor2);
		minor3 = _mm_mul_ps(det, minor3);
		_mm_storel_pi((__m64*)(des+12), minor3);
		_mm_storeh_pi((__m64*)(des+14), minor3);

		//return *this;
	}
}