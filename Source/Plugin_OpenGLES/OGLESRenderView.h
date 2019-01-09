#pragma once

#include <Core/Graphics/AresRenderView.h>
#include "OGLESPlatform.h"

namespace Ares
{
	//------------------------------------
	// OGLESRenderView 2013-2-6 帝林
	//------------------------------------
	class OGLESRenderView : public RenderView
	{
	protected:
		GLuint		m_texture;
		GLuint		m_fbo;
		GLuint		m_index;
	};

	//-----------------------------------------
	// OGLScreenRenderTargetRenderView 2013-4-13 帝林
	//-----------------------------------------
	class OGLScreenRenderTargetRenderView : public OGLESRenderView
	{
	public:
		OGLScreenRenderTargetRenderView( UINT width, UINT height, ElementFormat ef);
	};
	typedef boost::shared_ptr<OGLScreenRenderTargetRenderView> OGLScreenRenderTargetRenderViewPtr;

	//--------------------------------------
	// OGLScreenDepthStencilRenderView 
	//--------------------------------------
	class OGLScreenDepthStencilRenderView : public OGLESRenderView
	{
	public:
		OGLScreenDepthStencilRenderView( UINT width, UINT height, ElementFormat ef);
	
        
    };
	typedef boost::shared_ptr<OGLScreenDepthStencilRenderView> OGLScreenDepthStencilRenderViewPtr;

	//--------------------------------------
	// OGLTexture2DRenderView 2013-2-6 帝林
	//--------------------------------------
	class OGLTexture2DRenderView : public OGLESRenderView
	{
	public:
		OGLTexture2DRenderView( TexturePtr& texture, int arrayIndex, int level);
	};

	//-----------------------------------------
	// OGLDepthStencilRenderView 2013-2-6 帝林
	//-----------------------------------------
	class OGLESDepthStencilRenderView : public OGLESRenderView
	{
	public:
		OGLESDepthStencilRenderView( UINT width, UINT height, ElementFormat format, UINT sampleCount, UINT sampleQuality);
	
        // 绑定FramberBuffer
		virtual void OnAttachedFrameBuffer( FrameBuffer& frameBuffer, UINT attach);
        
    private:
		int				m_level;
        GLuint          m_rbo;
    };
	typedef boost::shared_ptr<OGLESDepthStencilRenderView> OGLDepthStencilRenderViewPtr;
}