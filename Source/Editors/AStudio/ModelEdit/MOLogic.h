#pragma once

#include "MOLogicHelper.h"
#include "MOCameraLogic.h"
#include "MOConfigMgr.h"
#include "MOModelEdit.h"
#include "MOLightmapUVBuild.h"
#include "MOLightmapUVRender.h"
#include "MOInputListener.h"
#include "MOCollisionBuildLogic.h"
#include "MOTransformLogic.h"
#include "MOLightEditLogic.h"
#include <Engine/SceneSystem/AresScene.h>
#include <EditorKit/Ares3DAxis.h>
#include <Core/AresCore.h>

namespace Ares
{
	//---------------------------------------
	// 编辑器逻辑入口	2010-07-20 帝林	
	//---------------------------------------
	class MOLogic
	{
	public:
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
		};

		MOLogic( HWND hwnd);
		~MOLogic();

	public:
		// 是否暂停中
		bool IsPaused();

		// 添加实体
		void AddEntity( char* namePath);

		// 播放动画
		void PlayAnimation(int index);

		// 更新
		void FrameMove( float fTime);

		// 获取子模型数量
		size_t  GetMeshNum(); 

		// 获取帧率
		float GetFPS()  { return m_fFPS; }

		// 设置编辑类型
		void SetHandleType( EHandleType type);
		
		// 更新帧率
		bool UpdateFPS( float fTime);

		// 设置添充方式
		void SetFillMode(WORD index);

	public:
		// 转换纹理
		void ChangeTextureToDDS( vector<string>& textures);

	public:
		// 显示骨骼坐标系
		void ShowBoneCoord( const Matrix44& coord);

	public:
		// 获取材质文件中材质数
		int GetMaterialNumInFile( const char* matFile);

		// 获取效果编辑逻辑
		ModelEdit* GetModelEdit() { return m_modelEdit; }

		// 获取输入监听器
		MOInputListener* GetInputListener() { return m_inputListener; }

		// 获取UVBuild
		LightmapUVBuild* GetLightmapUVBuild() { return m_lightmapUVBuild; }

		// 获取UVRender
		LightmapUVRender* GetLightmapUVRender() { return m_lightmapUVRender; }

		// 获取碰撞编辑逻辑
		CollisionBuildLogic* GetCollisionBuild() { return m_collisionBuildLogic; }

		// 转换逻辑
		TransformLogic* GetTransformLogic() { return m_transformLogic; }

		// 获取摄像机逻辑
		MOCameraLogic* GetCameraLogic() { return m_cameraLogic; }

		// 获取场景
		Scene*	GetScene() { return m_scene; }

	public:
		// 新建角色
		void NewCharacter( const char* resPath);

		// 更改窗口大小
		void ResizeWindow( int width, int height);

	private:
		EHandleType				m_flags;				// 操作标记
		float                   m_fFPS;					// frames per second

		// 鼠标位置点
		POINT                   m_lastPoint;

	private:
		ModelEdit*				m_modelEdit;			// 返回模型逻辑
		IImageProcessManager*	m_pImageProcessMgr;
		MOCameraLogic*			m_cameraLogic;		    // 摄像机管理
		MOInputListener*		m_inputListener;		// 输入逻辑
		LightmapUVBuild*		m_lightmapUVBuild;		// 光照图UV生成管理器
		LightmapUVRender*		m_lightmapUVRender;		// 光照图UV显示Renderable
		CollisionBuildLogic*	m_collisionBuildLogic;	// 碰撞编辑逻辑
		TransformLogic*			m_transformLogic;		// 转换逻辑
		LightEditLogic*			m_lightLogic;			// 光源管理
		Scene*					m_scene;				// 场景
	};

	extern  MOLogic*					g_MOLogic;
	#define MOLogic_Main				g_MOLogic
	#define MOLogic_Input				MOLogic_Main->GetInputListener()
	#define MOLogic_ModelEdit			MOLogic_Main->GetModelEdit()
	#define MOLogic_LightmapUVBuild		MOLogic_Main->GetLightmapUVBuild()
	#define MOLogic_LightmapUVRender	MOLogic_Main->GetLightmapUVRender()
	#define MOLogic_CollisionBuild		MOLogic_Main->GetCollisionBuild()
	#define MOLogic_Transform			MOLogic_Main->GetTransformLogic()
	#define MOLogic_Camera				MOLogic_Main->GetCameraLogic()
	#define MOLogic_Cfg					MOConfigMgr::GetSingletonPtr()
	#define EntityEdit_Scene			MOLogic_Main->GetScene()
	#define MOLogic_RenderSystem		MOLogic_Main->GetScene()->GetRenderSystem()
	#define MOLogic_VisualShpae			MOLogic_Main->GetScene()->GetRenderSystem().GetVisualShapeMgr()
	#define MOLogic_CameraSystem		MOLogic_Main->GetScene()->GetCameraSystem()
	#define MOLogic_RenderWindow		EntityEdit_Scene->GetRenderSystem().GetRenderWindow()
}