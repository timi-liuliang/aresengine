#ifndef MASS_SPRING_SYSTEM_H
#define MASS_SPRING_SYSTEM_H

#include <vector>
#include "PhysicsWorld.h"
#include <boost/array.hpp>

using namespace std;

namespace Ares
{
	//---------------------------------------
	// MassSpringSystem 2013-7-8 帝林
	//---------------------------------------
	class MassSpringSystem
	{
	public:
		static const int MaxSpringPerMassPoint = 12;

		// 质点
		struct MassPoint
		{
			bool		m_archor;		// 是否为锚点
			//float		m_mass;			// 质量
			Vector3		m_position;		// 位置
			Vector3		m_normal;		// 法线
			Vector2		m_uv;			// 质点uv
			Vector3		m_oldPositon;	// 原位置
			int			m_numSpring;	// 关连弹簧数量
			int			m_springs[MaxSpringPerMassPoint];	// 关连弹簧

			// 构造函数
			MassPoint()
			{
				m_archor	= false;
				//m_mass		= 0.01f;
				m_position	= Vector3::Zero;
				m_oldPositon= m_position;
				m_numSpring = 0;
			}
		};

		// 弹簧类型
		enum SpringType
		{
			ST_Structural=0,		// 结构性弹簧
			ST_Shear,				// 扭曲弹簧
			ST_Flexion,				// 弯曲弹簧
			ST_Total,
		};

		// 弹簧:产生质点之间的作用力
		struct Spring
		{
			SpringType	m_type;		// 类型
			int			m_left;		// 左端点
			int			m_right;	// 右端点
			float		m_rest;		// 弹簧原长度

			// 构造函数
			Spring()
			{
				m_type = ST_Structural;
				m_left = INVALID;
				m_right= INVALID;
				m_rest = 1.f;
			}
		};

		// 配置
		struct Settings
		{
			float							m_dc;			// Air damping coefficent
			boost::array<float, ST_Total>	m_ec;			// 结构性弹性系数(MassSpringSystem使用一致的弹性系数)
			Vector3							m_airFlotage;	// 空气浮力
			Rect3							m_localBounds;	// 本地包围盒

			// 构造函数
			Settings()
			{
				m_dc		 = 1.f;
				m_ec[0]		 = 2.f;
				m_ec[1]		 = 0.7f;
				m_ec[2]		 = 0.5f;
				m_airFlotage = Vector3( 0.f, 0.f, 0.f);
			}
		};

	public:
		MassSpringSystem();
		virtual ~MassSpringSystem();

		// 初始化
		void Init( const vector<WORD>& triIndices, const vector<MassPoint>& massPoints, const vector<Spring>& springs);

		// 设置物理世界
		void SetPhysicsWorld( PhysicsWorldPtr& world) { m_physicsWorld=world; }

		// 获取设备
		const Settings& GetSettings() const { return m_settings; }

		// 获取索引
		const vector<WORD>& GetTriIndices() const { return m_triIndices; }

		// 获取质点数量
		size_t GetNumMassPoint() const { return m_massPoints.size(); }

		// 获取质点
		MassPoint& GetMassPoint( size_t index) { return m_massPoints[index]; }

		// 获取弹簧
		const vector<Spring>& GetSprings() { return m_springs; }

		// 更新
		void FrameMove( float elapsedTime);

	private:
		// 更新质点法线
		void UpdateMassPointNormals();

		// 应用外力(重力,风,拖拽,碰撞)
		void ApplyForces( float timeStep);

		// 弹簧约束
		void SatisfyStructuralAndShearSpringConstraints( float timeStep);

		// 碰撞约束
		void SatisfyCollisionConstraints();

		// 响应
		float2 Responsiveness( MassPoint& p0, MassPoint& p1);

		// 距离限制
		void SpringDistanceConstraint( MassPoint& p0, MassPoint& p1, const Spring& spring, float timeStep);

		// 模拟完成
		void PrepareForNextSimulation( float timeStep);

	private:
		Settings				m_settings;				// 设置
		vector<WORD>			m_triIndices;			// 三角形索引(TriangleList)
		vector<MassPoint>		m_massPoints;			// 质点
		vector<Spring>			m_springs;				// 弹簧
		PhysicsWorldPtr			m_physicsWorld;			// 物理世界

		float					m_oldTimeStep;			// 模拟步长
	};
}

#endif