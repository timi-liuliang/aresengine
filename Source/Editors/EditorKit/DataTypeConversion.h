#pragma once

#include <string>
#include <Core/AresMath.h>

using namespace std;

namespace Ares
{
	//------------------------------------
	// 数据类型转换 2013-1-23 帝林
	//     方便编辑器开发
	//------------------------------------
	namespace DTC
	{
		// Vector3 转 string
		string Vector3ToString( const Vector3& vec);

		// string 转 Vector3
		Vector3 StringToVector3( const string& str);

		// ColorRGB 转 string
		string ColorRGBToString( const ColorRGB& color);

		// string 转 ColorRGB
		ColorRGB StringToColorRGB( const string& str);
	}

}