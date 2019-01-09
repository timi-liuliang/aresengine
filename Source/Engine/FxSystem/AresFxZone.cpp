#include <Engine/FxSystem/AresFxZone.h>

BOOST_CLASS_EXPORT( Ares::FxZone)

namespace Ares
{
	// constructor
	FxZone::FxZone( const Vector3& position) 
	{
		m_position;
		m_tfPosition;

		SetPosition( position);
	}

	// destructor
	FxZone::~FxZone()
	{

	}

	// set position
	void FxZone::SetPosition( const Vector3& pos)
	{
		m_position = m_tfPosition = pos;
	}
}