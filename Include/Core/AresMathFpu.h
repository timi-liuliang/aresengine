#pragma once

#include "AresPlatform.h"

namespace Ares
{
	// Multiplies two 4x4 matrices.
	//
	// @param Result	Pointer to where the result should be stored
	// @param Matrix1	Pointer to the first matrix
	// @param Matrix2	Pointer to the second matrix
	FORCEINLINE void VectorMatrixMultiply( void* result, const void* matrix1, const void* matrix2)
	{
		typedef float Float4x4[4][4];
		const Float4x4& A = *((const Float4x4*) matrix1);
		const Float4x4& B = *((const Float4x4*) matrix2);
		Float4x4 Temp;

		Temp[0][0] = A[0][0] * B[0][0] + A[0][1] * B[1][0] + A[0][2] * B[2][0] + A[0][3] * B[3][0];
		Temp[0][1] = A[0][0] * B[0][1] + A[0][1] * B[1][1] + A[0][2] * B[2][1] + A[0][3] * B[3][1];
		Temp[0][2] = A[0][0] * B[0][2] + A[0][1] * B[1][2] + A[0][2] * B[2][2] + A[0][3] * B[3][2];
		Temp[0][3] = A[0][0] * B[0][3] + A[0][1] * B[1][3] + A[0][2] * B[2][3] + A[0][3] * B[3][3];

		Temp[1][0] = A[1][0] * B[0][0] + A[1][1] * B[1][0] + A[1][2] * B[2][0] + A[1][3] * B[3][0];
		Temp[1][1] = A[1][0] * B[0][1] + A[1][1] * B[1][1] + A[1][2] * B[2][1] + A[1][3] * B[3][1];
		Temp[1][2] = A[1][0] * B[0][2] + A[1][1] * B[1][2] + A[1][2] * B[2][2] + A[1][3] * B[3][2];
		Temp[1][3] = A[1][0] * B[0][3] + A[1][1] * B[1][3] + A[1][2] * B[2][3] + A[1][3] * B[3][3];

		Temp[2][0] = A[2][0] * B[0][0] + A[2][1] * B[1][0] + A[2][2] * B[2][0] + A[2][3] * B[3][0];
		Temp[2][1] = A[2][0] * B[0][1] + A[2][1] * B[1][1] + A[2][2] * B[2][1] + A[2][3] * B[3][1];
		Temp[2][2] = A[2][0] * B[0][2] + A[2][1] * B[1][2] + A[2][2] * B[2][2] + A[2][3] * B[3][2];
		Temp[2][3] = A[2][0] * B[0][3] + A[2][1] * B[1][3] + A[2][2] * B[2][3] + A[2][3] * B[3][3];

		Temp[3][0] = A[3][0] * B[0][0] + A[3][1] * B[1][0] + A[3][2] * B[2][0] + A[3][3] * B[3][0];
		Temp[3][1] = A[3][0] * B[0][1] + A[3][1] * B[1][1] + A[3][2] * B[2][1] + A[3][3] * B[3][1];
		Temp[3][2] = A[3][0] * B[0][2] + A[3][1] * B[1][2] + A[3][2] * B[2][2] + A[3][3] * B[3][2];
		Temp[3][3] = A[3][0] * B[0][3] + A[3][1] * B[1][3] + A[3][2] * B[2][3] + A[3][3] * B[3][3];

		memcpy( result, &Temp, 16*sizeof(FLOAT) );
	}

