#pragma once

#include <AresPlugin.h>
#include "MainSystem.h"

namespace Ares
{
	//----------------------------------
	// CQtESPlugin 2011-08-04 帝林
	// 基于四叉树的场景管理系统 
	//----------------------------------
	class CMainPlugin : public IPlugin
	{
	public: 
		// 构造函数
		CMainPlugin();

		// 获取插件名, 插件名用来区分插件
		virtual const char* GetName() const;

		// 安装
		virtual void Install();

		// 初始化
		virtual void Init();

		// 关闭,当插件关闭时被调用
		virtual void Shutdown();

		// 卸载,(插件先关闭,再卸载)
		virtual void Uninstall();

	private:
		CMainSystem*		m_mainSystem;		// 引擎的主系统
	};
}