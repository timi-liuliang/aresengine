#include <Physics/MassSpringSystem.h>
#include <algorithm>

namespace Ares
{
	// 构造函数
	MassSpringSystem::MassSpringSystem()
		: m_oldTimeStep( -1.f)
	{

	}

	// 析构函数
	MassSpringSystem::~MassSpringSystem()
	{

	}

	// 初始化
	void MassSpringSystem::Init( const vector<WORD>& triIndices, const vector<MassPoint>& massPoints, const vector<Spring>& springs)
	{
		m_triIndices = triIndices;
		m_massPoints = massPoints;
		m_springs	 = springs;
	}

	// 更新质点法线(参考TBNComputeNormal)
	void MassSpringSystem::UpdateMassPointNormals()
	{
		// 归零
		for ( size_t i=0; i<m_massPoints.size(); i++)
			m_massPoints[i].m_normal = Vector3::Zero;

		size_t numTris = m_triIndices.size() / 3;
		for ( size_t i=0; i<numTris; i++)
		{
			int  idxB = i*3;
			WORD idx0 = m_triIndices[idxB+0];
			WORD idx1 = m_triIndices[idxB+1];
			WORD idx2 = m_triIndices[idxB+2];

			A_ASSERT( idx0<m_massPoints.size());
			A_ASSERT( idx1<m_massPoints.size());
			A_ASSERT( idx2<m_massPoints.size());

			Vector3 edge0 = m_massPoints[idx1].m_position - m_massPoints[idx0].m_position;
			Vector3 edge1 = m_massPoints[idx2].m_position - m_massPoints[idx0].m_position;

			// calc triangle normal
			Vector3 faceNormal = Vector3Cross( edge0, edge1);
			faceNormal.Normalize();

			m_massPoints[idx0].m_normal += faceNormal;
			m_massPoints[idx1].m_normal += faceNormal;
			m_massPoints[idx2].m_normal += faceNormal;
		}

		// 单位化
		for ( size_t i=0; i<m_massPoints.size(); i++)
			m_massPoints[i].m_normal.Normalize();
	}

	// 更新
	void MassSpringSystem::FrameMove( float elapsedTime)
	{
		elapsedTime = std::min<float>( elapsedTime, 0.1f);

		A_ASSERT( m_physicsWorld);

		// 更新法线
		UpdateMassPointNormals();

		// Step 1: Apply Force
		ApplyForces( elapsedTime);

		// Step 2:
		SatisfyStructuralAndShearSpringConstraints( elapsedTime);

		// Step 3:
		PrepareForNextSimulation( elapsedTime);

		// Step 4:
		SatisfyCollisionConstraints();
	}

	// 应用外力(重力,风,拖拽,碰撞)
	void MassSpringSystem::ApplyForces( float timeStep)
	{
		if( timeStep==0.f)
			return;

		m_oldTimeStep = m_oldTimeStep> 0.f ? m_oldTimeStep : timeStep;

		for ( size_t i=0; i<m_massPoints.size(); i++)
		{
			MassPoint& mp = m_massPoints[i];
			if( !mp.m_archor)
			{
				// Forces
				float3 force = Vector3::Zero;

				// Gravity
				force += m_physicsWorld ? m_physicsWorld->GetGravity() : Vector3::Zero;

				// Air flotage
				force += m_settings.m_airFlotage;

				// Wind
				float updir = rand() % 100 * 0.01f * 5.f + 1.f; 
				Vector3 wnd( 0.f, 1.80f, updir);
				force += Vector3Dot( wnd, mp.m_normal) * mp.m_normal;

				// Damping
				float speedCoeff = 0.99f;
				
				m_massPoints[i].m_position += (timeStep/m_oldTimeStep) * speedCoeff * ( mp.m_position - mp.m_oldPositon) +  force * timeStep * m_oldTimeStep;
			}
		}
	}

	// 响应
	float2 MassSpringSystem::Responsiveness( MassPoint& p0, MassPoint& p1)
	{
		if( p0.m_archor)
		{
			if( p1.m_archor) return float2( 0.f, 0.f);
			else			 return float2( 0.f, 1.f);
		}
		else
		{
			if( p1.m_archor) return float2(  1.f, 0.f);
			else			 return float2( 0.5f, 0.5f);
		}
	}

	// 距离限制
	void MassSpringSystem::SpringDistanceConstraint( MassPoint& p0, MassPoint& p1, const Spring& spring, float timeStep)
	{
		float3 delta		 = p1.m_position - p0.m_position;
		float  distance		 = std::max<float>( delta.Length(), 1e-6f);
		float  stretching	 = 1 - spring.m_rest / distance;
		float2 responsiveness = Responsiveness( p0, p1);

		delta		  *= stretching;
		delta		  *= m_settings.m_ec[spring.m_type];
		p0.m_position += responsiveness[0] * delta;
		p1.m_position -= responsiveness[1] * delta;
	}

	// 弹簧约束
	void MassSpringSystem::SatisfyStructuralAndShearSpringConstraints( float timeStep)
	{
		for ( size_t i=0; i<m_springs.size(); i++)
		{
			const Spring&  spring = m_springs[i];
			MassPoint&     p0     = m_massPoints[spring.m_left];
			MassPoint&     p1     = m_massPoints[spring.m_right];

			SpringDistanceConstraint( p0, p1, spring, timeStep);
		}
	}

	// 碰撞约束
	void MassSpringSystem::SatisfyCollisionConstraints()
	{

	}

	// 模拟完成
	void MassSpringSystem::PrepareForNextSimulation( float timeStep)
	{
		m_settings.m_localBounds.Reset();

		foreach_( MassPoint& mp, m_massPoints)
		{
			// remember old position
			mp.m_oldPositon = mp.m_position;

			m_settings.m_localBounds.UnionPoint( mp.m_position);
		}

		m_oldTimeStep = timeStep;
	}
}