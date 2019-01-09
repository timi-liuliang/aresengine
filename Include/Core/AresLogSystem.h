#pragma once

#include <boost/signals2/signal.hpp>

using namespace boost;
using namespace boost::signals2;

namespace Ares
{
	//------------------------------------
	// LogSystem	2012-9-28 ╣шаж
	//------------------------------------
	class LogSystem
	{
	public:
		// Constructor
		LogSystem();

		// Destructor
		virtual ~LogSystem();

		// Report Info
		void Info( const char* format, ...);

		// Report Warning
		void Warning( const char* format, ...);

		// ReportError
		void Error( const char* format, ...);

	public:
		// Info signals
		signal<void(const char* msg)> Signal_Info;

		// Warning signals
		signal<void(const char* msg)> Signal_Warning;

		// Error signals
		signal<void(const char* msg)> Signal_Error;
	};
}