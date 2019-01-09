#pragma once

#include <Core/Graphics/AresRenderView.h>

namespace Ares
{
	//------------------------------------
	// OGLRenderView 2013-2-6 帝林
	//------------------------------------
	class OGLRenderView : public RenderView
	{

	};

	//-----------------------------------------
	// OGLScreenRenderTargetRenderView 2013-4-13 帝林
	//-----------------------------------------
	class OGLScreenRenderTargetRenderView : public OGLRenderView
	{
	public:
		OGLScreenRenderTargetRenderView( UINT width, UINT height, ElementFormat ef);
	};
	typedef boost::shared_ptr<OGLScreenRenderTargetRenderView> OGLScreenRenderTargetRenderViewPtr;

	//--------------------------------------
	// OGLScreenDepthStencilRenderView 
	//--------------------------------------
	class OGLScreenDepthStencilRenderView : public OGLRenderView
	{
	public:
		OGLScreenDepthStencilRenderView( UINT width, UINT height, ElementFormat ef);
	};
	typedef boost::shared_ptr<OGLScreenDepthStencilRenderView> OGLScreenDepthStencilRenderViewPtr;

	//--------------------------------------
	// OGLTexture2DRenderView 2013-2-6 帝林
	//--------------------------------------
	class OGLTexture2DRenderView : public OGLRenderView
	{
	public:
		OGLTexture2DRenderView( TexturePtr& texture, int arrayIndex, int level);
	};

	//-----------------------------------------
	// OGLDepthStencilRenderView 2013-2-6 帝林
	//-----------------------------------------
	class OGLDepthStencilRenderView : public OGLRenderView
	{
	public:
		OGLDepthStencilRenderView( UINT width, UINT height, ElementFormat format, UINT sampleCount, UINT sampleQuality);
	};
	typedef boost::shared_ptr<OGLDepthStencilRenderView> OGLDepthStencilRenderViewPtr;
}