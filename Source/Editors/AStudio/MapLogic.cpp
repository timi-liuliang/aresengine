#include "MapLogic.h"
#include "SCMainWindow.h"
#include "SCLogic.h"
#include <Engine/RenderSystem/AresLight.h>
#include <Engine/FxSystem/AresFxDecal.h>
#include <Engine/PhysicsSystem/AresCollisionObjectUserData.h>
#include <Physics/AresRaycastVehicle.h>
#include <Physics/AresDefaultMotionState.h>

using namespace QT_UI;

#define BULLET_TEST 0

namespace Ares
{
	// 构造函数
	CMapLogic::CMapLogic()
		: m_massSpringSystem( NULL)
		, m_cloth( NULL)
		, m_springRender( NULL)
	{
		CreateWhiteGrid();
		CreateRedCross();

#if BULLET_TEST
		InitPhysics();
#endif
	}

	// 析构函数
	CMapLogic::~CMapLogic()
	{
#if BULLET_TEST
		for ( int i=0; i<m_collisionShapes.size(); i++)
			SAFE_DELETE( m_collisionShapes[i]);

		m_collisionShapes.clear();

		SAFE_DELETE( m_dynamicsWorld);
		SAFE_DELETE( m_solver);
		SAFE_DELETE( m_broadphase);
		SAFE_DELETE( m_dispatcher);
		SAFE_DELETE( m_collisionConfiguration);
#endif
	}

	// 地图更新
	void CMapLogic::FrameMove( float fTime)
	{
		if( SCLogic_Scene)
		{
			// 更新导航窗口
			SCLogic_Scene->SetNavWindow( SCLogic_Camera->GetPosition().x, SCLogic_Camera->GetPosition().y, 200.f);

#if BULLET_TEST
			m_dynamicsWorld->stepSimulation( fTime);
			m_bulletDebugDrawer->FrameMove( fTime);
#endif
		}
	}

	// 鼠标检选
	Entity* CMapLogic::SelectEntity()
	{
		Entity* pEntity = NULL;
		if ( SCLogic_Scene)
		{
			Vector3 rayOrig;
			Vector3 rayDir;	
			Logic_Physics->GetPickRay( rayOrig, rayDir);

			CollisionObject* object = NULL;
			SCLogic_Scene->GetPhysicsWorld()->RaycastSingle( rayOrig, rayDir, 1000.f, object);
			if( object)
			{
				CollisionObject::UserData* userData = object->GetUserData();
				if( userData)
				{
					switch( userData->m_type)
					{
					case COU_EntityFilter:
						{
							COUserDataEntityFilter* filter = dynamic_cast<COUserDataEntityFilter*>( userData);
							if( filter)
								pEntity = filter->m_entity;
						}
						break;
					}
				}
			}
		}

		return pEntity;
	}

	// 恢复上次关闭时状态
	void CMapLogic::Restore()
	{
		Signal_OnSceneChanged();
	}

	// 创建地图
	void CMapLogic::CreateScene( const char* name, const char* location)
	{
		SCLogic_Scene->Reset();
		SCLogic_Scene->SetName( name);
		SCLogic_Scene->SetLocation( location);

		SCUI_SceneInspectorModel->SetScene( SCLogic_Scene);

		// 设置场景默认大小
		SetSceneSize( 0, 0, 512, 512);

		Signal_OnSceneChanged();
	}

	// 打开地图
	void CMapLogic::OpenScene( const char* filename)
	{
		//SAFE_DELETE( m_scene);

		if( strlen( filename) && SCLogic_Scene)
		{
			Logic_Cfg->AddToRecentlyFile( filename);

			//m_scene = SCLogic_SceneSystem->CreateScene();

			SCLogic_Scene->Load( filename);

			// 硬编码
			//SetNavWindow( 0.f, 0.f, 200.f);
		}

		Signal_OnSceneChanged();
	}

