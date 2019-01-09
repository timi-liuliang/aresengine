#pragma once

#include "Core/AresPlatform.h"

namespace Ares
{
	//-------------------------------------
	// йс©з 2012-3-9  ╣шаж 
	//-------------------------------------
	struct CORE_EXPORT ViewPort
	{
		UINT	m_left;
		UINT	m_top;
		UINT	m_width;
		UINT	m_height;

		// constructor
		ViewPort() {}

		// constructor
		ViewPort( UINT _left, UINT _top, UINT _width, UINT _height)
			: m_left( _left),
			  m_top( _top),
			  m_width( _width),
			  m_height( _height)
		{

		}
	};
}