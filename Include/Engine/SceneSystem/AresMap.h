//#pragma once
//
//#include "AresArea.h"
//
//namespace Ares
//{
//	//---------------------------------
//	// 区域管理器接口 2011-08-16 帝林
//	//---------------------------------
//	class IMap
//	{
//	public:
//		// 虚析构函数
//		virtual ~IMap() {}
//
//	public:
//		// 设置Window(中心点x,y; 半径radius)
//		virtual void SetNavWindow( float x, float y, float radius)=0;
//
//		// 更新
//		virtual void FrameMove( float fTime)=0;
//
//		// 渲染
//		virtual void Render()=0;
//
//	public:
//		// 获取当前加载进度
//		virtual float GetLoadingProgress()=0;
//
//		// 获取包围盒
//		virtual const Rect3& GetBoundingBox()=0;
//
//		// 获取包围盒
//		virtual bool GetBoundingBoxByLayer( Rect3& box, const char* layer, bool update=false)=0;
//
//	public:
//		// 获取地图名
//		virtual const char* GetName()=0;
//
//		// 获取区域数量
//		virtual size_t GetAreaNum()=0;
//
//		// 获取区域
//		virtual IArea* GetArea( size_t idx)=0;
//
//		// 获取区域
//		virtual IArea*	GetArea( const char* areaName)=0;
//
//	public:
//		// 设置地图名
//		virtual void SetName( const char* name)=0;
//
//		// 添加区域
//		virtual bool AddArea( IArea* pArea)=0;
//
//	public:
//		// 加载
//		virtual void Load( const char* fileName)=0;
//
//		// 保存
//		virtual void Save( const char* fileName=0)=0;
//	};
//}