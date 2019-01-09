#pragma once

#include <vector>
#include "IGame.h"
#include <Engine/RenderSystem/M1/M1Loader.h>

using namespace std;
using namespace Ares;

namespace M1_IGame
{
	//---------------------------------------------------
	// MaxScene 材质管理 MtlKeeper 帝林 2010-10-29 16:08
	//---------------------------------------------------
	class CMtlKeeper
	{
	public:
		// 构造函数
		CMtlKeeper() {}

		// 构建材质
		void BuildMtls( CM1Material& M1);

		// 清除
		void Clear();

		// 获取材质ID
		int GetMtlID( IGameMaterial* pMtl);

		// 根据ID获取材质
		IGameMaterial*  GetMtl( size_t id);

		// 获取材质数量
		size_t GetCount();

		// 拷贝纹理到目的路径
		void  CopyTextureToTargetRoot( const string& texConv, const char* fileName, const char* aimName, const char* format=" DXT1 ");

	private:
		// 添加材质
		bool  AddMtl( IGameMaterial* pMtl);

		// 配置文件转换格式
		bool ConfigTransFormat();

	private:
		vector<IGameMaterial*>  m_mtlList;		// 材质列表
	};
}