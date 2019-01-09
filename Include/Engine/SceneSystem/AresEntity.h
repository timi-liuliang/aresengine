#pragma once

#include <stdio.h>
#include <string>
#include <Core/AresCore.h>
#include <Physics/Shapes.h>
#include <Physics/CollisionObject.h>
#include "Engine/RenderSystem/AresRenderable.h"
#include <Engine/CameraSystem/AresCameraSystem.h>

using namespace std;

namespace Ares
{
	//---------------------------------
	// 实体类型 2012-6-21 帝林
	//---------------------------------
	enum EntityType
	{
		ET_Entity,
		ET_Render	= 0,
		ET_Light,
		ET_Sound,
		ET_Physics,
	};

	//----------------------------------
	// 实体详细类型 2012-6-21 帝林
	//----------------------------------
	enum EntityTypeDetail
	{
		ED_Entity,
		ED_Model			= 0,
		ED_Terrain,
		ED_Ocean,
		ED_FxGroup,					// 粒子组
		ED_LightAmbient,
		ED_LightDirection,
		ED_LightPoint,
		ED_LightSpot,
		ED_Physics,
		ED_ClothFabric,				// 布料
		ED_VisualShpae,
	};

	//----------------------------------
	//  渲染实体：2010-07-05   帝林
	//----------------------------------
	class Scene;
	class Entity
	{
	public:
		Entity( EntityType type, EntityTypeDetail typeDetail);
		virtual ~Entity();
	
		// 获取实体名
		const char* GetName();

		// 获取类型
		EntityType GetType() { return m_type; }

		// 获取具体类型
		EntityTypeDetail GetTypeDetail() { return m_typeDetail; }

		// 获取转换对象
		const Transform& GetTransform();

		// 设置转换
		void SetTransform( const Transform& transform);

		// 获取碰撞对象
		CollisionObject& GetCollSift() { return m_collSift; }

		// 设置实体名(保证区域名名称唯一)
		void SetName( const char* name);

		// 获取包围盒
		Rect3 GetLocalBounds();

		// 获取世界包围盒
		Rect3 GetWorldBounds();

		// 获取所属场景
		Scene* GetScene() { return m_scene; }

		// 获取对应摄像机系统
		CameraSystem& GetCameraSystem();

		// 是否为静态渲染实体(unity3d)
		bool IsStatic() const { return m_static; }

		// 设置是否为静态
		void SetStatic( bool _static) { m_static = _static; }

		// 设置实体包围盒
		void  SetLocalBounds( const Rect3& rect);

	public:
		// 更新
		virtual void FrameMove( float fTime);

		// 添加到场景消息
		virtual void OnAddToScene( Scene* scene);

		// 提交渲染元素
		virtual void SubmitRenderElements( RenderElementMgr& renderElementMgr)=0;

	protected:
		bool				m_bEnabled;				// 是否可用
		string				m_name;					// 实体名(用于区分实体)	
		bool				m_static;				// 静态渲染实体可进行光照图烘焙, 静态碰撞实体会进行一定的优化
		EntityType			m_type;					// 类型
		EntityTypeDetail	m_typeDetail;			// 具体类型
		CollisionObject		m_collSift;				// 物理对象(用于可见性筛选等)
		Scene*				m_scene;				// 所属场景 
	};

	typedef s_ptr<Entity>  EntityPtr;
}