#pragma once

#include <vector>
#include "OGLESPlatform.h"
#include <Core/Graphics/AresElementFormat.h>
#include <Core/Graphics/AresGraphicsBuffer.h>

using namespace std;

namespace Ares
{
	//-----------------------------------------
	// OGLESGraphicsBuffer 2013-02-05 帝林
	//-----------------------------------------
	class OGLESGraphicsBuffer : public GraphicsBuffer
	{
	public:
		explicit OGLESGraphicsBuffer( BufferUsage usage, UINT accessHint, GLenum target, const ElementInitData* initData);
		~OGLESGraphicsBuffer();

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
		BufferAccess	m_access;
		vector<BYTE>	m_bufferData;
	};
}