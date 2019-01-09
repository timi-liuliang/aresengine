#pragma once

#include <string>

using namespace std;

namespace Ares
{
	//------------------------------------------
	// fwrites  2010-07-20  帝林
	//------------------------------------------
	void fwrites( const char* res, FILE* fileHandle);

	//----------------------------------------------
	// freads  2010-07-20  帝林
	//----------------------------------------------
	// 效率原因：只能读取小于1024字节字符串
	// 效率原因：使用string 而非char* c = new char[]
	bool freads( string& _s, FILE* fileHandle);
	bool freads( char*& _s, FILE* fileHandle);

	// 从全局路径中拆分出纹理名,可更改后缀名
	string splitNameExt( const char* fullPath, const char* aimExt=NULL);

	// 更换文件名
	void changeNameExt( char* pdst, const char* source, const char* nameExt);

	// 更换后缀名
	void changeExt( char* pdst, const char* source, const char* ext);
}