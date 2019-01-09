#pragma once

#include "ParticleFxPlugin.h"

#ifdef FX_EXPORTS
#define FX_API __declspec(dllexport)
#else
#define FX_API __declspec(dllimport)
#endif

#ifdef _DEBUG
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif

namespace Ares
{
	CParticleFxPlugin*     g_plugin = 0;		// 当前插件

	extern "C"
	{
		// 启动插件
		FX_API IPlugin*  DllStartPlugin()
		{
#ifdef _DEBUG
			_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

			g_plugin = new CParticleFxPlugin;

			return g_plugin;
		}

		// 结束插件
		FX_API void DllStopPlugin()
		{

		}
	}
}