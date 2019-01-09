#include "FxLogic.h"
#include <Engine/AresRoot.h>
#include <Physics/Rect3.h>
#include <Engine/FxSystem/AresFxSystem.h>
#include <string>

using namespace std;

namespace Ares
{ 
	// 全局变量
	FxLogic*		g_fxLogic        = NULL;

	// 构造函数
	FxLogic::FxLogic()
	{
		g_fxLogic = this;
		m_fxEdit  = new_ FxEdit;
	}

	// 编辑实体
	void FxLogic::PickEntity()
	{
		Entity* pEntity = this->Pick();

		if( pEntity && pEntity->GetTypeDetail() == ED_Model)
		{
			// 显示3D轴
			m_p3DAxis->Clear();
			m_p3DAxis->SetVisible( true);
			//m_p3DAxis->GetTransform()->SetPosition( &pEntity->GetSceneNode()->GetPosition());
			//m_p3DAxis->CatchTransform( &pEntity->GetTransform());
		}
	}

	// 渲染更新
	void FxLogic::FrameMove(float fTime)
	{
		//if( m_group)
		//	m_group->FrameMove( fTime);

		//IFxSystem* fxSystem = Root::GetSingleton()->GetFxSystem();
		//if( fxSystem)
		//	fxSystem->FrameMove( fTime);
		/*
		m_inputListener->Capture();

		if( m_cameraLogic)
		{
			m_auxiliaryDisplay->Refresh( fTime);

			m_cameraLogic->FrameMove( fTime);
		}

		// 
		m_fxEdit->FrameMove( fTime);

		Logic_Transform->FrameMove();
		
		// 更新帧率
		UpdateFPS( fTime);*/
	}

	// 渲染
	void FxLogic::Render()
	{
		// 渲染
		//ARenderSystem.Render();
	}


	// 鼠标左键按下
	void FxLogic::OnMouseLeftButtonDown()
	{
		m_mouseLeftButtonDown = true;

		Vector3 rayOrig;
		Vector3 rayDir;
		this->GetPickRay( rayOrig, rayDir);

		if ( m_flags == EM_HT_EDITOENTITY)
		{
			this->PickEntity();
		}

		m_p3DAxis->OnMouseDown( rayOrig, rayDir);
	}

	// 鼠标左键抬起
	void FxLogic::OnMouseLeftButtonUp()
	{
		m_mouseLeftButtonDown = false;

		if ( m_p3DAxis)
		{
			m_p3DAxis->OnMouseUp();
		}
	}

	// 鼠标移动
	void FxLogic::OnMouseMove()
	{

	}

	// 鼠标检选
	Entity* FxLogic::Pick( Vector3* intersectPos, float* pDist)
	{
		//if ( g_pScene)
		//{
		//	D3DXVECTOR3 rayOrig;
		//	D3DXVECTOR3 rayDir;
		//	
		//	this->GetPickRay( rayOrig, rayDir);

		//	D3DXVECTOR3 intersePos = m_pCameraLogic->GetPosition();
		//	IEntity* pEntity = g_pScene->Pick( rayOrig, rayDir, 0.0f, 1000.0f, intersePos, pDist);
		//	if ( intersectPos)
		//	{
		//		*intersectPos = intersePos;
		//	}

		//	// 显示包围盒
		//	if ( pEntity)
		//	{
		//		static IBox* pBox = NULL;
		//		if ( !pBox)
		//		{
		//			pBox = g_pSceneMgr->CreateBox();
		//		}

		//		CRect3D rect = pEntity->GetSceneNode()->GetWorldBounds();
		//		
		//		// 显示包围盒
		//		pBox->SetParam( rect.x0, rect.y0, rect.z0, rect.x1, rect.y1,rect.z1);
		//	}

		//	return pEntity;
		//}

		return NULL;
	}

	// 获取屏幕捡选射线
	void FxLogic::GetPickRay( Vector3& rayOrig, Vector3& rayDir)
	{
		//POINT ptPoint;
		//::GetCursorPos( &ptPoint);
		//::ScreenToClient( g_pApp->Hwnd(), &ptPoint);

		////g_pDevice->GetBackBuffer();
		//WindowPointToRay( rayOrig, rayDir, ptPoint.x, ptPoint.y, g_pApp->GetBackBufferWidth(), g_pApp->GetBackBufferHeight(), 
		//	g_pCameraLogic->GetView(), g_pCameraLogic->GetProj());
	}

	// 设置添充方式
	void FxLogic::SetFillMode(WORD index)
	{
		//if ( index == 0)
		//{
		//	g_pSceneMgr->SetFillMode( D3DFILL_POINT);
		//}
		//else if( index == 1)
		//{
		//	g_pSceneMgr->SetFillMode( D3DFILL_WIREFRAME);
		//}
		//else
		//{
		//	g_pSceneMgr->SetFillMode( D3DFILL_SOLID);
		//}
	}

	//
	// inline
	//
	bool FxLogic::UpdateFPS(float fTime)
	{
		static float fTotalTime = 0.0f;
		static DWORD dwFrames   = 0;

		fTotalTime += fTime;
		dwFrames++;

		if (fTotalTime > 0.5f)
		{
			m_fFPS = dwFrames / fTotalTime;

			fTotalTime = 0.0f;
			dwFrames   = 0;

			// 产生更新
			return true;
		}

		// 未更新
		return false;
	}

	// 设置编辑类型
	void FxLogic::SetHandleType( EHandleType type)
	{
		m_flags = type;

		switch( m_flags)
		{
		case EM_HT_EDITORTERRAIN:
			{
				m_p3DAxis->SetVisible( false);	
			}
			break;
		}
	}

	// 更改窗口大小
	void FxLogic::ResizeWindow( int width, int height)
	{
		//m_cameraLogic->SetProjParams( PI / 4.f, width, height, .1f, 500.0f);

		//ARenderSystem.ResizeWindow( width, height);
	}
}