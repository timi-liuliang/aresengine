#include <string>
#include "SCLogic.h"
#include <Physics/IntrCapsule3Triangle3.h>
#include <Engine/RenderSystem/AresRenderSystem.h>
#include <Physics/Intersect.h>
#include "WaterEditLogic.h"
#include <Engine/AresRoot.h>
#include <Engine/SceneSystem/AresMap.h>
#include <Engine/FxSystem/AresFx3DPatch.h>
#include <Physics/Spiral3.h>
#include <Physics/IntrBox3Triangle3.h>
#include "client.h"

using namespace std;

namespace Ares
{ 
	// 全局变量
	SCLogic*			  g_SCLogic        = NULL;

	// 构造函数
	SCLogic::SCLogic( HWND hwnd)
	{
		SCLogic_Main = this;

		m_hWnd = hwnd;
		RenderSettings tSettings( m_hWnd);

		m_handleType	= EM_HT_NULL;

		// 摄像机相关
		m_cameraLogic   = new_ CameraLogic();	
		if( m_scene = ARoot->BuildScene( m_cameraLogic, tSettings, NULL, NULL))
		{
			m_inputListener		= new_ InputListener(hwnd);
			m_mapLogic			= new_ CMapLogic;
			m_pEntityEditLogic  = new_ CEntityEditLogic();
			m_pEntityPutLogic   = new_ CEntityPutLogic();
			m_pAILogic			= new_ CAILogic();
			m_physicsLogic		= new_ PhysicsLogic;
			m_waterEditLogic    = new_ CWaterEditLogic;
			m_moveLogic			= new_ CMoveLogic;
			m_netLogic			= new_ NetLogic;
			m_settingLogic		= new_ SettingsLogic;
			//m_pxWorldDrawer		= new_ PxWorldDebugDraw( m_scene);

			m_inputListener->RegisterMouseEventCallback( m_cameraLogic);

			m_moveLogic->SetMoveType( CMoveLogic::EM_THIRD);
			m_moveLogic->SetProjParams( PI/4.f, tSettings.m_width,tSettings.m_height, 0.2f, 150.0f, ARenderDevice->GetType());
		}
	}

	// 析构函数
	SCLogic::~SCLogic()
	{
		SAFE_DELETE( m_settingLogic);
		SAFE_DELETE( m_netLogic);
		SAFE_DELETE( m_moveLogic);
		SAFE_DELETE( m_waterEditLogic);
		SAFE_DELETE( m_physicsLogic);
		SAFE_DELETE( m_pAILogic);
		SAFE_DELETE( m_pEntityPutLogic);
		SAFE_DELETE( m_pEntityEditLogic);
		SAFE_DELETE( m_mapLogic);
		SAFE_DELETE( m_inputListener);
		SAFE_DELETE( m_cameraLogic);
		SAFE_DELETE( m_scene);

		Root::Destory();
	}

	// 是否暂停中
	bool SCLogic::IsPaused()
	{
		if( !m_scene)
			return true;

		if( m_scene->IsPaused())
			return true;

		return false;
	}

	// 合并batch
	void  SCLogic::CombinationBatch()
	{
		//g_pScene->CombinationBatch();
	}

	// 鼠标是否在窗口中
	static bool IsCursorInWindow( HWND hwnd)
	{
		POINT ptPoint;
		GetCursorPos( &ptPoint);
		//ScreenToClient( hwnd, &ptPoint);

		RECT clientRect;
		GetWindowRect( hwnd, &clientRect);		

		if ( ptPoint.x > clientRect.left && ptPoint.x <clientRect.right
			&& ptPoint.y > clientRect.top && ptPoint.y <clientRect.bottom)
			return true;

		return false;
	}

	// 渲染更新
	void SCLogic::FrameMove(float fTime)
	{
		// 输入逻辑
		m_inputListener->Capture();

		m_mapLogic->FrameMove( fTime);

		if (m_moveLogic)
			m_moveLogic->FrameMove( fTime);

		//m_pxWorldDrawer->FrameMove( fTime);
		
		// 更新帧率
		UpdateFPS( fTime);

		switch( m_handleType)
		{
		case EM_HT_EDITORTERRAIN:
		case EM_HT_EDITTERRAINTEXTURE:
				SCLogic_Map->EditTerrainMeshOrTexture( fTime);
			break;

		case EM_HT_EDITOENTITY:
		case EM_HT_PUTENTITY:
			{
				SCLogic_EntityEdit->FrameMove();
				Logic_EntityPut->FrameMove( fTime, IsCursorInWindow( m_hWnd));
			}
			break;

		case EM_HT_AI:
				Logic_AI->FrameMove();
			break;
		}
	}

