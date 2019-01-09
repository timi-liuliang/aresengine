#pragma once

#include <Engine/SceneSystem/AresEntity.h>

namespace Ares
{
	//---------------------------------------------
	// 模型摆放结构体  2010-08-16  帝林
	//---------------------------------------------
	class CPutEntity
	{
	public:
		// 构造函数
		CPutEntity();

		// 设置
		void New( char* resPath);

		// 复制一份
		void Copy();

		// 清除
		void Clear();

		// 设置实体位置
		void SetPosition( Vector3& pos);

	private:
		s_ptr<Entity>    m_pEntity;		// 实体
		string			m_resPath;		// 资源路径
	};

	// 构造函数 
	inline CPutEntity::CPutEntity()
	{	
	}
}