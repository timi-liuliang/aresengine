#pragma once

#include <Engine/FxSystem/AresFxEmitter.h>
#include <EditorKit/AresWaterEk.h>
#include "CameraLogic.h"
#include "AILogic.h"
#include "EntityPutLogic.h"
#include "EntityEditLogic.h"
#include "MoveLogic.h"
#include "MapLogic.h"
#include "GpuLightMapBake.h"
#include "PhysicsLogic.h"
#include "ConfigMgr.h"
#include "InputListener.h"
#include "NetLogic.h"
#include "SettingsLogic.h"
#include "WaterEditLogic.h"
#include <Core/AresCore.h>
#include <Engine/FxSystem/AresFxEntity.h>
#include <Engine/SceneSystem/AresScene.h>
#include <EditorKit/BulletDebugDrawer.h>


namespace Ares
{
	// 所有操作类型
	enum EHandleType
	{
		EM_HT_NULL,					// Nothing
		EM_HT_EDITORTERRAIN,		// 编辑地形
		EM_HT_EDITTERRAINTEXTURE,	// 编辑地形纹理
		EM_HT_EDITOENTITY,			// 编辑除地形外的实体(移动 旋转 缩放)
		EM_HT_PUTENTITY,			// 摆放实体
		EM_HT_PUT_WATER,			// 摆放水
		EM_HT_SELECT_WATER,			// 选择水
		EM_HT_BRUSH_WATER,			// 刷水,更新水的高度(根据深度或高度)
		EM_HT_AI,					// AI编辑处理
	};

	//---------------------------------------
	// 编辑器逻辑入口	2010-07-20 帝林	
	//---------------------------------------
	class SCLogic
	{
	public:
		SCLogic( HWND hwnd);
		~SCLogic();

		// 是否暂停中
		bool IsPaused();

		// 更新
		void FrameMove( float fTime);

		// 获取帧率
		float GetFPS()  { return m_fFPS; }

		// 获取窗口句柄
		HWND GetHwnd() { return m_hWnd; }

		// 获取被选中的水片
		//vector<IWaterPatch*>& GetSelectedWaterPatch() { return m_selectedWaterPatch; }

		// 获取水片编辑数据
		SWaterEditorData& GetWaterEditorData() { return m_waterEditorData; }

		// 鼠标右键按下
		void OnMouseRightButtonDown();

		// 设置编辑类型
		void SetHandleType( EHandleType type);

		// 返回窗口句柄
		HWND GetHandle() { return m_hWnd; }

		// 返回编辑类型
		EHandleType GetHandleType() { return m_handleType; }
		
		// 更新帧率
		bool UpdateFPS( float fTime);

		// 设置Alpha测试值
		void SetAlphaTestValue( int value);

		// 设置阴影SoftNess
		void  SetShadowSampleSize( int size);

		// 设置光渗透减少参数
		void SetShadowLBR( float lbr);

		// 设置是否进行模糊
		void SetShadowBlur( bool blur);

		// 设置是否开启阴影
		void SetShadow( bool bShadow);

		// 合并batch
		void  CombinationBatch();

	public:
		// 显示某实体的世界盒
		void ShowEntityWorldBox( s_ptr<Entity>& entity);

		// 更改窗口大小
		void ResizeWindow( size_t width, size_t height);

	public:
		// 获取实体编辑管理器
		CEntityEditLogic* GetEntityEditLogic() { return m_pEntityEditLogic; }

		// 获取实体放置管理器
		CEntityPutLogic* GetEntityPutLogic() { return m_pEntityPutLogic; }

		// 获取地图编辑大
		CMapLogic* GetMapLogic() { return m_mapLogic; }

		// 获取AI逻辑
		CAILogic* GetAILogic() { return m_pAILogic; }

		// 获取物理管理逻辑
		PhysicsLogic* GetPhysicsLogic() { return m_physicsLogic; }

