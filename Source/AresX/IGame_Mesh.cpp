#include "IGame_Mesh.h"
#include "AeConfig.h"
#include <set>

using namespace std;

namespace M1_IGame
{
	extern SExportConfig	  g_config;	// 导出配置

	// 构造函数
	CMesh::CMesh(IGameNode* pGameNode, CMtlKeeper* pMtlKeeper, CBoneKeeper* pBoneKeeper)
	{ 
		m_pMtlKeeper  = pMtlKeeper;
		m_pBoneKeeper = pBoneKeeper;

		m_pMeshInf = new SMeshInf();

		// 记录结点
		m_pMeshInf->m_pGameNode = pGameNode;

		// 记录mesh
		m_pMeshInf->m_pGameMesh = (IGameMesh*)pGameNode->GetIGameObject();
		m_pMeshInf->m_pGameSkin = m_pMeshInf->m_pGameMesh->GetIGameSkin();


		if ( m_pMeshInf->m_pGameSkin)
		{
			//m_pMeshInf->m_pGameMesh->release;
			m_pMeshInf->m_pGameMesh = m_pMeshInf->m_pGameSkin->GetInitialPose();
		}
		else
		{
			m_pMeshInf->m_pGameMesh->InitializeData();
		}

		m_boundingBox[0] =  1e30f;
		m_boundingBox[1] =  1e30f;
		m_boundingBox[2] =  1e30f;
		m_boundingBox[3] = -1e30f;
		m_boundingBox[4] = -1e30f;
		m_boundingBox[5] = -1e30f;
	}

	// 构建数据
	void CMesh::BuildData()
	{
		BuildSubset();

		BuildPos();
		BuildBoneInfluence();
		BuildUV();
		BuildNormal();
	}

	// 构建Mesh
	void CMesh::BuildM1Mesh( IDCardMesh& mesh)
	{
		BuildM1MeshHeader( mesh.m_Header);

		BuildM1MeshMatter( mesh);
	}

	// 构建SubSet
	void CMesh::BuildSubset()
	{
		IGameMaterial* pMaterial = m_pMeshInf->m_pGameNode->GetNodeMaterial();

		if ( pMaterial->IsMultiType())
		{
			Tab<int> matlIDs = m_pMeshInf->m_pGameMesh->GetActiveMatIDs();

			SM1Subset subSet;
			for ( int i=0; i<matlIDs.Count(); i++)
			{
				Tab<FaceEx*> subsetFaces = m_pMeshInf->m_pGameMesh->GetFacesFromMatID( matlIDs[i]);

				// 子集信息
				IGameMaterial* pSubMaterial = pMaterial->GetSubMaterial( matlIDs[i] % pMaterial->GetSubMaterialCount());
				subSet.m_iMtlIndex          = m_pMtlKeeper->GetMtlID( pSubMaterial);
				subSet.m_iStartIndex        = m_m1Faces.size() * 3;
				subSet.m_iPrimitiveCount    = subsetFaces.Count();

				for ( int j=0; j<subsetFaces.Count(); j++)
					AddFace( subsetFaces[j]);

				// 清除无效三角形后,更新面数量
				subSet.m_iPrimitiveCount    = subsetFaces.Count();

				m_m1Subsets.push_back( subSet);
			}
		}
		else
		{
			SM1Subset subSet;
			subSet.m_iMtlIndex       = m_pMtlKeeper->GetMtlID( pMaterial);
			subSet.m_iStartIndex     = m_m1Faces.size() * 3;
			subSet.m_iPrimitiveCount = m_pMeshInf->m_pGameMesh->GetNumberOfFaces();

			for ( int i=0; i<m_pMeshInf->m_pGameMesh->GetNumberOfFaces(); i++)
				AddFace( m_pMeshInf->m_pGameMesh->GetFace( i));

			subSet.m_iPrimitiveCount = m_m1Faces.size() - subSet.m_iStartIndex;

			m_m1Subsets.push_back( subSet);
		}
	}

	// 构建面
	void CMesh::AddFace( FaceEx* pFace)
	{
		m_maxFaces.push_back( pFace);

		SM1Face m1Face;
		m1Face.m_uiIndices[0] = pFace->vert[0];
		m1Face.m_uiIndices[1] = pFace->vert[1];
		m1Face.m_uiIndices[2] = pFace->vert[2];

		m_m1Faces.push_back( m1Face);
	}

