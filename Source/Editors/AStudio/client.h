#pragma once

#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/system/error_code.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;

namespace Net
{
	//------------------------------------
	// 网络连接逻辑 2012-9-3 帝林
	//------------------------------------
	class client
	{
		typedef boost::shared_ptr<tcp::socket>  socket_ptr;
		typedef boost::shared_ptr<std::vector<char>> buffer_ptr;
	public:
		// 构造函数
		client();

		// 开始服务
		void run();

		// 连接服务器
		void connect( const char* ip, int port);

		// 异步读
		void read();

		// 异步写
		void write( const char* data);

	private:
		// internal run
		void internal_run();

		// 异步连接调用
		void conn_handler( const system::error_code& ec, socket_ptr sock);

		// 异步读处理
		void read_handler( const system::error_code& ec, buffer_ptr str);

		// 异步写处理
		void write_handler( const system::error_code& ec);

	private:
		io_service	m_ios;			// io_service
		socket_ptr	m_socket;		// 套接字
	};
}