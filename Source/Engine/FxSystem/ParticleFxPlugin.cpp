#include <AresRoot.h>
#include "ParticleFxPlugin.h"
#include <string>

using namespace std;

namespace Ares
{
	const string sPluginName = "ParticleFxPlugin";

	// 构造函数
	CParticleFxPlugin::CParticleFxPlugin()
	{
		m_particleSystem = NULL;
	}

	// 获取插件名, 插件名用来区分插件
	const char* CParticleFxPlugin::GetName() const
	{
		return sPluginName.c_str();
	}

	// 安装
	void CParticleFxPlugin::Install()
	{
		m_particleSystem = new CSystem;

		Root::SetFxSystem( m_particleSystem);
	}

	// 初始化
	void CParticleFxPlugin::Init()
	{

	}

	// 关闭,当插件关闭时被调用
	void CParticleFxPlugin::Shutdown()
	{

	}

	// 卸载,(插件先关闭,再卸载)
	void CParticleFxPlugin::Uninstall()
	{
		
	}
}