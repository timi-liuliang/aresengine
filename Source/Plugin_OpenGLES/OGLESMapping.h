#pragma once

#include "OGLESPlatform.h"
#include <Core/Graphics/AresElementFormat.h>

namespace Ares
{
	//--------------------------------------
	// OpenGL 映射 2013-4-7 帝林
	//--------------------------------------
	class OGLESMapping
	{
	public:
		// 映射格式
		static void MappingFormat( GLint& internalFormat, GLenum& glformat, GLenum& gltype, ElementFormat ef);

		// 映射数据类型大小
		static int MappingDataTypeSize( GLenum type);

		// 映射顶点格式
		static void MappingVertexFormat( GLenum& gltype, GLboolean& normalized, ElementFormat ef);

		// 映射
		static void Mapping( GLenum& primType, UINT& primCount, const RenderLayout& layout);

		// 映射Attribute
		static void Mapping( VertexElementUsage& usage, BYTE& usageIdx, const char* attributeName);
	};
}