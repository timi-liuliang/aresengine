#pragma once

#include <string>
#include <Core/AresMath.h>
#include <Engine/RenderSystem/AresMesh.h>
#include <Engine/RenderSystem/AresModel.h>

using namespace std;

namespace Ares
{
	//--------------------------------------
	// 模型转换
	//--------------------------------------
	class ModelConverter
	{
	public:
		// YND文件转换为Ares
		static void YNDToAres( const string& yndFile, const string& aresLoacation, FileUtils& fileUtils, bool isReplaceExistAres);

	private:
		// 重置
		static void Reset();

	private:
		static vector<Mesh::IDCard*>			m_meshIDCards;		// 模型数据
		static vector<SuperMaterial::IDCard*>	m_materialIDCards;// 材质数据
		static Model::IDCard*					m_modelIDcard;		// 模型数据
	};
}