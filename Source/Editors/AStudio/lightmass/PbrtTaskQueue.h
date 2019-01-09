#pragma once

#include <vector>
#include <string>
#include <Core/AresThreadLoopObject.h>
#include <Core/AresCriticalSection.h>
#include <boost/signals2/signal.hpp>

using namespace std;
using namespace boost;
using namespace boost::signals2;

struct Options;

namespace Ares
{
	//-----------------------------------------
	// Pbrt生成任务队列 2012-4-19  帝林
	//-----------------------------------------
	class PbrtTaskQueue : public CThreadLoopObject
	{
	public:
		// 虚方法 Loop
		virtual void Loop();

		// 添加pbrt任务
		void AddTask( const char* pbrtFileName, const Options& options);

		// 判断任务是否已存在
		bool IsTaskExist( const char* pbrtTask);

	public:
		// pbrt生成结束信号
		signal<void(const char*)> Signal_OnPbrtFinised;

	private:
		string					m_curTask;			// 当前任务
		map<string, Options>	m_tasks;			// 要执行生成的pbrt文件
		boost::recursive_mutex	m_criticalSection;	// 临界区(保护m_tasks任务列表)
	};
}