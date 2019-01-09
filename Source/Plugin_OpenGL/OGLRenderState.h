#pragma once

#include <Core/Graphics/AresRenderState.h>

namespace Ares
{
	//----------------------------------
	// OGL光栅化状态 2013-4-3 帝林
	//----------------------------------
	class OGLRasterizerState : public RasterizerState
	{
	public:
		OGLRasterizerState( const RasterizerStateDesc& desc);

		// 激活
		virtual void SetActive();
	};

	//----------------------------------
	// OGL深度模板状态 2013-4-3 帝林
	//----------------------------------
	class OGLDepthStencilState : public DepthStencilState
	{
	public:
		OGLDepthStencilState( const DepthStencilStateDesc& desc);

		// 激活
		virtual void SetActive( UINT frontStencilRef, UINT backStencilRef);
	};

	//----------------------------------
	// OGL混合状态 2013-4-3 帝林
	//----------------------------------
	class OGLBlendState : public BlendState
	{
	public:
		OGLBlendState( const BlendStateDesc& desc);

		// 激活
		virtual void SetActive( const ColorRGB& blendFactor, UINT sampleMask);
	};

	//----------------------------------
	// OpenGL纹理采样状态 2013-4-3 帝林
	//----------------------------------
	class OGLSamplerState : public SamplerState
	{
	public:
		explicit OGLSamplerState( const SamplerStateDesc& desc);
	};
}