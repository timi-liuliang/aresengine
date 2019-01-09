#pragma once

#include "SuperMaterialCompiler.h"

namespace Ares
{
	// D3D11MaterialUserInput
	struct MaterialD3D11UserInput
	{
		virtual ~MaterialD3D11UserInput() {}
	};

	// MaterialD3D11UserInputConstant
	struct MaterialD3D11UserInputConstant : public MaterialD3D11UserInput
	{
		float4		m_value;		// 值
		UINT		m_numComponents;// 组件数量

		// 构造函数
		MaterialD3D11UserInputConstant( const float4& value, UINT numComponents)
			: m_value( value), m_numComponents( numComponents)
		{}
	};

	//-------------------------------------------
	// D3DMaterialCompiler 2013-3-11 帝林
	//-------------------------------------------
	class D3D11MaterialCompiler : public MaterialCompiler
	{
	public:
		// 代码块 
		struct ShaderCodeChunk
		{
			string					m_code;		// 代码
			MaterialD3D11UserInput* m_input;	// 输入
			OutputType				m_type;		// 类型

			ShaderCodeChunk( const char* code, OutputType type)
				: m_code( code), m_type( type)
			{}

			ShaderCodeChunk( MaterialD3D11UserInput* input, const char* code, OutputType type)
				: m_input( input), m_code( code), m_type( type)
			{}
		};

	public:
		// 获取块代码
		const char* GetChunkCode( size_t chunkIdx);

		// 获取参数类型
		OutputType GetChunkCodeOutputType( size_t chunkIdx);

		// 常量
		virtual INT Constant( const float4& val, INT numComponents);

		// 纹理采样
		virtual INT TextureSample( int chunkIdx, INT coordinateIdx);

		// 掩码标记
		virtual INT ComponentMask( int chunkIdx, bool maskR, bool maskG, bool maskB, bool maskA);

		// 添加用户输入
		INT AddUserInput( const float4& constant, OutputType type, const char* format, ...);

		// AddCodeChunk - Adds a formatted code string to the code array and returns its index
		INT AddCodeChunk( OutputType type, DWORD flags, const char* format, ...);

	private:
		vector<ShaderCodeChunk>						m_codeChunks;		// 代码块
		vector<MaterialD3D11UserInput*>				m_userInputs;		// 用户输入
		vector<MaterialD3D11UserInputConstant*>		m_userConstants;	// 用户常量
	};
}