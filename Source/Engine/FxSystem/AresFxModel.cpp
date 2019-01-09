#include <Engine/FxSystem/AresFxModel.h>
#include <Engine/FxSystem/AresFxModelInterpolater.h>

namespace Ares
{
	const float FxModel::DEFAULT_VALUES[NB_PARAMS] =
	{
		1.f,	// RED
		1.f,	// GREEN
		1.f,	// BLUE
		1.f,	// ALPHA
		1.f,	// SIZE
		1.f,	// MASS
		0.f,	// ANGLE
		0.f,	// TEXTURE_INDEX
		0.f,	// ROTATION_SPEED
		0.f,	// CUSTOM_0
		0.f,	// CUSTOM_1
		0.f,	// CUSTOM_2
	};

	// 构造函数
	FxModel::FxModel()
	{

	}

	// 构造函数
	FxModel::FxModel( int enableFlag /*= FLAG_RED | FLAG_GREEN | FLAG_BLUE*/, int mutableFlag /*= FLAG_NONE*/, int randomFlag /*= FLAG_NONE*/, int interpolateFlag /*= FLAG_NONE*/)
		: m_lifeTimeMin( 1.f),
		  m_lifeTimeMax( 1.f),
		  m_immortal( false),
		  m_numRandomParams( 0)
	{
		enableFlag |= FLAG_RED | FLAG_GREEN | FLAG_BLUE;	
		m_enableFlag = enableFlag & (( 1<<(NB_PARAMS+1)) - 1);
		m_interpolatedFlag = interpolateFlag & m_enableFlag;
		m_mutableFlag      = mutableFlag & m_enableFlag;
		m_mutableFlag     &= ~m_interpolatedFlag;
		m_randomFlag	   = randomFlag & m_enableFlag;
		m_randomFlag	  &= ~m_interpolatedFlag;

		size_t	numParamSize			  = 0;
		int	numEnableParams			  = 0;
		int	numMutableParams		  = 0;
		int numIterpolatedParams	  = 0;
		int particleEnableParamsSize  = 0;
		int particleMutableParamsSize = 0;
		for ( size_t i=0; i<NB_PARAMS; ++i)
		{
			EModelParam param = static_cast<EModelParam>(i);

			int paramSize = 0;
			if( IsEnabled( param))
			{
				++numEnableParams;
				if( !IsInterpolated( param))
				{
					m_interpolators[i] = NULL;
					paramSize = 1;

					// 生命周期内变化的参数
					if( IsMutable( param))
					{
						paramSize=2;
						++numMutableParams;
					}

					// 随机类型参数
					if( IsRandom( param))
					{
						paramSize <<= 1;
						++m_numRandomParams;
					}
				}
				else
				{
					// Creates the interpolator
					m_interpolators[i] = new_ FxModelInterpolator;
					++numIterpolatedParams;
				}
			}
			else
			{
				m_interpolators[i] = NULL;
			}

			m_particleEnableIndices[i] = particleEnableParamsSize;
			m_particleMutableIndices[i]= particleMutableParamsSize;
			particleEnableParamsSize += IsEnabled( param) >> i;
			particleMutableParamsSize+= IsMutable( param) >> i;

			m_indices[i]  = numParamSize;
			numParamSize += paramSize;
		}

		// Creates the array of params for this model
		if( numParamSize > 0)
		{
			m_params.resize( numParamSize);
			size_t currentParamIndex = 0;
			size_t currentIndex		 = 0;
			while( currentIndex < numParamSize)
			{
				size_t numValues = GetNumValues( (EModelParam)currentParamIndex);
				for ( size_t i=0; i<numValues; ++i)
					m_params[currentIndex+i] = DEFAULT_VALUES[currentParamIndex];

				++currentParamIndex;
				currentIndex += numValues;
			}
		}
		else
			m_params.clear();

		if( numEnableParams > 0)
		{
			m_enableParams.resize( numEnableParams);
			size_t index = 0;
			for ( size_t i=0; i<NB_PARAMS; ++i)
				if( IsEnabled( (EModelParam)i))
					m_enableParams[index++] = i;
		}
		else
			m_enableParams .clear();

		if( numMutableParams>0)
		{
			m_mutableParams.resize( numMutableParams);
			size_t index = 0;
			for( size_t i=0; i<NB_PARAMS; ++i)
				if( IsMutable( (EModelParam)i))
					m_mutableParams[index++] = i;
		}
		else
			m_mutableParams.clear();

		if( numIterpolatedParams > 0)
		{
			m_interpolatedParams.resize( numIterpolatedParams);
			size_t index = 0;
			for( size_t i=0; i<NB_PARAMS; ++i)
				if( IsInterpolated( (EModelParam)i))
					m_interpolatedParams[index++] = i;
		}
		else
			m_interpolatedParams.clear();
	}

