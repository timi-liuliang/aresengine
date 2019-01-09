#pragma once

#include "AresDynLib.h"
#include "Core/AresPlatform.h"

using namespace std;

namespace Ares
{
	//---------------------------------------
	// AresDynLibManager 2011-08-01  帝林
	//---------------------------------------
	class CDynLibManager
	{
		typedef map<string, CDynLib*> DynLibList;
	public:
		// 构造函数
		CDynLibManager();

		// 析构函数
		~CDynLibManager();

		// 加载库
		CDynLib* Load( const char* fileName);

		// 卸载库
		void Unload( CDynLib* lib);

		// 获取库
		CDynLib* GetDynLib( const char* fileName);

	private:
		DynLibList		m_libList;			// 库列表
	};
}