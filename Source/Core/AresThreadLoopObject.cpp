#include "Core/AresThreadLoopObject.h"

using namespace boost;

namespace Ares
{
	// 构造函数
	CThreadLoopObject::CThreadLoopObject()
		: m_isFinishing( false),
		  m_thread( NULL),
		  m_exitFlag( false),
		  m_loopIntervel()
	{
		//Start( 20);
	}

	// 析函函数
	CThreadLoopObject::~CThreadLoopObject()
	{
		if( m_thread)
		{
			m_thread->join();

			SAFE_DELETE( m_thread);
		}
	}

	// 获取线程
	boost::thread* CThreadLoopObject::GetThread() const
	{
		return m_thread;
	}

	// 设置优先级
	void CThreadLoopObject::SetPriority( int priority)
	{
	}

	// 获取优先级
	int CThreadLoopObject::GetPriority() const
	{
		if( m_thread)
		{
		}

		return 0;
	}

	// 开始(创建线程,恢复线程)
	void CThreadLoopObject::Start(DWORD hz/*=0*/)
	{
		// 循环帧率控制
		if( hz == 0)
			m_loopIntervel = 0;
		else
			m_loopIntervel = 1000 / hz; 

		// 线程不存在,创建
		if( m_thread == NULL)
		{
			m_thread = new_ boost::thread( ThreadObjectFunPtr, this);
		}

		// 恢复
		//m_thread->Resume();
	}

	// 暂停(挂起线程)
	void CThreadLoopObject::Pause()
	{
		//if( m_thread)
		//	m_thread->Suspend();
	}

	// 完成任务
	void CThreadLoopObject::Finish()
	{
		m_isFinishing = true;

		Start();
		End();
		//Sleep(0);
	
		SAFE_DELETE( m_thread);

		m_isFinishing = false;
	}

	// 判断是否已完成任务
	bool CThreadLoopObject::IsFinishing() const
	{
		return m_isFinishing;
	}

	// 处理循环
	void CThreadLoopObject::ProcessLoop()
	{
		Init();

		while( !m_exitFlag)
		{ 
			Loop();

            this_thread::sleep(posix_time::seconds(m_loopIntervel));
		}
	}

	// 初始化
	void CThreadLoopObject::Init()
	{
		m_exitFlag = false;
	}

	// 结束
	void CThreadLoopObject::End()
	{
		m_exitFlag = true;
	}

	// 线程函数
	DWORD CThreadLoopObject::ThreadObjectFunPtr(void* pArg)
	{
		// 线程创建,开始循环
		CThreadLoopObject* pThreadObject = (CThreadLoopObject*)pArg;
		pThreadObject->ProcessLoop();

		return 0;
	}
}