	// Gets the number of float values in the particle extended array
	size_t FxModel::GetSizeOfParticleExtendedArray() const
	{
		// m_numMutable + m_numInterpolated * 3 
		return m_mutableParams.size() + ( m_interpolatedParams.size() << 1) + m_interpolatedParams.size();
	}

	// Sets the life time
	void FxModel::SetLifeTime( float lifeTimeMin, float lifeTimeMax)
	{
		m_lifeTimeMin = lifeTimeMin;
		m_lifeTimeMax = lifeTimeMax;
	}

	// Gets the minimum life time
	float FxModel::GetLifeTimeMin()
	{
		return m_lifeTimeMin;
	}

	// Gets the maximum life time
	float FxModel::GetLifeTimeMax()
	{
		return m_lifeTimeMax;
	}

	// Defines wheather to generate immortal particles or not
	void FxModel::SetImmortal( bool immortal)
	{
		m_immortal = immortal;
	}

	// Returns whether immortility is set or not
	bool FxModel::IsImmortal()
	{
		return m_immortal;
	}

	// Checks whether a parameter is enabled or not
	int FxModel::IsEnabled( EModelParam type) const
	{
		return m_enableFlag & ( 1 << type);
	}

	// Checks whether a parameter is mutalbe or not
	int FxModel::IsMutable( EModelParam type) const
	{
		return m_mutableFlag & ( 1<<type); 
	}

	// Checks whether a parameter is random or not
	int FxModel::IsRandom( EModelParam type) const
	{
		return m_randomFlag & ( 1<< type);
	}

	// Checks whether a parameter is interpolated or not
	int FxModel::IsInterpolated( EModelParam type) const
	{
		return m_interpolatedFlag & ( 1<< type);
	}

	// Sets a given parameter with 1 value
	bool FxModel::SetParam(EModelParam type, float value)
	{
		// 当前参数类型是否只有一个值
		if( GetNumValues( type) != 1)
			return false;

		m_params[m_indices[type]] = value;

		return true;
	}

	// Sets a given parameter with 2 value
	bool FxModel::SetParam( EModelParam type, float value0, float value1)
	{
		if( GetNumValues( type) != 2)
			return false;

		m_params[m_indices[type]]   = value0;
		m_params[m_indices[type]+1] = value1;

		return true;
	}

	// Sets a given parammeter with 4 values
	bool FxModel::SetParam( EModelParam type, float startMin, float startMax, float endMin, float endMax)
	{
		// If the given param doesn't have 4 values, return
		if( GetNumValues( type) != 4)
			return false;

		m_params[m_indices[type]]   = startMin;
		m_params[m_indices[type]+1] = startMax;
		m_params[m_indices[type]+2] = endMin;
		m_params[m_indices[type]+3] = endMax;

		return true;
	}

	// 获取指定类型参数的值数量
	size_t FxModel::GetNumValues( EModelParam type)const
	{
		int value = 1<<type;
		
		if( !(m_enableFlag & value) || (m_interpolatedFlag & value))
			return 0;

		if( !(m_mutableFlag & value) && !(m_randomFlag & value))
			return 1;

		if( (m_mutableFlag & value) && (m_randomFlag & value))
			return 4;

		return 2;
	}

	// 获取参数默认值(静态函数)
	float FxModel::GetDefaultValue( FxModel::EModelParam param)
	{
		return DEFAULT_VALUES[param];
	}
}