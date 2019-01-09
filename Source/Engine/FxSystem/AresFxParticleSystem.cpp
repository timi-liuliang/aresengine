#include <Engine/FxSystem/AresFxParticleSystem.h>
#include <Engine/FxSystem/AresFxEntity.h>
#include <Engine/AresRoot.h>

BOOST_CLASS_EXPORT( Ares::FxParticleSystem)

namespace Ares
{
	// 构造函数
	FxParticleSystem::FxParticleSystem()
		: FxObject( FOT_ParticleSystem)
		, m_model( NULL)
		, m_friction( 0.f)
		, m_gravity( 0.f, 0.f, 0.f)
		, m_sortingEnabled( false)
		, m_disComputeEnabled( false)
		, m_creationBuffer()
		, m_numBufferedParticles( 0)
		, m_fUpdate( NULL)
		, m_fBirth( NULL)
		, m_fDeath( NULL)
		, m_boundingBoxEnabled( false)
		, m_emitters()
		, m_modifiers()
		, m_activeModifiers()
		, m_life( 0.f)
	{
		m_renderable = MakeSharePtr( new FxRenderable());
	}

	// 构造函数
	FxParticleSystem::FxParticleSystem( FxModel* model, size_t capacity)
		: FxObject( FOT_ParticleSystem)
		, m_model( model ? (FxModel*)model : NULL/*&GetDefaultModel()*/)
		, m_friction( 0.f)
		, m_gravity( 0.f, 0.f, 0.f)
		, m_sortingEnabled( false)
		, m_disComputeEnabled( false)
		, m_creationBuffer()
		, m_numBufferedParticles( 0)
		, m_fUpdate( NULL)
		, m_fBirth( NULL)
		, m_fDeath( NULL)
		, m_boundingBoxEnabled( false)
		, m_emitters()
		, m_modifiers()
		, m_activeModifiers()
		, m_life( 0.f)
	{
		m_renderable = MakeSharePtr( new FxRenderable());

		SetCapacity( capacity);
	}

	// 设置容量
	void FxParticleSystem::SetCapacity( size_t capacity)
	{
		m_capacity	   = capacity;
		m_particlePool = CPool<FxParticle>(capacity);
		m_particleData = new_ FxParticle::SParticleData[capacity];
		m_particleCurrentParams = new_ float[capacity * m_model->GetSizeOfParticleCurrentArray()];
		m_particleExtendedParams= new_ float[capacity * m_model->GetSizeOfParticleExtendedArray()];
	}

	// updates this group by a step time
	void FxParticleSystem::FrameMove( float elapsedTime,  FxEntity* fxEntity)
	{
		m_life += elapsedTime;

		size_t numManualBorn = m_numBufferedParticles;
		size_t numAutoBorn   = 0;

		bool   hasActiveEmitters = false;

		// 更新发射器
		m_activeEmitters.clear();
		for ( vector<FxEmitter*>::const_iterator curIt = m_emitters.begin(); curIt != m_emitters.end(); ++curIt)
		{
			if( (*curIt)->IsActive())
			{
				int num = (*curIt)->UpdateNumber( elapsedTime);
				if( num > 0)
				{
					SEmitterData tData = { *curIt, num};
					m_activeEmitters.push_back( tData);
					numAutoBorn += num;
				}
			}

			hasActiveEmitters |= !(*curIt)->IsSleeping();
		}

		vector<SEmitterData>::iterator emitterIt = m_activeEmitters.begin();
		size_t numBorn = numAutoBorn + numManualBorn;

		// Prepare modifiers for processing
		m_activeModifiers.clear();
		for( size_t i=0; i<m_modifiers.size(); ++i)
		{
			FxModifier* tModifier = m_modifiers[i];
			tModifier->BeginProcess( *this);
			if( tModifier->IsActive())
			{
				m_activeModifiers.push_back( tModifier);
			}
		}

		// 更新粒子
		for ( size_t i=0; i<m_particlePool.GetNumActive(); ++i)
		{
			if( m_particlePool[i].Update( elapsedTime) || (m_fUpdate && ((*m_fUpdate)(m_particlePool[i], elapsedTime))))
			{
				if( m_fDeath)
					(*m_fDeath)( m_particlePool[i]);

				if( numBorn > 0)
				{
					m_particlePool[i].Init();
					LaunchParticle( m_particlePool[i], emitterIt, numManualBorn);
					--numBorn;
				}
				else
				{
					m_particleData[i].m_sqrDist = 0.f;
					m_particlePool.MakeInactive( i);
					--i;
				}
			}
			else
			{
				//if( m_boundingBoxEnabled)
				//	UpdateAABB( m_particlePool[i]);

				if( m_disComputeEnabled)
					m_particlePool[i].ComputeSqrDist();
			}
		}

		// Terminates modifiers processing
		for (vector<FxModifier*>::iterator it=m_modifiers.begin(); it!=m_modifiers.end(); ++it)
			(*it)->EndProcess( *this);

		// Emits new particles if some left
		for ( int i=numBorn; i>0; i--)
			PushParticle( emitterIt, numManualBorn);

		// Sorts particles if enabled
		if( m_sortingEnabled && m_particlePool.GetNumActive() > 1)
			SortParticles( 0, m_particlePool.GetNumActive() -1);

		if( !m_boundingBoxEnabled || m_particlePool.GetNumActive()==0)
		{

		}

		// 关键帧插值
		KeyFrameInterpolate();

		// 更新渲染
		UpdateBuffer();

		// 渲染更新
		m_renderable->FrameMove( elapsedTime, fxEntity, this);
	}

