#pragma once

#include <Core/AresCore.h>
#include <Engine/Foundation/Ares3DEngine_h.h>
#include <Engine/RenderSystem/AresRenderSystem.h>
#include <Engine/ResSystem/AresResSystem.h>
#include <Engine/SceneSystem/AresScene.h>
#include <Engine/FxSystem/AresFxSystem.h>
#include <Engine/SoundSystem/AresSoundSystem.h>
#include <Engine/CameraSystem/AresCameraSystem.h>
#include <Engine/UISystem/AresUISystem.h>

namespace Ares
{
	//-----------------------------------------------------------
	// Ares Root 2011-08-04  帝林
	// 参考: Ogre
	// The root class of the Ares
	// The root class represents a starting point for the client
	// applicatio. From here, the application can gain access to 
	// the fundamentals of the system, namely the rendering systems
	// available, management of saved configurations, logging, and
	// access to other objects may be reached, An instance of Root 
	// must be creted before any other Ares operations are called.
	// Once an instancce has been created, the same instance is 
	// accessible throughout the life of that object by using Root::
	// GetSingleton()
	class ARES_EXPORT Root : public Singleton<Root>
	{
	public:
		// 构造函数
		Root( RenderDeviceType api, const ResPaths& res);
		~Root();

		// 构建场景管理系统
		Scene* BuildScene( CameraSystem* cameraCallback, const RenderSettings& settings, const char* name, const char* location);

		// 逻辑模拟
		void FrameMove( float elpasedTime);

		// 渲染
		void Render();

	public:
		//! 获取资源管理系统
		ResSystem& GetResSystem();

		//! 获取渲染工厂
		RenderDevice* GetRenderDevice();

		//! 获取粒子特效系统
		FxSystem& GetFxSystem();

		// 获取界面系统
		MyGUI::Gui& GetUiSystem();

		//! 获取实体系统
		Scene& GetScene( size_t index);

		//! 获取声音系统
		SoundSystem& GetSoundSystem();

		// 日志系统
		LogSystem& GetLogSystem();

		// 设置渲染设备
		void SetRenderDevice( RenderDevice* device) { m_renderDevice = device; }

	private:
		// 构建渲染系统
		//RenderSystem* BuildRenderSystem( CameraSystem* cameraCallback, const RenderSettings& settings);

		// 构建渲染设备
		RenderDevice* BuildRenderDevice( RenderDeviceType api);

		// 构建特效系统
		FxSystem*	 BuildFxSystem();

		// 构建界面系统
		MyGUI::Gui*  BuildUiSystem();

		// 构建声音系统
		SoundSystem*  BuildSoundSystem();

		// 构建资源管理系统
		ResSystem*	 BuildResSystem( const ResPaths& res);

		// 构建日志系统
		LogSystem*   BuildLogSystem();
	
	private:
		ResSystem*			m_resSystem;		// 资源管理系统
		FxSystem*			m_fxSystem;		    // 特殊效果系统
		MyGUI::Gui*			m_uiSystem;			// 界面系统
		vector<Scene*>		m_scenes;			// 场景系统
		SoundSystem*		m_soundSystem;		// 声音系统
		LogSystem*			m_logSystem;		// 日志系统
		RenderDevice*		m_renderDevice;		// 渲染设备
	};

	#define ARoot					(Root::GetSingletonPtr())
	#define ARenderDevice			(ARoot->GetRenderDevice())
	#define AResSystem				(ARoot->GetResSystem())
	#define ALogSystem				(ARoot->GetLogSystem())
	#define AFxSystem				(ARoot->GetFxSystem())
	#define AUiSystem				(ARoot->GetUiSystem())
}