#pragma once

#include "AresTrigger.h"

namespace Ares
{
	//--------------------------------------------
	// 通用触发器系统 2011-10-3  帝林
	//--------------------------------------------
	class CTriggerSystem
	{
	public:
		// 构造函数
		CTriggerSystem();

		// 析构函数
		~CTriggerSystem();

	public:
		// 注册触发器
		size_t RegisterTrigger( ETriggerType triggerType, size_t priority, size_t idSender, const Vector3& pos, float radius, float duration, bool dynamicSourcePos);

		// 移除触发器
		void RemoveTrigger( size_t triggerID);

		// 更新
		void FrameMove();

	private:
		TRIGGER_MAP		m_triggerMap;	
	};
}