#pragma once

namespace Ares
{
	// 检测标记
	enum SegmentDetectFlag
	{
		SDF_NearestHitPoint   = 1<<0,	// 最近点
	};

	// 检测配置 
	struct SegmentDetectConfig
	{
		int		m_detectFlag;

		// 构造函数
		SegmentDetectConfig()
			: m_detectFlag( SDF_NearestHitPoint)
		{}
	};

	// 检测结果
	struct SegmentDetectResult
	{
		Vector3		m_closetHitPoint;		// 相交位置
		Vector3		m_closetHitNormal;		// 相交点法线
	};
}