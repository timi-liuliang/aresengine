#pragma once

namespace Ares
{
	//-----------------------------------------------
	// ActionInterface 2013-9-5 帝林
	// Basic interface to allow actions such as 
	// vehicles to be updated inside a PhysicsWorld
	//-----------------------------------------------
	class RigidBody;
	class PhysicsWorld;
	class ActionInterface
	{
	public:
		virtual ~ActionInterface() {}

		// 静态函数
		static RigidBody& GetFixedBody();

		// 更新
		virtual void UpdateAction( PhysicsWorld* physicsWorld, float deltaTimeStep)=0;
	};
}