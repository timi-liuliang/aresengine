#pragma once

#include "OGLPlatform.h"
#include <Core/Graphics/AresElementFormat.h>

namespace Ares
{
	//--------------------------------------
	// OpenGL ”≥…‰ 2013-4-7 µ€¡÷
	//--------------------------------------
	class OGLMapping
	{
	public:
		// ”≥…‰∏Ò Ω
		static void MappingFormat( GLint& internalFormat, GLenum& glformat, GLenum& gltype, ElementFormat ef);

		// ”≥…‰∂•µ„∏Ò Ω
		static void MappingVertexFormat( GLenum& gltype, GLboolean& normalized, ElementFormat ef);

		// ”≥…‰
		static void Mapping( GLenum& primType, UINT& primCount, const RenderLayout& layout);

		// ”≥…‰Attribute
		static void Mapping( VertexElementUsage& usage, BYTE& usageIdx, const char* attributeName);
	};
}