//#pragma once
//
//#include "GPWorld.h"
//#include "AresScene.h"
//#include "SceneNode.h"
//#include "LightmapMgr.h"
//
//namespace Ares
//{
//	//------------------------------------
//	// QtWorld 2011-08-18 帝林
//	//------------------------------------
//	class CQtGPWorld : public CGPWorld
//	{
//		//typedef map<string, s_ptr<Entity>> EntityList;
//	public:
//		// 构造函数
//		CQtGPWorld();
//
//		// 设置所属区域
//		//void SetScene( Scene* scene) { m_scene = scene; }
//
//		// 设置主节点
//		//void SetRootNode( const s_ptr<SceneNode>& rootNode);
//
//		// 设置Tile文件保存路径
//		//void  SetWorldLocation( const char* location) { m_location = location; } 
//
//		// 设置范围
//		//void  SetWorldLimits( const Scene::SSize& size, const CGPIndex& gpIdx);
//
//		// 添加实体
//		//bool AddEntity( const char* uniqueName, s_ptr<Entity> pEntity);
//
//		// 添加实体
//		//bool AddEntityToWorldOnly( const char* uniqueName, s_ptr<Entity> pEntity);
//
//		// 添加碰撞到碰撞图中
//		//void AddCollMesh( const char* uniqueName, BvhTriangleMesh* mesh);
//
//		// 更新实体
//		//void FrameMove( float elapsedTime);
//
//	public:
//		// 根据实体名获取实体
//		//s_ptr<Entity>GetEntity( const char* uniqueName);
//
//		// 获取区域
//		//Scene* GetScene() { return m_scene; }
//
//		// 获取包围盒
//		//bool GetBoundingBoxByLayer( Rect3& box, const char* layer, bool update=false);
//
//	public:
//		// 对应索引的区块是否存在
//		//virtual bool IsValidIndex( const CGPIndex& index);
//
//	public:
//		// 设置地形
//		//void SetTerrain( Terrain* terrain);
//
//		//// 保存地形数据
//		//void  SaveTerrain( const char* terLocation);
//
//		//// 加载地形数据
//		//void  LoadTerrain( const char* terLocation);
//
//		// 获取地形
//		//Terrain* GetTerrain() { return m_terrain; }
//
//		// 获取碰撞图
//		//CCollMap& GetCollMap() { return m_collMap; }
//
//		// 获取光照图管理器
//		//LightmapMgr& GetLightmapMgr() { return m_lightmapMgr; }
//
//	public:
//		// 保存
//		//void Save();
//
//	protected:
//		// 构建Tile
//		virtual CGPTile* BuildTile( const CGPIndex& index);
//
//		// 销毁Tile
//		virtual void  DestroyTile( const CGPIndex& index);
//
//	protected:
//		string				m_location;			// world文件存放路径
//		//int					m_xTiles;			// x方向块数量
//		//int					m_yTiles;			// y方向块数量
//		CGPIndex			m_mainGPIdx;		// 主GP索引,决定tile大小等
//		Scene::SSize		m_size;				// 在世界中范围
//		//Scene*				m_scene;			// 所属场景
//		//EntityList			m_entiys;			// 实体列表
//		//s_ptr<SceneNode>		m_rootNode;			// 场景根结点	
//
//		//Terrain*			m_terrain;			// 地形			★区域资源★
//		//CCollMap			m_collMap;			// 碰撞图		★碰撞数据★
//		//LightmapMgr			m_lightmapMgr;		// 光照图管理器 ★光照数据★
//
//		//CriticalSection		m_qtCritication;	// 保护实例列表
//		//CriticalSection		m_rtCritication;
//	};
//}