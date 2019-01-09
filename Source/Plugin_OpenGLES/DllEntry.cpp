#ifdef PLATFORM_WIN32
    #ifdef OPENGLES_EXPORTS
        #define OGLES_API __declspec(dllexport)
    #else										// Use dll
        #define OGLES_API __declspec(dllimport)
    #endif
#else
    #define OGLES_API
#endif

#include "OGLESRenderDevice.h"

namespace Ares
{
	extern "C"
	{
		// 启动插件
		OGLES_API void  DllStartRenderFactory( RenderDevice*& ptr)
		{
			if( OGLESRenderDevice::IsEnabled())
				ptr = OGLESRenderDevice::GetSingletonPtr();
			else
				ptr = new_ OGLESRenderDevice();
		}

		// 结束插件
		OGLES_API void DllStopRenderFactory()
		{

		}
	}
}