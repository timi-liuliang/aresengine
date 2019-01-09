#pragma once

#include <map>
#include <string>
#include "Engine/RenderSystem/AresTextureSet.h"

using namespace std;

namespace Ares
{
	//---------------------------------
	// 场景光照图管理器 2012-7-7 帝林
	//     添加,删除,合并,保存
	//---------------------------------
	class CORE_EXPORT LightmapMgr
	{
	public:
		// TextureTile
		struct TileInfo
		{
			string  m_uniqueName;	// 唯一标识
			int		m_setIdx;		// 纹理索引
			int		m_tileId;		// 纹理块ID
		};
		typedef map<string, TileInfo> TileInfoMap;

	public:
		LightmapMgr();
		LightmapMgr( int width, int height);
		~LightmapMgr();

		// 添加光照图(以实体名与mesh索引命名)
		bool AddLightmap( const char* uniqueName, TexturePtr& texture);

		// 查询
		bool Query( const char* uniqueName, TexturePtr& lightmap, Vector4& lightmapViewPort);

		// 清除所有
		void Clear();

		// 加载
		bool Load( const char* locationPath);

		// 保存
		bool Save( const char* locationPath);

		// 加载xml格式
		bool LoadXml( const char* locationPath);

		// 保存xml格式
		bool SaveXml( const char* locationPath);

	private:
		// 新建纹理集
		TextureSet* AllocateTextureSet( int width, int height);

		// 添加到指定纹理集
		bool AddLightmapToTextureSet( TextureSet* texSet, int setIdx, const char* uniqueName, TexturePtr& texture);

		// 覆写纹理集
		bool OverWriteLightmapToTextureSet( TextureSet* texSet, int tileId, TexturePtr& texture);

	private:
		int						m_tsWidth;		// TextureSet宽
		int						m_tsHeight;		// TextureSet高
		TileInfoMap				m_tileInfos;	// 块信息
		vector<TextureSet*>		m_texSets;		// 纹理集
	};
}