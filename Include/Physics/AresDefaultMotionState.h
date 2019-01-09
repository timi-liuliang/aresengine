#pragma once

#include "MotionState.h"

namespace Ares
{
	//-----------------------------------------
	// DefaultMotionState 2013-910 帝林
	//-----------------------------------------
	class DefaultMotionState : public MotionState
	{
	public:
		// 获取初始位置
		virtual void GetWorldTransform( Transform& worldTrans) const
		{

		}

		// 设置位置 the update only be calls for active objects
		virtual void SetWorldTransform( const Transform& worldTrans)
		{

		}

	public:
		
	};
}