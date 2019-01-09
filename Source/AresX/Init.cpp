#include "Init.h"
#include "IGame.h"

namespace M1_IGame
{
	IGameScene *  g_pIGame = NULL;

	// 打开新场景时,初始化场景
	void  InitScene()
	{
		g_pIGame = GetIGameInterface();

		// 设置坐标系
		IGameConversionManager* pCM = GetConversionManager();

		UserCoord IGame_Ares = 
		{
			0,	// Left handed coordinate system
			1,  // X Axis goes right
			4, // Y Axis goes in
			2,  // Z Axis goes up
			1,	// U Tex axis is Right
			1,  // V Tex axis is Down
		};

		pCM->SetUserCoordSystem( IGame_Ares);

		// 导出所有 or 只导出选择
		g_pIGame->InitialiseIGame( false);
		g_pIGame->SetStaticFrame( 0);
	}
}