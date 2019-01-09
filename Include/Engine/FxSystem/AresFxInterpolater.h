#pragma once

#include <vector>
#include <Core/AresMath.h>

using namespace std;

namespace Ares
{
	// FxKeyFrame
	struct FxKeyFrame 
	{
		float		m_time;			// 时间点
		ColorRGB	m_color;
	};

	//--------------------------------
	// FxInterpolater 2013-1-25 帝林
	//--------------------------------
	class FxInterpolater
	{
	public:
		// 添加关键帧
		void AddKeyFrame();

		// 插值颜色
		bool InterplateColor( ColorRGB& color, float life);

	private:
		vector<FxKeyFrame>	m_keyFrames;		// 关键帧
	};
}