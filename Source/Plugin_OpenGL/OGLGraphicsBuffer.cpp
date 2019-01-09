#include "OGLGraphicsBuffer.h"
#include "OGLRenderDevice.h"

namespace Ares
{
	// 构造函数
	OGLGraphicsBuffer::OGLGraphicsBuffer( BufferUsage usage, UINT accessHint, GLenum target, const ElementInitData* initData)
		: GraphicsBuffer( usage, accessHint)
		, m_target( target)
	{
		A_ASSERT( (GL_ARRAY_BUFFER==target)||(GL_ELEMENT_ARRAY_BUFFER==target));

		OGLDebug( glGenBuffers( 1, &m_buffer));
		if( initData!=NULL)
		{
			m_sizeInByte = initData->m_rowPitch;

			if( glloader_GL_EXT_direct_state_access)
			{
				glNamedBufferDataEXT( m_buffer, static_cast<GLsizeiptr>(m_sizeInByte), initData->m_data, (BU_Static==m_usage) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
			}
			else
			{
				PDRenderDevice.BindBuffer( m_target, m_buffer);
				OGLDebug( glBufferData( m_target, static_cast<GLsizeiptr>(m_sizeInByte), initData->m_data, (BU_Static==m_usage) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW));
			}
		}
	}

	// 析构函数
	OGLGraphicsBuffer::~OGLGraphicsBuffer()
	{
		PDRenderDevice.DeleteBuffers( 1, &m_buffer);
	}

	// 重置大小
	void OGLGraphicsBuffer::Resize( UINT sizeInByte)
	{
		GraphicsBuffer::Resize( sizeInByte);

		A_ASSERT( m_sizeInByte != 0);

		if( glloader_GL_EXT_direct_state_access())
		{
			glNamedBufferDataEXT( m_buffer, static_cast<GLsizeiptr>(m_sizeInByte), nullptr,(BU_Static == m_usage) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
		}
		else
		{
			PDRenderDevice.BindBuffer( m_target, m_buffer);
			OGLDebug( glBufferData( m_target, static_cast<GLsizeiptr>( m_sizeInByte), NULL, m_usage==BU_Static ? GL_STATIC_DRAW :GL_DYNAMIC_DRAW));
		}
	}

	// Map
	void* OGLGraphicsBuffer::Map( BufferAccess bufAccess)
	{
		GLenum flag = 0;
		switch( bufAccess)
		{
		case BA_ReadOnly:	flag = GL_READ_ONLY; break;
		case BA_WriteOnly:	flag = GL_WRITE_ONLY;break;
		case BA_ReadWrite:	flag = GL_READ_WRITE;break;
		}

		void* ptr;
		if( glloader_GL_EXT_direct_state_access)
		{
			ptr = glMapNamedBufferEXT( m_buffer, flag);
		}
		else
		{
			PDRenderDevice.BindBuffer( m_target, m_buffer);
			ptr = glMapBuffer( m_target, flag);
		}
		
		return ptr;
	}

	// Unmap
	void OGLGraphicsBuffer::Unmap()
	{
		if( glloader_GL_EXT_direct_state_access())
		{
			glUnmapNamedBufferEXT( m_buffer);
		}
		else
		{
			PDRenderDevice.BindBuffer( m_target, m_buffer);
			glUnmapBuffer( m_target);
		}
	}

	// 激活
	void OGLGraphicsBuffer::Active()
	{
		OGLDebug( glBindBuffer( m_target, m_buffer));
	}
}