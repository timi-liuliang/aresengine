#pragma once

#include <vector>
#include <boost/any.hpp>

using namespace std;
using namespace boost;

namespace Ares
{
	//-----------------------------------------
	// 任意参数类型  2010-11-19 thanks boost
	//-----------------------------------------
	typedef vector<any> AresAnyParams;

	//-----------------------------------------
	// 常规类成员函数回调 2010-11-19  帝林
	// 摘自 <<CEGUI.CGGUIMemberFunctionSlot.h>>
	//-----------------------------------------
	template< typename T, typename R>
	class CMemFunGeneralSlot
	{
		typedef R( T::*MemberFunctionType)( AresAnyParams);
	public:
		// 构造函数
		CMemFunGeneralSlot( MemberFunctionType func, T* obj):
			m_function( func),
			m_pObject( obj)
		{}

		// () 重载
		bool operator()( AresAnyParams argv){
			return (m_pObject->*m_function)( argv);
		}
		
	private:
		MemberFunctionType  m_function;		// 成员函数
		T*                  m_pObject;		// 类指针
	};

	//-----------------------------------------
	// 无参成员函数回调 2010-11-19  帝林
	//-----------------------------------------
	template< typename T, typename R>
	class CMemFun0ParamSlot
	{
		typedef R( T::*MemberFunctionType)();
	public:
		// 构造函数
		CMemFun0ParamSlot( MemberFunctionType func, T* obj):
		  m_function( func),
			  m_pObject( obj)
		  {}

		  // () 重载
		  bool operator()(){
			  return (m_pObject->*m_function)();
		  }

	private:
		MemberFunctionType  m_function;		// 成员函数
		T*                  m_pObject;		// 类指针
	};
}