#pragma once

#include <Core/AresCore.h>

#ifdef ARES_PLATFORM_WINDOWS
#include "glloader/glloader.h"
#endif

namespace Ares
{
	// µ÷ÊÔ¸¨Öúº¯Êý
	FORCEINLINE void _OGLDebug()
	{
#ifdef ARES_DEBUG_NULL
		GLenum error = glGetError();
		if( error != GL_NO_ERROR)
		{
			int a= 10;
		}
#endif
	}

#define OGLDebug( gl_function) gl_function; _OGLDebug()
}
