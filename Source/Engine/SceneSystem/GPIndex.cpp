#include <Engine/SceneSystem/GPIndex.h>
#include <Core/AresCore.h>

namespace Ares
{
	const CGPIndex CGPIndex::Global = CGPIndex( -65535, -65535, INVALID, INVALID);

	// 构造函数
	CGPIndex::CGPIndex( int x, int y, int width, int height)
		: m_x( x),
		  m_y( y),
		  m_width(  width),
		  m_height( height)
	{

	}

	// 构造函数
	CGPIndex::CGPIndex( const CGPIndex& ref)
		: m_x( ref.m_x),
		  m_y( ref.m_y),
		  m_width(  ref.m_width),
		  m_height( ref.m_height)
	{
		
	}

	// 设置
	void CGPIndex::Set(  int x, int y, int width, int height)
	{
		m_x = x;
		m_y = y;

		m_width  = width;
		m_height = height;
	}

	// 获取索引
	CGPIndex CGPIndex::GetIndex( int x, int y)
	{
		CGPIndex tGPIndex( x, y, m_width, m_height);

		return tGPIndex;
	}

	// 获取X维度向前的第n个GPIndex
	size_t CGPIndex::GetPreviousX( int n)
	{
		return m_x - n;
	}

	// 获取Y维度向前的第n个GPIndex
	size_t CGPIndex::GetPreviousY( int n)
	{
		return m_y - n;
	}

	// 获取X维度向后的第n个GPIndex
	size_t CGPIndex::GetNextX( int n)
	{
		return m_x + n;
	}

	// 获取Y维度向后的第n个GPIndex
	size_t CGPIndex::GetNextY( int n)
	{
		return m_y + n;
	}

	// 重载运算符 "<"
	bool CGPIndex::operator < (const CGPIndex indexRH) const
	{
		if( m_y < indexRH.m_y)
		{
			return true;
		}
		else if( m_y == indexRH.m_y)
		{
			if( m_x < indexRH.m_x)
				return true;

			return false;
		}

		return false;
	}

	// 重载运算符 "=="
	bool CGPIndex::operator == (const CGPIndex indexRH) const
	{
		if( m_x == indexRH.m_x && m_y == indexRH.m_y)
			return true;

		return false;
	}

	// 重载运算符 “<=”
	bool CGPIndex::operator <= (const CGPIndex indexRH) const
	{
		if( (*this) == indexRH || (*this) < indexRH)
			return true;

		return false;
	}
}