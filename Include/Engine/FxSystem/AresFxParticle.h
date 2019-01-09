#pragma once

#include <Core/AresMath.h>
#include <Engine/FxSystem/AresFxModel.h>

namespace Ares
{
	//----------------------------------------------
	// Particle 2011-9-14  帝林
	//     A Particle is the primitive on which all 
	// the SPARK engine relies. Particles are design-
	// ed to be handled in large amounts( called group)
	// This is why the user cannot update or render
	// directly a single particle
	//	   Note that the particle class is only a class
	// that presents an interface to the user, particles
	// data are stored in the groups. This is why copying
	// a particle will not copy its data.
	//----------------------------------------------
    class FxParticleSystem;
	class FxParticle
	{
		friend class FxParticleSystem;
		friend void SwapParticles( FxParticle& a, FxParticle& b);
	public:
		// 粒子数据
		struct SParticleData
		{
			Vector3 m_oldPosition;
			Vector3 m_position;
			Vector3 m_velocity;
			float	m_age;
			float	m_life;
			float	m_sqrDist;

			// constructor
			SParticleData()
			{	
				m_oldPosition = Vector3::Zero;
				m_position    = Vector3::Zero;
				m_velocity    = Vector3::Zero;
			}
		};

	public:
		// 初始化
		virtual void Init();

		// 更新
		virtual bool Update( float deltaTime);

	public:
		// 获取对应参数类型的当前值
		virtual float GetParamCurrentValue( FxModel::EModelParam type) const;

	public:
		// 获取位置
		virtual const Vector3& GetPosition()const { return m_data->m_position; }

		// 设置位置
		virtual void SetPosition( const Vector3& position) { m_data->m_position = position; } 

		// 获取原位置
		virtual const Vector3& GetOldPosition()const { return m_data->m_oldPosition; }

		// 设置原位置
		virtual void SetOldPosition( const Vector3& position) { m_data->m_oldPosition = position; }

		// 获取速度
		virtual const Vector3& GetVelocity()const { return m_data->m_velocity; }

		// 设置速度
		virtual void SetVelocity( const Vector3& velocity) { m_data->m_velocity = velocity; }

	public:
		// 内插参数
		void InterpolateParameters();

		// 计算距离的平方
		void ComputeSqrDist();

		// 获取粒子的存活时间 
		float GetAge() const { return m_data->m_age; }

		// 获取省余存活时间 
		float GetLifeLeft() const { return m_data->m_life; }

		// 获取颜色
		ColorRGB GetColor(); 

		// 设置颜色
		void SetColor( const ColorRGB& color);

	private:
		// 构造函数
		FxParticle( FxParticleSystem* group, size_t index);

		// 获取模型
		FxModel* GetModel() const;

	private:
		FxParticleSystem*	m_group;			// 组
		size_t				m_index;			// 索引

		SParticleData*		m_data;				// 数据
		float*				m_currentParams;	// 当前参数
		float*				m_extendedParams;	// 扩展参数
	};

	// swaps particle data. Used internally. Do not use with particles that are not from the same group
	extern void SwapParticles( FxParticle& a, FxParticle& b);
}