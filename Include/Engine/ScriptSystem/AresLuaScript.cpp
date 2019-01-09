#include "AresLuaScript.h"
#include "lua_AresScriptSystem.h"

namespace Ares
{
	// 构造函数
	LuaScript::LuaScript( const char* filePath)
		: m_filePath( filePath)
	{
		// 新建状态
		m_luaState = luaL_newstate();

		// 导出
		luaL_openlibs( m_luaState);
		//tolua_AresScriptSystem_open( m_luaState);

		// 加载文件 
		luaL_loadfile( m_luaState, filePath);
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

		lua_pcall( m_luaState, 0, 0, 0);
	}
}