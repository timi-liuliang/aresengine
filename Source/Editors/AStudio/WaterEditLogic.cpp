#include "WaterEditLogic.h"

namespace Ares
{
	// 构造函数
	CWaterEditLogic::CWaterEditLogic()
	{
		/*
		m_pMaterialWater[EM_WOW_WATER] = g_pResManager->CreateSurfaceMaterial();
		m_pMaterialWater[EM_WOW_WATER]->LoadEffectFile("Data\\Shader\\wow_water.fx");

		float speed = 1.f;
		m_pMaterialWater[EM_WOW_WATER]->SetStaticData("g_speed", &speed);

		m_pMaterialWater[EM_WOW_WATER]->SetTexture( 0, "data\\texture\\water\\wow_water.dds");
		//m_pMaterialWater[EM_WOW_WATER]->SetTexture( 1, g_pSceneMgr->GetWaterRefractionMap());
		m_pMaterialWater[EM_WOW_WATER]->SetTexture( 2, "data\\texture\\water\\wavebump.dds");

		//------------------Gpu gems 2 19 章节--------------------
		m_pMaterialWater[EM_GPU_GEMS_2_19] = g_pResManager->CreateSurfaceMaterial();
		m_pMaterialWater[EM_GPU_GEMS_2_19]->LoadEffectFile( "data\\shader\\water_6_14.fx");

		m_pMaterialWater[EM_GPU_GEMS_2_19]->SetTexture( 0, "data\\texture\\water\\wave1.dds");
		//m_pMaterialWater[EM_GPU_GEMS_2_19]->SetTexture( 1, g_pSceneMgr->GetWaterRefractionMap());
		m_pMaterialWater[EM_GPU_GEMS_2_19]->SetTexture( 2, "data\\texture\\water\\nvlobby_new_cube_mipmap.dds");

		//------------------- Water Mask -------------------------
		m_pMaterialWater[EM_WATER_MASK] = g_pResManager->CreateSurfaceMaterial();
		m_pMaterialWater[EM_WATER_MASK]->LoadEffectFile( "data\\shader\\WaterMask.fx");
		*/
	}

	// 新建水
	void CWaterEditLogic::New( const Vector3& pos, WaterType type/*=EM_GPU_GEMS_2_19*/)
	{
		static DWORD tTime = 0;
		DWORD curTime = GetTickCount();
		if( curTime - tTime < 500)
			return;

		// 创建水实体
		//SPtr<IRenderEntityGeneral> pRenderEntity = g_pScene->CreateWaterEntity();
		//pRenderEntity->SetMaterial( m_pMaterialWater[type], "main", "water");
		//pRenderEntity->SetMaterial( m_pMaterialWater[EM_WATER_MASK], "main", "waterMask");

		//SPtr<ISceneNode> pSceneNode = g_pScene->RootNode()->CreateChildSceneNode();
		//pRenderEntity->AttachSceneNode( pSceneNode);

		//pSceneNode->SetPosition( pos);

		tTime = GetTickCount();
	}
}