#include <string>
#include <AresRoot.h>
#include "MainPlugin.h"

using namespace std;

namespace Ares
{
	CMainSystem*	g_mainSystem = NULL;

	const string sPluginName = "QuadTreeEntitySystem";

	// 构造函数
	CMainPlugin::CMainPlugin()
	{
#ifdef _DEBUG
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

		m_mainSystem = NULL;
	}

	// 获取插件名, 插件名用来区分插件
	const char* CMainPlugin::GetName() const
	{
		return sPluginName.c_str();
	}

	// 安装
	void CMainPlugin::Install()
	{
		if( !m_mainSystem)
		{
			m_mainSystem = new CMainSystem;
			g_mainSystem  = m_mainSystem;
		}

		Root::SetMainSystem( m_mainSystem);
	}

	// 初始化
	void CMainPlugin::Init()
	{

	}

	// 关闭,当插件关闭时被调用
	void CMainPlugin::Shutdown()
	{

	}

	// 卸载,(插件先关闭,再卸载)
	void CMainPlugin::Uninstall()
	{
		if( m_mainSystem)
			delete m_mainSystem;

		m_mainSystem = 0;
	}
}