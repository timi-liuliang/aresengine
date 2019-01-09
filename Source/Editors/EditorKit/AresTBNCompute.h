#pragma once

#include <Core/AresMath.h>

namespace Ares
{
	//  Calculate normal
	void TBNComputeNormal( Vector3* normals, const Vector3* vertexs, int numVertex, const int* tris, int numTris);

	//  Calculate tangent binormal
	void TBNComputeTangentBinormal();
}