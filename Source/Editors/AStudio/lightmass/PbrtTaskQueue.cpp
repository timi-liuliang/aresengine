#include "PbrtTaskQueue.h"
#include <pbrt/core/api.h>
#include <pbrt/core/parser.h>
#include <pbrt/core/parallel.h>
#include <pbrt/core/api.h>
#include <pbrt/core/pbrt.h>

namespace Ares
{
	// 虚方法 Loop
	void PbrtTaskQueue::Loop()
	{
		// 获取任务
		string  tPbrt;
		Options tOptions;
		{
			ENTER_CRITICAL_SECTION( locker, m_criticalSection);
			if( !m_tasks.empty())
			{
				tPbrt    = m_tasks.begin()->first;
				tOptions = m_tasks.begin()->second;
				
				m_tasks.erase( m_tasks.begin());		
			}

			m_curTask = tPbrt;
		}

		if( tPbrt.size())
		{
			// 初始化
			pbrtInit( tOptions);

			// 分析场景文件
			ParseFile( tPbrt.c_str());

			// 清除数据
			pbrtCleanup();

			Signal_OnPbrtFinised( tOptions.imageFile.c_str());
		}
	}

	// 添加pbrt任务
	void PbrtTaskQueue::AddTask( const char* pbrtFileName, const Options& options)
	{
		ENTER_CRITICAL_SECTION( holder, m_criticalSection);
		
		m_tasks[pbrtFileName] = options;
	}

	// 判断任务是否已存在
	bool PbrtTaskQueue::IsTaskExist( const char* pbrtTask)
	{
		ENTER_CRITICAL_SECTION( locker, m_criticalSection);
		if( m_tasks.find( pbrtTask) != m_tasks.end())
			return true;

		if( m_curTask == pbrtTask)
			return true;

		return false;
	}
}