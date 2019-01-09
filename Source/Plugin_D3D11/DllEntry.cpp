#pragma once

#ifdef D3D11_EXPORTS
#define D3D11_API __declspec(dllexport)
#else										// Use dll
#define D3D11_API __declspec(dllimport)
#endif

#include "D3D11RenderDevice.h"

namespace Ares
{
	extern "C"
	{
		// 启动插件
		D3D11_API void  DllStartRenderFactory( RenderDevice*& ptr)
		{
			if( D3D11RenderDevice::IsEnabled())
				ptr = D3D11RenderDevice::GetSingletonPtr();
			else
				ptr = new_ D3D11RenderDevice();
		}

		// 结束插件
		D3D11_API void DllStopRenderFactory()
		{
			
		}
	}
}