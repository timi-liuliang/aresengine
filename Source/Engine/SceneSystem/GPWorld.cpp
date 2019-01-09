#include <Engine/SceneSystem/GPWorld.h>

namespace Ares
{
	// 构造函数
	CGPWorld::CGPWorld()
		: CThreadLoopObject()
		, m_unloadEnable(true)
	{
		ENTER_CRITICAL_SECTION( tHoder, m_critication);

		m_tilesMap.clear();
		m_loadUnloadTileList.clear();
		m_loadingProgress = 0.f;

		//SetPriority(THREAD_PRIORITY_BELOW_NORMAL);

		Start( 20);
	}

	// 析构函数
	CGPWorld::~CGPWorld()
	{
		ENTER_CRITICAL_SECTION( tHoder, m_critication);

		Finish();

		Clean();
	}

	// 获取Tile
	CGPTile* CGPWorld::GetTile( const CGPIndex& index)
	{
		ENTER_CRITICAL_SECTION( tHoder, m_critication);

		CGPTile* pTile = 0;

		if( IsValidIndex( index))
		{
			GPTileMap::const_iterator it = m_tilesMap.find( index);
			if( it == m_tilesMap.end())
			{
				pTile = BuildTile( index);
				if( pTile)
				{
					pTile->SetIndex( index);
					pTile->SetWorld( this);

					m_tilesMap[index] = pTile;
				}
			}
			else
			{
				pTile = it->second;
			}
		}

		return pTile;
	}

	// 加载区块 
	void CGPWorld::LoadTile( CGPTile* pTile)
	{
		ENTER_CRITICAL_SECTION( tHoder, m_critication);

		if( pTile)
		{
			bool found = false;

			deque<SGPTileListData>::iterator it=m_loadUnloadTileList.begin();
			for( ; it!=m_loadUnloadTileList.end(); it++)
			{
				if( pTile == it->m_tile)
				{
					found = true;
					break;
				}
			}

			// 该Tile已在加载卸载列表
			if( found)
			{
				if( it->m_state == EM_FOR_LOAD)
				{
					if( it->m_tile->GetState() == CGPTile::EM_LOADED)
						m_loadUnloadTileList.erase( it);
				}
				else if( it->m_state == EM_FOR_UNLOAD)
				{
					it->m_state = EM_FOR_LOAD;
				}
			}
			else
			{
				SGPTileListData tileListData;
				tileListData.m_state = EM_FOR_LOAD;
				tileListData.m_tile  = pTile;

				//if( !m_loadUnloadTileList.size())
				//	Start( 30);

				m_loadUnloadTileList.push_back( tileListData);
			}
		}
	}

	// 卸载区块
	void CGPWorld::UnloadTile( CGPTile* pTile)
	{
		ENTER_CRITICAL_SECTION( tHoder, m_critication);

		if( pTile)
		{
			bool found = false;

			deque<SGPTileListData>::iterator it=m_loadUnloadTileList.begin();
			for( ; it!=m_loadUnloadTileList.end(); it++)
			{
				if( pTile == it->m_tile)
				{
					found = true;
					break;
				}
			}

			// 该tile已在装载卸载列表
			if( found)
			{
				if( it->m_state == EM_FOR_LOAD)
				{
					it->m_state = EM_FOR_UNLOAD;
				}
				else if( it->m_state == EM_FOR_UNLOAD)
				{
					if( it->m_tile->GetState() == CGPTile::EM_UNLOADED)
						m_loadUnloadTileList.erase( it);
				}
			}
			else
			{
				SGPTileListData tileListData;
				tileListData.m_state = EM_FOR_UNLOAD;
				tileListData.m_tile  = pTile;

				//if( !m_loadUnloadTileList.size())
				//	Start( 30);

				m_loadUnloadTileList.push_back( tileListData);
			}
		}
	}

	// 开始(创建线程,恢复线程)
	void CGPWorld::Start(  DWORD hz)
	{
		ENTER_CRITICAL_SECTION( tHoder, m_critication);

		if( m_loadUnloadTileList.size())
			CThreadLoopObject::Start( hz);
	}

	// 更新
	void CGPWorld::Loop()
	{
		size_t needLoad = m_loadUnloadTileList.size();
		while( m_loadUnloadTileList.size())
		{
			// 移除
			SGPTileListData tileListData;
			{
				ENTER_CRITICAL_SECTION( tHoder, m_critication);

				tileListData = (*m_loadUnloadTileList.begin());
				m_loadUnloadTileList.erase( m_loadUnloadTileList.begin());
			}

			if( tileListData.m_state == EM_FOR_LOAD)
			{
				tileListData.m_tile->DoLoad();
			}
			else if( tileListData.m_state == EM_FOR_UNLOAD && m_unloadEnable)
			{
				tileListData.m_tile->DoUnload();

				DestroyTile( tileListData.m_tile->GetIndex());
			}

			float process = 1.f - (float)m_loadUnloadTileList.size() / (float)needLoad;
			SetLoadingProgress( process);
		}

		// 加载完毕
		SetLoadingProgress( 1.f);

		Pause();
	}

	// 获取当前加载进度
	float CGPWorld::GetLoadingProgress()
	{
		ENTER_CRITICAL_SECTION( tHoder, m_critication);

		return m_loadingProgress;
	}

	// 设置当前加载进度
	void CGPWorld::SetLoadingProgress( float progress)
	{
		ENTER_CRITICAL_SECTION( tHoder, m_critication);

		m_loadingProgress = progress;
	}

	// 销毁Tile
	void  CGPWorld::DestroyTile( const CGPIndex& index)
	{
		CGPTile* pTile = NULL;
		{
			ENTER_CRITICAL_SECTION( tHoder, m_critication);

			GPTileMap::iterator it = m_tilesMap.find( index);
			if( it != m_tilesMap.end())
			{
				pTile = it->second;
				pTile->SetWorld( 0);

				for ( deque<SGPTileListData>::iterator itd=m_loadUnloadTileList.begin(); itd!=m_loadUnloadTileList.end(); itd++)
				{
					if( pTile == itd->m_tile)
					{
						m_loadUnloadTileList.erase( itd);
						break;
					}
				}

				m_tilesMap.erase( it);
			}
		}

		SAFE_DELETE( pTile);
	}

	// 对应索引的区块是否存在
	bool CGPWorld::IsValidIndex( const CGPIndex& index)
	{
		// to redefine if it is needed
		return true;
	}

	// 构建Tile
	CGPTile* CGPWorld::BuildTile( const CGPIndex& index)
	{
		CGPTile* pTile = new_ CGPTile;
		
		return pTile;
	}

	// 清除资源
	void CGPWorld::Clean()
	{
		ENTER_CRITICAL_SECTION( tHolder, m_critication);

		m_loadUnloadTileList.clear();

		GPTileMap::iterator itEnd = m_tilesMap.end();
		for ( GPTileMap::iterator it = m_tilesMap.begin(); it!=itEnd; it++)
		{
			CGPTile* pTile = it->second;
			pTile->DoUnload();
			pTile->SetWorld( NULL);
			delete pTile;
			pTile = NULL;
		}

		m_tilesMap.clear();
	}
}