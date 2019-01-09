#pragma once

#include "OGLPlatform.h"
#include <Core/Graphics/AresElementFormat.h>
#include <Core/Graphics/AresGraphicsBuffer.h>

namespace Ares
{
	//-----------------------------------------
	// OGLESGraphicsBuffer 2013-02-05 帝林
	//-----------------------------------------
	class OGLGraphicsBuffer : public GraphicsBuffer
	{
	public:
		explicit OGLGraphicsBuffer( BufferUsage usage, UINT accessHint, GLenum target, const ElementInitData* initData);
		~OGLGraphicsBuffer();

		// 重置大小
		virtual void Resize( UINT sizeInByte);

		// 激活
		void Active();

	private:
		// Map
		virtual void* Map( BufferAccess bufAccess);

		// Unmap
		virtual void Unmap();

	private:
		GLuint			m_buffer;
		GLenum			m_target;
	};
}