#pragma once

#include "AresEntity.h"
#include "GPTile.h"
#include <string>
#include <map>
#include <vector>
#include <boost/shared_ptr.hpp>

using namespace std;
using namespace boost;

namespace Ares
{
	//----------------------------
	// QtGPTile 2011-08-18 帝林
	//----------------------------
	class SceneTile : public CGPTile
	{
		friend class Scene;
		typedef map<string, s_ptr<Entity> > EntityList;
	public:
		SceneTile( const char* fileLocation);
		~SceneTile();

		// 添加实体
		void AddEntity( const char* uniqueName, EntityPtr pEntity);

		// 删除实体
		void DelEntity( const char* name);

	public:
		// 获取存储位置
		const string& GetLocation() const { return m_location; }

		// 获取包围盒
		void GetBoundingBoxByLayer( Rect3& box, const char* layer, bool update=false);

	protected:
		// 加载数据
		virtual void Load();

		// 卸载数据
		virtual void Unload();

		// 保存Tile
		void Save();

	private:
		string				m_location;		// 块存放位置
		int					m_version;		// 版本号
		EntityList			m_entitys;		// 层
	};
}