#pragma once

#include <Core/AresMath.h>
#include "AresFxParticle.h"

namespace Ares
{
	//----------------------------------------------
	// FxZone 2011-9-14  ╣шаж
	//     An abstract class that defines a zone in
	// space.
	//     A Zone is used to :
	//     1.define the area of an emitter
	//     2.define the area of a modifier
	//----------------------------------------------
	class FxZone
	{
	public:
		// constructor
		FxZone( const Vector3& position = Vector3( 0.f, 0.f, 0.f));

		// destructor
		virtual ~FxZone();

		// Checks whether a point is within the zone
		virtual bool Contains( const Vector3& point) const=0;

		// Check whether a line intersects the zone
		virtual bool Intersects( const Vector3& v0, const Vector3& v1, Vector3* intersection, Vector3* normal)const=0;
	
		// Generates a position inside this zone for a given particle
		virtual void GeneratePosition( FxParticle& particle, bool full) const=0;

		// set position
		virtual void SetPosition( const Vector3& pos);

		// Serialize
		template< typename Archive> void serialize( Archive& ar, const unsigned int version)
		{
			ar & m_position;
			ar & m_tfPosition;
		}

	protected:
		Vector3 m_position;
		Vector3 m_tfPosition;		// transformed position
	};
}