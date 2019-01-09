#pragma once

#include <vector>
#include "AresFxObject.h"
#include <Engine/SceneSystem/AresEntity.h>

using namespace std;

namespace Ares
{
	//------------------------------------------------------
	// 效果实体 2011-9-14   帝林
	//------------------------------------------------------
	class FxEntity : public Entity
	{
	public:
		FxEntity();
		~FxEntity() {}

		// 获取对象数量
		int GetNumObjects() { return static_cast<int>(m_objects.size()); }

		// 获取对象
		FxObject* GetObjectByIndex( int idx);

		// 获取对象
		FxObject* GetObjectByName( const char* name);

		// 删除FxObject
		void DeleteObjectByIndex( int idx);

		// 添加对象
		void AddObject( FxObject* object);

		// 移除对象
		void RemoveObject( FxObject* object);

		// 更新
		virtual void FrameMove( float fTime);

		// 执行
		void SubmitRenderElements( RenderElementMgr& renderElementMgr);

		// 添加到场景
		virtual void OnAddToScene( Scene* scene);

		// 更新本地包围盒
		void UpdateLocalBounds();

		// Serialize
		template<typename Archive> void serialize( Archive& ar, const unsigned int version)
		{
			ar & m_objects;
		}

	private:
		vector<FxObject*>	m_objects;				// 效果对象列表
	};
	typedef s_ptr<FxEntity> FxEntityPtr;
}