#pragma once

#ifdef OPENGL_EXPORTS
#define OGL_API __declspec(dllexport)
#else										// Use dll
#define OGL_API __declspec(dllimport)
#endif

#include "OGLRenderDevice.h"

namespace Ares
{
	extern "C"
	{
		// 启动插件
		OGL_API void  DllStartRenderFactory( RenderDevice*& ptr)
		{
			ptr = new_ OGLRenderDevice();
		}

		// 结束插件
		OGL_API void DllStopRenderFactory()
		{

		}
	}
}