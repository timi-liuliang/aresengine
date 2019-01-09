#pragma once

#include <Core/Graphics/AresRenderState.h>

namespace Ares
{
	//----------------------------------
	// OGL光栅化状态 2013-4-3 帝林
	//----------------------------------
	class OGLESRasterizerState : public RasterizerState
	{
	public:
		OGLESRasterizerState( const RasterizerStateDesc& desc);

		// 激活
		virtual void SetActive();
	};

	//----------------------------------
	// OGL深度模板状态 2013-4-3 帝林
	//----------------------------------
	class OGLESDepthStencilState : public DepthStencilState
	{
	public:
		OGLESDepthStencilState( const DepthStencilStateDesc& desc);

		// 激活
		virtual void SetActive( UINT frontStencilRef, UINT backStencilRef);
	};

	//----------------------------------
	// OGL混合状态 2013-4-3 帝林
	//----------------------------------
	class OGLESBlendState : public BlendState
	{
	public:
		OGLESBlendState( const BlendStateDesc& desc);

		// 激活
		virtual void SetActive( const ColorRGB& blendFactor, UINT sampleMask);
	};

	//----------------------------------
	// OpenGL纹理采样状态 2013-4-3 帝林
	//----------------------------------
	class OGLESSamplerState : public SamplerState
	{
	public:
		explicit OGLESSamplerState( const SamplerStateDesc& desc);
	};
}