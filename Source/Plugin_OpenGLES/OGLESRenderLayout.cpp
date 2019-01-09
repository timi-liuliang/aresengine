#include "OGLESRenderLayout.h"
#include "OGLESGraphicsBuffer.h"
#include "OGLESTypedefs.h"
#include "OGLESMapping.h"

namespace Ares
{
	// 激活
	void OGLESRenderLayout::Active( const OGLESRenderMethod& method) const
	{
		for( size_t i=0; i<GetNumVertexStreams(); i++)
		{			
			const OGLESGraphicsBufferPtr& stream			= dynamic_pointer_cast<OGLESGraphicsBuffer>( m_vertexStreams[i].m_stream);
			UINT						  stride			= GetVertexStreamStride( i);
			const VertexElementType&	  vertexStreamFormat= GetVertexStreamFormat( i);

			BYTE* elemoffset = NULL;
			foreach_( const VertexElement& elem, vertexStreamFormat)
			{
				GLint attr = method.GetAttributeLocation( elem.m_usage, elem.m_usageIdx);
				if( attr != INVALID)
				{
					GLvoid*		offset		  = static_cast<GLvoid*>( elemoffset + GetStartVertexLocation() * stride);
					GLint		numComponents = static_cast<GLint>( NumComponents( elem.m_format));

					GLenum		type;
					GLboolean	normalized;
					OGLESMapping::MappingVertexFormat( type, normalized, elem.m_format);
					normalized  = ((VEU_Color == elem.m_usage) && !IsFloatFormat( elem.m_format)) ? GL_TRUE : normalized;

					stream->Active();
					OGLESDebug( glVertexAttribPointer( attr, numComponents, type, normalized, stride, offset));
					OGLESDebug( glEnableVertexAttribArray( attr));
				}

				elemoffset += elem.GetSize();
			}
		}
	}

	// 吊销
	void OGLESRenderLayout::Deactive( const OGLESRenderMethod& method) const
	{
		for ( size_t i=0; i<GetNumVertexStreams(); i++)
		{
			const VertexElementType& vertexStreamFormat = GetVertexStreamFormat( i);
			foreach_( const VertexElement& elem, vertexStreamFormat)
			{
				GLint attribute = method.GetAttributeLocation( elem.m_usage, elem.m_usageIdx);
				if( attribute!=INVALID)
					OGLESDebug( glDisableVertexAttribArray( attribute));
			}
		}
	}

	// 设置激活
	void OGLESRenderLayout::Render( OGLESRenderLayout* preLayout, OGLESRenderMethod* method)
	{
		// 激活顶点流
		Active( *method);

		size_t	vertexCount = GetNumIndices() ? GetNumIndices() : GetNumVertices();
		GLenum	mode;
		UINT	primCount;
		OGLESMapping::Mapping( mode, primCount, *this);

		GLenum	indexType	= GL_UNSIGNED_SHORT;
		BYTE*	indexOffset = NULL;
		if( IsUseIndices())
		{
			OGLESGraphicsBuffer& stream( *static_pointer_cast<OGLESGraphicsBuffer>(GetIndicesStream()));
			stream.Active();

			if( EF_R16UI==m_indexFormat)
			{
				indexType	 = GL_UNSIGNED_SHORT;
				indexOffset += GetStartIndexLocation() * 2;
			}
			else
			{
				indexType	 = GL_UNSIGNED_INT;
				indexOffset += GetStartIndexLocation() * 4;
			}
		}

		if( IsUseIndices())
		{
			OGLESDebug( glDrawElements( mode, /*primCount*/static_cast<GLsizei>( GetNumIndices()), indexType, indexOffset));
		}
		else
		{
			OGLESDebug( glDrawArrays( mode, GetStartVertexLocation(), /*primCount*/static_cast<GLsizei>( GetNumVertices())));
		}

		// 关闭顶点流
		Deactive( *method);
	}
}