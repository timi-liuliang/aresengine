#include "EntityEditLogic.h"
#include <Physics/Intersect.h>
#include "SCLogic.h"
#include "SCMainWindow.h"
#include "PropertyEditor.h"

using namespace QT_UI;

namespace Ares
{
	// 构造函数
	CEntityEditLogic::CEntityEditLogic()
		: m_entityWorldBox( NULL)
		, m_curEditEntity( NULL)
	{
		m_entityWorldBox = SELogic_VisualShape.CreateRect3();

		// 3D轴初始化
		m_p3DAxis = new CAres3DAxis( SELogic_VisualShape);
		m_p3DAxis->SetPosition( 0.0f, 0.0f, 0.0f);
		m_p3DAxis->SetVisible( false);

		m_bMouseLeftButtonDown = false;
	}

	// 每帧处理
	void CEntityEditLogic::FrameMove()
	{
		if( m_handleType == EM_SELECT)
			SelectEntity();

		// 移动
		MouseMove();

		// 鼠标左键抬起
		if ( m_bMouseLeftButtonDown && !SCLogic_Input->IsMouseDown( OIS::MB_Left))
		{
			m_bMouseLeftButtonDown = false;
			m_p3DAxis->OnMouseUp();
		}

		// 鼠标左键按下
		if ( !m_bMouseLeftButtonDown && SCLogic_Input->IsMouseDown( OIS::MB_Left))
		{
			m_bMouseLeftButtonDown = true;

			Vector3 rayOrig;
			Vector3 rayDir;
			Logic_Physics->GetPickRay( rayOrig, rayDir);
			m_p3DAxis->OnMouseDown( rayOrig, rayDir);
		}

		// 取消选中
		if( IsKeyDown( OIS::KC_ESCAPE))
		{
			SetSelEntity( NULL);
		}

		// 删除当前选中实体
		if( IsKeyDown( OIS::KC_DELETE))
		{
			DeleteSelectedEntity();
		}
	}

	// 向场景中添加Entity
	void CEntityEditLogic::AddEntity( const char* uName, s_ptr<Entity>& pEntity)
	{
		if( SCLogic_Scene)
			SCLogic_Scene->AddEntity( uName, pEntity);

		// 设置被选择
		SetSelEntity( pEntity.get());
	}

	// 设置编辑实体
	void CEntityEditLogic::SetSelEntity( Entity* pEntity)
	{
		if( pEntity)
		{
			// 2.如果当前有选择实体,保存实体,添加到区域中
			m_curEditEntity = pEntity;
			if( m_curEditEntity)
			{
				// 显示3D轴
				m_p3DAxis->Clear();
				m_p3DAxis->SetVisible( true);
				m_p3DAxis->SetPosition( m_curEditEntity->GetTransform().GetTrans());
				m_p3DAxis->CatchEntity( m_curEditEntity);
			}

			// 根据选择的实体类型, 显示属性控制
			SCUI_Property->ShowProperty( pEntity);
		}
		else
		{
			m_p3DAxis->Clear();
			m_p3DAxis->SetVisible( false);
			SCUI_Property->ShowProperty( NULL);

			m_curEditEntity = NULL;

			ShowWorldBox();
		}

		ShowWorldBox();
	}

	// 设置编辑实体
	void CEntityEditLogic::SetSelEntity( const string& entityName)
	{
		// 移除
		Scene* pScene = SCLogic_Scene;
		if( pScene)
		{
			EntityPtr& selEntity = pScene->GetEntity( entityName.c_str());
			if( selEntity)
			{
				SetSelEntity( selEntity.get());
			}
		}
	}

	// 删除当前选中实体
	void CEntityEditLogic::DeleteSelectedEntity()
	{
		Entity* entity = m_curEditEntity;
		if( entity)
		{
			// 置空选中
			SetSelEntity( NULL);

			// 移除
			Scene* pScene = SCLogic_Scene;
			if( pScene)
				pScene->DelEntity( entity->GetName());
		}
	}

	// 选择实体
	void CEntityEditLogic::SelectEntity()
	{
		if( IsMLBDown())
		{
			Entity* entity = SCLogic_Map->SelectEntity();

			SetSelEntity( entity);
		}
	}

	// 鼠标移动
	void  CEntityEditLogic::MouseMove()
	{
		static POINT m_lastPoint;

		POINT ptPoint;
		GetCursorPos( &ptPoint);
		ScreenToClient( SCLogic_Main->GetHwnd(), &ptPoint);

		// 鼠标左键按下(操作3D轴)
		if ( IsMLBDown())
		{	
			Vector3 raylastOrig;
			Vector3 raylastDir;
			Vector3 rayOrig;
			Vector3 rayDir;

			RECT clientRect;
			::GetClientRect( SCLogic_Main->GetHwnd(), &clientRect);

			long clientWidth  = clientRect.right  - clientRect.left; 
			long clientHeight = clientRect.bottom - clientRect.top;

			WindowPointToRay( raylastOrig, raylastDir, m_lastPoint.x, m_lastPoint.y, clientWidth, clientHeight, SCLogic_Camera->GetView(), SCLogic_Camera->GetProj());
			WindowPointToRay( rayOrig, rayDir, ptPoint.x, ptPoint.y, clientWidth, clientHeight, SCLogic_Camera->GetView(), SCLogic_Camera->GetProj());

			m_p3DAxis->OnMouseMove( raylastOrig, raylastDir, rayOrig, rayDir, &m_lastPoint, &ptPoint);

			if( !m_curEditEntity)
				SCUI_Property->ShowFrame( "Setting");

			// 根据选择的实体类型, 显示属性控制
			//if( m_pCurEditEntity->GetDetailType() == Entity::EM_DT_LIGHTPOINT)
			//	g_mainWindow->GetPropertyEditor()->ShowFrame( "PointLight");

			ShowWorldBox();
		}

		m_lastPoint = ptPoint;
	}


	// 设置编辑类型
	void CEntityEditLogic::SetEditType( EHandleType type)
	{
		m_handleType = type;

		switch ( type)
		{
		case EM_SELECT:
			{

			}
			break;
		case EM_MOVE:
			{
				m_p3DAxis->SetEditType( CAres3DAxis::EM_EDIT_TRANSLATE);
			}
			break;
		case EM_ROTATE:
			{
				m_p3DAxis->SetEditType( CAres3DAxis::EM_EDIT_ROTATE);
			}
			break;
		case EM_SCALE:
			{
				m_p3DAxis->SetEditType( CAres3DAxis::EM_EDIT_SCALE);
			}
			break;
		}
	}

	// 显示包围盒
	void CEntityEditLogic::ShowWorldBox()
	{
		// 显示包围盒
		if ( m_curEditEntity)
		{
			Rect3 rect = m_curEditEntity->GetWorldBounds();

			// 显示包围盒
			m_entityWorldBox->Set( rect);
			m_entityWorldBox->SetVisible( true);
		}
		else
		{
			m_entityWorldBox->SetVisible( false);
		}
	}
}