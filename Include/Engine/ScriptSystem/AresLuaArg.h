#pragma once

#include <vector>
#include <tolua++/tolua++.h>

extern "C"
{
#include <tolua++/lua/lualib.h>
};

using namespace std;

namespace Ares
{
	//--------------------------------------
	// 参数基类 2012-8-4 帝林
	//--------------------------------------
	struct LuaArg
	{
		// 插入值
		virtual void PushValue( lua_State* luaState)const=0;
	};

	//--------------------------------------
	// 整形参数 2012-8-4 帝林
	//--------------------------------------
	struct LuaIntArg : public LuaArg
	{
		int		m_value;

		// 构造函数
		explicit LuaIntArg( int v)
			: m_value( v)
		{}

		// 插入值
		virtual void PushValue(lua_State* luaState) const
		{
			lua_pushinteger( luaState, m_value);
		}
	};

	//-----------------------------------
	// 参数列表 2012-8-4 帝林
	//-----------------------------------
	class LuaArgList
	{
	public:
		// 添加Int参数
		void AddArg( int value)
		{
			LuaIntArg* arg = new_ LuaIntArg( value);
			m_args.push_back( arg);
		}

		// PushValue
		void PushValue( lua_State* luaState)
		{
			for ( size_t i=0; i<m_args.size(); i++)
				m_args[i]->PushValue( luaState);
		}

		// 参数数量
		int GetSize() { return (int)m_args.size(); }

	private:
		vector<LuaArg*>		m_args;
	};

	//--------------------------------------
	// 反回值基类 2012-8-4 帝林
	//--------------------------------------
	struct LuaRV
	{
		// 插入值
		virtual void ToValue( lua_State* luaState, int idx)const=0;
	};

	//-----------------------------------
	// 返回值列表 2012-8-4 帝林
	//-----------------------------------
	class LuaRVList
	{
	public:
		// PushValue
		void ToValue( lua_State* luaState)
		{
			for ( int i=1; i<=(int)m_rvs.size(); i++)
				m_rvs[i]->ToValue( luaState, -i);
		}

		// 参数数量
		int GetSize() { return (int)m_rvs.size(); }

	private:
		vector<LuaRV*>	m_rvs;
	};
}