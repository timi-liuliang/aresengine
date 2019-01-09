#pragma once

namespace Ares
{
	//----------------------------------------
	// AStudio 命令行解析 2012-8-16 帝林
	//----------------------------------------
	class CMDLine
	{
	public:
		// 解析主入口
		static bool Parser( int argc, char* argv[]);

		// 导航风格生成
		static void GenerateNavMesh( int argc, char* argv[]);
	};
}