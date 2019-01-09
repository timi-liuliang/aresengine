#ifndef ARES_FX_DECAL_H
#define ARES_FX_DECAL_H

#include "AresFxObject.h"
#include "AresFxRenderable.h"
#include <Physics/PhysicsWorld.h>

namespace Ares
{
	//--------------------------------------------------
	// 贴花  2013-7-3 帝林
	// 对任意表面应用贴花 <<游戏编程精粹2>> chapter 4.8
	//--------------------------------------------------
	class FxDecal : public FxObject
	{
	public:
		FxDecal();
		virtual ~FxDecal();

		// 设置参数
		void Set( const Vector3& center, const Vector3& normal, const Vector3& tangent, float width, float height, float depth, const Vector3& upDir);

		// 设置物理世界
		void SetPhysicsWorld( PhysicsWorldPtr& physicsWorld) { m_physicsWorld = physicsWorld; }

		// 获取Renderable
		FxRenderable& GetRenderable() { return m_renderable; }

		// 更新
		virtual void FrameMove( float elpasedTime, FxEntity* fxEntity);

		// 提交到渲染队列
		virtual void SubmitRenderElements( RenderElementMgr& renderElementMgr);

	private:
		// 更新Buffer
		void UpdateBuffer();

	private:
		Vector3					m_center;				// 中心点
		Vector3					m_normal;				// 法线
		vector<FxVertexFormat>	m_vertices;				// 顶点数据
		vector<WORD>			m_indices;				// 索引数据
		FxRenderable			m_renderable;			// 渲染

		PhysicsWorldPtr			m_physicsWorld;		// 物理世界
	};
	typedef s_ptr<FxDecal> FxDecalPtr;
}

#endif