#include "MOYNDMeshToAres.h"
#include "YNDMesh/YNDMeshFile.h"
#include <Engine/AresRoot.h>
#include <boost/filesystem.hpp>

namespace Ares
{
	vector<Mesh::IDCard*>			ModelConverter::m_meshIDCards;		// 模型数据
	vector<SuperMaterial::IDCard*>	ModelConverter::m_materialIDCards;// 材质数据
	Model::IDCard*					ModelConverter::m_modelIDcard = NULL;		// 模型数据

	// 执行转换
	void ModelConverter::YNDToAres( const string& yndFile, const string& aresLoacation, FileUtils& fileUtils, bool isReplaceExistAres)
	{
		using namespace boost::filesystem;

		if( !isReplaceExistAres)
		{
			path arespath(aresLoacation + path(yndFile).filename().string());
			arespath.replace_extension( ".model");
			if( FileUtils::IsFileExist( arespath.string()))
				return;
		}
		

		Reset();

		YNDMeshFile		tYNDMeshFile;
		if( !tYNDMeshFile.ReadDataFromFile( yndFile.c_str()))
		{
			ALogSystem.Warning( "%s not found!", yndFile.c_str());

			return;
		}

		//取得模型体数量
		int	tMeshNodeNum = tYNDMeshFile.Fun_GetMeshNodeNum();
		for(int m = 0 ; m < tMeshNodeNum ; m++)
		{
			//取得对应模型体
			SMeshNode_Temp*	tpMeshNodeInfo	= tYNDMeshFile.Fun_GetMeshNodePtr(m);
			if( !tpMeshNodeInfo/* || tYNDMeshFile.Fun_GetTextureNum()!=tpMeshNodeInfo->m_nSubMeshNum*/)
				return; 

			for(int subIdx = 0; subIdx<tpMeshNodeInfo->m_nSubMeshNum; subIdx++)
			{
				// Mesh::IDCard
				Mesh::IDCard* meshIDCard  = new_ Mesh::IDCard;
				Matrix44 subMeshMatrix = (Matrix44&)tpMeshNodeInfo->m_pSubMeshArray[subIdx].m_matSubMeshMatrix;

				meshIDCard->m_name = tpMeshNodeInfo->m_pSubMeshArray[subIdx].m_sSubMeshInfo.m_szSubMeshName;

				// 顶点转换
				const vector<SExport_Vertex>& origVertices = tpMeshNodeInfo->m_pSubMeshArray[subIdx].m_pVertexArray;
				for ( size_t i=0; i<origVertices.size(); i++)
				{
					meshIDCard->m_positions.push_back( float3( origVertices[i].m_fPosX, origVertices[i].m_fPosZ, origVertices[i].m_fPosY));
					meshIDCard->m_normals.push_back( float3( origVertices[i].m_fNPosX, origVertices[i].m_fNPosZ, origVertices[i].m_fNPosY));
					meshIDCard->m_uvsDiffuse.push_back( float2( origVertices[i].m_fU, origVertices[i].m_fV));

					// 转换顶点
					meshIDCard->m_positions[i] = subMeshMatrix.TransformVector3( meshIDCard->m_positions[i]);

					meshIDCard->m_boundingBox.UnionPoint( meshIDCard->m_positions[i]);
				}

				// 索引数据
				const vector<SExport_Face>& origFaces = tpMeshNodeInfo->m_pSubMeshArray[subIdx].m_pFaceArray;
				meshIDCard->m_faces.reserve( origFaces.size());
				for( size_t i=0; i<origFaces.size(); i++)
				{
					meshIDCard->m_faces.push_back( Int3( origFaces[i].m_nVertexIndex1, origFaces[i].m_nVertexIndex2, origFaces[i].m_nVertexIndex3));
				}
				m_meshIDCards.push_back( meshIDCard);

				// SuperMaterial::IDCard
				SuperMaterial::IDCard* materialIDCard = new_ SuperMaterial::IDCard;
				int texIdx = tpMeshNodeInfo->m_pSubMeshArray[subIdx].m_sSubMeshInfo.m_nMaterialID;
				STexture* tpTexture = tYNDMeshFile.Fun_GetTexturePtr( texIdx);
				if( tpTexture)
				{
					string texFilePath;
					FileUtils::ReplaceExt( texFilePath, tpTexture->m_szFileName, ".dds");

					materialIDCard->m_renderQueue = RQ_Opacity;
					materialIDCard->m_shader	  = "DiffuseTexture";
					materialIDCard->m_parameters.push_back( SuperMaterial::IDCard::Parameter( "World",				"SPT_FloatArray", "SPAB_World"));
					materialIDCard->m_parameters.push_back( SuperMaterial::IDCard::Parameter( "View",				"SPT_FloatArray", "SPAB_View"));
					materialIDCard->m_parameters.push_back( SuperMaterial::IDCard::Parameter( "ViewProj",			"SPT_FloatArray", "SPAB_ViewProj"));
					materialIDCard->m_parameters.push_back( SuperMaterial::IDCard::Parameter( "DiffuseTex",			"SPT_Texture",	  texFilePath.c_str()));
					materialIDCard->m_parameters.push_back( SuperMaterial::IDCard::Parameter( "FLAG_UseLighting",	"SPT_FloatArray", "1"));

					// 拷贝纹理
					try
					{
						string fromPath = fileUtils.FullPath( texFilePath.c_str());
						path toPath(aresLoacation + texFilePath);
						if( FileUtils::IsFileExist( fromPath))
						{
							boost::filesystem::copy_file( fromPath, toPath, copy_option::overwrite_if_exists);
						}
					}
					catch (...)
					{}
				}
				m_materialIDCards.push_back( materialIDCard);


				// Model::IDCard
				Model::IDCard::RenderInfo renderInfo;
				renderInfo.m_materialPath= path(yndFile).filename().replace_extension(".material").string();
				renderInfo.m_materialIdx = m_modelIDcard->m_renderInfos.size();
				renderInfo.m_meshFilePath= path(yndFile).filename().replace_extension(".mesh").string();
				renderInfo.m_meshIdx	 = m_modelIDcard->m_renderInfos.size();
				m_modelIDcard->m_renderInfos.push_back( renderInfo);
			}
		}

		// 1.计算存储路径
		path arespath(aresLoacation + path(yndFile).filename().string());

		// 1.保存网格文件
		arespath.replace_extension( ".mesh");
		Serialization::SerializeToBinaryFile( arespath.string().c_str(), m_meshIDCards);

		// 2.保存材质文件
		arespath.replace_extension( ".material");
		Serialization::SerializeToBinaryFile( arespath.string().c_str(), m_materialIDCards);

		// 3.动画文件
		arespath.replace_extension( ".skeleton");

		// 4.模型配置文件
		arespath.replace_extension( ".model");
		Serialization::SerializeToBinaryFile( arespath.string().c_str(), m_modelIDcard);
	}

	// 重置
	void ModelConverter::Reset()
	{
		// Mesh
		for( size_t i=0; i<m_meshIDCards.size(); i++)
			SAFE_DELETE( m_meshIDCards[i]);

		m_meshIDCards.clear();

		// material
		for( size_t i=0; i<m_materialIDCards.size(); i++)
			SAFE_DELETE( m_materialIDCards[i]);

		m_materialIDCards.clear();

		// Model
		SAFE_DELETE( m_modelIDcard);
		m_modelIDcard = new_ Model::IDCard;
	}
}