#pragma once

#include "AresPlatform.h"

namespace Ares
{
	//-----------------------------------------
	// 封装线程  2011-07-05  帝林
	//-----------------------------------------
	class CThread
	{
	public:
		// 线程函数
		typedef DWORD (WINAPI *ThreadFuncPtr)(void* arg);

		// 构造函数
		CThread( ThreadFuncPtr pFun, void* pArg);

		// 析构函数
		~CThread();

	public:
		// 获取句柄
		HANDLE GetHandle() const { return m_handle; }

		// 获取Identifier
		DWORD GetIdentifier() const { return m_ID; }

	public:
		// 设置优先级
		void SetPriority( int priority);

		// 获取优先级
		int GetPriority() const;

	public:
		// 恢复
		void Resume();

		// 挂起
		void Suspend();

		// 等待完成
		void WaitForFinish( DWORD milliseconds = 100);

		// 是否运行中
		bool IsRunning() const;

	private:
		HANDLE       m_handle;
		DWORD        m_ID;				// 线程ID

	private:
		// 禁用拷贝构造( Not Implemented)
		CThread( const CThread& ref);
	};
}