	// 构建位置数据
	void CMesh::BuildPos()
	{
		int numOfVertexs = m_pMeshInf->m_pGameMesh->GetNumberOfVerts();

		for ( int i=0; i<numOfVertexs; i++)
		{
			Point3 vertex;
			SM1Vertex v;
			m_pMeshInf->m_pGameMesh->GetVertex( i, vertex);
			Point3ToSM1Vertex( v, vertex);

			// 包围盒
			m_boundingBox[0] = min( m_boundingBox[0], v.m_fVert[0]);
			m_boundingBox[1] = min( m_boundingBox[1], v.m_fVert[1]);
			m_boundingBox[2] = min( m_boundingBox[2], v.m_fVert[2]);

			m_boundingBox[3] = max( m_boundingBox[3], v.m_fVert[0]);
			m_boundingBox[4] = max( m_boundingBox[4], v.m_fVert[1]);
			m_boundingBox[5] = max( m_boundingBox[5], v.m_fVert[2]);

			// 位置数据
			m_m1Vertexs.push_back( v);

			// 为UV数据做准备
			STexIndex* pTexIndex = NULL;
			m_pTexIndexs.push_back( pTexIndex);

			int aa = m_pMeshInf->m_pGameMesh->GetNumberOfVerts();
			int xx = m_pMeshInf->m_pGameMesh->GetNumberOfNormals();
			int cc = m_pMeshInf->m_pGameMesh->GetNumberOfBinormals();
			int dd = m_pMeshInf->m_pGameMesh->GetNumberOfTangents();


			// 法线数据
			//Point3 normal;
			//m_pMeshInf->m_pGameMesh->GetNormal( i, normal);
			//m_m1Normals.push_back( SM1Normal( normal.x, normal.y, normal.z));

			// 切线数据
			//Point3 tangent;
			//m_pMeshInf->m_pGameMesh->GetTangent( i, tangent);
			//m_m1Tangent.push_back(SM1Normal( tangent.x, tangent.y, tangent.z));

			// 副线数据
			//Point3 binormal;
			//m_pMeshInf->m_pGameMesh->GetBinormal( i, binormal);
			//m_m1Tangent.push_back(SM1Normal( binormal.x, binormal.y, binormal.z));

			m_vertexIndexs.push_back( i);
		}

		set<int> associateVertex;
		int numOfNormals = m_pMeshInf->m_pGameMesh->GetNumberOfNormals();
		for( int i=0; i<numOfNormals; i++)
		{
			int av = m_pMeshInf->m_pGameMesh->GetNormalVertexIndex( i);

			associateVertex.insert( av);
		}

		int a = 10;
	}

	// 构建法线
	void CMesh::BuildNormal()
	{
		vector<Vector3> tNormals;

		tNormals.resize( m_m1Vertexs.size());
		tNormals.assign( m_m1Vertexs.size(), Vector3::Zero);
		for ( size_t i=0; i<m_m1Faces.size(); i++)
		{
			int idx0 = m_m1Faces[i].m_uiIndices[0];
			int idx1 = m_m1Faces[i].m_uiIndices[1];
			int idx2 = m_m1Faces[i].m_uiIndices[2];

			idx0 = m_vertexIndexs[idx0];
			idx1 = m_vertexIndexs[idx1];
			idx2 = m_vertexIndexs[idx2];

			Vector3 edge0 = (Vector3&)m_m1Vertexs[idx1] - (Vector3&)m_m1Vertexs[idx0];
			Vector3 edge1 = (Vector3&)m_m1Vertexs[idx2] - (Vector3&)m_m1Vertexs[idx0];

			Vector3 faceNormal = Vector3Cross( edge0, edge1);
			faceNormal.Normalize();

			tNormals[idx0] += faceNormal;
			tNormals[idx1] += faceNormal;
			tNormals[idx2] += faceNormal;
		}

		// 单位化
		for ( size_t i=0; i<tNormals.size(); i++)
			tNormals[i].Normalize();

		m_m1Normals.clear();
		for ( size_t i=0; i<tNormals.size(); i++)
			m_m1Normals.push_back( (SM1Normal&)tNormals[i]);
	}

	// 构建权重数据
	void CMesh::BuildBoneInfluence()
	{
		// skin modifier
		IGameSkin* pGameSkin = m_pMeshInf->m_pGameSkin;
		if ( pGameSkin)
		{
			// Modifier 影响的顶点数
			int numV = pGameSkin->GetNumOfSkinnedVerts();
			for( int vertexIndex=0; vertexIndex<numV; vertexIndex++)
			{
				// total bones influencing this vertex
				int numBones = pGameSkin->GetNumberOfBones( vertexIndex);

				// 每个顶点最多附加四个骨骼
				if ( numBones > 4)
					numBones = 4;

				SM1BoneInfluence boneInfluence;
				for ( int boneIndex=0; boneIndex<numBones; boneIndex++)
				{
					// 骨骼索引
					IGameNode* pGameBone = pGameSkin->GetIGameBone( vertexIndex, boneIndex);
					boneInfluence.m_boneIndexs[boneIndex] = /*m_pBoneKeeper->GetBoneID( pGameBone)*/static_cast<float>(GetBoneID(pGameBone));

					// 权重
					boneInfluence.m_boneWeights[boneIndex] = pGameSkin->GetWeight( vertexIndex, boneIndex);
				}

				m_m1BoneInfluence.push_back( boneInfluence);
			}
		}
	}

