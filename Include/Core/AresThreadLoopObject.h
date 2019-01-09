#pragma once

#include "AresPlatform.h"
#include <boost/thread.hpp>

namespace Ares
{
	//----------------------------------------------------------
	// class CThreadLoopObject 2011-08-28 帝林
	// description:
	//     This class provides an easy way to implement a Thread
	// Object with its own process loop. It also provides the 
	// methods to control it.
	//     The user of CThreadLoopObject only have to implement the 
	// methode "virtual void Loop()=0;" which should contain the 
	// cide to be excute in each loop. the loop control is already
	// implemented in the method process loop.
	//----------------------------------------------------------
	class CThreadLoopObject
	{
	public:
		// 构造函数
		CThreadLoopObject();

		// 析函函数
		virtual ~CThreadLoopObject();

	public:
		// 获取线程
		boost::thread* GetThread() const;

	public:
		// 设置优先级
		void SetPriority( int priority);

		// 获取优先级
		int GetPriority() const;

	public:
		// 开始(创建线程,恢复线程)
		virtual void Start( DWORD hz=0);

		// 暂停(挂起线程)
		void Pause();

		// 完成任务
		void Finish();

		// 判断是否已完成任务
		bool IsFinishing() const;

		// 处理循环
		void ProcessLoop();

		// 初始化
		virtual void Init();

		// 结束
		virtual void End();

	protected:
		// 虚方法,被子类实现
		virtual void Loop()=0;

	protected:
		// 线程函数
		static DWORD ThreadObjectFunPtr(void* pArg);

	protected:
		boost::thread*	m_thread;			// 线程
		bool			m_isFinishing;		// 完成标记
		bool			m_exitFlag;			// 退出标记
		DWORD			m_loopIntervel;		// 循环间隔

	private:
		// copy constructor function, not implemented
		CThreadLoopObject( const CThreadLoopObject& ref);
	};
}