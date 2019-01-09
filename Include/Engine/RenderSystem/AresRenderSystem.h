#pragma once

#include <map>
#include <string>
#include <boost/shared_ptr.hpp>
#include <Core/Graphics/AresRenderWindow.h>
#include <Core/Graphics/AresRenderDevice.h>
#include "Engine/Foundation/Ares3DEngine_h.h"
#include "Engine/RenderSystem/DeferredRenderingMgr.h"
#include "Engine/RenderSystem/AresPostProcessChain.h"
#include "Engine/RenderSystem/AresGrayscale.h"
#include "Engine/RenderSystem/AresSpaceDistorted.h"
#include "Engine/RenderSystem/AresFXAA.h"
#include "Engine/RenderSystem/AresToneMapping.h"
#include <Engine/UISystem/MyGUI_RTTexture.h>

using namespace std;
using namespace boost;

namespace Ares
{
	//------------------------------------------
	// RenderSystem 2011-08-04 帝林
	// Plugin Interfaces of RenderSystem
	//------------------------------------------
	class IImageProcessManager;
	class ILightManager;
	class VisualShapeMgr;
	class ARES_EXPORT RenderSystem
	{
	public:
		static ShaderParamPtr  SPAB_SunDirection;		// 方向光朝向
		static ShaderParamPtr  SPAB_ScreenWidthHeight;	// 屏幕宽高

	public:
		// 构造函数
		RenderSystem( Scene& scene);
		~RenderSystem();

		// LoadRenderFactory, Unique
		BOOL LoadRenderFactory( const RenderSettings& settings);

		// 获取渲染窗口
		RenderWindow* GetRenderWindow() { return m_renderWindow; }

		// 更改窗口大小
		void ResizeWindow( size_t width, size_t height);

		// 更新
		void FrameMove( float elapsedTime);

		// 执行渲染
		void Render();

		// 提交渲染元素
		signal<void()> Signal_OnSubmitRenderElements;

	public:
		// 获取渲染设置
		const RenderSettings& GetRenderSettings() const { return m_renderSettings; }

		// 应用渲染设置
		void ApplyRenderSettings( const RenderSettings& settings);

		// 获取延迟渲染管理器指针
		const DeferredRenderingMgrPtr& GetDeferredRenderingMgr() { return m_deferredRenderingMgr; }

		// 获取后处理管理器
		PostProcessChainPtr& GetPostProcessChain() { return m_postProcessChain; }

		// 获取光源管理器
		ILightManager* GetLightManager(){ return NULL; }

		// 获取渲染队列管理器
		RenderElementMgr& GetRenderElementMgr() { return m_renderElementMgr; }

		// VisualShapeMgr
		VisualShapeMgr& GetVisualShapeMgr() { return *m_visualShapeMgr; }

		// 获取全局Shader参数
		ShaderParamPtr GetSPABWaterRefraction() { return m_spabWaterRefraction; }

		// 获取窗口宽高
		ShaderParamPtr GetSPABWindowWH()	{ return m_spabWindowWH; }

	private:
		// 初始化延迟渲染管理器
		void InitRenderingMgr( size_t width, size_t height);

		// 导出类(序列化)
		void ExportClass();

		// 组织渲染元素
		void OrganizeRenderElements();

	private:
		RenderSettings			m_renderSettings;		// 渲染设置
		RenderWindow*			m_renderWindow;			// 渲染窗口
		RenderElementMgr		m_renderElementMgr;		// 渲染队列管理器
		DeferredRenderingMgrPtr	m_deferredRenderingMgr;	// 延迟渲染管理器
		PostProcessChainPtr		m_postProcessChain;		// 后处理管理器
		VisualShapeMgr*			m_visualShapeMgr;		// 基本几何体管理器
		GrayscaleFilter*		m_grayScaleFilter;		// 黑白滤镜
		SpaceDistorted*			m_spaceDistortedFilter;	// 空间扭曲滤镜
		FXAA*					m_FXAA;					// 反锯齿
		ToneMappingPtr			m_toneMapping;			// 色调映射
		MyGUI::MyGUIRTTexture*	m_uiRTTexture;			// UI渲染
		Scene&					m_scene;				// 场景

		ShaderParamPtr			m_spabWaterRefraction;	// 折射贴图
		ShaderParamPtr			m_spabWindowWH;		// 窗口宽高
	};
}