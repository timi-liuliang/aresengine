#include "OGLESGraphicsBuffer.h"
#include "OGLESRenderDevice.h"

namespace Ares
{
	// 构造函数
	OGLESGraphicsBuffer::OGLESGraphicsBuffer( BufferUsage usage, UINT accessHint, GLenum target, const ElementInitData* initData)
		: GraphicsBuffer( usage, accessHint)
		, m_target( target)
	{
		A_ASSERT( (GL_ARRAY_BUFFER==target)||(GL_ELEMENT_ARRAY_BUFFER==target));

		OGLESDebug( glGenBuffers( 1, &m_buffer));
		if( initData!=NULL)
		{
			m_sizeInByte = initData->m_rowPitch;

			PDRenderDevice.BindBuffer( m_target, m_buffer);
			OGLESDebug( glBufferData( m_target, static_cast<GLsizeiptr>(m_sizeInByte), initData->m_data, (BU_Static==m_usage) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW));

			m_bufferData.assign( static_cast<const BYTE*>(initData->m_data), static_cast<const BYTE*>(initData->m_data) + m_sizeInByte);
		}
	}

	// 析构函数
	OGLESGraphicsBuffer::~OGLESGraphicsBuffer()
	{
		PDRenderDevice.DeleteBuffers( 1, &m_buffer);
	}

	// 重置大小
	void OGLESGraphicsBuffer::Resize( UINT sizeInByte)
	{
		GraphicsBuffer::Resize( sizeInByte);

		A_ASSERT( m_sizeInByte != 0);

		PDRenderDevice.BindBuffer( m_target, m_buffer);
		OGLESDebug( glBufferData( m_target, static_cast<GLsizeiptr>( m_sizeInByte), NULL, m_usage==BU_Static ? GL_STATIC_DRAW :GL_DYNAMIC_DRAW));

		m_bufferData.resize( m_sizeInByte);
	}

	// Map
	void* OGLESGraphicsBuffer::Map( BufferAccess bufAccess)
	{
		m_access = bufAccess;
		if( bufAccess==BA_WriteOnly)
			return m_bufferData.data();

		return m_bufferData.data();
	}

	// Unmap
	void OGLESGraphicsBuffer::Unmap()
	{
		switch( m_access)
		{
		case BA_WriteOnly:
		case BA_ReadWrite:
			{
				PDRenderDevice.BindBuffer( m_target, m_buffer);

				OGLESDebug( glBufferSubData( m_target, 0, static_cast<GLsizeiptr>( m_sizeInByte), m_bufferData.data()));
			}
			break;

		default:
			A_ASSERT( FALSE);
			break;
		}
	}

	// 激活
	void OGLESGraphicsBuffer::Active()
	{
		OGLESDebug( glBindBuffer( m_target, m_buffer));
	}
}