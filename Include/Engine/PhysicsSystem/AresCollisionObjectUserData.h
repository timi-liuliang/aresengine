#ifndef ARES_COLLISION_OBJECT_USERDATA_H
#define ARES_COLLISION_OBJECT_USERDATA_H

namespace Ares
{
	enum COUserDataType
	{
		COU_EntityFilter,		// 实体筛选
	};

	//------------------------------------
	// 实体筛选 2013-7-8 帝林
	//------------------------------------
	class Entity;
	struct COUserDataEntityFilter : public CollisionObject::UserData
	{
		Entity*		m_entity;

		// 构造函数
		COUserDataEntityFilter( Entity* entity)
			: CollisionObject::UserData( COU_EntityFilter)
			, m_entity( entity)
		{}
	};
}

#endif