#include "Engine/RenderSystem/AresRenderSystem.h"
#include <Engine/SceneSystem/GPWindow.h>
#include <Engine/SceneSystem/AresSceneTile.h>
#include <Engine/SceneSystem/AresScene.h>
#include <Engine/PhysicsSystem/AresCollisionObjectUserData.h>
#include <Core/AresChar.h>
#include <Physics/Intersect.h>
#include <Physics/DistLine3Point3.h>
#include <Engine/AresRoot.h>
#include <Engine/RenderSystem/AresTerrainTile.h>
#include <Physics/Collide.h>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#define TILE_SIZE 64

namespace Ares
{
	// 是否重叠包围盒
	bool SNavWindow::IsOverlapping( Rect3& rect)
	{
		if( m_x-m_radius>rect.m_max.x || m_x+m_radius<rect.m_min.x || m_y-m_radius>rect.m_max.y || m_y+m_radius<rect.m_min.y)
			return false;

		return true;
	}

	// 构造函数
	Scene::Scene(  CameraSystem* cameraCallback, const RenderSettings& settings)
		: m_cameraSystem( cameraCallback)
		, m_isActive( false)
	{
		m_renderSystem = new_ RenderSystem( *this);
		m_renderSystem->LoadRenderFactory( settings);

		m_version = 1;
		m_gpWindow= new_ CGPWindow;

		m_mainGPIdx.Set( 0, 0, TILE_SIZE, TILE_SIZE);
		m_gpWindow->SetWorld( this);

		m_physicsWorld = MakeSharePtr( new_ PhysicsWorld);

		m_submitRenderElementsConnection = m_renderSystem->Signal_OnSubmitRenderElements.connect( boost::bind<void>(&Scene::SubmitRenderElements, this));

		m_spabTime		= MakeShaderParam( float(0.f));
		m_spabSunDir	= MakeShaderParam( Vector3::ZAxis);
		m_spabSunColor	= MakeShaderParam( ColorRGB::Black);
	}

	// 析构函数
	Scene::~Scene()
	{
		// 清空物理世界
		m_physicsWorld.reset();

		// 断开信号槽
		Signal_OnEntitysChanged.disconnect_all_slots();

		m_renderSystem->Signal_OnSubmitRenderElements.disconnect( m_submitRenderElementsConnection);
	}

	// 新建场景
	Scene* Scene::Create( CameraSystem* cameraCallback, const RenderSettings& settings, const char* name, const char* location)
	{
		Scene* scene = new_ Scene( cameraCallback, settings);
		if( name && location)
		{
			scene->SetName( name);
			scene->SetLocation( location);
		}

		return scene;
	}

	// 设置导航窗口(中心点x,y; 半径radius)
	void Scene::SetNavWindow(  float x, float y, float radius)
	{
		m_navWindow.m_x = x;
		m_navWindow.m_y = y;
		m_navWindow.m_radius = radius;

		m_gpWindow->UpdatePosition( x, y, true);
		m_gpWindow->UpdateRadius( m_navWindow.m_radius);
	}

	// 更新
	void Scene::FrameMove( float fTime)
	{
		if( m_isActive)
		{
			if( m_cameraSystem)
				m_cameraSystem->FrameMove( fTime);

			if( m_renderSystem)
				m_renderSystem->FrameMove( fTime);

			// 更新ShaderParam
			m_spabTime->m_dataFloat[0] += fTime;


			for( EntityList::iterator it = m_entitys.begin(); it!=m_entitys.end(); it++)
			{
				it->second->FrameMove( fTime);
			}

			m_physicsWorld->FrameMove( fTime);
		}
	}

	// 渲染
	void Scene::Render()
	{
		if( m_isActive && m_renderSystem)
			m_renderSystem->Render();
	}

