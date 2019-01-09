#include "CMDLine.h"
#include <string>
#include "AILogic.h"

namespace Ares
{
	// 解析主入口
	bool CMDLine::Parser( int argc, char* argv[])
	{
		if( argc > 1)
		{
			// 生成导航网格
			if( std::string(argv[1]) == "gnav")
				GenerateNavMesh( argc, argv);

			return true;
		}

		return false;
	}

	// 导航网格生成
	void CMDLine::GenerateNavMesh( int argc, char* argv[])
	{
		CAILogic::BuildNavMesh( argv[2], argv[3], argv[4], argv[5]);
	}
}