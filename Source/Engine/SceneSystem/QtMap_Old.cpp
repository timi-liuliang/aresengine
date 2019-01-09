//#include "QtMap_.h"
//#include <Core/AresChar.h>
//#include "AresSceneSystem.h"
//#include "../AresRoot.h"
//#include "../RenderSystem/Light/AresLightEntity.h"
//#include "../CameraSystem/AresCameraCallback.h"
//#include "../RenderSystem/AresRenderSystem.h"
//
//namespace Ares
//{
//	//// 渲染方法
//	//static void RenderShadowMaps( const CRect3D& searchRect, const CFrustum* pOptionalFrustum)
//	//{
//	//	g_pSceneManager->RenderShadowMap( searchRect, pOptionalFrustum);
//	//}
//
//	// 构造函数
//	CQtMap::CQtMap()
//	{
//		m_version = 1;
//	}
//
//	// 析构函数
//	CQtMap::~CQtMap()
//	{
//		int a = 10;
//	}
//
//	// 获取地图名
//	const char* CQtMap::GetName()
//	{
//		return m_name.c_str();
//	}
//
//	// 设置Window(中心点x,y; 半径radius)
//	//void CQtMap::SetNavWindow( float x, float y, float radius)
//	//{
//	//	m_navWindow.m_x = x;
//	//	m_navWindow.m_y = y;
//
//	//	m_navWindow.m_radius = radius;
//
//	//	// 更新所有区域的导航窗口信息
//	//	for( AreaList::iterator it=m_areaList.begin(); it!=m_areaList.end(); it++)
//	//	{
//	//		(*it)->SetNavWindow( m_navWindow);
//	//	}
//	//}
//
//	// 更新
//	void CQtMap::FrameMove( float fTime)
//	{
//		for( AreaList::iterator it=m_areaList.begin(); it!=m_areaList.end(); it++)
//		{
//			(*it)->FrameMove( fTime);
//		}
//	}
//
//	// 获取区域
//	IArea* CQtMap::GetArea( const char* areaName)
//	{
//		for( AreaList::iterator it=m_areaList.begin(); it!=m_areaList.end(); it++)
//		{
//			if( strcmp( (*it)->GetName(), areaName) == 0)
//				return *it;
//		}
//
//		return NULL;
//	}
//
//	// 设置地图名
//	//void CQtMap::SetName( const char* name)
//	//{
//	//	m_name = name;
//	//}
//
//	//// 添加区域
//	//bool CQtMap::AddArea( IArea* pArea)
//	//{
//	//	IArea* tArea = GetArea( pArea->GetName());
//
//	//	if( !tArea)
//	//	{
//	//		string tLocation = m_location + pArea->GetName() + "/";
//	//		pArea->SetLocation( tLocation.c_str());
//
//	//		m_areaList.push_back( pArea);
//
//	//		return true;
//	//	}
//
//	//	return false;
//	//}
//
//	//// 获取当前加载进度
//	//float CQtMap::GetLoadingProgress()
//	//{
//	//	float progress = 0.f;
//
//	//	// 保存区域
//	//	if( m_areaList.size())
//	//	{
//	//		for( size_t i=0; i<m_areaList.size(); i++)
//	//			progress += m_areaList[i]->GetLoadingProgress();
//
//	//		progress /= m_areaList.size();
//	//	}
//	//	else
//	//		progress = 1.f;
//
//	//	return progress;
//	//}
//
//	//// 获取包围盒
//	//const Rect3& CQtMap::GetBoundingBox()
//	//{
//	//	static Rect3 boundingBox( 0.f, 512.f, 0.f, 512.f, 0.f, 20.f);
//
//	//	return boundingBox;
//	//}
//
//	// 获取包围盒
//	//bool CQtMap::GetBoundingBoxByLayer( Rect3& box, const char* layer, bool update)
//	//{
//	//	// 更新所有区域的导航窗口信息
//	//	for( AreaList::iterator it=m_areaList.begin(); it!=m_areaList.end(); it++)
//	//	{
//	//		(*it)->GetBoundingBoxByLayer( box, layer, update);
//	//	}
//
//	//	return true;
//	//}
//
//	// 加载
//	void CQtMap::Load( const char* fileName)
//	{
//		FILE* fileHandle = fopen( fileName, "rb");
//		if( fileHandle)
//		{
//			// 设置位置
//			char tLocation[1024];
//			changeNameExt( tLocation, fileName, "");
//			SetLocation( tLocation);
//
//			// 加载版本号
//			fread( &m_version, sizeof(m_version), 1, fileHandle);
//
//			switch( m_version)
//			{
//			case 1:
//				Load0_1( fileHandle, fileName);
//				break;
//			}
//
//			fflush( fileHandle);
//			fclose( fileHandle);
//		}
//	}
//
//	// 保存
//	void CQtMap::Save( const char* fileName)
//	{
//		string tLocationName;
//		if( !fileName)
//			tLocationName = m_location + m_name;
//		else
//			tLocationName = fileName;
//
//		FILE* fileHandle = fopen( tLocationName.c_str(), "wb");
//		if( fileHandle)
//		{
//			// 保存版本号
//			fwrite( &m_version, sizeof(m_version), 1, fileHandle);
//
//			// 保存地图名
//			fwrites( m_name.c_str(), fileHandle);
//
//			// 区域数量
//			size_t areaNum = m_areaList.size();
//			fwrite( &areaNum, sizeof( size_t), 1, fileHandle);
//
//			// 保存区域名
//			vector<string> areaLocations;
//			for( size_t i=0; i<m_areaList.size(); i++)
//			{
//				// 计算区域保存位置
//				char tAreaLocationName[1024];
//				sprintf( tAreaLocationName, "%s/%s.area", m_areaList[i]->GetName(),  m_areaList[i]->GetName());
//				fwrites( tAreaLocationName, fileHandle);
//
//				areaLocations.push_back( tAreaLocationName);
//			}
//
//			fflush( fileHandle);
//			fclose( fileHandle);
//
//			// 保存区域
//			for( size_t i=0; i<m_areaList.size(); i++)
//			{
//				// 更改路径
//				char tAreaFile[1024];
//				changeNameExt( tAreaFile, tLocationName.c_str(), areaLocations[i].c_str());
//
//				m_areaList[i]->Save( tAreaFile);
//				//m_areaList[i]->SaveTerrain( tAreaFile);
//			}
//		}
//	}
//
//	// 加载版本
//	void CQtMap::Load0_1( FILE* fileHandle, const char* fileName)
//	{
//		// 加载地图名
//		freads( m_name, fileHandle);
//
//		// 区域数量
//		size_t areaNum=0;
//		fread( &areaNum, sizeof( size_t), 1, fileHandle);
//
//		// 保存区域名
//		for( size_t i=0; i<areaNum; i++)
//		{
//			string tAreaName;
//			freads( tAreaName, fileHandle);
//
//			IArea* pArea = Root::GetSceneSystem()->CreateArea();
//			if( pArea)
//			{
//				// 更改路径
//				char tAreaFile[512];
//				changeNameExt( tAreaFile, fileName, tAreaName.c_str());
//
//				pArea->Load( tAreaFile);
//				//pArea->LoadTerrain( tAreaFile);
//
//				AddArea( pArea);
//			}
//			else
//			{
//				// error
//				// 区域 tAreaName 加载失败
//			}
//		}
//	}
//
//	// AABB包围盒投身AABB包围盒
//	static Rect3 AABBCastAABB( const Rect3& cast, const Vector3& castDir, const Rect3& accept)
//	{
//		Rect3 result = cast;
//
//		// 获取投射体的八个顶点
//		//D3DXVECTOR3 tVectors[8];
//		//	cast.GetEightPoints( tVectors);
//
//		for ( int i=0; i<8; i++)
//		{
//
//		}
//
//		return result;
//	}
//
//	// 渲染
//	void CQtMap::Render()
//	{
//		m_renderQueueMgr.Reset();
//		// 更新shadowmap
//		//RenderShadowmap();
//
//		// 渲染主画面
//		{
//			SearchRenderable( "main", Root::GetCameraSystem()->GetSearchRect(),  Root::GetCameraSystem()->GetFrustum());
//
//			m_renderQueueMgr.Render( "terrain");
//
//			m_renderQueueMgr.Render( "opacity");
//		}
//	}
//
//	// 渲染阴影贴图
//	void CQtMap::RenderShadowmap()
//	{/*
//		// 阴影图
//		SetShadowCastBoundingBox();
//
//		Root::GetRenderSystem()->GetLiSPSMManager()->Begin();
//
//		for( AreaList::iterator it=m_areaList.begin(); it!=m_areaList.end(); it++)
//		{
//			Entity* pEntityList = (*it)->BuildSearchResults( Root::GetRenderSystem()->GetLiSPSMManager()->GetLightSearchRect(), NULL);
//
//			//// Render all objects
//			Entity* pFirstMember = pEntityList;
//			while (pFirstMember)
//			{
//				// 只有模型产生阴影
//				if ( pFirstMember->GetType() == RenderEntity::EM_ENTITY_RENDER)
//				{				
//					RenderEntity* pRenderMember = (RenderEntity*)pFirstMember;
//					pRenderMember->SubmitRenderable( "shadow");
//					pRenderMember = NULL;
//				}
//
//				pFirstMember = pFirstMember->NextSearchLink();
//			}
//		}
//
//		Root::GetRenderSystem()->ExecRenderQueue("opacity");
//
//		Root::GetRenderSystem()->GetLiSPSMManager()->End();*/
//	}
//
//	// 渲染
//	void CQtMap::RenderScene()
//	{
//		//ARenderFactory->GetRenderWindow()->Present();
//
//		// 搜寻并整理可见实体
//		//SearchAndTidy( g_mainCamera->GetSearchRect(),  g_mainCamera->GetFrustum());
//
//		//RenderSystem* pRenderSystem = Root::GetRenderSystem();
//		//if( pRenderSystem)
//		//{
//		//	pRenderSystem->ExecRenderQueue("opacity");
//
//		//	//pRenderSystem->GetVisualShapeMgr()->GetRenderQueue()->;
//		//}
//
//		// VisualShape queue
//	}
//
//	// 搜寻场景,使可见Renderable分组
//	void CQtMap::SearchRenderable( const string& search, const Rect3& searchRect, const Frustum3* pOptionalFrustum)
//	{
//		for( AreaList::iterator it=m_areaList.begin(); it!=m_areaList.end(); it++)
//		{
//			Entity* pEntityList = (*it)->BuildSearchResults( searchRect, pOptionalFrustum);
//
//			// 渲染前处理
//			Entity* pFirstMember = pEntityList;
//			while ( pFirstMember)
//			{
//				RenderEntity* renderEntity = dynamic_cast<RenderEntity*>( pFirstMember);
//				if( renderEntity)
//				{
//					renderEntity->SubmitRenderable( m_renderQueueMgr, search);
//				}
//
//				pFirstMember = pFirstMember->NextSearchLink();
//			}
//		}
//	}
//
//	// 整理可见实体
//	/*void CQtMap::SearchAndTidy( const Rect3& searchRect, const Frustum3* pOptionalFrustum)
//	{
//		ILightManager* lightMgr = Root::GetRenderSystem()->GetLightManager();
//		// 清空光源管理中光源
//		lightMgr->ClearActiveLightList();
//
//		for( AreaList::iterator it=m_areaList.begin(); it!=m_areaList.end(); it++)
//		{
//			Entity* pEntityList = (*it)->BuildSearchResults( searchRect, pOptionalFrustum);
//
//			// 渲染前处理
//			Entity* pFirstMember = pEntityList;
//			while ( pFirstMember)
//			{
//				if ( pFirstMember && pFirstMember->GetType() == Entity::EM_ENTITY_RENDER)
//				{
//					RenderEntity* pRenderMember = (RenderEntity*)pFirstMember;
//					pRenderMember->PrepareForRender();
//					pRenderMember = NULL;
//				}
//
//				pFirstMember = pFirstMember->NextSearchLink();
//			}
//
//			// Render all objects
//			pFirstMember = pEntityList;
//			while (pFirstMember)
//			{
//				// 此时不要渲染水
//				if ( pFirstMember->GetType() == Entity::EM_ENTITY_RENDER)
//				{				
//					RenderEntity* pRenderMember = (RenderEntity*)pFirstMember;
//					pRenderMember->SubmitRenderable( "main");
//				}
//				else if ( pFirstMember->GetType() == Entity::EM_ENTITY_LIGHT)
//				{
//					lightMgr->AddActiveLight( (ILight*)pFirstMember);
//				}
//
//				pFirstMember = pFirstMember->NextSearchLink();
//			}
//		}*/
//	//}
//
//	// 获取阴影投射体包围盒
//	void CQtMap::SetShadowCastBoundingBox()
//	{
//		ILiSPSM* lispsm = Root::GetRenderSystem()->GetLiSPSMManager();
//
//		//CRect3D castBoundingBox;
//		//GetBoundingBoxByLayer( castBoundingBox, "default", true);
//
//		Rect3 castaccept(0.f, 512.f, 0.f, 512.f,  0.f, 5.f);//= AABBCastAABB( castBoundingBox, lispsm->GetLightDir(), GetBoundingBox());
//
//		Root::GetRenderSystem()->GetLiSPSMManager()->SetShadowCastBoundingBox( castaccept);
//	}
//}