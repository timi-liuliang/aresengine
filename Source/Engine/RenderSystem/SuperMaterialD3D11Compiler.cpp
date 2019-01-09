#include <Engine/RenderSystem/SuperMaterialD3D11Compiler.h>
#include <Engine/AresRoot.h>

namespace Ares
{
	// 获取参数类型
	OutputType D3D11MaterialCompiler::GetChunkCodeOutputType( size_t chunkIdx)
	{
		A_ASSERT( chunkIdx<m_codeChunks.size());

		return m_codeChunks[chunkIdx].m_type;
	}

	// 获取块代码
	const char* D3D11MaterialCompiler::GetChunkCode( size_t chunkIdx)
	{
		A_ASSERT( chunkIdx<m_codeChunks.size());
		//if( !m_codeChunks[chunkIdx].m_input || m_codeChunks[chunkIdx].m_input->ShouldEmbedCode())
		//	return m_codeChunks[chunkIdx].m_code.c_str();
		//else
		return m_codeChunks[chunkIdx].m_code.c_str();

	}

	// 常量
	INT D3D11MaterialCompiler::Constant( const float4& val, INT numComponents)
	{
		switch( numComponents)
		{
		case 1: return AddUserInput( val, OPT_Float1, "%f", val.x);
		case 2: return AddUserInput( val, OPT_Float2, "float2( %f, %f)", val.x, val.y);
		case 3: return AddUserInput( val, OPT_Float3, "float3( %f, %f, %f)", val.x, val.y, val.z);
		case 4: return AddUserInput( val, OPT_Float4, "float4( %f, %f, %f, %f)", val.x, val.y, val.z, val.w);
		default: A_ASSERT( false); break;
		}

		return INVALID;
	}

	// 纹理采样
	INT D3D11MaterialCompiler::TextureSample( int chunkIdx, INT coordinateIdx)
	{
		OutputType chunkType = GetChunkCodeOutputType( chunkIdx);
		string	   sampleCode;
		switch( chunkType)
		{
		case OPT_Texture2D:
			sampleCode = "%s.Sample( WrapSampler, %s)";
			break;
		}

		switch( chunkType)
		{
		case OPT_Texture2D:
			return AddCodeChunk( OPT_Float4, 0, sampleCode.c_str(), GetChunkCode( chunkIdx), "iDiffuseUV");
		}

		return INVALID;
	}

	// 掩码标记
	INT D3D11MaterialCompiler::ComponentMask( INT chunkIdx, bool maskR, bool maskG, bool maskB, bool maskA)
	{
		// 根据块索引获取变量类型
		OutputType chunkType = GetChunkCodeOutputType( chunkIdx);

		// 安全性判断
		if( maskR && ( chunkType & OPT_Float)<OPT_Float1 ||  
			maskG && ( chunkType & OPT_Float)<OPT_Float2 || 
			maskB && ( chunkType & OPT_Float)<OPT_Float3 || 
			maskA && ( chunkType & OPT_Float)<OPT_Float4)
			ALogSystem.Warning( "Not enough components!");

		OutputType resultType;
		switch( ( maskR?1:0) + ( maskG?1:0) + ( maskB?1:0) + ( maskA?1:0))
		{
		case  1: resultType = OPT_Float1; break;
		case  2: resultType = OPT_Float2; break;
		case  3: resultType = OPT_Float3; break;
		case  4: resultType = OPT_Float4; break;
		default: ALogSystem.Warning("Couldn't determine result type of component mask"); break;
		}

		return AddCodeChunk( resultType, 0, "%s.size%s%s%s%s", GetChunkCode( chunkIdx), maskR ? "r" : "", maskG ? "g" : "", maskB ? "b" : "", maskA ? "a" : "");
	}

	// 添加用户输入
	INT D3D11MaterialCompiler::AddUserInput( const float4& constant, OutputType type, const char* format, ...)
	{
		char buf[512];
		GET_VARARGS_RESULT( buf, 512, format);

		MaterialD3D11UserInputConstant* userInput = NULL;
		for( size_t i=0; i<m_userConstants.size(); i++)
		{
			if( m_userConstants[i]->m_value==constant)
			{
				userInput = m_userConstants[i];
				break;
			}
		}

		if( !userInput)
		{
			userInput = new_ MaterialD3D11UserInputConstant( constant, type==OPT_Float4 ? 4 : (type==OPT_Float3 ? 3 : (type==OPT_Float2 ? 2 : 1)));

			m_userInputs.push_back( userInput);
			m_userConstants.push_back( userInput);
		}

		INT codeIndex = static_cast<INT>(m_codeChunks.size());

		m_codeChunks.push_back( ShaderCodeChunk( userInput, buf, type));

		return codeIndex;
	}

	// AddCodeChunk - Adds a formatted code string to the code array and returns its index
	INT D3D11MaterialCompiler::AddCodeChunk( OutputType type, DWORD flags, const char* format, ...)
	{
		char buf[2048];
		GET_VARARGS_RESULT( buf, 2048, format);

		m_codeChunks.push_back( ShaderCodeChunk( buf, type));

		return m_codeChunks.size() - 1;
	}
}