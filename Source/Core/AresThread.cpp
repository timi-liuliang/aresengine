#include "Core/AresThread.h"

namespace Ares
{
	// 构造函数
	CThread::CThread( ThreadFuncPtr pFun, void* pArg)
		: m_handle( 0),
		  m_ID( -1)
	{
		m_handle = CreateThread( 0, 0, pFun, pArg, CREATE_SUSPENDED, &m_ID);
	}

	// 析构函数
	CThread::~CThread()
	{
		// 结束线程
		/*DWORD exitCode;
		GetExitCodeThread( m_handle, &exitCode);
		ExitThread( exitCode);*/

		// 关闭句柄
		CloseHandle( m_handle);
	}

	// 设置优先级
	void CThread::SetPriority( int priority)
	{
		SetThreadPriority( m_handle, priority);
	}

	// 获取优先级
	int CThread::GetPriority() const 
	{ 
		return GetThreadPriority( m_handle); 
	}

	// 恢复线程
	void CThread::Resume()
	{
		ResumeThread( m_handle);
	}

	// 挂起
	void CThread::Suspend()
	{
		SuspendThread( m_handle);
	}

	// 等待完成
	void CThread::WaitForFinish( DWORD milliseconds/* = 100*/)
	{
		WaitForSingleObject( m_handle, milliseconds);
	}

	// 是否运行中
	bool CThread::IsRunning() const
	{
		DWORD exitCode;
		GetExitCodeThread( m_handle, &exitCode);

		return (exitCode == STILL_ACTIVE);
	}
}