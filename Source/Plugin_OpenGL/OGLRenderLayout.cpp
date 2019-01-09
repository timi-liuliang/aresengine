#include "OGLRenderLayout.h"
#include "OGLGraphicsBuffer.h"
#include "OGLTypedefs.h"
#include "OGLMapping.h"
#include "OGLRenderDevice.h"

namespace Ares
{
	// 激活
	void OGLRenderLayout::Active( const OGLRenderMethod& method) const
	{
		//UINT maxVertexStreams	 = PDRenderDevice.GetDeviceCaps().m_maxVertexStreams;
		//vector<bool> usedStreams( maxVertexStreams, false);

		for( size_t i=0; i<GetNumVertexStreams(); i++)
		{			
			const OGLGraphicsBufferPtr& stream				= dynamic_pointer_cast<OGLGraphicsBuffer>( m_vertexStreams[i].m_stream);
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
					OGLMapping::MappingVertexFormat( type, normalized, elem.m_format);
					normalized  = ((VEU_Color == elem.m_usage) && !IsFloatFormat( elem.m_format)) ? GL_TRUE : normalized;
					
					stream->Active();
					OGLDebug( glVertexAttribPointer( attr, numComponents, type, normalized, stride, offset));
					OGLDebug( glEnableVertexAttribArray( attr));

					//usedStreams[attr] = true;
				}

				elemoffset += elem.GetSize();
			}
		}


		//for( GLuint i=0; i<maxVertexStreams; i++)
		//{
			//if( !usedStreams[i])
			//	OGLDebug( glDisableVertexAttribArray( i));
		//}
	}

	// 吊销
	void OGLRenderLayout::Deactive( const OGLRenderMethod& method) const
	{
		for ( size_t i=0; i<GetNumVertexStreams(); i++)
		{
			const VertexElementType& vertexStreamFormat = GetVertexStreamFormat( i);
			foreach_( const VertexElement& elem, vertexStreamFormat)
			{
				GLint attribute = method.GetAttributeLocation( elem.m_usage, elem.m_usageIdx);
				if( attribute!=INVALID)
					OGLDebug( glDisableVertexAttribArray( attribute));
			}
		}
	}

	// 设置激活
	void OGLRenderLayout::Render( OGLRenderLayout* preLayout, OGLRenderMethod* method)
	{
		// 激活顶点流
		Active( *method);

		size_t	vertexCount = GetNumIndices() ? GetNumIndices() : GetNumVertices();
		GLenum	mode;
		UINT	primCount;
		OGLMapping::Mapping( mode, primCount, *this);

		GLenum	indexType	= GL_UNSIGNED_SHORT;
		BYTE*	indexOffset = NULL;
		if( IsUseIndices())
		{
			OGLGraphicsBuffer& stream( *static_pointer_cast<OGLGraphicsBuffer>(GetIndicesStream()));
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
			OGLDebug( glDrawElements( mode, static_cast<GLsizei>( GetNumIndices()), indexType, indexOffset));
		}
		else
		{
			OGLDebug( glDrawArrays( mode, GetStartVertexLocation(), static_cast<GLsizei>( GetNumVertices())));
		}

		// 关闭顶点流
		Deactive( *method);
	}
}