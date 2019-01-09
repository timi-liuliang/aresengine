#pragma once

#include <Core/AresCore.h>

#ifdef ARES_PLATFORM_WINDOWS
    #include <EGL/egl.h>
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
#else
    #include <ES2/gl.h>
    #include <ES2/glext.h>
#endif

namespace Ares
{
#ifdef ARES_DEBUG
	// µ÷ÊÔ¸¨Öúº¯Êý
	FORCEINLINE void _OGLESDebug()
	{
		GLenum error = glGetError();
		if( error != GL_NO_ERROR)
		{
			int a= 10;
		}
	}

	#define OGLESDebug( gl_function) gl_function; _OGLESDebug()
#else
	#define OGLESDebug( gl_function)
#endif
}
