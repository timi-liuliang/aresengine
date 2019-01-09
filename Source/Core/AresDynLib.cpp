#include "Core/AresDynLib.h"

namespace Ares
{
	// constructor used by CDynLibManager
	CDynLib::CDynLib( const char* name)
	{
		m_name   = name;
		m_handle = NULL;

#ifdef ARES_PLATFORM_WINDOWS
		// Although LoadLibraryEx will add .dll itself when you only specify the library name,
		// if you include a relative path then it does not. So, add it to be sure
		if( m_name.substr( m_name.length()-4, 4) != ".dll")
			m_name += ".dll";
#endif
	}

	// destructor
	CDynLib::~CDynLib()
	{

	}

	// º”‘ÿ
	void CDynLib::Load()
	{
		m_handle = DYNLIB_LOAD( m_name.c_str());
		if( !m_handle)
		{

		}
	}

	// –∂‘ÿ
	void CDynLib::Unload()
	{
		if( !DYNLIB_UNLOAD( m_handle))
		{

		}
	}

	// ªÒ»°symbol
	void* CDynLib::GetSymbol( const char* strName)
	{
		return (void*)DYNLIB_GETSYM( m_handle, strName);
	}
}