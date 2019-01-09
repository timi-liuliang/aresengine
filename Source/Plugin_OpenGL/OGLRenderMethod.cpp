#include "OGLRenderMethod.h"
#include "OGLRenderDevice.h"
#include "OGLMapping.h"
#include <Core/AresKitFunction.h>
#include <boost/typeof/typeof.hpp>
#include <boost/algorithm/string.hpp>

namespace Ares
{
	// 构造函数
	OGLRenderMethod::OGLRenderMethod()
		: RenderMethod()
	{
		m_hasDiscard	   = true;
		m_hasTessellation  = false;
		m_isShaderValidate.assign( true);
	}

	// 析构函数
	OGLRenderMethod::~OGLRenderMethod()
	{

	}

	// 加载 *.hlsl
	bool OGLRenderMethod::Load( const char* fileName, const ShaderMacros& macros)
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
	bool OGLRenderMethod::LoadFromMemory( const void* code, size_t length)
	{
		// 顶点着色器
		m_vertexShader  = CompileToBytecode( code, length, ST_VertexShader);
		if( !m_vertexShader)
			return false;

		// 像素着色器
		m_fragmentShader= CompileToBytecode( code, length, ST_PixelShader);
		if( !m_fragmentShader)
			return false;

		// 着色器集合(Program)
		m_programHandle = OGLDebug( glCreateProgram());
		OGLDebug( glAttachShader( m_programHandle, m_vertexShader));
		OGLDebug( glAttachShader( m_programHandle, m_fragmentShader));
		OGLDebug( glLinkProgram( m_programHandle));

		GLint linkSuccess;
		OGLDebug( glGetProgramiv( m_programHandle, GL_LINK_STATUS, &linkSuccess));
		if( linkSuccess==GL_FALSE)
		{
			char infoLog[1024];
			OGLDebug( glGetProgramInfoLog( m_programHandle, sizeof(infoLog), 0, infoLog));

			PDLogSystem.Error( infoLog);
		}

		// 解析参数
		ParseParameters();

		// 解析默认状态
		ParseStates();

		return m_programHandle ? true : false;
	}

	// 编译
	GLuint OGLRenderMethod::CompileToBytecode( const void* pSrcData, size_t srcDataSize, ShaderType type)
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

		GLuint shaderHandle = OGLDebug( glCreateShader( shaderType));
		const char* shaderCode = (const char*)code.data();
		OGLDebug( glShaderSource( shaderHandle, 1, &shaderCode, NULL));
		OGLDebug( glCompileShader( shaderHandle));

		GLint compileSuccess;
		OGLDebug( glGetShaderiv( shaderHandle, GL_COMPILE_STATUS, &compileSuccess));
		if( compileSuccess==GL_FALSE)
		{
			char errMsg[1024];
			OGLDebug( glGetShaderInfoLog( shaderHandle, sizeof(errMsg), 0, errMsg));

			PDLogSystem.Error( errMsg);
		}

		return shaderHandle;
	}

	// 剖析参数
	void OGLRenderMethod::ParseParameters()
	{
		A_ASSERT( glIsProgram( m_programHandle));

		// uniforms
		GLint activeUniformLength;
		OGLDebug( glGetProgramiv( m_programHandle, GL_ACTIVE_UNIFORMS, &activeUniformLength));
		for( GLint i=0; i<activeUniformLength; i++)
		{
			char   unifromName[512];			
			GLint  uniformSize;
			GLenum uniformType;
			OGLDebug( glGetActiveUniform( m_programHandle, i, 512, NULL, &uniformSize, &uniformType, unifromName));

			GLint  uniformLocation = OGLDebug( glGetUniformLocation( m_programHandle, unifromName));

			GLSLVariablePtr variable = MakeGLSLVariable( unifromName, uniformType, uniformSize, uniformLocation);
			m_varDesc.push_back( variable);

			// 值关系描述
			AddVariable( unifromName, SPT_FloatArray);
		}

		// Attributes
		GLint activeAttributes;
		OGLDebug( glGetProgramiv( m_programHandle, GL_ACTIVE_ATTRIBUTES, &activeAttributes));
		for( GLint i=0; i<activeAttributes; i++)
		{
			char   attributeName[512];
			GLenum attributeType;
			GLint  attributeSize;
			OGLDebug( glGetActiveAttrib( m_programHandle, i, 512, NULL, &attributeSize, &attributeType, attributeName));

			VertexElementUsage usage;
			BYTE			   usageIdx;
			OGLMapping::Mapping( usage, usageIdx, attributeName);
			m_attributeLocations[make_pair( usage, usageIdx)] = OGLDebug( glGetAttribLocation( m_programHandle, attributeName));
		}
	}

	// 剖析状态
	void OGLRenderMethod::ParseStates()
	{
		RasterizerStateDesc		defaultRSDesc;
		DepthStencilStateDesc	defaultDSDesc;
		BlendStateDesc			defaultBSDesc;

		m_rasterState	    = PDRenderDevice.CreateRasterizerState( defaultRSDesc);
		m_depthStencilState = PDRenderDevice.CreateDepthStencilState( defaultDSDesc);
		m_blendState		= PDRenderDevice.CreateBlendState( defaultBSDesc);
	}

	// 绑定
	void OGLRenderMethod::Bind()
	{
		OGLDebug( glUseProgram( m_programHandle));

#if ARES_DEBUG
		OGLDebug( glValidateProgram( m_programHandle));

		GLint validated = false;
		OGLDebug( glGetProgramiv( m_programHandle, GL_VALIDATE_STATUS, &validated));
		if( !validated)
		{
			char infoLog[1024];
			OGLDebug( glGetProgramInfoLog( m_programHandle, sizeof(infoLog), NULL, infoLog));
			PDLogSystem.Error( infoLog);
		}
#endif
	}

	// 应用参数
	void OGLRenderMethod::SetParams(const RenderMaterial& mat)
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
	void OGLRenderMethod::AddVariable( const char* name, ShaderParamType type, ShaderParamPtr& defaultValue)
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
	GLint OGLRenderMethod::GetAttributeLocation( VertexElementUsage usage, BYTE usageIndex) const
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