	// 保存地图
	void CMapLogic::SaveMap( const char* filename)
	{
		if( SCLogic_Scene)
		{
			string sceneName = SCLogic_Scene->GetName();
			if( sceneName == string("UnSaved.map"))
			{
				SaveMapAs();
			}
			else
				SCLogic_Scene->Save( filename);			
		}
	}

	// 另存为
	void CMapLogic::SaveMapAs()
	{
		//if( m_scene)
		//{
		//	m_scene->SetLocation();
		//	m_scene->SetName();

		//	m_scene->Save();
		//}
	}

	// 新建区域
	void CMapLogic::SetSceneSize( size_t posX, size_t posY, int width, int height)
	{ 
		if( SCLogic_Scene)
		{
			// 设置区域大小
			//Scene::SSize tSize;
			//tSize.m_posX   = posX;
			//tSize.m_posY   = posY;
			//tSize.m_height = height;
			//tSize.m_width  = width;

			// 求半径, 中心点
			//float tRadius = (std::max)( tSize.m_height, tSize.m_width) / 2.f;
			//float tCenterX= tSize.m_posX + tSize.m_width / 2.f;
			//float tCenterY= tSize.m_posY + tSize.m_height/ 2.f; 
			//m_scene->SetNavWindow(  tCenterX, tCenterY, tRadius);
			//m_scene->SetSize( tSize);
			//m_scene->Save();

			//float progress = m_scene->GetLoadingProgress();
			//while( progress< 1.f)
			//{
			//	Sleep( 100);

			//	progress = m_scene->GetLoadingProgress();
			//}

			// 默认光源
			{
				// 白色, 充盈于整个区域
				LightPtr lightDir = SCLogic_Scene->CreateLight( "DefaultDirectionalLight", LT_Direction);
				lightDir->SetDirection( Vector3( 2.f, -2.f, -2.f));
				lightDir->SetColor( ColorRGB( 0.8f, 0.8f, 0.8f, 1.5f));
				lightDir->SetLocalBounds( Rect3::Infinity);
				lightDir->SetLightShaft( "data\\fx\\Sun.afx");
			}

			// 测试Vehicle tank
			//RaycastVehicle* tank = new RaycastVehicle;
			//SCLogic_Scene->GetPhysicsWorld()->AddVehicle( tank);

			// 测试 布料动画
			m_clothFabric = MakeSharePtr( new_ ClothFabric);

			GridClothBuilder clothBuilder( *m_clothFabric, 31, 31, Transform());

			SCLogic_Scene->AddEntity( "cloth 0", m_clothFabric);
		}
	}

	// 设置Window(中心点x,y; 半径radius)
	void CMapLogic::SetNavWindow( float x, float y, float radius)
	{
		if( SCLogic_Scene)
			SCLogic_Scene->SetNavWindow( x, y, radius);
	}

	#define lineNum 10

	// 创建白色网格,确定模型位置
	void CMapLogic::CreateWhiteGrid()
	{
		RenderSystem* renderSystem= &SCLogic_Scene->GetRenderSystem();

		m_whiteGrid = SELogic_VisualShape.CreateVisualShape();

		boost::array<Vector3, lineNum*8> vertexData;
		boost::array<WORD	 , lineNum*8> indexData;

		for ( int i=0; i<lineNum;i++)
		{
			int j=i+1;
			vertexData[i*8+0] = Vector3(  lineNum,        j, 0); 
			vertexData[i*8+1] = Vector3( -lineNum,        j, 0);
			vertexData[i*8+2] = Vector3(        j,  lineNum, 0);
			vertexData[i*8+3] = Vector3(        j, -lineNum, 0);
			vertexData[i*8+4] = Vector3(  lineNum,       -j, 0); 
			vertexData[i*8+5] = Vector3( -lineNum,       -j, 0);
			vertexData[i*8+6] = Vector3(	   -j,  lineNum, 0);
			vertexData[i*8+7] = Vector3(	   -j, -lineNum, 0);
		}

		for( size_t i=0; i<indexData.size(); i++)
			indexData[i] = i;

		int vertexSizeInByte  = sizeof( Vector3) * vertexData.size();
		int indicesSizeInByte = sizeof(WORD) * indexData.size();
		m_whiteGrid->Set( RenderLayout::TT_LineList, vertexData.data(), vertexSizeInByte, indexData.data(), indicesSizeInByte, EF_R16UI);
		m_whiteGrid->GetTransform()->SetTrans( 0.f, 0.f, 0.f);
		m_whiteGrid->SetColor( ColorRGB( 0.6f, 0.6f, 0.6f));
		m_whiteGrid->SetVisible( true);
	}

