#include "DllEntry.h"
#include <WTypes.h>
#include "AeExportClassDesc.h"

HINSTANCE g_hInstance;

using namespace M1_IGame;

//----------------------------------------
// 导出插件 2010-08-26   帝林
//----------------------------------------
extern   "C" 
{
	// 主入口函数
	BOOL WINAPI DllMain( HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved)
	{
#ifdef _DEBUG
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

		if( fdwReason == DLL_PROCESS_ATTACH)
		{
			g_hInstance = hinstDLL; 

			// Initialize Max's custom controls and Wind95 controls
			//InitCustomControls( g_hInstance);
			//InitCommonControls();
			//InitCommonControlsEx();

			DisableThreadLibraryCalls(g_hInstance);
		}

		return TRUE;
	}

	// 获取描述信息,一般用于描述插件的获取方式
	ARESX_MAX9_API const char* LibDescription()
	{
		return "AresEngine -- 帝林";
	}

	// 返回此dll所包含的类数量( 当添加新类时,必须修改)
	ARESX_MAX9_API int LibNumberClasses()
	{
		return 1;
	}

	// 返回插件的类描述
	ARESX_MAX9_API ClassDesc* LibClassDesc( int i)
	{
		switch(i)
		{
		case 0:
			return CAeExportClassDesc::GetInstance();
		default:
			return NULL;
		}

		return NULL;
	}

	// 返回库版本
	// This function returns a pre-defined constant indicating the version
	// of the system under which it was compiled It is used to allow ths 
	// System to catch obsolete DLLs
	ARESX_MAX9_API ULONG LibVersion()
	{
		return VERSION_3DSMAX;
	}

	// Let the plug-in register itself for deferred loading
	ARESX_MAX9_API ULONG CanAutoDefer()
	{
		return 1;
	}
};