	// 构建UV数据( 复制顶点.脑残的做法; 虽然脑残,但只影响带宽，不影响速度)
	void CMesh::BuildUV()
	{
		for ( size_t i=0; i<m_m1Faces.size(); i++)
		{
			for ( int j=0; j<3; j++)
			{
				// 顶点索引与UV索引
				int vertexIndex = m_m1Faces[i].m_uiIndices[j];
				int uvIndex     = m_maxFaces[i]->texCoord[j];

				// 获取对应索引的UV值
				Point2 texUV;
				m_pMeshInf->m_pGameMesh->GetTexVertex( uvIndex, texUV);

				if ( m_pTexIndexs[vertexIndex])
				{
					int tIndex;
					if ( m_pTexIndexs[vertexIndex]->GetIndex( tIndex, texUV))
						m_m1Faces[i].m_uiIndices[j] = tIndex;
					else
					{
						m_pTexIndexs[vertexIndex]->AddNew( texUV, m_vertexIndexs.size());

						m_m1Faces[i].m_uiIndices[j] = m_vertexIndexs.size();
						m_vertexIndexs.push_back( vertexIndex);
					}
				}
				else
				{
					STexIndex* pTexIndex = new STexIndex( texUV, vertexIndex);

					m_pTexIndexs[vertexIndex] = pTexIndex;
				}
			}
		}
	}

	// 是否无需导出的模型
	bool CMesh::IsBadMesh()
	{
		if( m_m1Vertexs.size()<=0) 
			return true;

		if( m_m1Faces.size()<=0)
			return true;

		return false;
	}

	// 构建顶点数据
	void CMesh::BuildM1MeshHeader(SM1MeshHeader& meshHeader)
	{
		strcpy( meshHeader.m_cName, m_pMeshInf->m_pGameNode->GetName());
		meshHeader.m_iNumUVs       = m_vertexIndexs.size();
		meshHeader.m_iNumVerts     = m_m1Vertexs.size();
		meshHeader.m_iNumTriangles = m_m1Faces.size();
		meshHeader.m_iVertexOffset = sizeof( SM1Vertex) * meshHeader.m_iNumVerts;	// Offset for the vertices

		meshHeader.m_iNumVAppend   = meshHeader.m_iNumUVs - meshHeader.m_iNumVerts;
		meshHeader.m_iVAppendOffset = sizeof(int) * meshHeader.m_iNumVAppend;

		meshHeader.m_bNormal       = true;
		meshHeader.m_iNormalOffset = meshHeader.m_bNormal ? sizeof( SM1Normal) * meshHeader.m_iNumVerts: 0;

		meshHeader.m_bVertexColor  = false;
		meshHeader.m_iColorOffset  = meshHeader.m_bVertexColor ? sizeof( SM1Color)  * meshHeader.m_iNumVerts: 0;

		meshHeader.m_bUV = true;
		meshHeader.m_iUVOffset     = meshHeader.m_bUV ? sizeof( SM1TexCoord) * meshHeader.m_iNumUVs: 0;

		meshHeader.m_bLightmapUV   = false;
		meshHeader.m_iLightmapUVOffset = 0;
		meshHeader.m_lightmapAR	   = 1.f;

		meshHeader.m_iFaceOffset   = sizeof( SM1Face) * m_m1Faces.size();	// Offset for the faces			
		meshHeader.m_iHeaderSize   = sizeof( SM1MeshHeader);

		meshHeader.m_iSubsetNum     = m_m1Subsets.size();
		meshHeader.m_iSubSetOffset = sizeof( SM1Subset) * meshHeader.m_iSubsetNum;

		meshHeader.m_bBI           = m_m1BoneInfluence.size() > 0;
		meshHeader.m_iBIOffset     = sizeof( SM1BoneInfluence) * m_m1BoneInfluence.size();

		memcpy( meshHeader.m_boundingBox, m_boundingBox, sizeof( m_boundingBox));

		meshHeader.m_iNumBones	   = m_boneIdxs.size();
		meshHeader.m_iBoneIdxOffset= meshHeader.m_iNumBones * sizeof(int);
	}

