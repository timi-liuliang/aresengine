#include "EntityPutLogic.h"
#include "SCLogic.h"
#include <Engine/RenderSystem/AresLight.h>
#include <Core/AresChar.h>
#include <Engine/RenderSystem/AresRenderSystem.h>
#include <Engine/RenderSystem/AresTerrain.h>
#include <Engine/RenderSystem/AresOcean.h>

namespace Ares
{
	// 构造函数 
	CEntityPutLogic::CEntityPutLogic()
	{	
		Reset();
	}

	// 清除
	void CEntityPutLogic::Reset()
	{
	}

	// 设置
	void CEntityPutLogic::New( EntityTypeDetail type)
	{
		m_typeDetail = type;
		Scene* pScene = SCLogic_Scene;
		if( !pScene)
			return;

		// 新建实体结点
		switch( type)
		{
		case ED_Model:
			{
				if( FileUtils::IsFileExist( m_resPath))
				{
					m_currEntity = AResSystem.LoadModel( m_resPath.c_str());

					pScene->AddEntity( "EntityPutCurrent", m_currEntity);
				}
			}
			break;
		case ED_LightPoint:
			{
				// 白色, 充盈于整个区域
				LightPtr lightPoint = AResSystem.MakeLight( LT_Point);
				lightPoint->SetColor( ColorRGB( 1.f, 1.f, 0.6f));

				m_currEntity = lightPoint;
			}
			break;
		/*case LIGHT_DIREC_DYNAMIC:
			{
				ILightDirectional* light = g_pRenderSystem->GetLightManager()->CreateDirectionalLight();
				m_currEntity = light;
			}
			break;*/
		case ED_LightAmbient:
			{
				LightPtr lightAmbient = AResSystem.MakeLight( LT_Ambient);
				lightAmbient->SetColor( ColorRGB( 0.2f, 0.2f, 0.2f));

				m_currEntity = lightAmbient;
			}
			break;
		case ED_Terrain:
			{
				TerrainPtr terrain  = MakeSharePtr( new_ Terrain);
				m_currEntity		= terrain;
				if( terrain)
				{
					terrain->Build( 64, 64);

					pScene->AddEntity( "EntityPutCurrent", m_currEntity);
				}
			}
			break;
		case ED_Ocean:
			{
				OceanPtr ocean = MakeSharePtr( new_ Ocean);
				m_currEntity   = ocean;
				if( ocean)
				{
					pScene->AddEntity( "EntityPutCurrent", m_currEntity);
				}
			}
			break;
		default:
			m_currEntity = s_ptr<Entity>();
		}

		if( m_currEntity)
		{
			// 临时
			m_currEntity->SetStatic( true);
		}
	}

	// 每帧更新
	void CEntityPutLogic::FrameMove( float fTime, bool IsCursorInWindow)
	{
		if ( m_currEntity)
		{
			m_currEntity->FrameMove( fTime);

			Vector3 pos( 0, 0, 0);
			if( Logic_Physics->Pick( &pos, NULL))
			{
				Transform transform = m_currEntity->GetTransform();
				transform.SetTrans( pos);
				m_currEntity->SetTransform( transform);

				if ( IsMLBDown() && IsCursorInWindow)
					Put();
			}
		}

		if ( IsMRBDown())
			Reset();
	}

	// 放置
	void CEntityPutLogic::Put()
	{
		static DWORD tTime = 0;
		DWORD curTime = GetTickCount();
		if( curTime - tTime < 500)
			return;

		if( !m_currEntity) 
			return;

		// 获取唯一实体名
		char tEntityName[512];
		if( !GetUniqueEntityName( tEntityName))
			return;

		// 移除
		Scene* pScene = SCLogic_Scene;
		if( pScene)
			pScene->DelEntity( "EntityPutCurrent");
	
		// 进入编辑状态, 并添加到区域中
		SCLogic_EntityEdit->AddEntity( tEntityName, m_currEntity);

		m_currEntity = s_ptr<Entity>();

		if( SCLogic_Input->IsKeyDown( OIS::KC_LCONTROL))
		{
			//New();
		}

		tTime = GetTickCount();
	}

	// 获取唯一实体名
	bool CEntityPutLogic::GetUniqueEntityName( char* uniqueName)
	{
		Scene* scene = SCLogic_Scene;
		if( !scene)
			return false;

		char tName[512];
		char tEntityName[512];
		if( m_typeDetail==ED_Model)
			_splitpath( m_resPath.c_str(), NULL, NULL, tName, NULL);
		else if( m_typeDetail==ED_Terrain)
			_splitpath( "Terrain", NULL, NULL, tName, NULL);
		else if( m_typeDetail==ED_Ocean)
			_splitpath( "Ocean", NULL, NULL, tName, NULL);
		else if( m_typeDetail==ED_LightAmbient)
			_splitpath( "LightAmbient", NULL, NULL, tName, NULL);
		else if( m_typeDetail==ED_LightPoint)
			_splitpath( "LightPoint", NULL, NULL, tName, NULL);
		else if( m_typeDetail==ED_LightDirection)
			_splitpath( "LightDirection", NULL, NULL, tName, NULL);

		for ( int i=0; i<1000000; i++)
		{
			sprintf( tEntityName, "%s_%d", tName, i);

			s_ptr<Entity> pEntity = scene->GetEntity( tEntityName);
			if( !pEntity)
			{
				strcpy( uniqueName, tEntityName);

				return true;
			}
		}

		return false;
	}
}