	// 创建红色中线
	void CMapLogic::CreateRedCross()
	{
		m_redCross	= SELogic_VisualShape.CreateVisualShape();

		Vector3 vertexData[4];
		WORD	indexData[4] = { 0, 1, 2, 3 };

		vertexData[0] = Vector3(  -lineNum,			   0,  0); 
		vertexData[1] = Vector3(   lineNum,			   0,  0);
		vertexData[2] = Vector3(			 0, -lineNum,  0);
		vertexData[3] = Vector3(			 0,	 lineNum,  0);

		int vertexSizeInByte  = sizeof( Vector3) *  4;
		int indicesSizeInByte = sizeof(WORD) * 4;
		m_redCross->Set( RenderLayout::TT_LineList, vertexData, vertexSizeInByte, indexData, indicesSizeInByte, EF_R16UI);
		m_redCross->GetTransform()->SetTrans( 0.f, 0.f, 0.f);
		m_redCross->SetColor( ColorRGB( 0.7f, 0.0f, 0.0f));
		m_redCross->SetVisible( true);
	}

		// 设置地形UV缩放
	void CMapLogic::SetUVScale(int index, float value)
	{
		Terrain* pTerrain = NULL;//GetScene()->GetTerrain();
		if( pTerrain)
		{/*
			SPtr<ISurfaceMaterial> pMaterial = pTerrain->GetMaterial();

			// 设置纹理缩放
			float  pUVScale[8] = { 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f};		

			if (pMaterial)
			{
				pMaterial->GetStaticData("g_uvScale", &pUVScale);

				pUVScale[index] = value;

				pMaterial->SetStaticData("g_uvScale", &pUVScale);
			}*/
		}
	}

	// 获取UV坐标值
	float CMapLogic::GetUVScale(int index)
	{
		Terrain* pTerrain = NULL;//GetScene()->GetTerrain();
		if( pTerrain)
		{
			/*SPtr<ISurfaceMaterial> pMaterial = pTerrain->GetMaterial();

			// 设置纹理缩放
			float  pUVScale[8] = { 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f};		

			if (pMaterial)
			{
				pMaterial->GetStaticData("g_uvScale", &pUVScale);

				return pUVScale[index];
			}*/
		}

		return 1.f;
	}

	// 向当前地形导入纹理混合度
	void CMapLogic::ImportTerrainTexBlend( int idx, const char* texPath)
	{
		if( strcmp( texPath, ":/icon/Icon/null.png")==0)
			return;
		
		Terrain* pTerrain = NULL;//GetScene()->GetTerrain();
		if( !pTerrain)
			return;
		/*
		SPtr<ITexture> tImage = g_pResManager->CreateTexture( texPath, D3DPOOL_MANAGED, false);
		tImage->LoadResource();

		int texWidth = tImage->GetWidth();
		int texHeight= tImage->GetHeight();

		for ( int i=0; i<texWidth; i++)
		{
			for ( int j=0; j<texHeight; j++)
			{
				// 临时变量
				DWORD  tBlend = pTerrain->GetTexBlend( i, j);

				// 获取纹理混合值
				DWORD color;
				color = tImage->GetColor( i, j);

				//if( idx ==0)
				//	memcpy( &tBlend.m_weight[0], &color, sizeof( DWORD));
				//else
				//	memcpy( &tBlend.m_weight[4], &color, sizeof( DWORD));

				pTerrain->SetTexBlend( i, j, tBlend);
			}
		}*/
	}

