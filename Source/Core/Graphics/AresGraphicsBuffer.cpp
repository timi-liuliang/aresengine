#include "Core/Graphics/AresGraphicsBuffer.h"

namespace Ares
{
	// 构造函数
	GraphicsBuffer::GraphicsBuffer( BufferUsage usage, UINT accessHint)
		: m_usage( usage),
		  m_accessHint( accessHint),
		  m_sizeInByte( 0)
	{

	}

	// 析构函数
	GraphicsBuffer::~GraphicsBuffer()
	{

	}
}