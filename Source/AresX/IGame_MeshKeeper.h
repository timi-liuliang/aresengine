#pragma once

#include <vector>
#include "IGame.h"
#include "IGame_Mesh.h"
#include "IGame_MtlKeeper.h"
#include "IGame_BoneKeeper.h"
#include "AeConfig.h"
#include "M1Export.h"

using namespace std;
using namespace Ares;

namespace M1_IGame
{
	//----------------------------------------------------
	// MaxScene Mesh管理 MeshKeeper 帝林 2010-11-01 11:53
	//----------------------------------------------------
	class CMeshKeeper
	{
	public:
		// 构造函数
		CMeshKeeper();

		// 初始化
		void  Init();

		// 构建M1
		void  BuildMeshs( SExportM1& M1);

	private:
		// 初始化BoneKeeper
		void InitBoneKeeper();

		// 构建碰撞简模
		void BuildCollisionTriMesh( CM1CollMesh& colliedMesh);

	private:
		vector<CMesh*>    m_pMeshs;				// 临时模型
		vector<CMesh*>	  m_collMeshs;			// 碰撞简模
		CMtlKeeper*		  m_pMtlKeeper;			// 材质
		CBoneKeeper*      m_pBoneKeeper;		// 骨骼

		vector<SM1Face>	  m_colFaces;			// 碰撞面信息
		vector<SM1Vertex> m_colVertexs;			// 碰撞位置信息
	};
}