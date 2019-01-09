#pragma once

#include "IGame_MeshKeeper.h"

using namespace Ares;

namespace M1_IGame
{
	//--------------------------------------
	// M1格式导出管理器   帝林  2010-11-01
	//--------------------------------------
	class CM1ExportManager
	{
	public:
		// 构造函数
		CM1ExportManager();

		// 构建
		void  Build( SExportM1& M1);

	private:
		// 构建M1文件
		void BuildExportM1( SExportM1& M1);

		// 构建M1
		void BuildM1( SExportM1& m1Export);

		// 构建M1Header
		void BuildM1ModelHeader( CM1Model& M1);

	private:
		CMeshKeeper*  m_pMeshKeeper;		// 模型列表
	};
}