#pragma once

#include <map>
#include "OGLESPlatform.h"
#include <Core/Graphics/AresRenderMethod.h>
#include "OGLESShaderVariable.h"

using namespace std;

namespace Ares
{
	//------------------------------------
	// OGLESRenderMethod 2013-4-3 帝林
	//------------------------------------
	class OGLESRenderMethod : public RenderMethod
	{
	public:
		OGLESRenderMethod();
		virtual ~OGLESRenderMethod();

		// 加载 *.hlsl
		virtual bool Load( const char* fileName, const ShaderMacros& macros);

		// 从内存加载
		virtual bool LoadFromMemory( const void* code, size_t length);

		// 绑定
		virtual void Bind();

		// 应用参数
		virtual void SetParams(const RenderMaterial& mat);

		// 获取AttributeLocation
		GLint GetAttributeLocation( VertexElementUsage usage, BYTE usageIndex) const;

	private:
		// 编译
		GLuint CompileToBytecode( const void* pSrcData, size_t srcDataSize, ShaderType type);

		// 剖析参数
		void ParseParameters();

		// 剖析状态
		void ParseStates();

		// 向接口中添加变量
		void AddVariable( const char* name, ShaderParamType type, ShaderParamPtr& defaultValue);

	private:
		GLuint				m_programHandle;
		GLuint				m_vertexShader;
		GLuint				m_fragmentShader;

		vector<GLSLVariablePtr>						m_varDesc;				// 描述信息
		map<pair<VertexElementUsage, BYTE>, GLint>	m_attributeLocations;	
	};
}