	// 编辑水
	void SCLogic::EditWaterMesh()
	{
		//IWater* pWater = NULL;//g_pScene->GetWarter();
		//if ( pWater)
		//{
		//	Vector3 intersectPos;
		//	if (this->Pick( &intersectPos))
		//	{
		//		switch( m_waterEditorData.type)
		//		{
		//			// 高度为依据
		//		case SWaterEditorData::EM_ET_HEIGHT:
		//			{
		//				//D3DXVECTOR3 waterBegin = pWater->get
		//				// 相交点
		//				Vector3 pos = intersectPos;// - terrainBegin;
		//				WORD x = (WORD)(pos.x - m_waterEditorData.size + 1);
		//				WORD y = (WORD)(pos.y - m_waterEditorData.size + 1);

		//				// 更新范围内的点
		//				for ( int i=0; i<m_waterEditorData.size * 2; i++)
		//				{
		//					for ( int j=0; j<m_waterEditorData.size * 2; j++)
		//					{
		//						WORD tpX = x + i;
		//						WORD tpY = y + j;

		//						pWater->SetWaterHeight( tpX, tpY, m_waterEditorData.height);

		//						// 更新深度
		//						float depth = m_waterEditorData.height - m_mapLogic.GetScene()->GetTerrain()->GetHeight( tpX, tpY);

		//						pWater->SetWaterDepth( tpX, tpY, depth);
		//					}
		//				}
		//			}
		//			break;

		//			// 深度为依据
		//		case SWaterEditorData::EM_ET_DEPTH:
		//			{
		//				Vector3 pos = intersectPos;// - terrainBegin;
		//				WORD x = (WORD)(pos.x - m_waterEditorData.size + 1);
		//				WORD y = (WORD)(pos.y - m_waterEditorData.size + 1);

		//				// 更新范围内的点
		//				for ( int i=0; i<m_waterEditorData.size * 2; i++)
		//				{
		//					for ( int j=0; j<m_waterEditorData.size * 2; j++)
		//					{
		//						WORD tpX = x + i;
		//						WORD tpY = y + j;

		//						pWater->SetWaterDepth( tpX, tpY, m_waterEditorData.depth);

		//						// 更新深度
		//						float height = m_waterEditorData.depth + m_mapLogic.GetScene()->GetTerrain()->GetHeight( tpX, tpY);

		//						pWater->SetWaterHeight( tpX, tpY, height);
		//					}
		//				}
		//			}
		//			break;
		//		}
		//	}	
		//}
	}

	// 设置Alpha测试值
	void SCLogic::SetAlphaTestValue( int value)
	{
		//g_pSceneMgr->GetDevice()->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE);
		//g_pSceneMgr->GetDevice()->SetRenderState( D3DRS_ALPHAREF, value);
		//g_pSceneMgr->GetDevice()->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	}

	// 设置阴影SoftNess
	void  SCLogic::SetShadowSampleSize( int size)
	{
		//ILiSPSM* pLiSPSM = g_pSceneMgr->GetShadowManager();
		
		//pLiSPSM->SetSampleSize( size);
	}

	// 设置光渗透减少参数
	void SCLogic::SetShadowLBR( float lbr)
	{
		//ILiSPSM* pLiSPSM = g_pSceneMgr->GetShadowManager();

		//pLiSPSM->SetLBR( lbr);
	}

	// 设置是否开启阴影
	void SCLogic::SetShadow( bool bShadow)
	{
		//g_pSceneMgr->SetShadow( bShadow);
	}

	// 设置是否进行模糊
	void SCLogic::SetShadowBlur( bool blur)
	{
	//	ILiSPSM* pLiSPSM = g_pSceneMgr->GetShadowManager();

	//	pLiSPSM->SetBlur( blur);
	}

	//
	// inline
	//
	bool  SCLogic::UpdateFPS(float fTime)
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
	void SCLogic::SetHandleType( EHandleType type)
	{
		m_handleType = type;

		switch( m_handleType)
		{
		case EM_HT_EDITORTERRAIN:
			{
				//m_p3DAxis->SetVisible( false);

				// 清除
				m_pEntityPutLogic->Reset();			
			}
			break;
		}
	}

	// 获取场景系统
	Scene* SCLogic::GetScene() 
	{ 
		return m_scene; 
	}

	// 更改窗口大小
	void SCLogic::ResizeWindow( size_t width, size_t height)
	{
		static size_t oWidth = 65535, oHeight = 65535;
		if( oWidth!=width || oHeight!=height)
		{
			m_moveLogic->SetProjParams( PI/4.f, width, height, SCLogic_Camera->GetNear(), SCLogic_Camera->GetFar(), ARenderDevice->GetType());

			SELogic_RenderSystem.ResizeWindow( width, height);

			oWidth = width;
			oHeight= height;
		}
	}
}