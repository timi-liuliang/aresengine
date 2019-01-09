#include <Engine/FxSystem/AresFxParticle.h>
#include <Engine/FxSystem/AresFxModel.h>
#include <Engine/FxSystem/AresFxParticleSystem.h>
#include <Engine/FxSystem/AresFxModelInterpolater.h>
#include <Core/AresKitFunction.h>

namespace Ares
{
	// 构造函数
	FxParticle::FxParticle( FxParticleSystem* group, size_t index)
		: m_group( group),
		  m_index( index),
		  m_data( m_group->m_particleData + index),
		  m_currentParams( m_group->m_particleCurrentParams + index*group->m_model->GetSizeOfParticleCurrentArray()),
		  m_extendedParams(m_group->m_particleExtendedParams+ index*group->m_model->GetSizeOfParticleExtendedArray())
	{
		Init();
	}

	// 初始化
	void FxParticle::Init()
	{
		const FxModel* model = (FxModel*)m_group->GetModel();

		m_data->m_age  = 0.f;
		m_data->m_life = Random( model->m_lifeTimeMin, model->m_lifeTimeMax);

		// creates pseudo-iterators to parse arrays
		float* particleCurrentIt      = m_currentParams;
		float* particleMutableIt      = m_extendedParams;
		float* particleInterpolatedIt = m_extendedParams + model->m_mutableParams.size();
		const int* paramIt            = model->m_enableParams.data();

		// initializes params
		for ( size_t i=model->m_enableParams.size(); i!=0; --i)
		{
			FxModel::EModelParam param = (FxModel::EModelParam)(*paramIt);
			const float* templateIt = &model->m_params[model->m_indices[param]];

			if( model->IsInterpolated( param))
			{
				*particleCurrentIt++      = FxModel::DEFAULT_VALUES[param];
				*particleInterpolatedIt++ = Random( 0.f, 1.f);

				FxModelInterpolator* interpolator = model->m_interpolators[param];
				float offsetVariation = interpolator->GetOffsetXVariation();
				float scaleVariation  = interpolator->GetScaleXVariation();

				*particleInterpolatedIt++ = Random( -offsetVariation, offsetVariation);
				*particleInterpolatedIt++ = 1.f + Random( -scaleVariation, scaleVariation);
			}
			else if( model->IsRandom( param))
			{
				*particleCurrentIt++ = Random( *templateIt, *(templateIt+1));

				if( model->IsMutable( param))
					*particleMutableIt++ = Random( *(templateIt+2), *(templateIt+3));
			}
			else
			{
				*particleCurrentIt++ = *templateIt;
				if( model->IsMutable( param))
					*particleMutableIt++ = *(templateIt+1);
			}

			++paramIt;
		}
	}

	// 更新
	bool FxParticle::Update( float deltaTime)
	{
		const FxModel* tModel = (FxModel*)m_group->GetModel();
		m_data->m_age += deltaTime;

		if( !tModel->m_immortal)
		{
			// computes the ratio between the life of the particle and its lifetime
			float tRatio = (std::min)( 1.f, deltaTime/m_data->m_life);
			m_data->m_life -= deltaTime;

			// updates mutable parameters
			for ( size_t i=0; i<tModel->m_mutableParams.size(); ++i)
			{
				size_t tIdx       = tModel->m_mutableParams[i];
				size_t tEnableIdx = tModel->m_particleEnableIndices[tIdx];
				m_currentParams[tEnableIdx] += ( m_extendedParams[i] - m_currentParams[tEnableIdx]) * tRatio;
			}
		}

		// Updates interpolated parameters
		InterpolateParameters();

		// 更新位置
		m_data->m_oldPosition = m_data->m_position;
		m_data->m_position += m_data->m_velocity * deltaTime;

		// 更新速度
		m_data->m_velocity += m_group->GetGravity() * deltaTime;

		std::vector<FxModifier*>::const_iterator end = m_group->m_activeModifiers.end();
		for ( std::vector<FxModifier*>::const_iterator it = m_group->m_activeModifiers.begin(); it!=end; ++it)
		{
			(*it)->Process( *this, deltaTime);
		}

		if( m_group->GetFriction() != 0.f)
			m_data->m_velocity *= 1.f - (std::min)( 1.f, m_group->GetFriction()*deltaTime / GetParamCurrentValue( FxModel::PARAM_MASS));
	
		return m_data->m_life <= 0.f;
	}

	// 内插参数
	void FxParticle::InterpolateParameters()
	{
		const FxModel* model = (FxModel*)m_group->GetModel();

		float* interpolatedIt = m_extendedParams + model->m_mutableParams.size();
		for ( size_t i=0; i<model->m_interpolatedParams.size(); ++i)
		{
			size_t index = model->m_interpolatedParams[i];
			size_t enableIndex = model->m_particleEnableIndices[index];
			m_currentParams[enableIndex] = model->m_interpolators[index]->Interpolate( *this, (FxModel::EModelParam)(index), interpolatedIt[0], interpolatedIt[1], interpolatedIt[2]);
			interpolatedIt += 3;
		}
	}

	// 获取对应参数类型的当前值
	float FxParticle::GetParamCurrentValue( FxModel::EModelParam type) const
	{
		const FxModel* model = (FxModel*)m_group->GetModel();
		if( model->IsEnabled( type))
		{
			return m_currentParams[model->m_particleEnableIndices[type]];
		}

		return FxModel::DEFAULT_VALUES[type];
	}

	// 计算距离的平方
	void FxParticle::ComputeSqrDist()
	{
		m_data->m_sqrDist = GetPosition().LengthSq();
	}

	// swaps particle data. Used internally. Do not use with particles that are not from the same group
	extern void SwapParticles( FxParticle& a, FxParticle& b)
	{
		std::swap( *a.m_data, *b.m_data);

		for ( size_t i=0; i<a.GetModel()->GetSizeOfParticleCurrentArray(); ++i)
			std::swap( a.m_currentParams[i], b.m_currentParams[i]);
		
		for( size_t i=0; i<a.GetModel()->GetSizeOfParticleExtendedArray(); ++i)
			std::swap( a.m_extendedParams[i], b.m_extendedParams[i]);

		// swap additional data( groups are assumed to be the same)
		//for ( std::set<IFxBuffer*>::iterator it=a.m_group->m_swappableBuffers.begin(); it!=a.m_group->m_swappableBuffers.end(); ++it)
		//	(*it)->Swap( a.m_index, b.m_index);
	}

	// 获取模型
	FxModel* FxParticle::GetModel() const 
	{ 
		return (FxModel*)m_group->GetModel(); 
	}

	// 获取颜色
	ColorRGB FxParticle::GetColor()
	{
		return ColorRGB( m_currentParams[FxModel::PARAM_RED], m_currentParams[FxModel::PARAM_GREEN], m_currentParams[FxModel::PARAM_BLUE], m_currentParams[FxModel::PARAM_ALPHA]);
	}

	// 设置颜色
	void FxParticle::SetColor( const ColorRGB& color)
	{
		m_currentParams[FxModel::PARAM_RED]   = color.r;
		m_currentParams[FxModel::PARAM_GREEN] = color.g;
		m_currentParams[FxModel::PARAM_BLUE]  = color.b;
		m_currentParams[FxModel::PARAM_ALPHA] = color.a; 
	}
}