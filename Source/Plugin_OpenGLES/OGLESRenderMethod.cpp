#include <fstream>
#include "OGLESRenderMethod.h"
#include "OGLESRenderDevice.h"
#include "OGLESMapping.h"
#include <Core/AresKitFunction.h>
#include <boost/typeof/typeof.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;

namespace Ares
{
	// 构造函数
	OGLESRenderMethod::OGLESRenderMethod()
		: RenderMethod()
	{
		m_hasDiscard	   = true;
		m_hasTessellation  = false;
		m_isShaderValidate.assign( true);
	}
	
	// 析构函数
	OGLESRenderMethod::~OGLESRenderMethod()
	{

	}

	// 加载 *.hlsl
	bool OGLESRenderMethod::Load( const char* fileName, const ShaderMacros& macros)
	{
		RenderMethod::Load( fileName, macros);

		string	 code;
		ifstream ifs( fileName);
		if( ifs.is_open())
		{
			ostringstream oss; oss << ifs.rdbuf();
			code = oss.str();

			m_isEnabled = LoadFromMemory( code.c_str(), code.size());

			return m_isEnabled;
		}

		return false;
	}

	// 从内存加载
	bool OGLESRenderMethod::LoadFromMemory( const void* code_, size_t length)
	{
        string code    = (const char*)code_;
        size_t vsBegin = code.find( "VS_BEGIN");
        size_t vsEnd   = code.find( "VS_END");
        size_t psBegin = code.find( "PS_BEGIN");
        size_t psEnd   = code.find( "PS_END");
        if( !(vsBegin!=string::npos && vsEnd!=string::npos && psBegin!=string::npos && psEnd!=string::npos))
            return false;

        string vsCode = code.substr( vsBegin+8, vsEnd-vsBegin-8);
        string psCode = code.substr( psBegin+8, psEnd-psBegin-8);
        
		// 顶点着色器
		m_vertexShader  = CompileToBytecode( vsCode.c_str(), vsCode.length(), ST_VertexShader);
		if( !m_vertexShader)
			return false;

		// 像素着色器
		m_fragmentShader= CompileToBytecode( psCode.c_str(), psCode.length(), ST_PixelShader);
		if( !m_fragmentShader)
			return false;

		// 着色器集合(Program)
		m_programHandle = OGLESDebug( glCreateProgram());
		OGLESDebug( glAttachShader( m_programHandle, m_vertexShader));
		OGLESDebug( glAttachShader( m_programHandle, m_fragmentShader));
		OGLESDebug( glLinkProgram( m_programHandle));

		GLint linkSuccess;
		OGLESDebug( glGetProgramiv( m_programHandle, GL_LINK_STATUS, &linkSuccess));
		if( linkSuccess==GL_FALSE)
		{
			char infoLog[1024];
			OGLESDebug( glGetProgramInfoLog( m_programHandle, sizeof(infoLog), 0, infoLog));

			PDLogSystem.Error( infoLog);
		}

		// 解析参数
		ParseParameters();

		// 解析默认状态
		ParseStates();

		return m_programHandle ? true : false;
	}

	// 编译
	GLuint OGLESRenderMethod::CompileToBytecode( const void* pSrcData, size_t srcDataSize, ShaderType type)
	{
		GLuint shaderType;
		string code = (const char*)pSrcData;
		switch( type)
		{
		case ST_VertexShader:
			{
				shaderType = GL_VERTEX_SHADER;
				boost::replace_first( code, "VS", "main");
			}
			break;

		case ST_PixelShader:
			{
				shaderType = GL_FRAGMENT_SHADER;
				boost::replace_first( code, "PS", "main");
			}
			break;
		}

		GLuint shaderHandle = OGLESDebug( glCreateShader( shaderType));
		const char* shaderCode = (const char*)code.data();
		OGLESDebug( glShaderSource( shaderHandle, 1, &shaderCode, NULL));
		OGLESDebug( glCompileShader( shaderHandle));

		GLint compileSuccess;
		OGLESDebug( glGetShaderiv( shaderHandle, GL_COMPILE_STATUS, &compileSuccess));
		if( compileSuccess==GL_FALSE)
		{
			char errMsg[1024];
			OGLESDebug( glGetShaderInfoLog( shaderHandle, sizeof(errMsg), 0, errMsg));

			PDLogSystem.Error( errMsg);
		}

		return shaderHandle;
	}

