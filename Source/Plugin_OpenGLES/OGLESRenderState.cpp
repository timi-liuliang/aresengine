#include "OGLESRenderState.h"

namespace Ares
{
	// 构造函数
	OGLESRasterizerState::OGLESRasterizerState( const RasterizerStateDesc& desc)
		: RasterizerState( desc)
	{

	}

	// 激活
	void OGLESRasterizerState::SetActive()
	{

	}

	// 构造函数
	OGLESDepthStencilState::OGLESDepthStencilState( const DepthStencilStateDesc& desc)
		: DepthStencilState( desc)
	{

	}

	// 激活
	void OGLESDepthStencilState::SetActive( UINT frontStencilRef, UINT backStencilRef)
	{

	}

	// 构造函数
	OGLESBlendState::OGLESBlendState( const BlendStateDesc& desc)
		: BlendState( desc)
	{

	}

	// 激活
	void OGLESBlendState::SetActive( const ColorRGB& blendFactor, UINT sampleMask)
	{

	}

	// 构造函数
	OGLESSamplerState::OGLESSamplerState( const SamplerStateDesc& desc)
		: SamplerState( desc)
	{

	}
}