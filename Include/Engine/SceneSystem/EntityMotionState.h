#pragma once

#include <Physics/Collision/MotionState.h>

namespace Ares
{
	//------------------------------------------
	// EntityMotionState 2012-7-30 帝林
	// Synchronize world transform with graphics
	//------------------------------------------
	class EntityMotionStae : public MotionState
	{
	public:
		// 构造函数
		EntityMotionStae( Entity* entity)
			: m_entity(entity)
		{}

		// 获取初始位置
		virtual void GetWorldTransform( Transform& worldTrans) const
		{

		}

		// 设置位置 the update only be calls for active objects
		virtual void SetWorldTransform( const Transform& worldTrans)
		{

		}

	private:
		Entity*		m_entity;			// 场景实体
	};
}