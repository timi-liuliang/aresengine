#pragma once

#include <EditorKit/BaseCamera.h>

namespace Ares
{
	//----------------------------------------------
	// 默认场景摄像机控制 2013-8-8 帝林
	//----------------------------------------------
	class DefaultProxy
	{
	public:
		DefaultProxy();
		~DefaultProxy();

		// 激活
		void Active();

		// 更新
		void FrameMove( float elapsedTime);

		// 设置速度
		void SetSpeed( float speed) { m_fSpeed = speed; }

		// 获取位置
		const Vector3& GetPosition() { return m_vPos; }

		// 获取摄像机
		Camera* GetCamera() { return m_camera; }

	private:
		Camera*		m_camera;
		Vector3		m_vPos;
		float		m_fSpeed;
	};
}