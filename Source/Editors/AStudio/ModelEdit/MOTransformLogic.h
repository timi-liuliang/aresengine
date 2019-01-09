#pragma once

#include <EditorKit/Ares3DAxis.h>

namespace Ares
{
	//---------------------------------------
	// 转换编辑逻辑	2011-01-18 帝林	
	//---------------------------------------
	class TransformLogic
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
		TransformLogic();

		// 每帧处理
		void  FrameMove();

		// 设置编辑类型
		void SetTranformType( CAres3DAxis::EditType type);

		// 设置编辑实体
		void SetSelected( Transform* transform);

		// 取消选择
		void UnSelect();

		// 选择实体
		void  SelectEntity();

		// 设置编辑类型
		void  SetEditType( EHandleType type);

		// 设置3D轴缩放
		void SetAxisScale( float scale) { m_3dAxis->SetScale( scale); }

	public:
		// 获取当前编辑实体
		Transform* GetCurTransform() { return m_curTransform; }

		// 获取屏幕捡选射线
		void GetPickRay( Vector3& rayOrig, Vector3& rayDir);

		// 获取轴点南射线
		void Get3DAxisPickRay( Vector3& rayOrig, Vector3& rayDir);

	private:
		// 每帧更新
		void  MouseMove();

	private:
		EHandleType			m_handleType;			// 处理类型
		CAres3DAxis*		m_3dAxis;				// 3D轴
		Transform*			m_curTransform;			// 当前控制器
		bool				m_bMouseLeftButtonDown;
	};
}