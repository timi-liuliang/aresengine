#include "MOLogicHelper.h"
#include <Engine/AresRoot.h>
#include <Engine/RenderSystem/AresRenderSystem.h>

namespace Ares
{
	// 设置
	void CPutEntity::New( char* resPath)
	{
		// 先清空
		//SAFE_RELEASE( m_pEntity);

		// 新建实体结点
		//m_resPath = resPath;
		//m_pEntity =  Root::GetSingleton()->GetRenderSystem()->CreateM1Entity(resPath);
		//m_pSceneNode   = g_pScene->RootNode()->CreateChildSceneNode();
		//m_pEntity->AttachSceneNode( m_pSceneNode);
	}

	// 复制一份
	void CPutEntity::Copy()
	{
		//// 无需清除
		//if ( !m_resPath.empty())
		//{
		//	m_pEntity = Root::GetSingleton()->GetRenderSystem()->CreateM1Entity( m_resPath.c_str());
		//	m_pSceneNode = g_pScene->RootNode()->CreateChildSceneNode();
		//	m_pEntity->AttachSceneNode( m_pSceneNode);
		//}
	}

	// 清除
	void CPutEntity::Clear()
	{
		// 应该是清除实体的
		m_pEntity	 = s_ptr<Entity>();
		m_resPath.clear();
	}

	// 设置实体位置
	void CPutEntity::SetPosition( Vector3& pos)
	{
		if ( m_pEntity)
		{
			Transform transform = m_pEntity->GetTransform();
			transform.SetTrans( pos);
			m_pEntity->SetTransform( transform);
		}
	}
}