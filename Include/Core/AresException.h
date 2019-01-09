#pragma once

#include <string>
#include <stdexcept>
#include <boost/exception/all.hpp>

namespace Ares
{
	//-------------------------------------
	// 异常 2012-3-9 帝林
	//-------------------------------------
	struct ares_exception : virtual std::exception, virtual boost::exception
	{};

	// 异常信息类型定义
	typedef boost::error_info<struct exception_str, std::string> err_str;

	// 抛出异常
	#define THR \
	{ \
		stringstream ss; \
		ss << x <<  ":" << __FILE__ << ": " << __LINE__; \
		throw std::runtime_error( ss.str()); \
	}

	// 如果错误，就抛出错误代码
	#define TIF(hr)	\
	{ \
		if ( FAILED(hr)) \
		{ \
			std::stringstream ss; \
			ss << __FILE__ << ": " << __LINE__; \
			throw std::runtime_error( ss.str()); \
		} \
	}
}