#pragma once

#include "AresPlatform.h"

namespace Ares
{
	//----------------------------------------------------
	// Singleton 2011-11-18		帝林
	//	摘自 ogre, 缘于boost singleton的不适用
	// template class for creating  single-instance
	// global classes.
	//---------------------------------------------------
	template<typename T>
	class Singleton
	{
	private:
		// 禁用拷贝构造
		Singleton(const Singleton<T>&);

		// 禁用 "="
		Singleton& operator=(const Singleton<T>&);

	public:
		// 构造函数
		Singleton()
		{
			// 两次实例化,断言报错
			assert( !m_singleton);

			m_singleton = static_cast<T*>(this);
		}

		// 析构函数
		virtual ~Singleton()
		{
			assert( m_singleton);

			m_singleton = 0;
		}

		// 是否可用
		static bool IsEnabled()
		{
			return m_singleton ? true : false;
		}

		// 获取单例
		static T& GetSingleton()
		{
			assert( m_singleton);

			return *m_singleton;
		}

		// 获取单例指针
		static T* GetSingletonPtr()
		{
			assert( m_singleton);

			return m_singleton;
		}

		// 返回类名称
		static const char* getClassTypeName()
		{
			return "Singleton";
		}

		// 销毁
		static void Destory()
		{
			SAFE_DELETE( m_singleton);
		}

	protected:
		static T*		m_singleton;			// 单例实例
	};
}