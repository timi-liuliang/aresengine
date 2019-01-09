//#pragma once
//
//#include "AresEntity.h"
//#include <Physics/CollMap.h>
//#include <Physics/Rect3.h>
//#include <Physics/Frustum3.h>
//#include "Engine/RenderSystem/Terrain/AresTerrain.h"
//
//namespace Ares
//{
//	//
//	// SMapWindow
//	//
//	//struct SNavWindow
//	//{
//	//	float  m_x;			// 中心点x
//	//	float  m_y;			// 中心点y
//	//	float  m_radius;	// 半径
//	//};
//
//	//---------------------------------
//	// 区域接口 2011-08-16 帝林
//	//---------------------------------
//	class IArea
//	{
//	public:
//		//
//		// 2D区域
//		//
//		struct SSize
//		{
//			size_t  m_posX;
//			size_t  m_posY;
//			size_t  m_width;
//			size_t  m_height;
//		};
//
//	public:
//		// 虚析构函数
//		virtual ~IArea() {}
//
//	public:
//		// 设置导航窗口(中心点x,y; 半径radius)
//		virtual void SetNavWindow( const SNavWindow& window)=0;
//
//		// 更新
//		virtual void FrameMove( float fTime)=0;
//
//		// 搜寻
//		virtual Entity* BuildSearchResults(const Rect3& worldRect, const Frustum3* pOptionalFrustum/*=NULL*/)=0;
//
//	public:
//		// 获取当前加载进度
//		virtual float GetLoadingProgress()=0;
//
//		// 获取包围盒
//		virtual bool GetBoundingBoxByLayer( Rect3& box, const char* layer, bool update=false)=0;
//
//	public:
//		// 获取地图名
//		virtual const char* GetName()=0;
//
//		// 获取领域
//		virtual const SSize& GetSize()=0;
//
//		// 获取世界包围盒
//		virtual const Rect3& GetWorldBounds()=0;
//
//		// 获取根结点
//		virtual SPtr<SceneNode> GetRootNode()=0;
//
//		// 根据实体名获取实体
//		virtual SPtr<Entity> GetEntity( const char* uniqueName)=0;
//
//		// 获取地形
//		virtual Terrain* GetTerrain()=0;
//
//		// 获取碰撞图
//		virtual CCollMap& GetCollMap()=0;
//
//	public:
//		// 设置地图名
//		virtual void SetName( const char* name)=0;
//
//		// 设置区域保存位置
//		virtual void SetLocation( const char* location)=0;
//
//		// 设置领域
//		virtual void SetSize( const SSize& size)=0;
//
//		// 设置地形
//		virtual void SetTerrain( Terrain* terrain)=0;
//
//		// 添加实体
//		virtual bool AddEntity( const char* uniqueName, SPtr<Entity> pEntity)=0;
//
//	public:
//		// 加载
//		virtual void Load( const char* fileName)=0;
//
//		// 保存
//		virtual void Save( const char* fileName)=0;
//	};
//}