	// 向当前地形导入高度图
	void CMapLogic::ImportTerrainHeight( const char* heightPath)
	{
		Terrain* pTerrain = NULL;//GetScene()->GetTerrain();
		if( !pTerrain)
			return;

		int terrainWidth = pTerrain->GetHeightField().GetWidth() + 1;
		int terrainHeight= pTerrain->GetHeightField().GetHeight()+ 1;

		FILE* fileHandle = fopen( heightPath, "rb");

		if( fileHandle)
		{
			// 1.读取地形宽高
			int width, height, origWidth;
			fread( &width, sizeof( float), 1, fileHandle);
			fread( &height, sizeof( float), 1, fileHandle);

			origWidth = width;
			width = min( width, terrainWidth);
			height= min( height, terrainHeight); 

			float* heightData = NULL;
			int  heightNum = width * height;
			if( heightNum)
			{
				heightData = new float[heightNum];

				fread( heightData, heightNum * sizeof(float), 1, fileHandle);

				for ( int x=0; x<width; x++)
				{
					for( int y=0; y<height; y++)
					{
						int index = y * origWidth + x;
						//mapY*(m_iTableWidth+1) + mapX
						pTerrain->SetHeight( x, height -1 -y, heightData[index]);
					}
					
				}

				SAFE_DELETE( heightData);
			}

			fflush( fileHandle);
			fclose( fileHandle);
		}
	}

		// 设置地形用纹理
	void CMapLogic::SetTerrainTexture( int index, const char* resPath)
	{
		/*SPtr<ITexture> pTexture = g_pResManager->CreateTexture( resPath, D3DPOOL_DEFAULT);

		if( pTexture)
		{
			Terrain* pTerrain = m_mapLogic.GetScene()->GetTerrain();
			if( pTerrain)
			{
				
				SPtr<ISurfaceMaterial> pMaterial = pTerrain->GetMaterial();
				if( pMaterial)
				{
					pMaterial->SetTexture( index, pTexture);
				}
			}
		}*/
	}

	// 设置地形用纹理
	const char* CMapLogic::GetTerrainTexture( int index)
	{
		if( SCLogic_Scene)
		{
			Terrain* pTerrain = NULL;//GetScene()->GetTerrain();
			if( pTerrain)
			{/*
				SPtr<ISurfaceMaterial> pMaterial = pTerrain->GetMaterial();
				if( pMaterial && pMaterial->IsResourceLoaded())
				{
					SPtr<ITexture> pTexture = pMaterial->GetTexture( index);
					if( pTexture)
						return  pTexture->GetResPath().c_str();
					else
						return NULL;
				}*/
			}
		}

		return NULL;
	}