	// 提交渲染元素
	void Scene::SubmitRenderElements()
	{
		RenderElementMgr&	elementMgr	 = m_renderSystem->GetRenderElementMgr();
		static vector<Entity*> results;
		BuildSearchResults( results, m_cameraSystem->GetFrustum());
		foreach_( Entity* entity, results)
		{
			if( !entity)
				continue;

			switch( entity->GetType())
			{
			case ET_Render:
				{
					entity->SubmitRenderElements( elementMgr);
				}
				break;

			case ET_Light:
				{
					Light* light = dynamic_cast<Light*>(entity);
					if( entity->GetTypeDetail()==ED_LightDirection)
					{
						elementMgr.SetDomainLight( m_domainLight);

						*m_spabSunColor = m_domainLight->GetColor();
						*m_spabSunDir   = m_domainLight->GetDirection();
					}
					else
						elementMgr.AddLight( light);

					light->SubmitRenderElements( elementMgr);
				}
				break;
			}
		}
	}

	// 渲染场景深度
	void Scene::RenderDepth( const Box3& searchBox, const Matrix44& viewCrop)
	{
		vector<Entity*> entitys;
		BuildSearchResults( entitys, &searchBox);

		m_zWriteMgr.Write( entitys, viewCrop);
	}

	// 搜寻
	void Scene::BuildSearchResults( vector<Entity*>& results, const Shape* shape)
	{
		results.clear();

		A_ASSERT( shape->GetShapeType()==ST_Frustum || shape->GetShapeType()==ST_Rect3 || shape->GetShapeType()==ST_Box3);

		vector<CollisionObject*> objects;
		m_physicsWorld->RegionQueryCollisionObjects( objects, shape);
		for ( size_t i=0; i<objects.size(); i++)
		{
			CollisionObject::UserData* userData = objects[i]->GetUserData();
			if( userData)
			{
				switch( userData->m_type)
				{
				case COU_EntityFilter:
					{
						COUserDataEntityFilter* filter = dynamic_cast<COUserDataEntityFilter*>( userData);
						if( filter)
							results.push_back( filter->m_entity);
					}
					break;
				}
			}
		}
	}

	// 搜寻光源
	void Scene::SearchLights( vector<Light*>& lights, DirectionalLightPtr& domainLight)
	{
		domainLight = m_domainLight;

		for ( EntityList::iterator it=m_entitys.begin(); it!=m_entitys.end(); it++)
		{
			if( it->second->GetType() == ET_Light)
			{
				if( !domainLight || ( domainLight->GetName() != it->second->GetName()))
					lights.push_back( dynamic_cast<Light*>( it->second.get()));
			}
		}
	}

	// 获取地图名
	const char* Scene::GetName()
	{
		return m_name.c_str();
	}

	// 根据实体名获取实体
	s_ptr<Entity> Scene::GetEntity( const char* uniqueName)
	{
		ENTER_CRITICAL_SECTION( tHoder, m_qtCritication);

		EntityList::const_iterator it = m_entitys.find( uniqueName);
		if( it != m_entitys.end())
		{
			return it->second;
		}

		return s_ptr<Entity>();
	}

	// 设置地图名
	void Scene::SetName( const char* name)
	{
		m_name = name;
	}

	// 添加实体
	bool Scene::AddEntity( const char* uniqueName, s_ptr<Entity> pEntity, bool isSave)
	{
		if( !pEntity)
			return false;

		AddEntityToWorldOnly( uniqueName, pEntity);

		if( !isSave)
			return true;

		// 添加到各块中
		const Rect3& worldbounds = pEntity->GetWorldBounds();
		if( pEntity->GetLocalBounds() == Rect3::Infinity)
		{
			SceneTile* pTile = dynamic_cast<SceneTile*>( GetTile( CGPIndex::Global));
			if( pTile)
				pTile->AddEntity( uniqueName, pEntity);
		}
		else
		{
			int minX  = ((int)worldbounds.m_min.x /*- m_size.m_posX*/) / m_mainGPIdx.GetWidth();
			int maxX = ((int)worldbounds.m_max.x /*- m_size.m_posX*/) / m_mainGPIdx.GetWidth();
			int minY  = ((int)worldbounds.m_min.y /*- m_size.m_posY*/) / m_mainGPIdx.GetHeight();
			int maxY = ((int)worldbounds.m_max.y /*- m_size.m_posY*/) / m_mainGPIdx.GetHeight();

			for ( int i=minX; i<=maxX; i++)
			{
				for( int j=minY; j<=maxY; j++)
				{
					CGPIndex tIdx = m_mainGPIdx.GetIndex( i, j);
					SceneTile* pTile = dynamic_cast<SceneTile*>(GetTile( tIdx));
					if( pTile)
						pTile->AddEntity( uniqueName, pEntity);
				}
			}
		}		

		SetAutoUnload( false);

		Signal_OnEntitysChanged( *this);

		return true;
	}

