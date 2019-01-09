#pragma once

// STL
#include <set>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <bitset>

#ifdef PLATFORM_WIN32
	#include "AresVcWin32.h"
#elif defined PLATFORM_MAC
	#include "AresXcodeMac.h"
#elif defined PLATFORM_IOS
	#include "AresXcodeIOS.h"
#endif

// Boost
#include <boost/tr1/detail/config.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/mem_fn.hpp>
#include <boost/bind.hpp>
#include <boost/any.hpp>
#include <boost/function.hpp>
#include <boost/variant.hpp>
#include <boost/noncopyable.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/foreach.hpp>
#include <boost/array.hpp>

// TinyXml
//#include "3rdParty/TinyXml/tinyxml.h"

// RapidXml
//#include "3rdParty/RapidXml/rapidxml_utils.hpp"
//#include "3rdParty/RapidXml/rapidxml_print.hpp"


#define foreach_		 BOOST_FOREACH
#define reverse_foreach_ BOOST_REVERSE_FOREACH