#include <stdarg.h>
#include "Core/AresLogSystem.h"

#define MaxBufferLen 2048

namespace Ares
{	
	// Constructor
	LogSystem::LogSystem()
	{}

	// Destructor
	LogSystem::~LogSystem() 
	{}

	// Report Info
	void LogSystem::Info( const char* format, ...)
	{
		if( !Signal_Info.empty())
		{
			char    buf[MaxBufferLen];
			va_list args;

			va_start(args, format);
			vsnprintf( buf, MaxBufferLen, format, args); 
			va_end(args);

			Signal_Info( buf);
		}
	}

	// Report Warning
	void LogSystem::Warning( const char* format, ...)
	{
		if( !Signal_Warning.empty())
		{
			char    buf[MaxBufferLen];
			va_list args;

			va_start(args, format);
			vsnprintf( buf, MaxBufferLen, format, args); 
			va_end(args);

			Signal_Warning( buf);
		}
	}

	// ReportError
	void LogSystem::Error( const char* format, ...)
	{
		if( !Signal_Error.empty())
		{
			char    buf[MaxBufferLen];
			va_list args;

			va_start(args, format);
			vsnprintf( buf, MaxBufferLen, format, args); 
			va_end(args);

			Signal_Error( buf);
		}
	}
}