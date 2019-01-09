#pragma once

#include <boost/thread.hpp>

namespace Ares
{
	////-----------------------------------------
	//// 封装临界区  2011-04-14  帝林
	////-----------------------------------------
	//class CriticalSection
	//{
	//public:
	//	// 临界区操作者
	//	class CHolder
	//	{
	//	public:
	//		// 构造
	//		explicit CHolder( CriticalSection& cs)
	//			: m_crit(cs)
	//		{
	//			m_crit.Enter();
	//		}

	//		// 析构
	//		~CHolder()
	//		{
	//			m_crit.Leave();
	//		}

	//	private:
	//		CriticalSection&	m_crit;

	//		// 禁用复制构造函数( 不实现)
	//		CHolder( const CHolder& cowner);
	//		CHolder & operator =( const CHolder& cowner);
	//	};

	//	// 构造函数
	//	CriticalSection()
	//	{
	//		InitializeCriticalSection( &m_criticalSection);
	//	}

	//	// 析构函数
	//	~CriticalSection()
	//	{
	//		DeleteCriticalSection( &m_criticalSection);
	//	}

	//	// 标识临界区
	//	void Enter()
	//	{
	//		EnterCriticalSection( &m_criticalSection);
	//	}

	//	// 退出临界区 
	//	void Leave() 
	//	{
	//		LeaveCriticalSection( &m_criticalSection);
	//	}

	//private:
	//	CRITICAL_SECTION	m_criticalSection;

	//	// 禁用复制构造函数( 不要实现)
	//	CriticalSection( const CriticalSection& cs);
	//	CriticalSection &operator = ( const CriticalSection& cs);
	//};

	#define ENTER_CRITICAL_SECTION( locker, cs) boost::recursive_mutex::scoped_lock locker(cs)
}