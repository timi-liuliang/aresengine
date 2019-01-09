#pragma once

#include "D3D11Typedefs.h"
#include "D3D11ShaderDesc.h"
#include <Core/Graphics/AresRenderMethod.h>

using namespace std;

namespace Ares
{
	//-------------------------------------------
	// D3D11RenderMethod 2012-8-29 帝林
	//-------------------------------------------
	class D3D11RenderMethod : public RenderMethod
	{
	public:
		D3D11RenderMethod();
		virtual ~D3D11RenderMethod();

		// 加载 *.hlsl
		virtual bool Load( const char* fileName, const ShaderMacros& macros);

		// 从内存加载
		virtual bool LoadFromMemory( const void* code, size_t length);

		// 绑定
		virtual void Bind();

		// 解绑
		void Unbind();

		// 应用参数
		virtual void SetParams(const RenderMaterial& mat);

		// 保存ByteCode
		virtual void SaveByteCode( const char* fileName, ShaderType type);

		// 获取IASignature
		ID3D10Blob* GetVSCode() { return m_shaderCodes[ST_VertexShader].get(); }

	private:
		// 编译
		ID3DBlobPtr CompileToBytecode( const void* pSrcData, size_t srcDataSize, ShaderType type);

		// 附加汇编
		void AttachByteCode( const ID3DBlobPtr& byteCode, ShaderType type);

		// 剖析参数
		void ParseParameters( const ID3DBlobPtr& byteCode, ShaderType type);

		// 剖析状态
		void ParseStates();

		// 刷新常量Buffer数据
		void RefreshBufferData( const RenderMaterial& mat, ShaderType type);

		// 向接口中添加变量
		void AddVariable( const char* name, ShaderParamType type, ShaderParamPtr& defaultValue=MakeShaderParam());

	private:
		ID3D11VertexShaderPtr		m_vertexShader;		// 顶点着色器
		ID3D11PixelShaderPtr		m_pixelShader;		// 象素着色器
		ID3D11GeometryShaderPtr		m_geometryShader;	// 几何着色器
		ID3D11ComputeShaderPtr		m_computeShader;	// 计算着色器
		ID3D11HullShaderPtr			m_hullShader;		// what's this
		ID3D11DomainShaderPtr		m_domainShader;		// 领域着色器

		boost::array<ID3DBlobPtr,							ST_NumShaderTypes>	m_shaderCodes;		// 汇编代码
		boost::array<HLSL::ShaderDesc,						ST_NumShaderTypes>	m_shaderDescs;		// 描述信息
		boost::array<vector<ID3D11BufferPtr>,				ST_NumShaderTypes>	m_constantBuffers;	// 常量参数
		boost::array<vector<ID3D11SamplerStatePtr>,			ST_NumShaderTypes>	m_samplers;			// 采样状态
		boost::array<vector<ID3D11ShaderResourceViewPtr>,	ST_NumShaderTypes>	m_srvs;				// 纹理
	};
}