	// 更新Buffer
	void FxParticleSystem::UpdateBuffer()
	{
		Vector3 bP0 = Vector3( -0.5f, -0.5f, 0.f) * 0.15f;
		Vector3 bP1 = Vector3( -0.5f,  0.5f, 0.f) * 0.15f;
		Vector3 bP2 = Vector3(  0.5f, -0.5f, 0.f) * 0.15f;
		Vector3 bP3 = Vector3(  0.5f,  0.5f, 0.f) * 0.15f;

		Vector3  pos;		// 粒子位置	
		float    size;		// 位置和尺寸决定了绘理坐标
		ColorRGB color;		// 粒子颜色
		vector<FxRenderable::VertexFormat> vertices;
		vector<WORD>					   indices;
		for ( size_t i=0; i<GetNumParticles(); ++i)
		{
			FxParticle& particle = GetParticle(i);
			pos   = particle.GetPosition();
			size  = particle.GetParamCurrentValue( FxModel::PARAM_SIZE);
			color = particle.GetColor();

			WORD offset = static_cast<WORD>(vertices.size());
			indices.push_back( offset+0); indices.push_back( offset+1); indices.push_back( offset+2);
			indices.push_back( offset+1); indices.push_back( offset+3); indices.push_back( offset+2);

			// 创建辅助图象处理的模型(顶点缓冲)
			FxRenderable::VertexFormat patch3d[4];
			patch3d[0].m_position = bP0 + pos; patch3d[0].m_color = color; patch3d[0].m_texcoord = float2( 0.f, 1.f);
			patch3d[1].m_position = bP1 + pos; patch3d[1].m_color = color; patch3d[1].m_texcoord = float2( 0.f, 0.f);
			patch3d[2].m_position = bP2 + pos; patch3d[2].m_color = color; patch3d[2].m_texcoord = float2( 1.f, 1.f);
			patch3d[3].m_position = bP3 + pos; patch3d[3].m_color = color; patch3d[3].m_texcoord = float2( 1.f, 0.f);

			vertices.push_back( patch3d[0]);
			vertices.push_back( patch3d[1]);
			vertices.push_back( patch3d[2]);
			vertices.push_back( patch3d[3]);
		}

		if( !m_renderable)
			m_renderable = MakeSharePtr( new FxRenderable);

		m_renderable->UpdateBuffer( RenderLayout::TT_TriangleList, vertices.data(), sizeof(FxVertexFormat)*vertices.size(), indices.data(), sizeof(WORD) * indices.size());
	}

	// Renders this Group
	void FxParticleSystem::SubmitRenderElements( RenderElementMgr& renderElementMgr)
	{
		if( m_renderable)
			m_renderable->SubmitRenderElements( renderElementMgr);
	}

	// Adds an emitter in this group, if emitter is already in the group, it will no be inserted again.
	void FxParticleSystem::AddEmitter( FxEmitter* emitter)
	{
		if( !emitter)
			return;

		// checks if the emitter is already in the group
		vector<FxEmitter*>::const_iterator it = std::find( m_emitters.begin(), m_emitters.end(), emitter);
		if( it != m_emitters.end())
			return;

		m_emitters.push_back( emitter);
	}

