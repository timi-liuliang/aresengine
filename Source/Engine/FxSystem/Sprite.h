//#pragma once
//
//namespace Ares
//{
//	//---------------------------------------
//	// 点精灵 2011-04-02  帝林
//	//---------------------------------------
//	struct SSPrite
//	{
//		Vector3		 m_vPos;	  // 粒子当前位置
//		float        m_fLifeTime; // 生命周期
//		float        m_fAge;	  // 已存在时间
//		float        m_fSpeed;	  // 移动速度
//		Vector3		 m_vMoveDir;  // 移动方向	
//		float		 m_fSize;	  // 粒子尺寸 
//		DWORD		 m_color;     // 粒子颜色
//		bool         m_bIdle;	  // 己死亡(不占茅坑)
//
//		// 构造函数
//		SSPrite() 
//			: m_fSize(1.f),
//			  m_fLifeTime(1.f),
//			  m_fAge(0.f),
//			  m_fSpeed(1.f),
//			  m_bIdle(true)
//		{ 
//		}
//
//		// 更新
//		bool FrameMove(float fTime)
//		{
//			// Age the Particle
//			m_fAge += fTime;
//
//			// If this particle's age is greater than it's lifetime, it dies
//			if(m_fAge > m_fLifeTime) 
//			{
//				m_bIdle = true;
//				return false;
//			}
//
//			m_vPos += m_vMoveDir * fTime;
//
//			return true;
//		}
//	};
//}