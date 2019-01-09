#include <Engine/FxSystem/SphericEmitter.h>
#include <algorithm>
#include <Core/AresKitFunction.h>
#include <Engine/FxSystem/AresFxParticle.h>

BOOST_CLASS_EXPORT( Ares::FxSphericEmitter)

namespace Ares
{
	// constructor
	FxSphericEmitter::FxSphericEmitter( const Vector3& dir, float angleA, float angleB)
	{
		SetDirection( dir);
		SetAngles( angleA, angleB);
	}

	// A pure method, that generates the volocity of the particle in function of a speed
	void FxSphericEmitter::GenerateVelocity( FxParticle& particle, float speed) const
	{
		float a = Random( m_cosAngleMax, m_cosAngleMin);
		float theta = std::acos(a);
		float phi	= Random( 0.f, 2.f * PI);

		float sinTheta = std::sin( theta);
		float x = sinTheta * std::cos( phi);
		float y = sinTheta * std::sin( phi);
		float z = std::cos( theta);

		Vector3 velocity;
		velocity.x = speed * (m_matrix[0] * x + m_matrix[1] * y + m_matrix[2] * z);
		velocity.y = speed * (m_matrix[3] * x + m_matrix[4] * y + m_matrix[5] * z);
		velocity.z = speed * (m_matrix[6] * x + m_matrix[7] * y + m_matrix[8] * z);

		particle.SetVelocity( velocity);
	}

	// Sets the direction of this sphericEmitter
	void FxSphericEmitter::SetDirection( const Vector3& direction)
	{
		m_tfDirection = direction;

		ComputeMatrix();

		m_direction = m_tfDirection;
	}

	// Sets the angles of this sphericemitter
	void FxSphericEmitter::SetAngles( float angleA, float angleB)
	{
		if( angleB < angleA)
			std::swap( angleA, angleB);

		angleA = (std::min)( 2.f * PI, (std::max)( 0.f, angleA));
		angleB = (std::min)( 2.f * PI, (std::max)( 0.f, angleB));

		m_angleMin = angleA;
		m_angleMax = angleB;

		m_cosAngleMin = std::cos( m_angleMin * 0.5f);
		m_cosAngleMax = std::cos( m_angleMax * 0.5f);
	}

	// copute the matrix
	void FxSphericEmitter::ComputeMatrix()
	{
		m_tfDirection.Normalize();
		if( m_tfDirection.x == 0.f && m_tfDirection.y == 0.f)
		{
			m_matrix[0] = m_tfDirection.z;
			m_matrix[1] = 0.f;
			m_matrix[2] = 0.f;
			m_matrix[3] = 0.f;
			m_matrix[4] = m_tfDirection.z;
			m_matrix[5] = 0.f;
			m_matrix[6] = 0.f;
			m_matrix[7] = 0.f;
			m_matrix[8] = m_tfDirection.z;
		}
		else
		{
			Vector3 axis;

			axis = Vector3Cross( m_tfDirection, Vector3( 0.f, 0.f, 1.f));

			float cosA = m_tfDirection.z;
			float sinA = -m_tfDirection.Length();
			axis /= -sinA;

			float x = axis.x;
			float y = axis.y;
			float z = axis.z;

			m_matrix[0] = x * x + cosA * ( 1.f - x * x);
			m_matrix[1] = x * y + ( 1.f - cosA) - z * sinA;
			m_matrix[2] = m_tfDirection.x;
			m_matrix[3] = x * y + ( 1.f - cosA) + z*sinA;
			m_matrix[4] = y * y + cosA * ( 1.f - y * y);
			m_matrix[5] = m_tfDirection.y;
			m_matrix[6] = x * z + ( 1.f - cosA) - y * sinA;
			m_matrix[7] = y * z * ( 1.f - cosA) + x * sinA;
			m_matrix[8] = m_tfDirection.z;
		}
	}
}