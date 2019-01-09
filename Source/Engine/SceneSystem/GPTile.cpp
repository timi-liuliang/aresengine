#include <Engine/SceneSystem/GPTile.h>

namespace Ares
{
	// 构造函数
	CGPTile::CGPTile()
		: m_world( 0),
		  m_state( EM_UNLOADED)
	{

	}

	// 析构函数
	CGPTile::~CGPTile()
	{
		m_world = 0;
	}

	// 执行加载
	void CGPTile::DoLoad()
	{
		if( m_state != EM_LOADED && m_state != EM_LOADING)
		{
			m_state = EM_LOADING;

			Load();

			m_state = EM_LOADED;
		}
	}

	// 执行卸载
	void CGPTile::DoUnload()
	{
		if( m_state != EM_UNLOADED && m_state != EM_UNLOADING)
		{
			m_state = EM_UNLOADING;

			Unload();

			m_state = EM_UNLOADED;
		}
	}
}