	// 剖析参数
	void OGLESRenderMethod::ParseParameters()
	{
		A_ASSERT( glIsProgram( m_programHandle));

		// uniforms
		GLint activeUniformLength;
		OGLESDebug( glGetProgramiv( m_programHandle, GL_ACTIVE_UNIFORMS, &activeUniformLength));
		for( GLint i=0; i<activeUniformLength; i++)
		{
			char   unifromName[512];			
			GLint  uniformSize;
			GLenum uniformType;
			OGLESDebug( glGetActiveUniform( m_programHandle, i, 512, NULL, &uniformSize, &uniformType, unifromName));

			GLint  uniformLocation = OGLESDebug( glGetUniformLocation( m_programHandle, unifromName));

			GLSLVariablePtr variable = MakeGLSLVariable( unifromName, uniformType, uniformSize, uniformLocation);
			m_varDesc.push_back( variable);

			// 值关系描述
            ShaderParamPtr defaultShaderParam = MakeShaderParam();
			AddVariable( unifromName, SPT_FloatArray, defaultShaderParam);
		}

		// Attributes
		GLint activeAttributes;
		OGLESDebug( glGetProgramiv( m_programHandle, GL_ACTIVE_ATTRIBUTES, &activeAttributes));
		for( GLint i=0; i<activeAttributes; i++)
		{
			char   attributeName[512];
			GLenum attributeType;
			GLint  attributeSize;
			OGLESDebug( glGetActiveAttrib( m_programHandle, i, 512, NULL, &attributeSize, &attributeType, attributeName));

			VertexElementUsage usage;
			BYTE			   usageIdx;
			OGLESMapping::Mapping( usage, usageIdx, attributeName);
			m_attributeLocations[make_pair( usage, usageIdx)] = OGLESDebug( glGetAttribLocation( m_programHandle, attributeName));
		}
	}

	// 剖析状态
	void OGLESRenderMethod::ParseStates()
	{
		RasterizerStateDesc		defaultRSDesc;
		DepthStencilStateDesc	defaultDSDesc;
		BlendStateDesc			defaultBSDesc;

		m_rasterState	    = PDRenderDevice.CreateRasterizerState( defaultRSDesc);
		m_depthStencilState = PDRenderDevice.CreateDepthStencilState( defaultDSDesc);
		m_blendState		= PDRenderDevice.CreateBlendState( defaultBSDesc);
	}

	// 绑定
	void OGLESRenderMethod::Bind()
	{
		OGLESDebug( glUseProgram( m_programHandle));

#if 1
		OGLESDebug( glValidateProgram( m_programHandle));

		GLint validated = false;
		OGLESDebug( glGetProgramiv( m_programHandle, GL_VALIDATE_STATUS, &validated));
		if( !validated)
		{
			char infoLog[1024];
			OGLESDebug( glGetProgramInfoLog( m_programHandle, sizeof(infoLog), NULL, infoLog));
			PDLogSystem.Error( infoLog);
		}
#endif
	}

	// 应用参数
	void OGLESRenderMethod::SetParams(const RenderMaterial& mat)
	{
		// 常量
		for ( size_t i=0; i<m_varDesc.size(); i++)
		{
			const ShaderParamPtr& pParam = mat.GetShaderParam( m_varDesc[i]->m_name);
			if( pParam)
			{
				if( pParam->IsType( SPT_FloatArray))
				{
					m_varDesc[i]->Uniform( (const GLfloat*)pParam->m_data);
				}
			}
		}
	}

	// 向接口中添加变量
	void OGLESRenderMethod::AddVariable( const char* name, ShaderParamType type, ShaderParamPtr& defaultValue)
	{
		// 值关联描述
		ShaderParamDesc paramDesc;
		paramDesc.m_name = name;
		paramDesc.m_type = type;
		if( std::find( m_paramDescs.begin(), m_paramDescs.end(), paramDesc)==m_paramDescs.end())
		{
			m_paramDescs.push_back( paramDesc);
			m_curParamValues.push_back( defaultValue);
		}
	}

	// 获取AttributeLocation
	GLint OGLESRenderMethod::GetAttributeLocation( VertexElementUsage usage, BYTE usageIndex) const
	{
		BOOST_AUTO( iter, m_attributeLocations.find( make_pair( usage, usageIndex)));
		if( iter!=m_attributeLocations.end())
		{
			return iter->second;
		}
		else
		{
			return INVALID;
		}
	}
}