		// 获取光照图烘焙管理器
		GpuLightmapBakerMgr* GetLightMapBakeMgr() { return &m_bakerMgr; }

		// 摄像机逻辑
		CameraLogic* GetCameraLogic() { return m_cameraLogic; }

		// 后处理管理器
		//IImageProcessManager* GetImageProcessMgr() { return m_pImageProcessMgr; }

		// 获取移动逻辑
		CMoveLogic*	GetMoveLogic() { return m_moveLogic; }

		// 获取输入监听器
		InputListener* GetInputListener() { return m_inputListener; }

		// 获取网络管理器
		NetLogic* GetNetLogic() { return m_netLogic; }

		SettingsLogic* GetSettingLogic() { return m_settingLogic; }

		// 获取场景系统
		Scene* GetScene();

	private:
		// 编辑水
		void EditWaterMesh();

	private:
		HWND							m_hWnd;					// 窗口句柄
		float							m_fFPS;					// frames per second   
		SWaterEditorData				m_waterEditorData;		// 水编辑数据
		//vector<IWaterPatch*>			m_selectedWaterPatch;	// 已选择的水片 
		InputListener*					m_inputListener;		// 输入逻辑
		CEntityEditLogic*				m_pEntityEditLogic;		// 实体编辑逻辑	
		CEntityPutLogic*				m_pEntityPutLogic;		// 要摆放的实体
		CWaterEditLogic*				m_waterEditLogic;		// 水编辑逻辑
		CAILogic*						m_pAILogic;				// AI逻辑
		PhysicsLogic*					m_physicsLogic;			// 物理逻辑
		CameraLogic*					m_cameraLogic;		    // 摄像机管理
		//IImageProcessManager*			m_pImageProcessMgr;		// 图象后处理
		CMoveLogic*						m_moveLogic;			// 移动逻辑
		CMapLogic*						m_mapLogic;				// 地图逻辑,负责与引擎交互
		GpuLightmapBakerMgr				m_bakerMgr;				// 光照图烘焙管理器
		NetLogic*						m_netLogic;				// 网络逻辑
		SettingsLogic*					m_settingLogic;			// 设置逻辑
		Scene*							m_scene;				// 当前场景

	public:
		EHandleType						m_handleType;			// 操作标记
	};

	extern  SCLogic*				g_SCLogic;
	#define SCLogic_Main			g_SCLogic
	#define SCLogic_WindowHandle	SCLogic_Main->GetHandle()
	#define SCLogic_Map				SCLogic_Main->GetMapLogic()
	#define Logic_AI				SCLogic_Main->GetAILogic()
	#define Logic_Physics			SCLogic_Main->GetPhysicsLogic()
	#define SCLogic_EntityEdit		SCLogic_Main->GetEntityEditLogic()
	#define Logic_EntityPut			SCLogic_Main->GetEntityPutLogic()
	#define Logic_LightmapBake		SCLogic_Main->GetLightMapBakeMgr()
	#define Logic_TextureSetBuild	SCLogic_Main->GetTextureSetBuild()
	#define SCLogic_Camera			SCLogic_Main->GetCameraLogic()
	#define Logic_ImageProcess		SCLogic_Main->GetImageProcessMgr()
	#define SCLogic_Move			SCLogic_Main->GetMoveLogic()
	#define Logic_Cfg				ConfigMgr::GetSingletonPtr()
	#define SCLogic_Input			SCLogic_Main->GetInputListener()
	#define Logic_Net				SCLogic_Main->GetNetLogic()
	#define Logic_Setting			SCLogic_Main->GetSettingLogic()
	#define SCLogic_Scene			SCLogic_Main->GetScene()
	#define SELogic_RenderSystem	SCLogic_Main->GetScene()->GetRenderSystem()
	#define SELogic_VisualShape		SCLogic_Main->GetScene()->GetRenderSystem().GetVisualShapeMgr()
}