	// 设置主光源
	bool Scene::SetDomainLight( LightPtr light, size_t resolution)
	{
		if( light->GetLightType() == LT_Direction)
		{
			m_domainLight = dynamic_pointer_cast<DirectionalLight>(light);
			m_domainLight->GetCSMMgr()->SetShadowMapResolution( resolution);
			m_domainLight->GetCSMMgr()->SetScene( this);

			return true;
		}

		ALogSystem.Warning( "Only Direction light can be set as domain light!");

		return false;
	}

	// 添加实体
	bool Scene::AddEntityToWorldOnly( const char* uniqueName, s_ptr<Entity> pEntity)
	{
		ENTER_CRITICAL_SECTION( tHoder, m_qtCritication);

		if( GetEntity( uniqueName))
			return false;

		pEntity->SetName( uniqueName);
		m_entitys[uniqueName] = pEntity;

		// 实体消息
		pEntity->OnAddToScene( this);

		// 
		if( pEntity->GetTypeDetail()==ED_LightDirection)
		{
			DirectionalLightPtr dirLight = dynamic_pointer_cast<DirectionalLight>( pEntity);
			if( dirLight)
				SetDomainLight( dirLight, 2048);
		}

		return true;
	}

	// 删除实体(当前实体包围盒与NavWindow不重合且引用计数为1)
	void Scene::DelEntity( const char* uniqueName)
	{
		EntityList::iterator eit=m_entitys.find( uniqueName);
		if( eit!=m_entitys.end())
		{
			// 从物理世界中卸载
			if( m_physicsWorld)
				m_physicsWorld->RemoveCollisionObject( &(eit->second->GetCollSift()));

			m_entitys.erase(eit);
		}

		// 遍历所有块
		for( GPTileMap::iterator it=m_tilesMap.begin(); it!=m_tilesMap.end(); it++)
		{
			SceneTile* pTile = dynamic_cast<SceneTile*>( it->second);
			if( pTile)
			{
				pTile->DelEntity( uniqueName);
			}
		}

		// 禁用自动卸载
		SetAutoUnload( false);

		Signal_OnEntitysChanged( *this);
	}

	// 设置区域保存位置
	void Scene::SetLocation( const char* location)
	{
		m_location = location;
	}

	// 创建实体
	//ModelPtr Scene::CreateModel( const char* uniqueName, const char* filePath)
	//{
	//	ModelPtr model = MakeSharePtr( new_ Model);
	//	AddEntity( uniqueName, model);

	//	Model::IDCard* idcard = NULL;
	//	Serialization::UnSerializeFromBinaryFile( filePath, idcard);
	//	model->SetFilePath( filePath);
	//	idcard->RebuildModel( *model);

	//	return model;
	//}

	// 创建光源
	LightPtr Scene::CreateLight( const char* uniqueName, LightType type)
	{
		LightPtr result;
		switch( type)
		{
		case LT_Direction:
			result = MakeSharePtr( new_ DirectionalLight);
			break;
		}

		AddEntity( uniqueName, result);

		return result;
	}

	// inline
	// 获取包围盒
	bool Scene::GetBoundingBoxByLayer( Rect3& box, const char* layer, bool update)
	{
		if( !update)
		{
			int a = 10;
		}
		else
		{
			for( GPTileMap::const_iterator it=m_tilesMap.begin(); it!=m_tilesMap.end(); it++)
			{
				SceneTile* qtGPTile = dynamic_cast<SceneTile*>(it->second);
				if( qtGPTile)
					qtGPTile->GetBoundingBoxByLayer( box, layer, update);
			}
		}

		return true;
	}

