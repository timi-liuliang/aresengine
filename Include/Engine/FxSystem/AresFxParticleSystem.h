#pragma once

#include <vector>
#include <deque>
#include <map>
#include "AresFxObject.h"
#include "AresFxEmitter.h"
#include "AresFxModifier.h"
#include "Pool.h"
#include "AresFxModel.h"
#include "AresFxParticle.h"
#include "AresFxRenderable.h"
#include "AresFxInterpolater.h"
#include <Physics/Shapes.h>

using namespace std;

namespace Ares
{
	//------------------------------------------------------
	// 效果组 2011-9-14   帝林
	//------------------------------------------------------
	class FxParticleSystem : public FxObject
	{
		friend class FxParticle;
		friend void SwapParticles( FxParticle& a, FxParticle& b);
	private:
		// 组类型
		enum EGroupType
		{
			PARTICLE,		// 粒子组
			STRIPE,			// 条带
			MODEL,			// 模型
		};

		// 发射器数据
		struct SEmitterData
		{
			FxEmitter*		m_emitter;
			size_t			m_numParticles;
		};

		// 创建数据
		struct SCreationData
		{
			size_t			m_num;
			Vector3			m_position;
			Vector3			m_velocity;
			const FxZone*  m_zone;
			FxEmitter*		m_emitter;
			bool			m_full;
		};

		typedef vector<SEmitterData> EmitterDataList;
		typedef vector<FxModifier*>  ModifiersList;

	public:
		FxParticleSystem();
		FxParticleSystem( FxModel* model, size_t capacity=CPool<FxParticle>::DEFAULT_CAPACITY);

		// 获取粒子数
		virtual size_t GetNumParticles() { return m_particlePool.GetNumActive(); }

		// 获取渲染对象
		FxRenderablePtr GetRenderable() { return m_renderable; }

	public:
		// 设置容量
		void SetCapacity( size_t capacity);

		// 强制设定包围盒
		virtual void SetBoundingBox( const Rect3& boundingBox) { m_boundingBox = boundingBox; }

		// 获取包围盒
		virtual const Rect3& GetBoundingBox() { return m_boundingBox; }

		// updates this group by a step time
		virtual void FrameMove( float elapsedTime,  FxEntity* fxEntity);

		// 渲染
		virtual void SubmitRenderElements( RenderElementMgr& renderElementMgr);

	public:
		// 获取粒子模型 
		virtual FxModel* GetModel() { return m_model; }

		// Gets the friction coefficient of this group
		virtual float GetFriction() const { return m_friction; }

		// 获取粒子
		FxParticle& GetParticle( size_t index) { return m_particlePool[index]; }

	public:
		// 获取重力
		const Vector3& GetGravity() const { return m_gravity; }

		// 设置重力
		void SetGravity( const Vector3& gravity) { m_gravity = gravity; }

		// Adds an emitter in this group, if emitter is already in the group, it will no be inserted again.
		virtual void AddEmitter( FxEmitter* emitter);

		// Removes an emitter from this group
		virtual void RemoveEmitter( FxEmitter* emitter);

		// Adds an modifier in this group
		virtual void AddModifier( FxModifier* modifier);

		// Removes an modifier from this group
		virtual void RemoveModifier( FxModifier* modifier);

		// Serialize
		BOOST_SERIALIZATION_SPLIT_MEMBER()
		template<typename Archive> void save( Archive & ar, const unsigned int version) const
		{
			ar & boost::serialization::base_object<FxObject>(*this);
			ar & m_model;
			ar & m_emitters;
			//ar & m_modifiers;
			ar & m_friction;
			ar & m_gravity;
			ar & m_capacity;
			ar & m_numBufferedParticles;
			//ar & m_render;
		}
		template<typename Archive> void load( Archive& ar, const unsigned int version)
		{
			ar & boost::serialization::base_object<FxObject>(*this);
			ar & m_model;
			ar & m_emitters;
			//ar & m_modifiers;
			ar & m_friction;
			ar & m_gravity;
			ar & m_capacity;
			ar & m_numBufferedParticles;
			//ar & m_render;

			SetCapacity( m_capacity);
		}

	private:
		// 粒子排序
		void SortParticles();

		// 排序粒子
		void SortParticles( int start, int end);

		// 计算距离
		void ComputeDistance();

		// 添加粒子
		void PushParticle( EmitterDataList::iterator& emitterIt, size_t& numManualBorn);

		// 发射粒子
		void LaunchParticle( FxParticle& particle, EmitterDataList::iterator& emitterIt, size_t& numManualBorn);

		// 手工删除
		void PopNextManualAdding( size_t& numManualBorn);

		// 更新AABB包围盒
		void UpdateAABB( const FxParticle& particle);

		// 关键帧插值
		void KeyFrameInterpolate();

		// 更新Buffer
		void UpdateBuffer();

	private:
		float				m_life;					// 生命
		EGroupType			m_type;					// 组类型
		FxModel*			m_model;				// model
		vector<FxEmitter*>  m_emitters;				// 粒子发射器
		vector<FxModifier*> m_modifiers;			// 修改器

		EmitterDataList		m_activeEmitters;		// 活跃的发射器
		ModifiersList		m_activeModifiers;		// 修改器

		FxInterpolater		m_interpolater;			// 插值器

		float				m_friction;				// 摩擦力
		Vector3				m_gravity;				// 重力

		size_t						m_capacity;		// 容量
		CPool<FxParticle>			m_particlePool;	// 粒子池
		FxParticle::SParticleData*	m_particleData;	// 粒子数据	

		float*			m_particleCurrentParams;	// Sotres the current parameters values of the particles
		float*			m_particleExtendedParams;	// Stores the extended parameters values of the particles

		deque<SCreationData>m_creationBuffer;		// createion Buffer
		size_t				m_numBufferedParticles;	// 粒子数

		bool (*m_fUpdate)(FxParticle&, float);		// 粒子更新函数
		void (*m_fBirth) (FxParticle&);				// 粒子出生函数
		void (*m_fDeath) (FxParticle&);				// 粒子死亡函数

		bool				m_boundingBoxEnabled;	// 是否使用包围盒
		bool				m_sortingEnabled;		// 是否排序
		bool				m_disComputeEnabled;	// 是否进行距离计算
		Rect3				m_boundingBox;			// 包围盒

		FxRenderablePtr		m_renderable;				// 渲染器
	};
}