	// Removes an emitter from this group
	void FxParticleSystem::RemoveEmitter( FxEmitter* emitter)
	{

	}

	// Adds an modifier in this group
	void FxParticleSystem::AddModifier( FxModifier* modifier)
	{
		if( !modifier)
			return;

		m_modifiers.push_back( modifier);
	}

	// Removes an modifier from this group
	void FxParticleSystem::RemoveModifier( FxModifier* modifier)
	{

	}

	// 发射粒子
	void FxParticleSystem::LaunchParticle( FxParticle& particle, EmitterDataList::iterator& emitterIt, size_t& numManualBorn)
	{
		if( numManualBorn ==0)
		{
			emitterIt->m_emitter->Emit( particle);
			if( --emitterIt->m_numParticles == 0)
				++emitterIt;
		}
		else
		{
			SCreationData creationData = m_creationBuffer.front();

			if( creationData.m_zone)
				creationData.m_zone->GeneratePosition( particle, creationData.m_full);
			else
				particle.SetPosition( creationData.m_position);

			if( creationData.m_emitter)
				creationData.m_emitter->GenerateVelocity( particle);
			else
				particle.SetVelocity( creationData.m_velocity);

			PopNextManualAdding( numManualBorn);
		}

		// 重置原有位置
		particle.SetOldPosition( particle.GetPosition());

		// first parameter interpolation must be
		// here so that the velocity has already been initialized
		particle.InterpolateParameters();

		if( m_fBirth)
			(*m_fBirth)( particle);

		if(m_boundingBoxEnabled)
			UpdateAABB( particle);

		if( m_disComputeEnabled)
			particle.ComputeSqrDist();
	}

	// 添加粒子
	void FxParticleSystem::PushParticle( EmitterDataList::iterator& emitterIt, size_t& numManualBorn)
	{
		FxParticle* ptr = m_particlePool.MakeActive();
		if( !ptr)
		{
			if( m_particlePool.GetNumEmpty() > 0)
			{
				FxParticle particle( this, m_particlePool.GetNumActive());
				LaunchParticle( particle, emitterIt, numManualBorn);
				m_particlePool.PushActive( particle);
			}
			else if( numManualBorn > 0)
			{
				PopNextManualAdding( numManualBorn);
			}
		}
		else
		{
			ptr->Init();
			LaunchParticle( *ptr, emitterIt, numManualBorn);
		}
	}

	// 粒子排序
	void FxParticleSystem::SortParticles()
	{
		ComputeDistance();

		if( m_sortingEnabled)
			SortParticles( 0, m_particlePool.GetNumActive()-1);
	}

	// 排序粒子
	void FxParticleSystem::SortParticles( int start, int end)
	{
		if( start < end)
		{
			int i = start - 1;
			int j = end   + 1;
			float pivot = m_particleData[(start+end)>>1].m_sqrDist;
			while( true)
			{
				do 
				{
					++i;
				} while( m_particleData[i].m_sqrDist > pivot);

				do
				{
					--j;
				} while( m_particleData[j].m_sqrDist < pivot);

				if( i<j)
					SwapParticles( m_particlePool[i], m_particlePool[j]);
				else
					break;
			}

			SortParticles( start, j);
			SortParticles( j+1, end);
		}
	}

	// 计算距离
	void FxParticleSystem::ComputeDistance()
	{
		if( m_disComputeEnabled)
		{
			CPool<FxParticle>::const_iterator endIt = m_particlePool.end();
			for ( CPool<FxParticle>::iterator it=m_particlePool.begin(); it!=endIt; ++it)
			{
				it->ComputeSqrDist();
			}
		}
	}

	// 手工添加
	void FxParticleSystem::PopNextManualAdding( size_t& numManualBorn)
	{
		--m_creationBuffer.front().m_num;
		--numManualBorn;
		--m_numBufferedParticles;
		if( m_creationBuffer.front().m_num <=0)
			m_creationBuffer.pop_back();
	}

	// 更新AABB包围盒
	void FxParticleSystem::UpdateAABB( const FxParticle& particle)
	{
		const Vector3& position = particle.GetPosition();

		m_boundingBox.UnionPoint( position);
	}

	// 关键帧插值
	void FxParticleSystem::KeyFrameInterpolate()
	{
		// 1.颜色
		ColorRGB color;// = m_model->GetColor();
		if( m_interpolater.InterplateColor( color, m_life))
		{
			//m_model->SetColor( color);
		}
	}
}