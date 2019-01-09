#include "Engine/InputSystem/OISMouse.h"
#include <algorithm>

namespace OIS
{
	// ×¢²á¼àÌıÆ÷
	bool Mouse::RegisterEventCallback( MouseListener* mouseListener ) 
	{ 
		if( mouseListener && std::find( m_listeners.begin(), m_listeners.end(), mouseListener)==m_listeners.end())
		{
			m_listeners.push_back( mouseListener);

			return true;
		}
		
		return false;
	}

	// ÒÆ³ı×¢²á
	bool Mouse::UnRegisterEventCallback( MouseListener* mouseListener)
	{
		m_listeners.erase( std::remove( m_listeners.begin(), m_listeners.end(), mouseListener), m_listeners.end());

		return true;
	}
}