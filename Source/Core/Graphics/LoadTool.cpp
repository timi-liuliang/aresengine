#include "Core/Graphics/LoadTool.h"

namespace Ares
{
	namespace Cg
	{
		// 构造函数
		Heap::Heap()
		{
			m_data		 = NULL;
			m_size		 = 0;
			m_bufferSize = 0;
		}

		// 析构函数
		Heap::~Heap()
		{
			SAFE_DELETE_ARRAY( m_data);
		}
	}
}