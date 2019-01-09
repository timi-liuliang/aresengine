#pragma once

#include <Core/Graphics/AresRenderWindow.h>
#include <Engine/FxSystem/AresFxSystem.h>
#include <EditorKit/Ares3DAxis.h>
#include "FxEdit.h"

namespace Ares
{
	//---------------------------------------
	// 编辑器逻辑入口	2010-07-20 帝林	
	//---------------------------------------
	class FxLogic
	{
	public:
		// 所有操作类型
		enum EHandleType
		{
			EM_HT_NULL,					// Nothing
			EM_HT_EDITORTERRAIN,		// 编辑地形
			EM_HT_EDITTERRAINTEXTURE,	// 编辑地形纹理
			EM_HT_EDITOENTITY,			// 编辑除地形外的实体(移动 旋转 缩放)
			EM_HT_PUTENTITY,			// 摆放实体
			EM_HT_PUT_WATER,			// 摆放水
			EM_HT_SELECT_WATER,			// 选择水
			EM_HT_BRUSH_WATER,			// 刷水,更新水的高度(根据深度或高度)
		};

		// 构造函数
		FxLogic();

		// 更新
		void FrameMove( float fTime);

		// 返回3D轴
		CAres3DAxis* Get3DAxis() { return m_p3DAxis; }

		// 获取帧率
		float GetFPS()  { return m_fFPS; }

		// 获取屏幕捡选射线
		void GetPickRay( Vector3& rayOrig, Vector3& rayDir);

		// 鼠标左键按下
		void OnMouseLeftButtonDown();

		// 鼠标左键抬起
		void OnMouseLeftButtonUp();

		// 鼠标移动
		void OnMouseMove();

		// 鼠标检选
		Entity* Pick( Vector3* intersectPos=NULL, float* pDist=NULL);

		// 渲染
		void Render();

		// 设置编辑类型
		void SetHandleType( EHandleType type);
		
		// 更新帧率
		bool UpdateFPS( float fTime);

		// 设置添充方式
		void SetFillMode(WORD index);

	private:
		// 编辑实体
		void PickEntity();

	public:
		// 获取效果编辑逻辑
		FxEdit* GetFxEdit() { return m_fxEdit; }

		// 更改窗口大小
		void ResizeWindow( int width, int height);

	private:
		HWND                    m_hWnd;
		EHandleType				m_flags;				// 操作标记
		float                   m_fFPS;					// frames per second
		CAres3DAxis*			m_p3DAxis;				// 3D轴   

		// 鼠标位置点
		POINT                   m_lastPoint;
		bool                    m_mouseLeftButtonDown;	// 左键是否按下   

	public:
		FxEdit*					m_fxEdit;				// 效果编辑逻辑
	};

	extern  FxLogic*			g_fxLogic;
	#define FxLogic_Main		g_fxLogic
	#define FxLogic_FxEdit		FxLogic_Main->GetFxEdit()
}