	// 构建Tile
	CGPTile* Scene::BuildTile( const CGPIndex& index)
	{
		if( !IsValidIndex( index))
			return 0;

		// tile文件存放路径
		char tileLocation[256];
		sprintf( tileLocation,"%s%d_%d.tile", m_location.c_str(), index.GetX(), index.GetY());

		SceneTile* pTile = new_ SceneTile( tileLocation);

		return pTile;
	}

	// 销毁Tile
	void  Scene::DestroyTile( const CGPIndex& index)
	{
		CGPWorld::DestroyTile( index);

		ENTER_CRITICAL_SECTION( tHoder, m_qtCritication);

		// 清除引用计数为1的实体
		for( EntityList::iterator it=m_entitys.begin(); it!=m_entitys.end(); )
		{
			// 清除实体
            Rect3 bouding = it->second->GetWorldBounds();
			if( it->second.unique() && !m_navWindow.IsOverlapping( bouding))
				m_entitys.erase(it++);
			else
				++it;
		}
	}

	// 整理实体
	void Scene::TidyEntityByTile()
	{
		for( EntityList::iterator eit=m_entitys.begin(); eit!=m_entitys.end(); eit++)
		{
			// 遍历所有块
			for( GPTileMap::iterator it=m_tilesMap.begin(); it!=m_tilesMap.end(); it++)
			{
				SceneTile* pTile = dynamic_cast<SceneTile*>( it->second);
				if( pTile)
				{
					pTile->DelEntity( eit->first.c_str());

					AddEntity( eit->first.c_str(), eit->second);
				}
			}
		}
	}

	// 加载
	void Scene::Load( const char* fileName)
	{
		// 加载数据
		ifstream ifs( fileName);
		if( ifs.is_open())
		{
			//SSize tSize;

			boost::archive::binary_iarchive biarchive( ifs);
			biarchive >> m_version >> m_name /*>> tSize*/ >> m_mainGPIdx;

			ifs.close();

			// 数据组织
			//this->SetSize( tSize);

			string twLocation;
			FileUtils::ReplaceNameExt( twLocation, fileName,"");
			SetLocation( twLocation.c_str());

			// 加载光照图
			string lightmapPath = m_location + "lightmap/";
			GetLightmapMgr().LoadXml( lightmapPath.c_str());
		}
	}

	// 保存
	void Scene::Save( const char* fileName)
	{
		// 整理实体
		TidyEntityByTile();

		// 1.写文件
		string tLocationName = fileName ? fileName : m_location + m_name;
		ofstream ofs( tLocationName.c_str());
		if( ofs.is_open())
		{
			boost::archive::binary_oarchive biarchive( ofs);
			biarchive << m_version << m_name /*<< m_size*/ << m_mainGPIdx;

			ofs.close();
		}

		// 2.保存光照图
		GetLightmapMgr().SaveXml( (m_location + "lightmap/").c_str());

		// 4.保存区块数据
		for( GPTileMap::const_iterator it=m_tilesMap.begin(); it!=m_tilesMap.end(); it++)
		{
			SceneTile* qtGPTile = dynamic_cast<SceneTile*>(it->second);

			qtGPTile->Save();
		};

		SetAutoUnload( true);
	}

	//// 加载地形数据
	//void  Scene::LoadTerrain( const char* terLocation)
	//{
	//	char tTerLocation[512];
	//	changeExt( tTerLocation, terLocation, ".ter");
	//	if( INVALID_FILE_ATTRIBUTES != GetFileAttributesA( tTerLocation))
	//	{
	//		// 存在地形
	//		TerrainPtr  pTerrain = AResSystem.MakeTerrain();
	//		if( pTerrain)
	//		{
	//			SetTerrain( pTerrain);

	//			pTerrain->Load( tTerLocation);
	//		}
	//	}
	//}
}