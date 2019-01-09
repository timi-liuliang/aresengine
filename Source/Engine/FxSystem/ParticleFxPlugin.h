#include <AresPlugin.h>
#include "System.h"

namespace Ares
{
	//------------------------------------
	// ParticleFxPlugin 2011-09-13 帝林
	// Plugin instance for ParticleSystem
	//------------------------------------
	class CParticleFxPlugin : public IPlugin
	{
	public: 
		// 构造函数
		CParticleFxPlugin();

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
		CSystem*		m_particleSystem;	// 此插件粒子系统
	};
}