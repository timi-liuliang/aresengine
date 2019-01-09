#pragma once

#include <Engine/FxSystem/AresFxZone.h>
#include <Physics/Box3.h>

namespace Ares
{
	//----------------------------------
	// Box3区域 2013-1-23 帝林
	//----------------------------------
	class FxBox3Zone : public FxZone
	{
	public:
		FxBox3Zone();
		FxBox3Zone( const Box3& box);

		// Checks whether a point is within the zone
		virtual bool Contains( const Vector3& point) const;

		// Check whether a line intersects the zone
		virtual bool Intersects( const Vector3& v0, const Vector3& v1, Vector3* intersection, Vector3* normal)const;

		// Generates a position inside this zone for a given particle
		virtual void GeneratePosition( FxParticle& particle, bool full) const;

		// 序列化
		template <typename Archive> void serialize( Archive& ar, const unsigned int version)
		{
			ar & boost::serialization::base_object<FxZone>(*this);
			ar & m_box3;
		}

	private:
		Box3		m_box3;
	};
}