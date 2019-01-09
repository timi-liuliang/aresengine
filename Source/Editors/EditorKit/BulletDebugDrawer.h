#pragma once

#include <map>
#include <Physics/PhysicsWorld.h>
#include <Engine/RenderSystem/AresVisualShape.h>
#include <Physics/Bullet/btBulletDynamicsCommon.h>

using namespace std;

namespace Ares
{
	//---------------------------------
	// 物理世界辅助显示 2013-9-10 帝林
	//---------------------------------
	class BulletDebugDrawer
	{
	public:
		BulletDebugDrawer( Scene* scene, btDynamicsWorld* dynamicsWorld);
		~BulletDebugDrawer();

		// 更新
		void FrameMove( float timeStep);

	private:
		// 获取VisualShape
		VisualShape* GetVisualShape();

		// 渲染
		void Draw( Matrix44& m, const btCollisionShape* shape, const btVector3& color, const btVector3& worldBoundsMin, const btVector3& worldBoundsMax);

	private:
		Scene*					m_scene;				// 场景
		btDynamicsWorld*		m_dynamicsWorld;		// 物理世界
		int						m_numActive;			// 当前活动数量
		vector<VisualShape*>	m_visualShapes;			// 几何体
	};
}