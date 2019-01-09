#include <Engine/AresRoot.h>
#include <Engine/Foundation/AresPlugin.h>
#include <Engine/SceneSystem/AresScene.h>
#ifdef ARES_PLATFORM_WINDOWS
#include <Core/AresDynLibManager.h>
#elif defined ARES_PLATFORM_IOS
#include <Core/Graphics/AresRenderDevice.h>
#endif

namespace Ares
{
	template<> Root* Singleton<Root>::m_singleton = NULL;

	// 构造函数
	Root::Root( RenderDeviceType api, const ResPaths& res)
		: m_resSystem(NULL)
		//, m_renderSystem(NULL)
		, m_fxSystem(NULL)
		, m_uiSystem(NULL)
		, m_soundSystem(NULL)
		, m_logSystem(NULL)
		, m_renderDevice( NULL)
	{
		// 日志系统
		BuildLogSystem();

		// 资源系统
		BuildResSystem( res);

		// 效果系统

		// 声音系统
		BuildSoundSystem();

		// 构建渲染设备
		BuildRenderDevice( api);
	}

	Root::~Root()
	{
		SAFE_DELETE( m_resSystem);
		SAFE_DELETE( m_fxSystem);
		SAFE_DELETE( m_uiSystem);
		SAFE_DELETE( m_soundSystem);
		SAFE_DELETE( m_logSystem);
		SAFE_DELETE( m_renderDevice);
	}

	//// 初始化
	//bool Root::Init( CameraSystem* cameraCallback, const RenderSettings& settings, const ResPaths& res)
	//{
	//	bool result = true;

	//	// 日志系统
	//	//result &= BuildLogSystem() ? true : false;

	//	// 资源管理系统
	//	//BuildResSystem( res);

	//	// 渲染系统
	//	//result &= BuildRenderSystem( cameraCallback, settings) ? true : false;

	//	// 效果系统
	//	//result &= BuildFxSystem() ? true : false;

	//	// 界面系统
	//	//result &= BuildUiSystem() ? true : false;

	//	// 场景管理系统
	//	//result &= BuildSceneSystem( cameraCallback) ? true : false;

	//	// 声音系统
	//	//result &= BuildSoundSystem() ? true : false;

	//	return result;
	//}

	// 逻辑模拟
	void Root::FrameMove( float elpasedTime)
	{
		//if( m_uiSystem)
		//	m_uiSystem->frameEvent( elpasedTime);

		if( elpasedTime == 0.f)
			return;

		foreach_( Scene* scene, m_scenes)
		{
			scene->FrameMove( elpasedTime);
		}
	}

	// 渲染
	void Root::Render()
	{
		foreach_( Scene* scene, m_scenes)
		{
			scene->Render();
		}
	}

	// 构建渲染设备
	RenderDevice* Root::BuildRenderDevice( RenderDeviceType api)
	{
		typedef void (*CREATER_RENDER_FACTORY)(RenderDevice*&);

#ifdef ARES_PLATFORM_WINDOWS
		static CDynLibManager*	dynLibMgr = new_ CDynLibManager;		// 动态库管理器

		CDynLib* pLib = NULL;
#if ARES_DEBUG_MODE
		if( api==D3D11)
			pLib = dynLibMgr->Load( "Plugin_D3D11.dll");
		else if( api==OPENGL)
			pLib = dynLibMgr->Load( "Plugin_OpenGL.dll");
		else if( api==OPENGLES)
			pLib = dynLibMgr->Load( "Plugin_OpenGLES.dll");
#else
		if( api==D3D11)
			pLib = dynLibMgr->Load( "Plugin_D3D11.dll");
		else if( api==OPENGL)
			pLib = dynLibMgr->Load( "Plugin_OpenGL.dll");
		else if( api==OPENGLES)
			pLib = dynLibMgr->Load( "Plugin_OpenGLES.dll");
#endif
		if( pLib)
		{
			CREATER_RENDER_FACTORY pFunc = (CREATER_RENDER_FACTORY)pLib->GetSymbol("DllStartRenderFactory");
			if( pFunc)
				(*pFunc)( m_renderDevice);
		}

#elif defined ARES_PLATFORM_IOS
		// 调用入口函数
		DllStartRenderFactory( m_renderDevice);
#endif

		A_ASSERT( m_renderDevice->GetType() == api);

		return m_renderDevice;
	}

	// 构建特效系统
	FxSystem* Root::BuildFxSystem()
	{
		if( !m_fxSystem)
		{
			m_fxSystem = new_ FxSystem;

			A_ASSERT( m_fxSystem);
		}

		return m_fxSystem;
	}

	// 构建界面系统
	MyGUI::Gui*  Root::BuildUiSystem()
	{
		if( !m_uiSystem)
		{
			m_uiSystem = new_ MyGUI::Gui();

			A_ASSERT( m_uiSystem);
		}

		return m_uiSystem;
	}

	// 构建场景管理系统
	Scene* Root::BuildScene( CameraSystem* cameraCallback, const RenderSettings& settings, const char* name, const char* location)
	{
		Scene* scene = Scene::Create( cameraCallback, settings, name, location);

		m_scenes.push_back( scene);

		return scene;
	}

	// 构建声音系统
	SoundSystem* Root::BuildSoundSystem()
	{
		if( !m_soundSystem)
		{
			m_soundSystem = SoundSystem::Create();

			A_ASSERT( m_soundSystem);
		}

		return m_soundSystem;
	}

	// 构建资源管理系统
	ResSystem* Root::BuildResSystem( const ResPaths& res)
	{
		if( !m_resSystem)
		{
			m_resSystem = new_ ResSystem( res);

			A_ASSERT(  m_resSystem);
		}

		return m_resSystem;
	}

	// 构建日志系统
	LogSystem* Root::BuildLogSystem()
	{
		if( !m_logSystem)
		{
			m_logSystem = new_ LogSystem;

			A_ASSERT( m_logSystem);
		}

		return m_logSystem;
	}

	//!
	//! 获取资源管理系统		
	ResSystem& Root::GetResSystem()
	{
		A_ASSERT( m_resSystem);

		return *m_resSystem;
	}

	// 获取渲染工厂
	RenderDevice* Root::GetRenderDevice()
	{
		return m_renderDevice;
	}

	//// 获取渲染系统
	//RenderSystem& Root::GetRenderSystem()
	//{
	//	A_ASSERT( m_renderSystem);

	//	return *m_renderSystem;
	//}

	// 获取粒子特效系统
	FxSystem& Root::GetFxSystem()
	{
		A_ASSERT( m_fxSystem);

		return *m_fxSystem;
	}

	// 获取界面系统
	MyGUI::Gui& Root::GetUiSystem()
	{
		A_ASSERT( m_uiSystem);

		return *m_uiSystem;
	}

	// 获取实体系统
	Scene& Root::GetScene( size_t index)
	{
		A_ASSERT(index < m_scenes.size());

		return *m_scenes[index];
	}

	// 获取声音系统
	SoundSystem& Root::GetSoundSystem()
	{
		A_ASSERT( m_soundSystem);

		return *m_soundSystem;
	}

	// 日志系统
	LogSystem& Root::GetLogSystem()
	{
		A_ASSERT( m_logSystem);

		return *m_logSystem;
	}
}