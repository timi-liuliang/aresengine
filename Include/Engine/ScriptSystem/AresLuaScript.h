#pragma once

#include <string>
#include <Core/AresCore.h>
#include "Engine/Foundation/Ares3DEngine_h.h"
#include "AresLuaArg.h"

using namespace std;

namespace Ares
{
	//-----------------------------------------
	// Lua脚本 2012-8-3  帝林
	//-----------------------------------------
	class ARES_EXPORT LuaScript
	{
		typedef int(*ExportFun)(lua_State*);
	public:
		// 构造函数
		LuaScript( const char* filePath);

		// 析构函数
		~LuaScript();

		// 执行函数(0参)
		void ExecFunc( const char* functionName);

		// 通用执行
		void ExecFunc( const char* functionName, const char*sig, ...);

		// 通用执行
		void ExecFunc( const char* functionName, LuaArgList* args, LuaRVList* rv);

		// 判断函数是否存在
		bool IsFuncExist( const char* functionName) const;

		// 设置变量值
		void SetGlobal( const char* name, void* value, const char* type);

		// 获取LuaState
		lua_State* GetLuaState() { return m_luaState; }

		// 注册导出函数
		static void RegisterExportFun(ExportFun fun) { m_exportFun.push_back( fun); }

	private:
		string					 m_filePath;	// 脚本资源
		lua_State*				 m_luaState;	// 脚本解释器指针
		static vector<ExportFun> m_exportFun;	// 导出函数
	};

	typedef s_ptr<LuaScript> LuaScriptPtr;
}