	// 构建mesh主题数据
	void CMesh::BuildM1MeshMatter( IDCardMesh& mesh)
	{
		// Mesh数据
		const SM1MeshHeader& meshHeader = mesh.m_Header;
		mesh.m_positions = new SM1Vertex[meshHeader.m_iNumVerts];
		if ( meshHeader.m_bNormal)      mesh.m_pNormals       = new SM1Normal[meshHeader.m_iNumVerts];
		if ( meshHeader.m_bVertexColor)	mesh.m_pVertColors    = new SM1Color[meshHeader.m_iNumVerts];
		if ( meshHeader.m_bUV)          mesh.m_pTexCoords     = new SM1TexCoord[meshHeader.m_iNumUVs];
		if( meshHeader.m_bBI)			
			mesh.m_pBoneInfluence = new SM1BoneInfluence[meshHeader.m_iNumVerts];

		for ( int i=0; i<meshHeader.m_iNumVerts; i++)
		{
			// 1. 顶点位置
			mesh.m_positions[i] = m_m1Vertexs[i];

			// 2. 法线信息
			if ( meshHeader.m_bNormal)	
				mesh.m_pNormals[i] = m_m1Normals[i];

			// 3. 顶点色(全黑)
			//if ( meshHeader.m_bVertexColor){}

			// 骨骼权重
			if( meshHeader.m_bBI)
				mesh.m_pBoneInfluence[i] = m_m1BoneInfluence[i];
		}

		// 4. UV坐标( 现在只取第0层)
		for ( size_t i=0; i<m_pTexIndexs.size(); i++)
		{
			STexIndex* pTexIndex = m_pTexIndexs[i];

			while ( pTexIndex)
			{
				mesh.m_pTexCoords[ pTexIndex->m_iIndex].m_fTexCoord[0] = pTexIndex->m_texCoord[0]; 
				mesh.m_pTexCoords[ pTexIndex->m_iIndex].m_fTexCoord[1] = pTexIndex->m_texCoord[1]; 

				pTexIndex = pTexIndex->m_pNext;
			}
		}

		// 5. 面信息
		mesh.m_pFaces = new SM1Face[meshHeader.m_iNumTriangles];
		for ( int i=0; i<meshHeader.m_iNumTriangles; i++)
		{
			mesh.m_pFaces[i] = m_m1Faces[i];
		}

		// 6.顶点重复信息,用于匹配UV坐标
		int tAppendNum = m_vertexIndexs.size() - m_m1Vertexs.size();
		mesh.m_pVertexIdxAppend = new int[tAppendNum];
		for ( int i=0; i<tAppendNum; i++)
		{
			mesh.m_pVertexIdxAppend[i] = m_vertexIndexs[ i + m_m1Vertexs.size()];
		}

		// 子集信息
		mesh.m_pSubsets = new SM1Subset[meshHeader.m_iSubsetNum];
		for ( int i=0; i<meshHeader.m_iSubsetNum; i++)
		{
			mesh.m_pSubsets[i] = m_m1Subsets[i];
		}

		// 骨骼索引
		if( m_boneIdxs.size())
		{
			mesh.m_pBoneIdxs = new int[ m_boneIdxs.size()];

			for ( size_t i=0; i<m_boneIdxs.size(); i++)
			{
				mesh.m_pBoneIdxs[i] = m_boneIdxs[i];
			}
		}
	}

	// 获取骨骼ID
	int  CMesh::GetBoneID( IGameNode* pGameNode)
	{
		if( !pGameNode)
			int a = 10;

		int idx = m_pBoneKeeper->GetBoneID( pGameNode);

		for ( size_t i=0; i<m_boneIdxs.size(); i++)
		{
			if( m_boneIdxs[i] == idx)
			{
				return i;
			}
		}

		// 不存在
		m_boneIdxs.push_back( idx);

		return m_boneIdxs.size() - 1;
	}

	// 构建碰撞数据
	void CMesh::BuildCollisionData( vector<SM1Vertex>& colVertexs, vector<SM1Face>& colFaces)
	{
		IGameMesh* gamemesh = m_pMeshInf->m_pGameMesh;

		// 保存顶点数据
		int numOfVertexs = gamemesh->GetNumberOfVerts();
		for ( int i=0; i<numOfVertexs; i++)
		{
			Point3 vertex;
			SM1Vertex v;
			gamemesh->GetVertex( i, vertex);		
			Point3ToSM1Vertex( v, vertex);

			colVertexs.push_back( v);
		}

		// 保存面数据
		int numOfFaces = m_pMeshInf->m_pGameMesh->GetNumberOfFaces();
		for( int i=0; i<numOfFaces; i++)
		{
			SM1Face m1Face;
			FaceEx*  tFace = gamemesh->GetFace( i);
			m1Face.m_uiIndices[0] = tFace->vert[0];
			m1Face.m_uiIndices[1] = tFace->vert[1];
			m1Face.m_uiIndices[2] = tFace->vert[2];

			colFaces.push_back( m1Face);
		}
	}
}