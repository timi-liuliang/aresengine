#pragma once

#include <boost/noncopyable.hpp>
#include "Engine/RenderSystem/AresRenderable.h"

namespace Ares
{
	//------------------------------------------
	// 帧滤镜 2012-8-8  帝林
	//------------------------------------------
	class Scene;
	class FrameFilter : boost::noncopyable
	{
	public:
		virtual ~FrameFilter();

		// 执行滤镜
		virtual void Execute();

	public:
		// 获取ScreenQuadRenderLayout
		static RenderLayoutPtr& GetScreenQuadRenderLayout();

	protected:
		FrameFilter( Scene& scene);

	protected:
		Renderable				m_renderable;
		Scene&					m_scene;		// 所属场景系统
	};
}