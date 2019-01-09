#include "client.h"
#include <string>
#include <vector>

namespace Net
{
	// 构造函数
	client::client()
	{
	}

	// 开始服务
	void client::run()
	{
		thread* thre = new thread( boost::bind( &client::internal_run, this));
	}

	// internal run
	void client::internal_run()
	{
		m_ios.run();
	}

	// 连接服务器
	void client::connect(  const char* ip, int port)
	{
		m_socket = socket_ptr( new tcp::socket( m_ios));

		tcp::endpoint ep( ip::address::from_string(ip), port);

		m_socket->async_connect( ep, bind(&client::conn_handler, this, placeholders::error, m_socket));
	}

	// 异步连接调用
	void client::conn_handler( const system::error_code& ec, socket_ptr sock)
	{
		if( ec) return;

		read();
	}

	// 异步读
	void client::read()
	{
		// 接收数据
		buffer_ptr str( new std::vector<char>( 1024, 0));
		m_socket->async_read_some( buffer(*str), boost::bind( &client::read_handler, this, placeholders::error, str));
	}

	// 异步读处理
	void client::read_handler( const system::error_code& ec, buffer_ptr str)
	{
		if( ec) return;

		read();
	}

	// 异步写
	void client::write( const char* data)
	{
		std::string s_data( data);
		m_socket->async_write_some( buffer(s_data), boost::bind( &client::write_handler, this, placeholders::error));
	}

	// 异步写处理
	void client::write_handler( const system::error_code& ec)
	{
		if( ec)
		{
			int a = 10;
		}
	}
}