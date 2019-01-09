#pragma once

#include <vector>
#include <Core/AresMath.h>
#include "AresActionInterface.h"
#include "AresVehicleRaycaster.h"
#include "RigidBody.h"

using namespace std;

namespace Ares
{
	//----------------------------------------
	// RaycastVehicle 2013-9-4 帝林
	//----------------------------------------
	class RaycastVehicle : public ActionInterface
	{
	public:
		// 车轮
		struct Wheel
		{
			struct RaycastInfo
			{
				Vector3		m_contactNormalWS;			// 法线
				Vector3		m_contactPostionWS;			// 接触点
				float		m_suspensionLength;			// 悬浮长度
				Vector3		m_hardPositionWS;			// 射线起点
				Vector3		m_wheelDirectionWS;			// 轮子朝向
				Vector3		m_wheelAxisWS;				// 轴向
				bool		m_isInContact;				// 是否接触
				void*		m_groundObject;				// 碰撞对象
			};

			RaycastInfo m_raycastInfo;					// 射线检测信息
			Transform	m_worldTransform;				// 世界转换
			float		m_engineForce;					// 
			Vector3		m_chassisConnectionPointCS;		// 与底盘链接点
			float		m_suspensionRestLenght1;		// 常量
			float		m_suspensionStiffness;			// 常量
			float		m_maxSuspensionTravelCm;		// 
			Vector3		m_wheelDirectionCS;				// 车轮朝向
			Vector3		m_wheelAxisCS;					// 车轮轴向(常量,或被Steering修改)
			float		m_wheelRadius;					// 车轮半径
			float		m_steering;						// 转向
			float		m_rotation;						// 车轮旋转
			float		m_deltaRotation;				// 旋转
			float		m_rollInfluence;
			float		m_maxSuspensionForce;			// 最大悬浮力
			float		m_wheelSuspensionForce;			// 悬浮力
			float		m_clippedInvContactDotSuspension;
			float		m_suspensionRelativeVelocity;
			float		m_wheelsDampingCompression;
			float		m_wheelsDampingRelaxation;
			float		m_wheelsSuspensionForce;		// 车轮悬浮力
			float		m_frictionSlip;					// 摩擦滑动
			float		m_brake;
			float		m_skidInfo;						// 滑行

			// 返回悬浮重置长度
			float GetSuspensionRestLength() const
			{
				return m_suspensionRestLenght1;
			}
		};

	public:
		virtual ~RaycastVehicle() {}

		// 获取刚体
		RigidBody* GetRigidBody() { return m_chassis; }

		// 获取刚体
		const RigidBody* GetRigidBody() const { return m_chassis; }

		// 获取轮子数量
		int GetNumWheels() const { return (int)m_wheels.size();}

		// 获取轮子Transform
		const Transform& GetWheelTransformWS( int idx) const;

	private:
		// 更新
		virtual void UpdateAction( PhysicsWorld* physicsWorld, float deltaTimeStep);

		// 更新车辆
		void UpdateVehicle( float timeStep);

		// 更新指定车轮
		void UpdateWheelTransform( int wheelIdx, bool interpolateTransform=true);

		// 更新车指定车轮
		void UpdateWheeltransformWS( Wheel& wheel, bool isInterpolatedTransform=true);

		// 更新
		void UpdateSuspension( float deltaTime);

		// 更新摩擦力
		void UpdateFriction( float timeStep);

		// 获取车底盘转换
		const Transform& GetChassisWorldTransform();

		// 射线检测
		float RayCast( Wheel& wheel);

	private:
		float				m_currentVehicleSpeedKmHour;	// 速度

		RigidBody*			m_chassis;						// 底盘
		vector<Wheel>		m_wheels;						// 轮子
		VehicleRaycaster*	m_raycaster;					// 射线检测器
		vector<Vector3>		m_forwardWS;					// 
		vector<Vector3>		m_axle;
		vector<float>		m_forwardImpulse;
		vector<float>		m_sideImpulse;
		int					m_indexUpAxis;					// 上方向
	};
}