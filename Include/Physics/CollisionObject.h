#pragma once

#include <boost/any.hpp>
#include <Core/AresMath.h>
#include <Core/AresTransform.h>
#include "Broadphase.h"
#include "Shape.h"

namespace Ares
{
	// CollisionFlags
	enum CollisionFlags
	{
		CF_StaticObject			= 1,
		CF_KinematicObject		= 2,
		CF_NoContactResponse	= 4,
	};

	// CollisionObjectTypes
	enum CollisionObjectTypes
	{
		COT_CollisionObject = 1,
		COT_RigidBody,
		COT_GhostObject,
	};

	//--------------------------------------------------------------------
	// CollisionObject can be used to manage collision detection objects
	// CollisionObject maintains all information that is needed for a 
	// collision detection: Shape Transform and AABB proxy, They can be
	// added to the CollisionWorld 2012-7-28 帝林 excerpt bullet
	//--------------------------------------------------------------------
	class CollisionObject
	{
		friend class CollisionObject;
	public:
		// 标记
		enum COFlags
		{
			CF_ActiveTag			= 1,
			CF_IslandSleeping		= 2,
			CF_WantsDeactivation	= 3,
			CF_DisableDeactivation	= 4,
			CF_DisableSimulation	= 5,
		};

		// 用户数据
		struct UserData
		{
			INT			m_type;			// 用户数据类型

			// 构造函数
			UserData( INT type) : m_type( type){}
			virtual ~UserData() {}
		};

	public:
		CollisionObject();
		~CollisionObject();

		// 是否处于活动状态
		bool IsActive() const { return (GetActivationState()!=CF_IslandSleeping) && (GetActivationState()!=CF_DisableSimulation); }

		// 是否为运动学对象
		bool IsKinematicObject() { return ( m_collisionFlags & CF_KinematicObject) != 0; }

		// 设置用户数据
		void SetUserData( UserData* userData) { m_userData = userData; }

		// 获取用户数据
		UserData* GetUserData() { return m_userData; }

		// 获取状态
		int GetActivationState() const { return m_activationState1; }

		// 设置状态
		void SetActivationState( int newState) const;

		// 设置世界转换
		void SetTransform( const Transform& transform);

		// 获取世界转换
		const Transform& GetTransform();

		// 设置碰撞模型
		void SetCollisionShape( Shape* collisionShape) { m_collisionShape = collisionShape; }

		// 获取碰撞模型
		Shape* GetCollisionShape() { return m_collisionShape; }

		// 获取碰撞模型
		const Shape* GetCollisionShape() const  { return m_collisionShape; }

		// 获取代理
		BroadphaseProxy* GetBroadphaseProxy() { return m_broadphaseProxy; }

		// 获取代理
		const BroadphaseProxy* GetBroadphaseProxy() const { return m_broadphaseProxy; }

		// 设置代理
		void SetBroadphaseProxy( BroadphaseProxy* proxy) { m_broadphaseProxy = proxy; }

		// 设置代理外形
		void SetLocalBounds( const Rect3* rect) { m_localBounds = *rect; }

		// 获取包围盒
		Rect3 GetLocalBounds() { return m_localBounds; }

		// 构建AABB包围盒
		void BuildAABB( Rect3& box);

		// 添加子结点
		void AddChild( CollisionObject* child);

		// 删除子结点
		void DelChile( CollisionObject* child);

	protected:
		UserData*				m_userData;						// 用户数据
		int						m_internalType;					// 类型
		int						m_collisionFlags;				// 碰撞标记
		mutable int				m_activationState1;				// 激活状态
		Transform				m_localTransform;				// 本地转换(以父结点为参照物)
		Transform				m_worldTransform;				// 世界转换(只应用其旋转与平移)
		Shape*					m_collisionShape;				// 碰撞模型
		Rect3					m_localBounds;					// 本地包围盒
		BroadphaseProxy*		m_broadphaseProxy;				// 代理
		CollisionObject*		m_parent;						// 父对象
		vector<CollisionObject*>m_childs;						// 子对象

		Transform				m_interpolationWorldTransform;	// 线性插值
		Vector3					m_interpolationLinearVelocity;	// 线性速度插值
		Vector3					m_interpolationAngularVelocity;	// 度速度插值

		float					m_friction;						// 摩擦力
		float					m_restitution;					// 恢复力???
		float					m_rollingFriction;				// 旋转摩擦力
	};
}