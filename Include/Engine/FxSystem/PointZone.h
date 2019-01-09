#pragma once

#include <Engine/FxSystem/AresFxZone.h>

namespace Ares
{
	//---------------------------------------
	// PointZone 2011-9-19  µ€¡÷
	//---------------------------------------
	class FxPointZone : public FxZone
	{
	public:
		// constructor
		FxPointZone( const Vector3& position = Vector3::Zero);

	public:
		// Checks whether a point is within the zone
		virtual bool Contains( const Vector3& point) const;

		// Check whether a line intersects the zone
		virtual bool Intersects( const Vector3& v0, const Vector3& v1, Vector3* intersection, Vector3* normal)const;

		// Generates a position inside this zone for a given particle
		virtual void GeneratePosition( FxParticle& particle, bool full) const;

		// –Ú¡–ªØ
		template <typename Archive> void serialize( Archive& ar, const unsigned int version)
		{
			ar & boost::serialization::base_object<FxZone>(*this);
		}
	};
}