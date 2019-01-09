#pragma once

#include "GPIndex.h"

namespace Ares
{
	//---------------------------------------
	// 搜索空间中的块 2011-08-18  帝林
	//     Tile对应一块地形,其位置和大小是由
	// 索引来定义的
	//---------------------------------------
	class CGPWorld;
	class CGPTile
	{
	public:
		// 加载状态
		enum GPtileState
		{
			EM_LOADING,
			EM_UNLOADING,
			EM_LOADED,
			EM_UNLOADED,
		};

	public:
		// 构造函数
		CGPTile();

		// 析构函数
		virtual ~CGPTile();

	public:
		// 获取状态
		GPtileState GetState() { return m_state; }

		// 获取索引
		const CGPIndex& GetIndex() { return m_index; }

		// 获取世界
		CGPWorld* GetWorld(){ return m_world; }

	public:
		// 设置状态
		void SetState( GPtileState state) { m_state = state; }

		// 设置索引
		void SetIndex( const CGPIndex& index) { m_index = index; }

		// 设置世界
		void SetWorld( CGPWorld* pWorld) { m_world = pWorld; }

	public:
		// 执行加载(状态切换)
		void DoLoad();

		// 执行卸载(状态切换)
		void DoUnload();

	protected:
		// 加载数据
		virtual void Load() { /* To Redefine */}

		// 卸载数据
		virtual void Unload() { /* To Redefine */}

	protected:
		CGPWorld*	m_world;			// 世界

		CGPIndex	m_index;			// 块索引
		GPtileState m_state;			// 状态
	};
}