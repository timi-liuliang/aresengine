#pragma once

#include <map>
#include <deque>
#include "GPTile.h"
#include <Core/AresCriticalSection.h>
#include <Core/AresThreadLoopObject.h>
#include <boost/signals2/signal.hpp>

using namespace std;
using namespace boost;
using namespace boost::signals2;

namespace Ares
{
	//---------------------------------------------------------
	// "World" of GP 2011-08-18 帝林
	//  Description:
	//      The GPWorld  loads and unloads the GPTiles that the
	//  GPWindows needs. when a GPTile is unloaded and no other
	//  GPWindow needs it, the GPWorld automatically destroys it 
	//  int order to get a better performance and an optimal use
	//  of the system memory( and other resources)
	//---------------------------------------------------------
	class CGPWorld : public CThreadLoopObject
	{
	public:
		// 状态
		enum SGRPTileAction
		{
			EM_FOR_LOAD,
			EM_FOR_UNLOAD
		};

		// GPTileListData
		struct SGPTileListData
		{
			SGRPTileAction	m_state;
			CGPTile*		m_tile;
		};

		typedef map<CGPIndex, CGPTile*> GPTileMap;

	public:
		// 构造函数
		CGPWorld();

		// 析构函数
		virtual ~CGPWorld();

	public:
		// 获取当前加载进度
		float GetLoadingProgress();

		// 设置当前加载进度
		void SetLoadingProgress( float progress);

		// 开始(创建线程,恢复线程)
		virtual void Start( DWORD hz=0);

		// 进度信号
		signal<void(float)> Signal_LoadingProgress;

	protected:
		// 更新
		virtual void Loop();

	public:
		// 获取Tile
		CGPTile* GetTile( const CGPIndex& index);

	public:
		// 加载区块 
		void LoadTile( CGPTile* pTile);

		// 卸载区块
		void UnloadTile( CGPTile* pTile);

	public:
		// 对应索引的区块是否存在
		virtual bool IsValidIndex( const CGPIndex& index);

		// 设置卸载标记
		void SetAutoUnload( bool unload) { m_unloadEnable=unload; }

	protected:
		// 构建Tile
		virtual CGPTile* BuildTile( const CGPIndex& index);

		// 销毁Tile
		virtual void  DestroyTile( const CGPIndex& index);

	public:
		// 清除资源
		void Clean();

	protected:
		GPTileMap				m_tilesMap;				// 世界中当前区块
		deque<SGPTileListData>	m_loadUnloadTileList;	// 加载卸载队列
		boost::recursive_mutex	m_critication;			// 临界区
		float					m_loadingProgress;		// 资源加载进度
		bool					m_unloadEnable;			// 是否可卸载
	};
}