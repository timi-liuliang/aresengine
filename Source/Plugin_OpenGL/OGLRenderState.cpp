#include "OGLRenderState.h"

namespace Ares
{
	// 构造函数
	OGLRasterizerState::OGLRasterizerState( const RasterizerStateDesc& desc)
		: RasterizerState( desc)
	{

	}

	// 激活
	void OGLRasterizerState::SetActive()
	{

	}

	// 构造函数
	OGLDepthStencilState::OGLDepthStencilState( const DepthStencilStateDesc& desc)
		: DepthStencilState( desc)
	{

	}

	// 激活
	void OGLDepthStencilState::SetActive( UINT frontStencilRef, UINT backStencilRef)
	{

	}

	// 构造函数
	OGLBlendState::OGLBlendState( const BlendStateDesc& desc)
		: BlendState( desc)
	{

	}

	// 激活
	void OGLBlendState::SetActive( const ColorRGB& blendFactor, UINT sampleMask)
	{

	}

	// 构造函数
	OGLSamplerState::OGLSamplerState( const SamplerStateDesc& desc)
		: SamplerState( desc)
	{

	}
}