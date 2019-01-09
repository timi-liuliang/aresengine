#include <WTypes.h>
#include "AeUtility.h"
#include "AeExportClassDesc.h"

extern HINSTANCE    g_hInstance;
extern Interface*   g_pI;

M1_IGame::CAeUtility*		g_utility = NULL;

namespace M1_IGame
{
	CAeExportClassDesc* CAeExportClassDesc::instance = NULL;

	// show this is create branch
	int	CAeExportClassDesc::IsPublic()
	{
		return 1;
	}

	// 返回导出类实例
	void* CAeExportClassDesc::Create(BOOL loading/*=FALSE*/)
	{
		if( !g_utility)
			g_utility = new CAeUtility;

		return g_utility;
	}

	// 返回导出类名称
	const char* CAeExportClassDesc::ClassName()
	{
		return "AresX";
	}

	// 决定插件类型 dle, dlu 等等
	SClass_ID CAeExportClassDesc::SuperClassID()
	{
		//return SCENE_EXPORT_CLASS_ID;
		return UTILITY_CLASS_ID;
	}

	// 插件的唯一标识符,max用来区分插件
	Class_ID CAeExportClassDesc::ClassID()
	{
#ifdef _DEBUG
		// 记得重新获取一个唯一值
		return Class_ID(0x77ff6121, 0x6b7c31);
#else
		// 记得重新获取一个唯一值
		return Class_ID(0x77ff6121, 0x6b7c31);
#endif
	}

	// 种类 类别
	const char* CAeExportClassDesc::Category()
	{
		return "AresEngine";
	}

	//// 导出类内部名
	//const char* CAeExportClassDesc::InternalName()
	//{
	//	return "CAeUtility";
	//}

	// dll 实例句柄
	HINSTANCE CAeExportClassDesc::HInstance()
	{
		return g_hInstance;
	}
}