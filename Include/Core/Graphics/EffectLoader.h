#pragma once

#include <vector>
#include <string>
#include "LoadTool.h"
#include "Core/AresPlatform.h"

using namespace std;

namespace Ares
{
	//----------------------------------
	// EffectLoad 2012-4-5 帝林
	//----------------------------------
	class CgEffectLoader
	{
	public:
		// 构造函数
		CgEffectLoader( const char* fileName);

		// 构造函数
		CgEffectLoader( const void* effectBuffer, size_t size);

		// 析构函数
		~CgEffectLoader();

		// 剖析
		bool Parser();

	private:
		vector<string>		m_errors;			// 错误与警告	

		BYTE*				m_data;				// 被解析数据
		UINT				m_dataSize;			// size of data buffer in bytes

		Cg::Heap*		m_reflection;		// Reflection object

	};
}