#include <Engine/FxSystem/AresFxModelInterpolater.h>
#include <Engine/FxSystem/AresFxModel.h>
#include <Engine/FxSystem/AresFxParticle.h>

namespace Ares
{
	FxModelInterpolator::ComputeXFun FxModelInterpolator::m_computeXFun[4] =
	{
		&FxModelInterpolator::ComputeXLifeTime,
		&FxModelInterpolator::ComputeXAge,
		&FxModelInterpolator::ComputeXParam,
		&FxModelInterpolator::ComputeXVelocity,
	};

	FxModelInterpolator::FxModelInterpolator()
	{
	}

	// 插值
	float FxModelInterpolator::Interpolate( const FxParticle& particle, int interpolateParam, float ratioY, float offsetX, float scaleX)
	{
		SInterpolatorEntry currentKey(( this->*FxModelInterpolator::m_computeXFun[m_type])(particle));
		currentKey.m_x += offsetX;
		currentKey.m_x *= scaleX;

		if( m_loopingEnabled)
		{
			// If the graph has less tha two entries, we cannot loop
			if( m_graph.size() < 2)
			{
				if( m_graph.empty())
					return FxModel::GetDefaultValue( FxModel::EModelParam(interpolateParam));
				else
					return InterpolateY( *( m_graph.begin()), ratioY);
			}

			// Else finds the current X in the range
			const float beginX = m_graph.begin()->m_x;
			const float	rangeX = m_graph.rbegin()->m_x;
			float newX = ( currentKey.m_x - beginX) / rangeX;
			newX -= (int)newX;

			if( newX < 0.f)
				newX = 1.f + newX;

			currentKey.m_x = beginX + newX * rangeX;
		}

		// Get the entry that is immediatly after the current X
		set<SInterpolatorEntry>::const_iterator nextIt = m_graph.upper_bound( currentKey);

		// If the current X is higher than the one of the last entry
		if( nextIt == m_graph.end())
		{
			if( m_graph.empty())
				return FxModel::GetDefaultValue( FxModel::EModelParam(interpolateParam));
			else
				return InterpolateY( *(--nextIt), ratioY);
		}
		else if( nextIt == m_graph.begin())
		{
			return InterpolateY( *nextIt, ratioY);
		}
		else
		{
			// Interpolated between the entries before and after the current X
			const SInterpolatorEntry& nextEntry     = *nextIt;
			const SInterpolatorEntry& previousEntry = *(--nextIt);
			float y0 = InterpolateY( previousEntry, ratioY);
			float y1 = InterpolateY( nextEntry, ratioY);

			float ratioX = ( currentKey.m_x - previousEntry.m_x) / ( nextEntry.m_x - previousEntry.m_x);

			return y0 + ratioX * ( y1 - y0);
		}
	}

	// 计算生命时间
	float FxModelInterpolator::ComputeXLifeTime( const FxParticle& particle) const
	{
		return particle.GetAge() / (particle.GetAge() + particle.GetLifeLeft());
	}

	// 计算Age
	float FxModelInterpolator::ComputeXAge( const FxParticle& particle) const
	{
		return particle.GetAge();
	}

	// 计算X参数
	float FxModelInterpolator::ComputeXParam( const FxParticle& particle) const
	{
		return particle.GetParamCurrentValue( FxModel::EModelParam( m_param));
	}

	float FxModelInterpolator::ComputeXVelocity( const FxParticle& particle) const
	{
		const Vector3& v = particle.GetVelocity();

		return v.x*v.x + v.y*v.y + v.z*v.z;
	}

	// 插值Y 
	float FxModelInterpolator::InterpolateY( const SInterpolatorEntry& entry, float ratio)
	{
		return entry.m_y0 + (entry.m_y1 - entry.m_y0) * ratio;
	}
}