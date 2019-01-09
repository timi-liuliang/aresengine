#pragma once

namespace Ares
{
	//---------------------------------------
	// NetLogic 2012-9-3 帝林
	//---------------------------------------
	//class Net::client;
	class NetLogic
	{
	public:
		NetLogic();

		// 提交Bug
		void SubmitBug( const char* bugDesc);

	private:
		//Net::client*		m_client;		// 网络通迅
	};
}