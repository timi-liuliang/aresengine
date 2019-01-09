#pragma once

#include <map>
#include <Engine/RenderSystem/AresModel.h>

using namespace std;

namespace Ares
{
	//---------------------------------------------
	// 模型摆放结构体  2010-08-16  帝林
	//---------------------------------------------
	class CEntityPutLogic
	{
	public:
		typedef map<string, s_ptr<Model>> EntityMap;
	public:
		// 构造函数
		CEntityPutLogic();

		// 每帧更新
		void FrameMove( float fTime, bool IsCursorInWindow);

		// 设置
		void SetResPath( const char* resPath) { m_resPath = resPath; }

		// 设置
		void New( EntityTypeDetail type);
		
		// 清除
		void Reset();

		// 放置
		void Put();

	private:
		// 获取唯一实体名
		bool GetUniqueEntityName( char* uniqueName);

	private:
		EntityTypeDetail	m_typeDetail;		// 实体类别
		s_ptr<Entity>		m_currEntity;		// 当前实体
		string				m_resPath;			// 资源路径
		EntityMap			m_dynamicEntityList;
	};
}