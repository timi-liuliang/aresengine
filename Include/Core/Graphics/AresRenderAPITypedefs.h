#pragma once

#include "Core/AresPlatform.h"

namespace Ares
{
	class RenderView;
	typedef boost::shared_ptr<RenderView> RenderViewPtr;
	class FrameBuffer;
	typedef boost::shared_ptr<FrameBuffer> FrameBufferPtr;
	struct RenderMethod;
	typedef boost::shared_ptr<RenderMethod> RenderMethodPtr;
	class ShaderParam;
	typedef boost::shared_ptr<ShaderParam> ShaderParamPtr;
	class VisualTextureParam;
	typedef boost::shared_ptr<VisualTextureParam> VisualTextureParamPtr;
	class RasterizerState;
	typedef boost::shared_ptr<RasterizerState> RasterizerStatePtr;
	class DepthStencilState;
	typedef boost::shared_ptr<DepthStencilState> DepthStencilStatePtr;
	class BlendState;
	typedef boost::shared_ptr<BlendState> BlendStatePtr;
	class RenderLayout;
	typedef boost::shared_ptr<RenderLayout>	RenderLayoutPtr;
	class GraphicsBuffer;
	typedef boost::shared_ptr<GraphicsBuffer> GraphicBufferPtr;
	class Texture;
	typedef boost::shared_ptr<Texture> TexturePtr;
	class SamplerState;
	typedef boost::shared_ptr<SamplerState> SamplerStatePtr;
	class RenderMaterial;
	typedef boost::shared_ptr<RenderMaterial> RenderMaterialPtr;
}