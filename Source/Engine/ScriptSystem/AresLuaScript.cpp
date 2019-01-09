#include <Engine/ScriptSystem/AresLuaScript.h>
#include <stdarg.h>

namespace Ares
{
	vector<LuaScript::ExportFun> LuaScript::m_exportFun;	// 导出函数

	// 构造函数
	LuaScript::LuaScript( const char* filePath)
		: m_filePath( filePath)
	{
		// 新建解释器
		m_luaState = luaL_newstate();

		// 导出
		luaopen_base( m_luaState);

		for( size_t i=0; i<m_exportFun.size(); i++)
			(*m_exportFun[i])( m_luaState);

		// 加载源码到内存,然后编译,压栈
		luaL_dofile( m_luaState, filePath);
	}

	// 析构函数
	LuaScript::~LuaScript()
	{
		lua_close( m_luaState);
	}

	// 调用函数(无参)
	void LuaScript::ExecFunc( const char* functionName)
	{
		lua_getglobal( m_luaState, functionName);
	
		int result = lua_pcall( m_luaState, 0, 0, 0);
		if( result)
		{
			string errMsg = lua_tostring( m_luaState, -1);
			
			lua_pop( m_luaState, 1);
		}
	}

	// 通用调用函数
	void LuaScript::ExecFunc( const char* functionName, const char*sig, ...)
	{
		int narg=0;
		int nres=0;

		va_list vl;
		va_start( vl, sig);

		// 压入函数
		lua_getglobal( m_luaState, functionName);	

		// 压入参数
		
		while( *sig)
		{
			// 检查栈中空间
			luaL_checkstack( m_luaState, 1, "too many arguments");

			switch( *sig++)
			{
			case 'd':
					lua_pushnumber( m_luaState, va_arg( vl, double));
				break;

			case 'i':
					lua_pushinteger( m_luaState, va_arg( vl, int));
				break;

			case 's':
					lua_pushstring( m_luaState, va_arg( vl, char*));
				break;

			case 'u':
				{
					void*       value = va_arg( vl, void*); 
					const char* type  = va_arg( vl, const char*);
	
					tolua_pushusertype( m_luaState, value, type);
				}
				break;

			case ';':
				goto EndArgs;
			}

			narg++;
		}
	
	EndArgs:
		// 返回值数量
		nres = strlen( sig);

		if( lua_pcall( m_luaState, narg, nres, 0) != 0)
		{
			// 错误处理
		}

		// 检索结果
		nres = -nres;
		while( *sig)
		{
			switch( *sig++)
			{
			case 'd':
					*va_arg(vl, double*) = lua_tonumber( m_luaState, nres);
				break;

			case 'i':
					*va_arg( vl, int*) = lua_tointeger( m_luaState, nres);
				break;

			case 's':
					*va_arg( vl, const char**) = lua_tostring( m_luaState, nres);
				break;
			}

			nres++;
		}

		va_end( vl);
	}

	// 通用执行
	void LuaScript::ExecFunc( const char* functionName, LuaArgList* args, LuaRVList* rv)
	{
		int narg = args ? args->GetSize() : 0;
		int nres = rv   ? rv->GetSize()   : 0;

		// 压入函数
		lua_getglobal( m_luaState, functionName);

		if( args)
			args->PushValue( m_luaState);

		if( lua_pcall( m_luaState, narg, nres, 0) == 0)
		{
			if( rv)
				rv->ToValue( m_luaState);
		}
	}

	// 判断函数是否存在
	bool LuaScript::IsFuncExist( const char* functionName) const
	{
		bool result;

		lua_getglobal( m_luaState, functionName);
		result = !lua_isnil( m_luaState, -1) && lua_isfunction( m_luaState, -1);
		lua_pop( m_luaState, 1);

		return result;
	}

	// 设置全局变量值
	void LuaScript::SetGlobal( const char* name, void* value, const char* type)
	{
		tolua_pushusertype( m_luaState, value, type);
		lua_setglobal( m_luaState, name);
	}
}