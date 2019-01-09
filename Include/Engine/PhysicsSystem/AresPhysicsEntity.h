#ifndef ARES_PHYSICS_ENTITY_H
#define ARES_PHYSICS_ENTITY_H

#include <Physics/CollisionObject.h>
#include <Engine/SceneSystem/AresEntity.h>

namespace Ares
{
	//----------------------------------------
	// 物理实体 2013-7-4 帝林 
	//----------------------------------------
	class PhysicsEntity : public Entity
	{
	public:
		PhysicsEntity();

		// 设置碰撞对象
		void SetCollisionShape( Shape* collShape) { m_collSift.SetCollisionShape( collShape); }

		// 提交渲染元素
		virtual void SubmitRenderElements( RenderElementMgr& renderElementMgr){}
	};
	typedef s_ptr<PhysicsEntity> PhysicsEntityPtr;
}

#endif