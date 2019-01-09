#pragma once

#include <Core/AresPlatform.h>
#include "AresGodRay.h"
#include "AresDepthOfField.h"
#include "AresFloodColor.h"

namespace Ares
{
	//-----------------------------
	// 后处理管理器 2013-1-29 帝林
	//-----------------------------
	class PostProcessChain
	{
	public:
		PostProcessChain( size_t width, size_t height, Scene& scene);

		// 更改窗口大小
		void Resize( size_t width, size_t height);

		// 获取GodRay后处理
		GodRayPtr GetGodRayPostProcess(){ return m_godRay; }

		// 获取景深后处理
		DepthOfFieldPtr GetDepthOfField() { return m_depthOfField; }

		// 获取泛光色
		FloodColorPtr GetFloodColor() { return m_floodColor; }

	private:
		GodRayPtr		m_godRay;		// 上帝射线
		DepthOfFieldPtr m_depthOfField; // 景深
		FloodColorPtr	m_floodColor;	// 泛光色
	};
	typedef s_ptr<PostProcessChain> PostProcessChainPtr;
}