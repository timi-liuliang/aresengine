#pragma once

#include <Engine/AresRoot.h>

namespace Ares
{
	//-------------------------------------
	// 模型编辑器光源编辑逻辑 2013-1-9 帝林
	//-------------------------------------
	class LightEditLogic
	{
	public:
		LightEditLogic();

		// 每帧更新
		void FrameMove();

		// 获取光源列表
		const vector<LightPtr>& GetLights() { return m_lights; }

		// 获取主光源
		DirectionalLightPtr GetDominantLight() { return m_dominantLight; }

	private:
		DirectionalLightPtr		m_dominantLight;	// 主光源
		DirectionalLightPtr		m_dirLight;
		vector<LightPtr>		m_lights;			// 光源
	};
}