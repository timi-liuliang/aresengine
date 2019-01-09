#pragma once

#include <string>
#include <Core/AresMath.h>
#include "OGLESPlatform.h"

using namespace std;

namespace Ares 
{
	// Variable desc
	struct GLSLVariable
	{
		string		m_name;						// 变量名称
		GLint		m_location;					// 位置
		GLint		m_size;						// 大小
		GLenum		m_type;						// 类型

		// 传入
		virtual void Uniform( const GLfloat* data)=0;
	};
	typedef s_ptr<GLSLVariable> GLSLVariablePtr;

	// VariableDescVec4
	struct GLSLVariableVec4 : public GLSLVariable
	{
		virtual void Uniform( const GLfloat* data)
		{
			OGLESDebug( glUniform4fv( m_location, 1, data));
		}
	};

	// VariableDescVec4
	struct GLSLVariableMat4 : public GLSLVariable
	{
		virtual void Uniform( const GLfloat* data)
		{
			OGLESDebug( glUniformMatrix4fv( m_location, 1, false, data));
		}
	};

	// 构建变量参数
	FORCEINLINE GLSLVariablePtr MakeGLSLVariable( const char* uniformName, GLenum uniformType, GLint uniformSize, GLint uniformLocation)
	{
		A_ASSERT( uniformSize>=1);

		GLSLVariablePtr result; 

		if( uniformSize==1)
		{
			switch( uniformType)
			{
				case GL_FLOAT:  break;
				case GL_FLOAT_VEC2: break;
				case GL_FLOAT_VEC3:break;
				case GL_FLOAT_VEC4: result = MakeSharePtr( new_ GLSLVariableVec4); break;
				case GL_FLOAT_MAT2:break;
				case GL_FLOAT_MAT3:break;
				case GL_FLOAT_MAT4: result = MakeSharePtr( new_ GLSLVariableMat4); break;
			}
		}
		else
		{

		}

		result->m_name	  = uniformName;
		result->m_location= uniformLocation;
		result->m_size	  = uniformSize;
		result->m_type	  = uniformType;

		return result;
	}
}