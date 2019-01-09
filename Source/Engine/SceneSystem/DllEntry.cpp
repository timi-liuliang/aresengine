#pragma once

#include "MainPlugin.h"

#ifdef VANGUARD_EXPORTS
#define VANGUARD_API __declspec(dllexport)
#else
#define VANGUARD_API __declspec(dllimport)
#endif

namespace Ares
{
	CMainPlugin*       g_plugin = 0;			// 当前插件

	extern "C"
	{
		// 启动插件
		VANGUARD_API IPlugin*  DllStartPlugin()
		{
			g_plugin = new CMainPlugin;

			return g_plugin;
		}

		// 结束插件
		VANGUARD_API void DllStopPlugin()
		{

		}
	}
}