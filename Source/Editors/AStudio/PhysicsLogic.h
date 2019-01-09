#pragma once

#include <Engine/AresRoot.h>
#include <Physics/PhysicsWorld.h>

namespace Ares
{
	//---------------------------------------
	// 物理逻辑(当前仅有碰撞) 2012-08-13 帝林
	//---------------------------------------
	class  PhysicsLogic 
	{
	public:
		PhysicsLogic();
		~PhysicsLogic();

		// 鼠标检选
		bool Pick( Vector3* intersectPos, float* pDist);

		// 获取屏幕捡选射线
		void GetPickRay( Vector3& rayOrig, Vector3& rayDir);

		// 添加CollisionObject

	private:
		VisualSegment3*			m_segment;
	};
}