	// 编辑地形或纹理	
	void CMapLogic::EditTerrainMeshOrTexture( float fTime)
	{
		// 鼠标左键未按下
		if ( !SCLogic_Input->IsMouseDown(OIS::MB_Left) /*|| !IsCursorInWindow( m_hWnd)*/) 
			return;

		if ( SCLogic_Scene)
		{
			Vector3 intersectPos;
			Entity* pEntity = SCLogic_Map->SelectEntity();
			Logic_Physics->Pick( &intersectPos, NULL);

			Terrain* pTerrain = SCLogic_EntityEdit->GetEditEntity<Terrain>();
			if ( pTerrain)
			{
				Vector3 terrainBegin = pTerrain->GetWorldBounds().GetMinPoint();

				// 相交点
				Vector3 pos = intersectPos - terrainBegin;
				WORD x = (WORD)(pos.x - m_meshEditorData.size + 1);
				WORD y = (WORD)(pos.y - m_meshEditorData.size + 1);

				// 求出选择点平均高度( 仅为平滑地表使用)
				float avgHeight = 0.0f;
				if ( SCLogic_Main->GetHandleType() == EM_HT_EDITORTERRAIN && m_meshEditorData.type == STerrainMeshEditorData::EM_CALM1)
				{
					static float totalHeight;
					totalHeight = 0.0f;

					for ( int i=0; i<m_meshEditorData.size * 2; i++)
					{
						for ( int j=0; j<m_meshEditorData.size * 2; j++)
						{
							WORD tpX = x + i;
							WORD tpY = y + j;

							totalHeight += pTerrain->GetHeight( tpX, tpY);
						}
					}

					avgHeight = totalHeight / pow(m_meshEditorData.size * 2.0f, 2);
				}

				// 求出要修改的所有点
				for ( int i=0; i<m_meshEditorData.size * 2; i++)
				{
					for ( int j=0; j<m_meshEditorData.size * 2; j++)
					{
						WORD tpX = x + i;
						WORD tpY = y + j;

						float relaHeight= 0.f;
						float tpDis     = sqrtf( (tpX - pos.x)*(tpX - pos.x) + (tpY - pos.y) * (tpY - pos.y));
						tpDis           = tpDis * 1.414f * PI / 4.f / (float)m_meshEditorData.size;

						// 圆形
						if ( tpDis > m_meshEditorData.size && 	m_meshEditorData.shape == STerrainMeshEditorData::EM_TYPE_CIRCULAR)
							continue;

						if ( SCLogic_Main->GetHandleType() == EM_HT_EDITORTERRAIN)
						{
							switch( m_meshEditorData.type)
							{
							case STerrainMeshEditorData::EM_UP_HEAVAL:		// 隆起地表
								{
									relaHeight = cos(tpDis)  * 0.1f; 
								}
								break;
							case STerrainMeshEditorData::EM_DEBASE:			// 降低地表
								{
									relaHeight = -cos(tpDis) * 0.1f; 
								}
								break;
							case STerrainMeshEditorData::EM_CALM1:          // 光滑
								{
									relaHeight = pTerrain->GetHeight( tpX, tpY);
									float tpHeight = cos(tpDis) * 0.1f;
									if ( relaHeight > avgHeight)
									{
										if ( relaHeight - tpHeight > avgHeight) relaHeight = -tpHeight;
										else return;
									}
									else
									{
										if ( relaHeight + tpHeight < avgHeight) relaHeight = tpHeight;
										else return;
									}
								}
								break;
							case STerrainMeshEditorData::EM_CALM2:          // 强制整平
								{
									relaHeight = m_meshEditorData.height - pTerrain->GetHeight( tpX, tpY);
								}
								break;
							case STerrainMeshEditorData::EM_CRAGGEDNESS:	// 陡峭地表
								{
									relaHeight = 0.0f;
								}
								break;
							}

							relaHeight += pTerrain->GetHeight( tpX, tpY);

							pTerrain->SetHeight( tpX, tpY , relaHeight);
						}
						else if ( SCLogic_Main->GetHandleType() == EM_HT_EDITTERRAINTEXTURE)
						{/*
							static float tTimeAdd = 0.f;
							tTimeAdd += fTime;

							if( tTimeAdd > 0.2f)
							{
								// 临时变量
								Terrain::STexBlend  tBlend;
								float fBlend[ITerrain::EM_MAX_BLEND_TEX];

								// 首先获取混合度
								Terrain*  pTerrain = m_mapLogic.GetActiveArea()->GetTerrain();
								if( !pTerrain->GetTexBlend( tpX, tpY, tBlend))
									return;

								for ( int bIndex=0; bIndex<ITerrain::EM_MAX_BLEND_TEX; bIndex++)
									fBlend[bIndex] = (float)tBlend.m_weight[bIndex];

								//float relaAdd = 30.f * tTimeAdd; 
								float relaAdd = cos(tpDis) * 25.f; 

								// 添加混合度
								fBlend[ m_meshEditorData.activeTexture] += relaAdd;

								// 归一化
								float total = 0;
								for ( int bIndex=0; bIndex<ITerrain::EM_MAX_BLEND_TEX; bIndex++)
									total += fBlend[bIndex];

								for ( int bIndex=0; bIndex<ITerrain::EM_MAX_BLEND_TEX; bIndex++)
									fBlend[bIndex] = fBlend[bIndex] / total * 256.f;

								// 更新混合度
								for ( int bIndex=0; bIndex<ITerrain::EM_MAX_BLEND_TEX; bIndex++)
									tBlend.m_weight[bIndex] = Clamp( (int)fBlend[bIndex], 0, 255);

								pTerrain->SetTexBlend( tpX, tpY, tBlend);

								tTimeAdd = 0.f;
							}*/
						}
					}
				}

				pTerrain->UpdateDisplay();
			}
		}
	}

