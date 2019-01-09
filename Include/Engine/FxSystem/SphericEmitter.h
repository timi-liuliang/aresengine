#pragma once

#include <Engine/FxSystem/AresFxEmitter.h>

namespace Ares
{
	//------------------------------------------
	// SphericaEmitter 2011-9-15 帝林
	//------------------------------------------
	class FxSphericEmitter : public FxEmitter
	{
	public:
		// constructor
		FxSphericEmitter( const Vector3& dir = Vector3(0.f, 0.f, -1.f), float angleA=0.f, float angleB=0.f);

		// A pure method, that generates the volocity of the particle in function of a speed
		virtual void GenerateVelocity( FxParticle& particle, float speed) const;
	
	public:
		// Sets the direction of this sphericEmitter
		void SetDirection( const Vector3& direction);

		// Sets the angles of this sphericemitter
		void SetAngles( float angleA, float angleB);

		// 序列化
		template <typename Archive> void serialize( Archive& ar, const unsigned int version)
		{
			ar & boost::serialization::base_object<FxEmitter>(*this);
			ar & m_direction;
			ar & m_tfDirection;
			ar & m_angleMin;
			ar & m_angleMax;
			ar & m_cosAngleMin;
			ar & m_cosAngleMax;
			ar & m_matrix;
		}

	private:
		// copute the matrix
		void ComputeMatrix();

	private:
		Vector3		m_direction;			// 朝向
		Vector3		m_tfDirection;			// 转换后朝向

		float		m_angleMin;
		float		m_angleMax;

		float		m_cosAngleMin;
		float		m_cosAngleMax;

		boost::array<float, 9>	m_matrix;
	};
}