	//
	// Calculate the inverse of an FMatrix.
	//
	// @param DstMatrix		FMatrix pointer to where the result should be stored
	// @param SrcMatrix		FMatrix pointer to the Matrix to be inversed
	FORCEINLINE void VectorMatrixInverse(void* result, const void* srcMatrix)
	{
		typedef FLOAT Float4x4[4][4];
		const Float4x4& M = *((const Float4x4*) srcMatrix);
		Float4x4		Result;
		FLOAT			Det[4];
		Float4x4		Tmp;

		Tmp[0][0]	= M[2][2] * M[3][3] - M[2][3] * M[3][2];
		Tmp[0][1]	= M[1][2] * M[3][3] - M[1][3] * M[3][2];
		Tmp[0][2]	= M[1][2] * M[2][3] - M[1][3] * M[2][2];

		Tmp[1][0]	= M[2][2] * M[3][3] - M[2][3] * M[3][2];
		Tmp[1][1]	= M[0][2] * M[3][3] - M[0][3] * M[3][2];
		Tmp[1][2]	= M[0][2] * M[2][3] - M[0][3] * M[2][2];

		Tmp[2][0]	= M[1][2] * M[3][3] - M[1][3] * M[3][2];
		Tmp[2][1]	= M[0][2] * M[3][3] - M[0][3] * M[3][2];
		Tmp[2][2]	= M[0][2] * M[1][3] - M[0][3] * M[1][2];

		Tmp[3][0]	= M[1][2] * M[2][3] - M[1][3] * M[2][2];
		Tmp[3][1]	= M[0][2] * M[2][3] - M[0][3] * M[2][2];
		Tmp[3][2]	= M[0][2] * M[1][3] - M[0][3] * M[1][2];

		Det[0]		= M[1][1]*Tmp[0][0] - M[2][1]*Tmp[0][1] + M[3][1]*Tmp[0][2];
		Det[1]		= M[0][1]*Tmp[1][0] - M[2][1]*Tmp[1][1] + M[3][1]*Tmp[1][2];
		Det[2]		= M[0][1]*Tmp[2][0] - M[1][1]*Tmp[2][1] + M[3][1]*Tmp[2][2];
		Det[3]		= M[0][1]*Tmp[3][0] - M[1][1]*Tmp[3][1] + M[2][1]*Tmp[3][2];

		FLOAT Determinant = M[0][0]*Det[0] - M[1][0]*Det[1] + M[2][0]*Det[2] - M[3][0]*Det[3];
		const FLOAT	RDet = 1.0f / Determinant;

		Result[0][0] =  RDet * Det[0];
		Result[0][1] = -RDet * Det[1];
		Result[0][2] =  RDet * Det[2];
		Result[0][3] = -RDet * Det[3];
		Result[1][0] = -RDet * (M[1][0]*Tmp[0][0] - M[2][0]*Tmp[0][1] + M[3][0]*Tmp[0][2]);
		Result[1][1] =  RDet * (M[0][0]*Tmp[1][0] - M[2][0]*Tmp[1][1] + M[3][0]*Tmp[1][2]);
		Result[1][2] = -RDet * (M[0][0]*Tmp[2][0] - M[1][0]*Tmp[2][1] + M[3][0]*Tmp[2][2]);
		Result[1][3] =  RDet * (M[0][0]*Tmp[3][0] - M[1][0]*Tmp[3][1] + M[2][0]*Tmp[3][2]);
		Result[2][0] =  RDet * (
						M[1][0] * (M[2][1] * M[3][3] - M[2][3] * M[3][1]) -
						M[2][0] * (M[1][1] * M[3][3] - M[1][3] * M[3][1]) +
						M[3][0] * (M[1][1] * M[2][3] - M[1][3] * M[2][1])
					);
		Result[2][1] = -RDet * (
						M[0][0] * (M[2][1] * M[3][3] - M[2][3] * M[3][1]) -
						M[2][0] * (M[0][1] * M[3][3] - M[0][3] * M[3][1]) +
						M[3][0] * (M[0][1] * M[2][3] - M[0][3] * M[2][1])
					);
		Result[2][2] =  RDet * (
						M[0][0] * (M[1][1] * M[3][3] - M[1][3] * M[3][1]) -
						M[1][0] * (M[0][1] * M[3][3] - M[0][3] * M[3][1]) +
						M[3][0] * (M[0][1] * M[1][3] - M[0][3] * M[1][1])
					);
		Result[2][3] = -RDet * (
						M[0][0] * (M[1][1] * M[2][3] - M[1][3] * M[2][1]) -
						M[1][0] * (M[0][1] * M[2][3] - M[0][3] * M[2][1]) +
						M[2][0] * (M[0][1] * M[1][3] - M[0][3] * M[1][1])
					);
		Result[3][0] = -RDet * (
						M[1][0] * (M[2][1] * M[3][2] - M[2][2] * M[3][1]) -
						M[2][0] * (M[1][1] * M[3][2] - M[1][2] * M[3][1]) +
						M[3][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1])
					);
		Result[3][1] =  RDet * (
						M[0][0] * (M[2][1] * M[3][2] - M[2][2] * M[3][1]) -
						M[2][0] * (M[0][1] * M[3][2] - M[0][2] * M[3][1]) +
						M[3][0] * (M[0][1] * M[2][2] - M[0][2] * M[2][1])
					);
		Result[3][2] = -RDet * (
						M[0][0] * (M[1][1] * M[3][2] - M[1][2] * M[3][1]) -
						M[1][0] * (M[0][1] * M[3][2] - M[0][2] * M[3][1]) +
						M[3][0] * (M[0][1] * M[1][2] - M[0][2] * M[1][1])
					);
		Result[3][3] =  RDet * (
					M[0][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1]) -
					M[1][0] * (M[0][1] * M[2][2] - M[0][2] * M[2][1]) +
					M[2][0] * (M[0][1] * M[1][2] - M[0][2] * M[1][1])
				);

		memcpy( result, &Result, 16*sizeof(FLOAT) );
	}
}