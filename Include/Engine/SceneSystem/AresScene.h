#pragma once

#include <string>
#include "GPWindow.h"
#include "GPWorld.h"
#include "LightmapMgr.h"
#include <Core/Graphics/AresRenderSettings.h>
#include <Physics/Shapes.h>
#include "Engine/Foundation/Ares3DEngine_h.h"
#include "Engine/RenderSystem/AresRenderSystem.h"
#include "Engine/RenderSystem/AresTerrain.h"
#include <Engine/RenderSystem/ZWriteMgr.h>
#include "Engine/CameraSystem/AresCameraSystem.h"
#include <Physics/PhysicsWorld.h>
#include <Physics/Bullet/btBulletDynamicsCommon.h>

using namespace std;

namespace Ares
{
	// SMapWindow
	struct SNavWindow
	{
		float  m_x;			// 中心点x
		float  m_y;			// 中心点y
		float  m_radius;	// 半径

		// 是否重叠包围盒
		bool IsOverlapping( Rect3& rect);
	};

	//---------------------------------
	// Scene 2011-08-16 帝林
	//---------------------------------
	class SceneTile;
	class Scene : public CGPWorld
	{
	public:
		friend class SceneTile;
		friend class SceneSystem;
		typedef map<string, EntityPtr> EntityList;
	public:
		~Scene();

		// 新建场景
		static Scene* Create( CameraSystem* cameraCallback, const RenderSettings& settings, const char* name, const char* location);

		// 设置导航窗口(中心点x,y; 半径radius)
		void SetNavWindow(  float x, float y, float radius);

		// 更新
		void FrameMove( float fTime);

		// 渲染
		void Render();

		// 提交渲染元素
		void SubmitRenderElements();

	public:
		// 设置物理世界
		void SetPhysicsWorld( PhysicsWorldPtr& physicsWorld) { m_physicsWorld=physicsWorld; }

		// 获取物理世界
		PhysicsWorldPtr& GetPhysicsWorld() { return m_physicsWorld; }

		// 搜寻(Frustum Box3)
		void BuildSearchResults( vector<Entity*>& results, const Shape* shape);

		// 搜寻光源
		void SearchLights( vector<Light*>& lights, DirectionalLightPtr& domainLight);

		// 获取场景中所有静态实体
		const EntityList& GetStaticEntitys() const { return m_entitys; }

	public:
		// 获取包围盒
		bool GetBoundingBoxByLayer( Rect3& box, const char* layer, bool update=false);

	public:
		// 获取地图名
		const char* GetName();

		// 是否暂停
		bool IsPaused() const { return !m_isActive; }
		// 重置
		void Reset() {}

		// 获取摄像机系统
		CameraSystem& GetCameraSystem() { return *m_cameraSystem; }

		// 获取渲染系统
		RenderSystem& GetRenderSystem() { return *m_renderSystem; }

		// 获取SPAB
		ShaderParamPtr GetSPABElapsedTime() { return m_spabTime; }
		ShaderParamPtr GetSPABSunColor()	{ return m_spabSunColor; }
		ShaderParamPtr GetSPABSunDir()		{ return m_spabSunDir; }

		// 获取存储位置
		const string& GetLocation() const { return m_location; }

		// 获取世界包围盒
		const Rect3& GetWorldBounds() { return m_worldBounds; }

		// 根据实体名获取实体
		EntityPtr GetEntity( const char* uniqueName);

		// 获取光照图管理器
		LightmapMgr& GetLightmapMgr() { return m_lightmapMgr; }

		// 获取所属场景系统
		SceneSystem* GetSceneSystem() { return m_sceneSystem; }

	public:
		// 设置地图名
		virtual void SetName( const char* name);

		// 设置区域保存位置
		virtual void SetLocation( const char* location);

		// 设置是否活动
		void SetActived( bool isActive) { m_isActive = isActive; }

		// 创建光源
		LightPtr CreateLight( const char* uniqueName, LightType type);

		// 添加实体
		bool AddEntity( const char* uniqueName, s_ptr<Entity> pEntity, bool isSave=true);

		// 设置主光源
		bool SetDomainLight( LightPtr light, size_t resolution);

		// 获取主光源
		DirectionalLightPtr GetDomainLight() { return m_domainLight; }

		// 删除实体
		void DelEntity( const char* uniqueName);

		// 实体变动信号
		signal<void( const Scene&)> Signal_OnEntitysChanged;

	public:
		// 构建Tile
		virtual CGPTile* BuildTile( const CGPIndex& index);

		// 销毁Tile
		virtual void  DestroyTile( const CGPIndex& index);

		// 加载
		virtual void Load( const char* fileName);

		// 保存
		virtual void Save( const char* fileName=0);

		// 渲染场景深度
		void RenderDepth( const Box3& searchBox, const Matrix44& viewCrop);

	private:
		Scene( CameraSystem* cameraCallback, const RenderSettings& settings);

		// 整理实体
		void TidyEntityByTile();

		// 添加实体
		bool AddEntityToWorldOnly( const char* uniqueName, s_ptr<Entity> pEntity);

	private:
		int						m_version;			// 当前版本号
		string					m_name;				// 区域名
		string					m_location;			// 位置

		SNavWindow				m_navWindow;		// 导航窗口

		Rect3					m_worldBounds;		// 世界包围盒
		CGPIndex				m_mainGPIdx;		// 主GP索引,决定tile大小等

		CGPWindow*				m_gpWindow;			// 导航窗口
		
		PhysicsWorldPtr			m_physicsWorld;		// 物理世界
		btDynamicsWorld*		m_dynamicsWorld;	// Bullet物理世界

		EntityList				m_entitys;			// 实体列表
		DirectionalLightPtr		m_domainLight;		// 主光源
		LightmapMgr				m_lightmapMgr;		// 光照图管理器 ★光照数据★
		ZWriteMgr				m_zWriteMgr;		// 深度渲染管理器

		SceneSystem*			m_sceneSystem;

		boost::recursive_mutex	m_qtCritication;	// 保护实例列表
		boost::recursive_mutex	m_rtCritication;

		boost::signals2::connection m_submitRenderElementsConnection;	// 渲染链接

		ShaderParamPtr			m_spabTime;			// 场景时间
		ShaderParamPtr			m_spabSunDir;		// 主光源朝向
		ShaderParamPtr			m_spabSunColor;		// 主光源颜色

		bool					m_isActive;				// 活跃状态
		CameraSystem*			m_cameraSystem;			// 摄像机系统
		RenderSystem*			m_renderSystem;			// 渲染系统
	};
}