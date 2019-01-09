#include "MOLogic.h"
#include <Engine/AresRoot.h>
#include <Core/AresCore.h>
#include <Engine/RenderSystem/AresAnimationController.h>

using namespace std;

namespace Ares
{ 
	// 全局变量
	MOLogic*	g_MOLogic        = NULL;

	// 构造函数
	MOLogic::MOLogic( HWND hwnd)
	{
		MOLogic_Main = this;

		m_flags			= EM_HT_NULL;

		RenderSettings tSettings( hwnd);
		tSettings.m_title      = "ModelEditor";

		// 摄像机相关
		m_cameraLogic = new_ MOCameraLogic();	
		m_cameraLogic->SetProjParams( PI / 4.f, tSettings.m_width, tSettings.m_height, .2f, 200.0f);
		//if( m_sceneSystem = ARoot->BuildSceneSystem( m_cameraLogic, tSettings))
		{
			m_scene					= ARoot->BuildScene( m_cameraLogic, tSettings, NULL, NULL);
			m_modelEdit				= new_ ModelEdit;
			m_lightmapUVBuild		= new_ LightmapUVBuild;
			m_lightmapUVRender		= new_ LightmapUVRender;
			m_collisionBuildLogic	= new_ CollisionBuildLogic;
			m_transformLogic		= new_ TransformLogic;
			m_lightLogic			= new_ LightEditLogic;
			m_inputListener			= new_ MOInputListener(hwnd);

			m_inputListener->RegisterMouseEventCallback( m_cameraLogic);
		}
	}

	// 析构函数
	MOLogic::~MOLogic()
	{
		MOConfigMgr::Destory();

		SAFE_DELETE(g_lightmapUVBuild);
	}

	// 是否暂停中
	bool MOLogic::IsPaused()
	{
		if( !m_scene)
			return true;

		if( m_scene->IsPaused())
			return true;

		return false;
	}

	// 更改窗口大小
	void MOLogic::ResizeWindow( int width, int height)
	{
		static size_t oWidth = 65535, oHeight = 65535;
		if( oWidth!=width || oHeight!=height)
		{
			m_cameraLogic->SetProjParams( PI / 4.f, width, height, .2f, 200.0f);

			MOLogic_RenderSystem.ResizeWindow( width, height);

			oWidth = width;
			oHeight= height;
		}
	}

	// 渲染更新
	void MOLogic::FrameMove(float fTime)
	{
		m_inputListener->Capture();

		if( m_cameraLogic)
			m_cameraLogic->FrameMove( fTime);

		if( m_lightLogic)
			m_lightLogic->FrameMove();

		MOLogic_ModelEdit->FrameMove( fTime);

		MOLogic_Transform->FrameMove();

		if( m_scene)
		{
			m_scene->FrameMove( fTime);
		}
		
		// 更新帧率
		UpdateFPS( fTime);
	}

	// 添加实体
	void MOLogic::AddEntity( char* namePath)
	{
	}

	// 设置添充方式
	void MOLogic::SetFillMode(WORD index)
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

	// 播放动画
	void MOLogic::PlayAnimation(int index)
	{/*
		if( m_currEntity)
		{
			m_currEntity->GetAnimationController()->PlayAnimationByIndex( index, true);
		}*/
	}

	// 获取子模型数量
	size_t  MOLogic::GetMeshNum()
	{
		//IMd3Entity* pMd3Entity = m_pEntityList[0];
		//if ( pMd3Entity)
		//{
		//	return pMd3Entity->GetMeshNum();
		//}

		return 0;
	}

	// 新建角色
	void MOLogic::NewCharacter( const char* resPath)
	{/*
		ICharacterEntity* pEntity = Root::GetRenderSystem()->CreateCharacterEntity();

		pEntity->Save( resPath);*/
	}

	// 获取材质文件中材质数
	int MOLogic::GetMaterialNumInFile( const char* matFile)
	{
		// 加载材质
		//SPtr<ISurfaceMaterial> pMaterial = g_pResManager->CreateSurfaceMaterial( matFile, false);
		//pMaterial->LoadResource();

		//return pMaterial->GetMaterialNum();
		return 0;
	}

	// 显示骨骼坐标系
	void MOLogic::ShowBoneCoord( const Matrix44& coord)
	{
		//m_p3DAxis->SetVisible( true);
		//m_p3DAxis->SetPosition( coord._41, coord._42, coord._43);
	}

	// 转换纹理
	void MOLogic::ChangeTextureToDDS( vector<string>& textures)
	{
		for ( size_t i=0; i<textures.size(); i++)
		{
			//D3DXCreateTexture( g_pDevice, iWidth, iHeight, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_DXT1, D3DX, &m_pTexture)
		}
	}

	//
	// inline
	//
	bool MOLogic::UpdateFPS(float fTime)
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
	void MOLogic::SetHandleType( EHandleType type)
	{
		m_flags = type;
	}
}