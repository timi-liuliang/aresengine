#pragma once

#include <EditorKit/Ares3DAxis.h>
#include <Engine/SceneSystem/AresEntity.h>

namespace Ares
{
	//---------------------------------------
	// 实体编辑逻辑	2011-01-18 帝林	
	//---------------------------------------
	class CEntityEditLogic
	{
	public:
		enum EHandleType
		{
			EM_PUT,		// 摆放
			EM_SELECT,	// 选择
			EM_MOVE,	// 移动
			EM_ROTATE,	// 旋转
			EM_SCALE,	// 缩放
		};

		// 构造函数
		CEntityEditLogic();

		// 每帧处理
		void  FrameMove();

		// 向场景中添加Entity
		void AddEntity( const char* uName, s_ptr<Entity>& pEntity);

		// 设置编辑实体
		void SetSelEntity( Entity* pEntity);

		// 设置编辑实体
		void SetSelEntity( const string& entityName);

		// 选择实体
		void  SelectEntity();

		// 删除当前选中实体
		void DeleteSelectedEntity();

		// 设置编辑类型
		void  SetEditType( EHandleType type);

		// 设置3D轴缩放
		void SetAxisScale( float scale) { m_p3DAxis->SetScale( scale); }

	public:
		// 获取当前编辑实体
		template<typename T> T* GetEditEntity() const { return static_cast<T*>( m_curEditEntity);}

	private:
		// 每帧更新
		void  MouseMove();

		// 显示包围盒
		void ShowWorldBox();

	private:
		EHandleType			m_handleType;			// 处理类型
		CAres3DAxis*		m_p3DAxis;				// 3D轴
		bool				m_bMouseLeftButtonDown;
		Entity*				m_curEditEntity;		// 当前编辑的实体
		VisualRect3*		m_entityWorldBox;		// 世界包围盒
	};
}