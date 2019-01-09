#include <Engine/RenderSystem/AresPostProcessChain.h>

namespace Ares
{
	// 构造函数
	PostProcessChain::PostProcessChain(  size_t width, size_t height, Scene& scene)
	{
		m_godRay       = GodRayPtr( new_ GodRay( width, height, scene));
		m_depthOfField = DepthOfFieldPtr( new_ DepthOfField( scene));
		m_floodColor   = FloodColorPtr( new_ FloodColor( scene));
	}

	// 更改窗口大小
	void PostProcessChain::Resize( size_t width, size_t height)
	{
		m_godRay->Resize( width, height);
		m_depthOfField->Resize( width, height);
		m_floodColor->Resize( width, height);
	}
}