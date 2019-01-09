#pragma once

#include <Engine/FxSystem/AresFxZone.h>

namespace Ares
{
	//-------------------------------------
	// PlaneZone 2011-9-19  帝林
	//-------------------------------------
	class FxPlaneZone : public FxZone
	{
	public:
		// constructor
		FxPlaneZone(const Vector3& position=Vector3(0.f,0.f,0.f), const Vector3& normal=Vector3(0.f,0.f,1.f));

		// Checks whether a point is within the zone
		virtual bool Contains( const Vector3& point) const;

		// Check whether a line intersects the zone
		virtual bool Intersects( const Vector3& v0, const Vector3& v1, Vector3* intersection, Vector3* normal)const;

		// Generates a position inside this zone for a given particle
		virtual void GeneratePosition( FxParticle& particle, bool full) const;

		// 设置法线
		void SetNormal( const Vector3& normal);

		// 序列化
		template <typename Archive> void serialize( Archive& ar, const unsigned int version)
		{
			ar & boost::serialization::base_object<FxZone>(*this);
			ar & m_normal;
			ar & m_tNormal;
		}
	
	private:
		Vector3		m_normal;
		Vector3		m_tNormal;
	};
}