#pragma once

#include <Core/AresCore.h>

namespace Ares
{
	//----------------------------------
	// GP索引 2011-08-17  帝林
	//----------------------------------
	class CGPIndex
	{
	public:
		static const CGPIndex Global;

	public:
		CGPIndex() {}
		CGPIndex( int x, int y, int width, int height);
		CGPIndex( const CGPIndex& ref);
		~CGPIndex(){}

	public:
		// 设置
		void Set(  int x, int y, int width, int height);

		// 设置X位置
		void SetX( int x) { m_x = x; }

		// 设置Y位置
		void SetY( int y) { m_y = y; }

		// 设置宽
		void SetWidth( int width) { m_width = width; }

		// 设置高
		void SetHeight( int height) { m_height = height; }

	public:
		// 获取X位置
		int GetX() const { return m_x; }

		// 获取Y位置
		int GetY() const { return m_y; }

		// 获取宽
		int GetWidth() const { return m_width; }

		// 获取高
		int GetHeight() const { return m_height; }

	public:
		// 获取索引
		CGPIndex GetIndex( int x, int y);

		// 获取X维度向前的第n个GPIndex
		size_t GetPreviousX( int n);

		// 获取Y维度向前的第n个GPIndex
		size_t GetPreviousY( int n);

		// 获取X维度向后的第n个GPIndex
		size_t GetNextX( int n);

		// 获取Y维度向后的第n个GPIndex
		size_t GetNextY( int n);

	public:
		// 获取维度(当前为2维)
		int GetDimensions() { return 2; }

	public:
		// 重载运算符 "<"
		bool operator < (const CGPIndex indexRH) const;

		// 重载运算符 "=="
		bool operator == (const CGPIndex indexRH) const;

		// 重载运算符 “<=”
		bool operator <= (const CGPIndex indexRH) const;

		// 序列化
		template <typename Archive> void serialize( Archive& ar, const unsigned int version)
		{
			ar & m_x & m_y & m_width & m_height;
		}

	private:
		int		m_x;
		int		m_y;
		int		m_width;
		int		m_height;
	};
}