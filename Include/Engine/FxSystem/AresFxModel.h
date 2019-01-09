#pragma once

#include <vector>
#include <Core/AresSerialization.h>
#include <Core/AresPlatform.h>
#include "AresFxModelInterpolater.h"

using namespace std;

namespace Ares
{
	//--------------------------------------------------
	// FxModel 2011-9-13  帝林
	//	   Defines a behavior for particles generation
	// and evolution over time. A Particle is generated 
	// under a model that cannot be changed during its
	// life time.
	//--------------------------------------------------
	class FxParticle;
	class FxModel
	{
		friend class FxParticle;
	public:
		// Modelparam
		enum EModelParam
		{
			PARAM_RED			 = 0,			// the red component of the particle
			PARAM_GREEN			 = 1,			// the green component of the particle
			PARAM_BLUE			 = 2,			// the blue component of the particle
			PARAM_ALPHA			 = 3,			// the alpha componet of the particle
			PARAM_SIZE			 = 4,			// the size of the particle
			PARAM_MASS			 = 5,			// the mass of the particle
			PARAM_ANGLE          = 6,			// the angle of the texture of the particle
			PARAM_TEXTURE_INDEX  = 7,			// the index of the texture of the particle
			PARAM_ROTATION_SPEED = 8,			// the rotation speed of the particle( must be used with a rotator modifier)
			PARAM_CUSTOM_0		 = 9,			// Reserved for a user custom parameter
			PARAM_CUSTOM_1		 = 10,			// Reserved for a user custom parameter
			PARAM_CUSTOM_2		 = 11,			// Reserved for a user custom parameter
		};

		// ModelParamFlag
		enum EModelParamFlag
		{
			FLAG_NONE			= 0,							// the flag bit for no paramete
			FLAG_RED			= 1 << PARAM_RED,				// the flag bit for PARAM_RED
			FLAG_GREEN			= 1 << PARAM_GREEN,				// the flag bit for PARAM_GREEN
			FLAG_BLUE			= 1 << PARAM_BLUE,				// the flag bit for PARAM_BLUE
			FLAG_ALPHA			= 1 << PARAM_ALPHA,				// the flag bit for PARAM_ALPHA
			FLAG_SIZE			= 1 << PARAM_SIZE,				// the flag bit for PARAM_SIZE
			FLAG_MASS			= 1 << PARAM_MASS,				// the flag bit for PARAM_MASS
			FLAG_ANGLE			= 1 << PARAM_ANGLE,				// the flag bit for PARAM_ANGLE
			FLAG_TEXTURE_INDEX  = 1 << PARAM_TEXTURE_INDEX,		// the flag bit for PARAM_TEXTURE_INDEX
			FLAG_ROTATION_SPEED = 1 << PARAM_ROTATION_SPEED,	// the flag bit for PARAM_ROTATION_SPEED
			FLAG_CUSTOM_0	    = 1 << PARAM_CUSTOM_0,			// the flag bit for PARAM_CUSTOM_0
			FLAG_CUSTOM_1       = 1 << PARAM_CUSTOM_1,			// the flag bit for PARAM_CUSTOM_1
			FLAG_CUSTOM_2	    = 1 << PARAM_CUSTOM_2,			// the flag bit for PARAM_CUSTOM_2
		};

	public:
		FxModel();
		FxModel( int enableFlag, int mutableFlag = FLAG_NONE, int randomFlag = FLAG_NONE, int interpolateFlag = FLAG_NONE);
	
		// Sets the life time
		void SetLifeTime( float lifeTimeMin, float lifeTimeMax);

		// Gets the minimum life time
		float GetLifeTimeMin();

		// Gets the maximum life time
		float GetLifeTimeMax();

	public:
		// Defines wheather to generate immortal particles or not
		void SetImmortal( bool immortal);

		// Returns whether immortility is set or not
		bool IsImmortal();

	public:
		// Checks whether a parameter is enabled or not
		int IsEnabled( EModelParam type) const;

		// Checks whether a parameter is mutalbe or not
		int IsMutable( EModelParam type) const;

		// Checks whether a parameter is random or not
		int IsRandom( EModelParam type) const;

		// Checks whether a parameter is interpolated or not
		int IsInterpolated( EModelParam type) const;

	public:
		// 获取指定类型参数的值数量
		size_t GetNumValues( EModelParam type)const;

		// Sets a given parameter with 1 value
		bool SetParam( EModelParam type, float value);

		// Sets a given parameter with 2 values
		bool SetParam( EModelParam type, float value0, float value1);

		// Sets a given parammeter with 4 values
		bool SetParam( EModelParam type, float startMin, float startMax, float endMin, float endMax);

		// 获取参数默认值(静态函数)
		static float GetDefaultValue( EModelParam param);

		// Gets the number of float values in the particle current array
		size_t GetSizeOfParticleCurrentArray() const { return m_enableParams.size(); }

		// Gets the number of float values in the particle extended array
		size_t GetSizeOfParticleExtendedArray() const;

		// 序列化
		template <typename Archive> void serialize( Archive& ar, const unsigned int version)
		{
			ar & m_lifeTimeMin;
			ar & m_lifeTimeMax;
			ar & m_immortal;
			ar & m_interpolators;
			ar & m_enableFlag;
			ar & m_mutableFlag;
			ar & m_randomFlag;
			ar & m_interpolatedFlag;
			ar & m_particleEnableIndices;
			ar & m_particleMutableIndices;
			ar & m_indices;
			ar & m_enableParams;
			ar & m_mutableParams;
			ar & m_interpolatedParams;
			ar & m_numRandomParams;
			ar & m_params;
		}

	private:
		static const size_t NB_PARAMS = 12;				// total number of params
		static const float  DEFAULT_VALUES[NB_PARAMS];	// default values for the parameters

		float			m_lifeTimeMin;							
		float			m_lifeTimeMax;							
		bool			m_immortal;								

		boost::array<FxModelInterpolator*, NB_PARAMS>	m_interpolators;

		int				m_enableFlag;
		int				m_mutableFlag;
		int				m_randomFlag;
		int				m_interpolatedFlag;						// Model flag

		int				m_particleEnableIndices[NB_PARAMS];		// array storing the index of a parameter in the enableParams array
		int				m_particleMutableIndices[NB_PARAMS];	// array storing the index of a parameter in the  mutableParams array
		int				m_indices[NB_PARAMS];					// array storing the index of a parameter in the model param array

		vector<int>		m_enableParams;							// array storing the parameters that are mutable
		vector<int>		m_mutableParams;						// array storing the parameters that are mutable
		vector<int>		m_interpolatedParams;					// array storing the parameters that are interpolated

		size_t			m_numRandomParams;						// number of random parameters

		// arrays storing the values of parameters for the model following that form
		// enable  : 1 value -> value
		// mutable : 2 values-> start value | end value
		// random  : 2 values-> min value   | max value
		// mutable and random : 4 values-> start min value | start max value | end min value | end max value
		// interpolated : 0 value
		vector<float>	m_params;
	};
}