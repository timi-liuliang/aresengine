#include <Core/AresTriggerSystem.h>

namespace Ares
{
	// 注册触发器
	size_t CTriggerSystem::RegisterTrigger( ETriggerType triggerType, size_t priority, size_t idSender, const Vector3& pos, float radius, float duration, bool dynamicSourcePos)
	{
		// Create a trigger signalslot, and fill it in
		//STriggerSignal* triggerSignal = new STriggerSignal( triggerType, idSender, pos, radius, duration, dynamicSourcePos);

		// Trigger records are sorted by priority
		//m_triggerMap.insert( TRIGGER_MAP::value_type( priority, triggerSignal));

		//return triggerSignal->m_triggerID;

		return 0;
	}

	// 移除触发器
	void CTriggerSystem::RemoveTrigger( size_t triggerID)
	{
		TRIGGER_MAP::iterator it = m_triggerMap.begin();
		while( it != m_triggerMap.end())
		{
			if( it->second->m_triggerID == triggerID)
			{
				delete( it->second);
				
				return;
			}

			++it;
		}
	}

	// 更新
	void CTriggerSystem::FrameMove()
	{
		// 删除过期的触发器消息

		// 刷新动态位置的触发器消息

		// 通知与某个触发器消息有关的所有智能体
	}
}