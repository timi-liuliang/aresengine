#pragma once

#include <Core/AresMath.h>

namespace Ares
{
	//-------------------------------------------------------------
	//  凸包生成  http://www.eecs.tufts.edu/~mhorn01/comp163/
	//  Randomized Incremental Algorithm 
	//	http://www.eecs.tufts.edu/~mhorn01/comp163/algorithm.html
	//							    2010-12-01
	//-------------------------------------------------------------
	class  CConvexHull3D
	{
	public:
		enum ALGORITM_TYPE
		{
			EM_RANDOMIZED_INCREMENTAL = 0,
		};

		// 构造函数
		CConvexHull3D();

		// 析构函数
		~CConvexHull3D();

		// 通过顶点集构建ConvexHull
		bool Build( Vector3* pVertices, size_t iNum, ALGORITM_TYPE a_t=EM_RANDOMIZED_INCREMENTAL);

	private:
		size_t      m_iNumVertex;		// 顶点总数	
		size_t      m_iNumFace;			// 面总数	
		Vector3*	m_pVertexList;		// 顶点列表
		size_t*		m_pFaceList;		// 面列表(三个一组)
	};
}