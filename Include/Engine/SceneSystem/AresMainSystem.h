#pragma once

#include "AresMap.h"
#include "../RenderSystem/AresCameraCallback.h"
#include "../Foundation/AresErrorCallback.h"

namespace Ares
{
	//-----------------------------------
	// IMainSystem 2011-08-16 帝林
	//-----------------------------------
	class IMainSystem
	{
	public:
		// 初始化
		virtual void Init( ICameraCallback* mainCamera, IErrorCallback* errorCallback)=0;

	public:
		// 创建区域管理器, 地图名,存放位置
		virtual IMap* CreateMap( const char* mapName=0, const char* location=0)=0;

		// 新建区域
		virtual IArea* CreateArea( const char* areaName=0)=0;
	};
}