	#define SCALING 1.
	#define START_POS_X 0
	#define START_POS_Y 0
	#define START_POS_Z 10

	#define ARRAY_SIZE_X 5
	#define ARRAY_SIZE_Y 5
	#define ARRAY_SIZE_Z 5

	// 初始化物理
	void CMapLogic::InitPhysics()
	{
		// 世界初始化
		m_collisionConfiguration = new btDefaultCollisionConfiguration();
		m_dispatcher		     = new btCollisionDispatcher( m_collisionConfiguration);
		m_broadphase			 = new btDbvtBroadphase();
		m_solver				 = new btSequentialImpulseConstraintSolver;
		m_dynamicsWorld			 = new btDiscreteDynamicsWorld( m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);
		m_bulletDebugDrawer		 = new BulletDebugDrawer( SCLogic_Scene, m_dynamicsWorld);

		// 设置重力
		m_dynamicsWorld->setGravity( btVector3( 0, 0, -10));

		///create a few basic rigid bodies
		btBoxShape* groundShape = new btBoxShape(btVector3(btScalar(50.),btScalar(50.),btScalar(1.)));
		m_collisionShapes.push_back(groundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0,-0,0));

		//We can also use DemoApplication::localCreateRigidBody, but for clarity it is provided here:
		{
			btScalar mass(0.);

			//rigidbody is dynamic if and only if mass is non zero, otherwise static
			bool isDynamic = (mass != 0.f);
			btVector3 localInertia(0,0,0);
			if (isDynamic)
				groundShape->calculateLocalInertia(mass,localInertia);

			//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
			btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,groundShape,localInertia);
			btRigidBody* body = new btRigidBody(rbInfo);

			//add the body to the dynamics world
			m_dynamicsWorld->addRigidBody(body);
		}

		{
			//create a few dynamic rigidbodies
			// Re-using the same collision is better for memory usage and performance

			btBoxShape* colShape = new btBoxShape(btVector3(SCALING*1,SCALING*1,SCALING*1));
			//btCollisionShape* colShape = new btSphereShape(btScalar(1.));
			m_collisionShapes.push_back(colShape);

			/// Create Dynamic Objects
			btTransform startTransform;
			startTransform.setIdentity();

			btScalar	mass(1.f);

			//rigidbody is dynamic if and only if mass is non zero, otherwise static
			bool isDynamic = (mass != 0.f);

			btVector3 localInertia(0,0,0);
			if (isDynamic)
				colShape->calculateLocalInertia(mass,localInertia);

			float start_x = START_POS_X - ARRAY_SIZE_X/2;
			float start_y = START_POS_Y;
			float start_z = START_POS_Z - ARRAY_SIZE_Z/2;

			for (int k=0;k<ARRAY_SIZE_Y;k++)
			{
				for (int i=0;i<ARRAY_SIZE_X;i++)
				{
					for(int j = 0;j<ARRAY_SIZE_Z;j++)
					{
						startTransform.setOrigin(SCALING*btVector3( btScalar(2.0*i + start_x), btScalar(2.0*k + start_y), btScalar(20+2.0*j + start_z)));

						//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
						btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
						btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
						btRigidBody* body = new btRigidBody(rbInfo);

						m_dynamicsWorld->addRigidBody(body);
					}
				}
			}
		}
	}
}