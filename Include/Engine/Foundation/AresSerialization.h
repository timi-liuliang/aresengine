#pragma once

#include <Core/AresMath.h>
#include <Physics/Segment3.h>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

using namespace Ares;
using namespace boost::archive;

#pragma warning( disable:4308)

namespace boost{
namespace serialization
{
	//// Vector3 serialization
	//template<class archive>
	//void serialize( archive& ar, Vector3& v, const unsigned int version)
	//{
	//	ar & v.x;
	//	ar & v.y;
	//	ar & v.z;
	//}

	// Segment3 serialization
	template<class archive>
	void save( archive& ar, const Segment3& seg, const unsigned int)
	{
		Vector3 v0 = seg.GetVertex0();
		Vector3 v1 = seg.GetVertex1();

		ar << v0 << v1;
	}

	template<class archive>
	void load( archive& ar, Segment3& seg, const unsigned int)
	{
		Vector3 v0, v1;

		ar >> v0 >> v1;

		seg.Set( v0, v1);
	}

	template<class archive>
	void serialize( archive& ar, Segment3& seg, const unsigned int version)
	{
		serialization::split_free( ar, seg, version);
	}

	// Capsule3 serialization
	template<class archive>
	void serialize( archive& ar, Capsule3& c, const unsigned int version)
	{
		ar & c.m_segment;
		ar & c.m_radius;
	}
}}