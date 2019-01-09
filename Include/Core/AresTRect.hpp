#pragma once

namespace Ares
{
	// Rectangle
	template< typename T> 
	struct TRect
	{
		T left,	top, width, height;

		// 构造函数
		TRect() 
			: left(0), top(0), width(0), height(0)
		{}

		// 构造函数
		TRect( T const& _left, T const& _top, T const& _width, T const& _height)
			: left(_left), top(_top), width(_width), height(_height) 
		{}

		// 获取面积
		T GetArea() const { return width * height; }
	};
}