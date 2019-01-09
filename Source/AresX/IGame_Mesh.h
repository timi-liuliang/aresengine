#pragma once

#include <vector>
#include "IGame.h"
#include "IGameModifier.h"
#include "IGame_MtlKeeper.h"
#include "IGame_BoneKeeper.h"
#include <Core/AresMath.h>
#include <Physics/Shapes/Triangle3.h>
#include "M1Export.h"

using namespace std;
using namespace Ares;

namespace M1_IGame
{	
	// 蒙皮信息
	struct SMeshInf
	{
		IGameNode*		m_pGameNode;
		IGameMesh*		m_pGameMesh;
		IGameSkin*      m_pGameSkin;

		// 构造函数
		SMeshInf()
		{
			memset( this, 0, sizeof( SMeshInf));
		}
	};

	// 辅助定点复制计算
	struct STexIndex
	{
		Point2        m_texCoord;	// UV 坐标值
		int           m_iIndex;		// 对应索引位置

		STexIndex*    m_pNext;

		// 构造函数
		STexIndex( Point2& texCoord, int iIndex)
		{
			m_texCoord = texCoord;
			m_iIndex   = iIndex;

			m_pNext = NULL;
		}

		// 添加New
		void  AddNew( Point2& texCoord, int iIndex)
		{
			STexIndex* pTexIndex = new STexIndex( texCoord, iIndex);

			// 添加到列表中
			pTexIndex->m_pNext = this->m_pNext;
			this->m_pNext = pTexIndex;
		}

		// 获取索引
		bool GetIndex( int& index, const Point2& texCoord)
		{
			STexIndex* pTexIndex = this;
			while ( pTexIndex)
			{
				if( pTexIndex->m_texCoord == texCoord)
				{
					index = pTexIndex->m_iIndex;
					return true;
				}

				pTexIndex = pTexIndex->m_pNext;
			}

			return false;
		}
	};

	//---------------------------------------
	// CMesh 场景整理 帝林 2010-11-01 
	//---------------------------------------
	class CMesh
	{
	public:
		// 构造函数
		CMesh( IGameNode* pGameNode, CMtlKeeper* pMtlKeeper, CBoneKeeper* pBoneKeeper);

		// 构建数据
		void BuildData();

		// 构建碰撞数据
		void BuildCollisionData( vector<SM1Vertex>& colVertexs, vector<SM1Face>& colFaces);

		// 构建Mesh
		void BuildM1Mesh( IDCardMesh& mesh);

		// 获取Mesh信息
		const SMeshInf* GetMeshInf() { return m_pMeshInf; }

		// 是否无需导出的模型
		bool IsBadMesh();

	private:
		// 构建顶点数据
		void BuildM1MeshHeader(SM1MeshHeader& meshHeader);

		// 构建mesh主题数据
		void BuildM1MeshMatter( IDCardMesh& mesh);

		// 构建SubSet
		void BuildSubset();

		// 构建位置数据
		void BuildPos();

		// 构建权重数据
		void BuildBoneInfluence();

		// 构建UV数据( 复制顶点.脑残的做法)
		void BuildUV();

		// 构建法线
		void BuildNormal();

		// 更新包围盒
		void BuildBoundingBox();

		// 构建面
		void AddFace(  FaceEx* pFace);

		// 获取骨骼ID
		int  GetBoneID( IGameNode* pGameNode);

	private:
		SMeshInf*					m_pMeshInf;
		vector<SM1Subset>			m_m1Subsets;		// 子集 
		vector<SM1Face>				m_m1Faces;			// 临时面信息
		vector<SM1Vertex>			m_m1Vertexs;		// 临时顶点位置
		vector<SM1BoneInfluence>    m_m1BoneInfluence;	// 权重信息
		vector<SM1TexCoord>			m_m1Texcoords;		// 临时纹理坐标

		vector<SM1Normal>			m_m1Normals;		// 法线
		vector<SM1Normal>			m_m1Tangent;		// 切线
		vector<SM1Normal>			m_m1Binormal;		// 副线

		vector<FaceEx*>             m_maxFaces;			// 面数据
		vector<int>					m_boneIdxs;			// 骨骼索引

		CMtlKeeper*					m_pMtlKeeper;		// 材质管理器
		CBoneKeeper*                m_pBoneKeeper;		// 骨骼管理器
		float                       m_boundingBox[6];	// 包围盒                    

	private:
		vector<STexIndex*>			m_pTexIndexs;		// 辅助数据
		vector<int>					m_vertexIndexs;		// 顶点索引(重复数据)
	};
}