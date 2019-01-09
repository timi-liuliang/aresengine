#pragma once

#include "Ares3DEngine_h.h"

namespace Ares
{
	//---------------------------------------------------
	// AresPlugin Interface 2011-08-04 帝林
	// @Remarks
	//	  This class abstracts the generic interface that
	// all plugin must support.
	class ARES_EXPORT IPlugin
	{
	public:
		// 构造函数
		IPlugin() {}

		// 析构函数
		virtual ~IPlugin() {}

		// 获取插件名, 插件名用来区分插件
		virtual const char* GetName() const = 0;

		// 安装
		virtual void Install()=0;

		// 初始化
		virtual void Init()=0;

		// 关闭,当插件关闭时被调用
		virtual void Shutdown()=0;

		// 卸载,(插件先关闭,再卸载)
		virtual void Uninstall()=0;
	};

	typedef IPlugin* (*DLL_START_PLUGIN)(void);
	typedef void     (*DLL_STOP_PLUGIN) (void);
}