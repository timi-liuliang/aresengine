#include <Physics/IntrCapsule3Capsule3.h>
#include <Physics/DistSegment3Segment3.h>

namespace Ares
{
	// constructor
	IntrCapsule3Capsule3::IntrCapsule3Capsule3( const Capsule3& capsule0, const Capsule3& capsule1)
		: m_capsule0( capsule0)
		, m_capsule1( capsule1)
	{
	}

	// test intersection query
	bool IntrCapsule3Capsule3::Test()
	{
		float distance = DistSegment3Segment3( m_capsule0.m_segment, m_capsule1.m_segment).Get();
		float sum	   = m_capsule0.m_radius + m_capsule1.m_radius;
		if( distance<=sum)
			return true;
	
		return false;
	}
}