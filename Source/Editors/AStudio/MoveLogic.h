#pragma once

#include <Core/AresMath.h>

#include "DefaultProxy.h"
#include "CharacterProxy.h"

namespace Ares
{
	//----------------------------
	// 移动逻辑 2011-05-04
	//----------------------------
	class CCharacterProxy;
	class CMoveLogic
	{
	public:
		enum MoveType
		{
			EM_THIRD,
			EM_FOLLOW,
		};

		// 构造函数
		CMoveLogic();

		// 更新
		void FrameMove( float fDeltaTime);

		// 设置辅助胶囊体是否可见
		void SetCapsuleVisible( bool visible);

	public:
		// 设置移动类型
		void SetMoveType( MoveType type);

		// 设置投影参数
		void  SetProjParams( float fovW, int screenWidth, int screenHeight, float fNearPlane, float fFarPlane, RenderDeviceType type);

		// 获取移动类型
		MoveType GetMoveType() { return m_moveType; }

		// 获取默认移动代理
		DefaultProxy* GetDefaultProxy() { return m_default; }

		// 获取移动窗口中心点
		Vector3 GetCenterPos();

	private:
		DefaultProxy*			m_default;			// 场景默认操作
		CCharacterProxy*		m_character;		// 主角
		MoveType				m_moveType;			// 移动类型
	};
}