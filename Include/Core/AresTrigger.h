#pragma once

#include <map>
#include "AresMath.h"

namespace Ares
{
	//-------------------------------
	// 触发器类型 2011-10-3 帝林
	//-------------------------------
	enum ETriggerType
	{
		TRIGGER_NONE		= 0,
		TRIGGER_EXPLOSION	= (1<<0),
		TRIGGER_ENEMYNEAR	= (1<<1),
		TRIGGER_GUNFIRE		= (1<<2),
	};

	//-------------------------------
	// 触发器消息 2011-10-03
	//-------------------------------
	struct STriggerSignal
	{
		ETriggerType	m_triggerType;		// 触发器类型
		size_t			m_triggerID;		// 触发器ID
		size_t			m_idSource;			// 触发器消息创建对象ID
		Vector3			m_pos;				// 位置
		float			m_radius;			// 半径
		size_t			m_timeStamp;		// 时间印记
		size_t			m_timeExpiration;	// 终结时刻
		bool			m_dynamicSourcePos;	// Lookup position of trigger source every update

		static size_t	m_nextTriggerID;	// Next unique registration ID for trigger	
	};

	typedef std::multimap<size_t /*priority*/, STriggerSignal*> TRIGGER_MAP;
}