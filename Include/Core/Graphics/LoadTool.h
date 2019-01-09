#pragma once

#include "Core/AresMath.h"

namespace Ares
{
	namespace Cg
	{
		//-----------------------------------------
		// Heap 2012-4-6   帝林
		// Assist adding data to a block of memory
		//-----------------------------------------
		class Heap
		{
		public:
			// 构造函数
			Heap();

			// 析构函数
			~Heap();

		protected:
			UINT		m_size;
			UINT		m_bufferSize;
			BYTE*		m_data;
		};
	}
}