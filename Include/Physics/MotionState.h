#pragma once

namespace Ares
{
	//---------------------------------------------------------
	// MotionState 2012-7-30  帝林 抄bullet
	// The MotionState interface class allows the dynamics
	// world to synchronized and interpolate the updated world
	// transforms with graphics For optimizations, potentially
	// only moving objects get synchronized.
	//---------------------------------------------------------
	class MotionState
	{
	public:
		// 虚析构函数
		virtual ~MotionState()
		{}

		// 获取初始位置
		virtual void GetWorldTransform( Transform& worldTrans) const=0;

		// 设置位置 the update only be calls for active objects
		virtual void SetWorldTransform( const Transform& worldTrans)=0;
	};
}