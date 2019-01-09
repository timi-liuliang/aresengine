#pragma  once

#include <Core/AresMath.h>

namespace Ares
{
	// quad_tree_rect
	class quad_tree_rect
	{
	public:
		// 构造函数
		quad_tree_rect() 
		{
			x0 =  FLT_MAX; x1 = -FLT_MAX; y0 =  FLT_MAX; y1 = -FLT_MAX;
		}

		// 运算符重载 "()"
		quad_tree_rect(const quad_tree_rect& src)
		{
			x0 = src.x0; x1 = src.x1; y0 = src.y0; y1 = src.y1;
		}

		quad_tree_rect(float _x0, float _x1, float _y0, float _y1)
		{
			x0 = _x0; x1 = _x1; y0 = _y0; y1 = _y1;
		}

		// 析构函数
		~quad_tree_rect() {}

		//// 重载运算符 "="
		//FORCEINLINE void operator = ( const Rect2& r2)
		//{
		//	x0 = r2.x0; x1=r2.x1; y0=r2.y0; y1=r2.y1;
		//}

		// 重载运算符 "+="
		inline const quad_tree_rect& operator += ( const Vector3& vec)
		{
			x0 += vec[0]; x1 += vec[0]; y0 += vec[1]; y1 += vec[1];

			return *this;
		}

		// 重载运算符 "*="
		inline const quad_tree_rect& operator *=(const Vector3& vec)
		{
			x0 *= vec[0]; x1 *= vec[0]; y0 *= vec[1]; y1 *= vec[1];

			return *this;
		}

		// 合并点
		inline void union_point(float v0, float v1)
		{
			x0 = (std::min)( x0, v0);
			y0 = (std::min)( y0, v1);

			x1 = (std::max)( x1, v0);
			y1 = (std::max)( y1, v1);
		}

	public:
		float	x0, y0, x1, y1;		// 范围
	};

	// quad_tree_byte_rect 
	class quad_tree_byte_rect
	{
	public:
		// 构造函数
		quad_tree_byte_rect() {}

		// 析构函数
		~quad_tree_byte_rect() {}

		// 运算符重载 "()"
		quad_tree_byte_rect(const quad_tree_byte_rect& src)
		{
			x0 = src.x0;
			x1 = src.x1;
			y0 = src.y0;
			y1 = src.y1;
		}

		quad_tree_byte_rect(int _x0, int _x1, int _y0, int _y1)
		{
			x0 = _x0;
			x1 = _x1;
			y0 = _y0;
			y1 = _y1;
		}

		void convert(const quad_tree_rect& worldRect, const Vector3& offset, const Vector3& scale)
		{
			quad_tree_rect convertedRect(worldRect);

			// reposition and scale world coordinates to quad tree coordinates
			convertedRect += offset;
			convertedRect *= scale;

			// reduce by a tiny amount to handle tiled data
			convertedRect.x1 -= 0.01f;
			convertedRect.y1 -= 0.01f;

			convertedRect.x1 = (std::max)(convertedRect.x0, convertedRect.x1);
			convertedRect.y1 = (std::max)(convertedRect.y0, convertedRect.y1);

			// convert to integer values, taking the floor of each real
			x0 = static_cast<int>(std::floor(convertedRect.x0));
			x1 = static_cast<int>(std::floor(convertedRect.x1));
			y0 = static_cast<int>(std::floor(convertedRect.y0));
			y1 = static_cast<int>(std::floor(convertedRect.y1));

			// must be position
			x0 = clamp(x0, 0, 65534);
			y0 = clamp(y0, 0, 65534);

			// we must at least one unit large
			x1 = clamp(x1, x0+1, 65535);
			y1 = clamp(y1, y0+1, 65535);
		}

	public:
		int  x0;
		int  x1;
		int  y0;
		int  y1;
	};

	//-----------------------------------------------------------------
	// Tree Member 2010-03-28 15:51 --  未知    帝林 
	//-----------------------------------------------------------------
	template<typename T>
	class quad_tree_member
	{
	public:
		// 默认构造函数
		quad_tree_member();

		// 构造函数
		quad_tree_member( T _value, quad_tree_rect& _key);

		// ...
		quad_tree_member* rear_tree_link() { return m_pRearTreeLink; }

		//... 
		quad_tree_member* forward_tree_link() { return m_pForwardTreeLink; }

		//...
		quad_tree_member* rear_search_link() { return m_pRearSearchLink; }

		//...
		quad_tree_member* forward_search_link() { return m_pForwardSearchLink; }

		// 设置后置树链接
		void   set_rear_tree_link(quad_tree_member* pRearLink){ m_pRearTreeLink = pRearLink; }

		// 设置前置树链接
		void   set_forward_tree_link(quad_tree_member* pForwardLink) { m_pForwardTreeLink = pForwardLink; }

		// 设置后置树链接
		void   set_rear_search_link(quad_tree_member* pRearSearchLink){ m_pRearSearchLink = pRearSearchLink; }

		// 设置前置树链接
		void   set_forward_search_link(quad_tree_member* pForwardSearchLink) { m_pForwardSearchLink = pForwardSearchLink; }

	public:
		quad_tree_rect		key;
		T					value;							// 数据	
		quad_tree_member*	m_pForwardTreeLink;
		quad_tree_member*	m_pRearTreeLink;
		quad_tree_member*	m_pForwardSearchLink;
		quad_tree_member*	m_pRearSearchLink;
	};

	// inline

	// 默认构造函数
	template<typename T>
	INLINE quad_tree_member<T>::quad_tree_member()
	{
		m_pForwardTreeLink   = NULL;
		m_pRearTreeLink      = NULL;
	}

	template<typename T>
	inline quad_tree_member<T>::quad_tree_member( T _value, quad_tree_rect& _key)
	{
		value = _value;
		key   = _key;

		m_pForwardTreeLink   = NULL;
		m_pRearTreeLink      = NULL;
	}

	// HighestBitSet
	template<class T>
	inline int highest_bit_set(T input)
	{
		register int result;

#ifdef ARES_PLATFORM_IOS

#elif defined ARES_PLATFORM_APPLE
        asm bsr eax, input
		asm mov result, eax
#elif defined ARES_PLATFORM_WINDOWS
		_asm bsr eax, input
		_asm mov result, eax
#endif

		return result;
	}
}
