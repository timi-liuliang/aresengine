#include <Engine/SceneSystem/AresEntity.h>
#include <Engine/SceneSystem/AresScene.h>
#include <Engine/SceneSystem/AresScene.h>
#include <Physics/PhysicsWorld.h>
#include <Engine/PhysicsSystem/AresCollisionObjectUserData.h>

namespace Ares
{
	// 构造函数
	Entity::Entity( EntityType type, EntityTypeDetail typeDetail)
		: m_type( type),
		  m_typeDetail( typeDetail),
		  m_static( false)
		, m_scene( NULL)
	{
		// 客户端使用时一般为动态类型
		m_bEnabled           = true;

		m_collSift.SetUserData( new_ COUserDataEntityFilter(this));
	}

	// 虚析构函数
	Entity::~Entity()
	{
	}

	// 获取实体名
	const char* Entity::GetName()
	{
		return m_name.c_str();
	}

	// 设置实体名
	void Entity::SetName( const char* name)
	{
		m_name = name;
	}

	// 获取转换对象
	const Transform& Entity::GetTransform()
	{ 
		return m_collSift.GetTransform();
	}

	// 设置转换
	void Entity::SetTransform( const Transform& transform)
	{
		m_collSift.SetTransform( transform);
	}

	// 获取包围盒
	Rect3 Entity::GetLocalBounds()
	{
		return m_collSift.GetLocalBounds();
	}

	// 获取世界包围盒
	Rect3 Entity::GetWorldBounds() 
	{ 
		Rect3 result;

		m_collSift.BuildAABB( result);

		return result;
	}

	// 获取对应摄像机系统
	CameraSystem& Entity::GetCameraSystem() 
	{ 
		return m_scene->GetCameraSystem(); 
	}

	// 设置实体包围盒
	void Entity::SetLocalBounds( const Rect3& rect)
	{
		m_collSift.SetLocalBounds( &rect);
	}

	// 更新
	void Entity::FrameMove( float fTime)
	{
	}

	// 添加到场景消息
	void Entity::OnAddToScene( Scene* scene) 
	{
		A_ASSERT( scene);

		m_scene = scene;

		PhysicsWorldPtr& physicsWorld = scene->GetPhysicsWorld();
		if( physicsWorld)
		{
			physicsWorld->AddCollisionObject( &m_collSift);
		}
	}
}