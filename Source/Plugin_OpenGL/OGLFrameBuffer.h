#pragma once

#include "OGLPlatform.h"
#include <Core/Graphics/AresFrameBuffer.h>

namespace Ares
{
	//-----------------------------------
	// OGLFrameBuffer 2013-4-3 µ€¡÷
	//-----------------------------------
	class OGLFrameBuffer : public FrameBuffer
	{
	public:
		OGLFrameBuffer( RenderDevice* device);
		virtual ~OGLFrameBuffer();

		// Clear
		virtual void Clear( const ColorRGB& color, float depth, UINT stencil, UINT flags=CBM_Depth|CBM_Stencil|CBM_Color);

		// ∞Û∂®
		virtual void Bind();

	private:
		GLuint		m_fbo;		// Frame Buffer Object
	};
}