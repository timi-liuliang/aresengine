#include "CharacterProxy.h"
#include <Physics/Shapes.h>
#include <Engine/RenderSystem/AresRenderSystem.h>
#include <Engine/RenderSystem/AresAnimationController.h>
#include <Engine/AresRoot.h>
#include "SCLogic.h"
#include <vector>

using namespace std;

// 摄像机左右旋转绽放 上下旋转缩放 半径增加缩短缩放
#define CAMERA_ROTATIONY_SCALAR  0.1f
#define CAMERA_ROTATIONZ_SCALAR  0.3f
#define CAMERA_RADIUS_SCALAR     0.02f


#define  GRAVITY_G	-6.f

namespace Ares
{
	//----------------------------------------
	// 跳跃  2011-05-06   林
	//----------------------------------------
	struct SJump
	{
		Vector3 m_startPos;			// 起跳位置
		Vector3	m_forward;			// 跳跃方向
		float		m_totalTime;		// 持续时间
		float		m_startMoveTime;	// 开始移动时间
		float		m_endMoveTime;		// 结束移动时间
		float		m_jumpVelocity;		// 起跳速度

		// 起跳
		void Jump( const Vector3& startPos, const Vector3& forward)
		{
			m_startPos = startPos;
			m_forward  = forward;
			m_totalTime= 0.f;
			m_startMoveTime = 0.f;
			m_endMoveTime= 0.f;
			m_jumpVelocity = 3.f;
		}

		// 设置跳跃方向
		void SetJumpDir( Vector3& dir)
		{
			if( m_forward == Vector3( 0.f, 0.f, 0.f))
			{
				m_forward = dir;
				m_startMoveTime = m_totalTime;
			}
		}

		// 清除移动方向
		void ClearMoveDir()
		{
			if( !m_endMoveTime)
				m_endMoveTime  = m_totalTime;
		}

		// 每帧更新
		Vector3 FrameMove( float fDeltaTime)
		{
			m_totalTime += fDeltaTime;

			// 水平方向
			Vector3 nowPos;

			if( m_endMoveTime)
				nowPos = m_startPos + m_forward * ( m_endMoveTime - m_startMoveTime);
			else
				nowPos = m_startPos + m_forward * ( m_totalTime - m_startMoveTime);

			// 上下方向
			nowPos.z = m_startPos.z + 0.5f * GRAVITY_G * m_totalTime * m_totalTime + m_jumpVelocity * m_totalTime;

			return nowPos;
		}
	};

	//--------------------------------------
	// 角色移动属性结构体 2011-05-10 
	//--------------------------------------
	struct SProxyInfo
	{
		Vector3			m_pos;					// 位置
		Vector3			m_up;					// 上方向
		Vector3			m_headerPos;			// 头顶位置
		Vector3			m_moveDir;				// 移动方向
		float			m_radius;				// 半径
		float			m_height;				// 人物高度
		float			m_velocity;				// 移动速度
		//float			m_downGlideVelocity;	// 下滑速度
		float			m_upDownAngle;			// 此角度位置提升
		float			m_timeLand;				// 在地上多久可以再次起跳

		float			m_horizonAngle;			// 水平旋转角度(顺时针)
		float			m_verticleAngle;		// 垂直旋转角度(顺时针)

		//float			m_tolerance;			// 误差范围

		// 构造函数
		SProxyInfo()
		{
			m_pos       = Vector3( 0.f, 0.f, 0.f);
			m_moveDir	= Vector3( 0.f, 0.f, 0.f);
			m_height    = 0.95f;
			m_up		= Vector3( 0.f, 0.f, 1.f);
			m_headerPos = m_pos + m_height * m_up;
			m_upDownAngle = PI / 4.f;

			m_radius = 0.2f;
			m_timeLand = 0.3f;
			//m_tolerance = 1e-6f;

			m_horizonAngle = 0.f;
			m_verticleAngle= 0.f;

			m_velocity	= 3.f;
		}

		// 根据配置生成capsue
		void  GenerateCapsule(  Vector3& pos, Capsule3& pOut)
		{
			Segment3 segment( pos + Vector3( 0, 0, m_radius), pos + Vector3( 0, 0, m_height - m_radius));

			pOut.Set( segment, m_radius);
		}
	};

	// 构造函数
	CCharacterProxy::CCharacterProxy()
	{
		m_showCapsule = false;

		for ( int i=0; i<MAX_COLL; i++)
		{
			m_lines[i] = SELogic_VisualShape.CreateSegment();
			m_lines[i]->SetColor(0xFFFFF00F);
			m_lines[i]->SetVisible( false);
		}

		m_info = new_ SProxyInfo;
		m_jump = new_ SJump;

		m_capsule   = SELogic_VisualShape.CreateVisualShape();
		m_character = AResSystem.LoadModel( "d_zhuyan.model");
		Scene* pScene = SCLogic_Scene;
		if( pScene)
		{
			pScene->AddEntity( "Character", m_character, false);
		}

		// 测试用
		Capsule3 capsule( Segment3( Vector3( 5.5f, 5.6f, 0.1f), Vector3( 5.5f, 5.6f, 1.f)), 0.3f);

		m_moveState = EM_FALL;
		m_timeOnLand   = 0.f;

		m_camera = new_ CFollowViewerCamera;
		m_camera->SetBindPos( &m_info->m_headerPos);

		SetStepHZ( 60.f);

		// 显示胶囊体
		ShowCapsule( capsule);
	}

	// FrameMove
	void CCharacterProxy::FrameMove( float fDeltaTime)
	{
		// 暂时防止调试后,时间跨度太大
		if( fDeltaTime > 0.2f)
			return;

		m_timeOnLand += fDeltaTime;

		// 更换状态
		SwitchMoveStateBefore();

		Vector3 nextPos;
		if( ResponseInputAndCalcNextPos( nextPos, fDeltaTime))
		{
			AdjustNextPos( nextPos);
		}

		m_camera->FrameMove( fDeltaTime);

		static vector<LandMoveState> stateListener;

		if( stateListener.size() == 0 || (m_moveState != stateListener.back() ||m_moveState==EM_FALL))
			stateListener.push_back( m_moveState);
	}

	// 改变运动状态
	void CCharacterProxy::SwitchMoveStateBefore()
	{
		if( m_moveState == EM_LAND)
		{
			// land to jump
			if( SCLogic_Input->IsKeyDown( OIS::KC_SPACE) && m_timeOnLand > m_info->m_timeLand)
			{
				m_jump->Jump( m_info->m_pos, m_info->m_moveDir * m_info->m_velocity);

				m_moveState = EM_JUMP;	
			}
		}
	}

	// 响应输入
	void CCharacterProxy::RespondInput( float fDeltaTime)
	{
		static float backVelocity = 0.8f;

		if( IsKeyDown( OIS::KC_W) && IsKeyDown( OIS::KC_A))
			m_info->m_moveDir = GetDir( EM_LEFT_FORWARD);

		else if(IsKeyDown( OIS::KC_A) && IsKeyDown( OIS::KC_S))
			m_info->m_moveDir = GetDir( EM_LEFT_BACK) * backVelocity;

		else if(IsKeyDown( OIS::KC_W) && IsKeyDown( OIS::KC_D))
			m_info->m_moveDir = GetDir( EM_RIGHT_FORWARD);

		else if(IsKeyDown( OIS::KC_S) && IsKeyDown( OIS::KC_D))
			m_info->m_moveDir = GetDir( EM_RIGHT_BACK) * backVelocity;

		else if(IsKeyDown(OIS::KC_W))
			m_info->m_moveDir = GetDir( EM_FORWARD);

		else if(IsKeyDown(OIS::KC_S)) 
			m_info->m_moveDir = GetDir( EM_BACK) * backVelocity;

		else if(IsKeyDown(OIS::KC_A)) 
			m_info->m_moveDir = GetDir( EM_LEFT);

		else if(IsKeyDown(OIS::KC_D)) 
			m_info->m_moveDir = GetDir( EM_RIGHT);
		else
			m_info->m_moveDir = Vector3( 0, 0, 0);

		//if( m_characterNode)
			SetDir();
	}

	// 根据状态计算新的位置
	bool CCharacterProxy::ResponseInputAndCalcNextPos( Vector3& nextPos, float fDeltaTime)
	{
		// 摄像机响应操作
		CameraRespondInput( fDeltaTime);

		// 计算nextPos
		nextPos = m_info->m_pos;

		RespondInput( fDeltaTime);

		PlayCharacterAnimation();

		switch( m_moveState)
		{
		case EM_LAND:
			{
				nextPos = m_info->m_pos + m_info->m_moveDir * m_info->m_velocity * fDeltaTime;
			}
			break;

		case EM_JUMP:
			{
				nextPos = m_jump->FrameMove( fDeltaTime);
				m_jump->SetJumpDir( m_info->m_moveDir * m_info->m_velocity);
			}
			break;

		case EM_FALL:
			{
				//nextPos.z -= 5.f * fDeltaTime;
				nextPos.z -= m_info->m_radius - (m_info->m_radius * cos(m_info->m_upDownAngle)) - 0.001f;
			}
			break;
		}

		return nextPos != m_info->m_pos;
	}

	// 更新角色动画
	void CCharacterProxy::PlayCharacterAnimation()
	{/*
		if( !m_character->GetAnimationController()->GetAnimation())
			return;

		if( !m_character->GetAnimationController()->GetAnimation()->IsResourceEnabled())
			return;

		// 临时

		IAnimationController::SAnimationDesc* animationDesc = NULL;
		static size_t tIndex = 0;
		static bool tInit = false;
		if( !tInit)
		{
			// 1.移动类动作在第0层
			if( animationDesc = m_character->GetAnimationController()->GetAnimationDesc( "跑"))
				animationDesc->m_loop = true;

			if( animationDesc = m_character->GetAnimationController()->GetAnimationDesc( "后跑"))
				animationDesc->m_loop = true;

			//if( animationDesc = m_character->GetAnimationController()->GetAnimationDesc( "待机"))
			//	animationDesc->m_loop = true;

			if( animationDesc = m_character->GetAnimationController()->GetAnimationDesc( "战备"))
				animationDesc->m_loop = true;

			// 2.战斗类动作在第1层
			if( animationDesc = m_character->GetAnimationController()->GetAnimationDesc( "出场动画"))
				animationDesc->m_layer = 1;

			if( animationDesc = m_character->GetAnimationController()->GetAnimationDesc( "瞬发"))
				animationDesc->m_layer = 1;

			if( animationDesc = m_character->GetAnimationController()->GetAnimationDesc( "格斗"))
				animationDesc->m_layer = 1;

			if( animationDesc = m_character->GetAnimationController()->GetAnimationDesc( "攻击3"))
				animationDesc->m_layer = 2;

			// 3.受伤类在第2层

			tInit = true;
		}

		switch( m_moveState)
		{
		case EM_LAND:
			{
				if( g_pInput->IsKeyDown( DIK_W))
				{
					if( tIndex !=0)
						m_character->GetAnimationController()->CrossFade( "跑");
					
					tIndex = 0;
				}
				else if( g_pInput->IsKeyDown( DIK_S))
				{
					if( tIndex !=1)
						m_character->GetAnimationController()->CrossFade( "后跑");

					tIndex = 1;
				}
				else if(g_pInput->IsKeyDown( DIK_A) || g_pInput->IsKeyDown( DIK_D))
				{
					if( tIndex !=0)
						m_character->GetAnimationController()->CrossFade( "跑");

					tIndex = 0;
				}
				else if( tIndex != 2)
				{
					if( tIndex !=2)
					{
						//m_character->GetAnimationController()->CrossFade( "待机");
						m_character->GetAnimationController()->CrossFade( "战备");
					}

					tIndex = 2;
				}
				
				if( g_pInput->IsKeyDown( DIK_1))
				{
					m_character->GetAnimationController()->CrossFade( "格斗", 0.15f, IAnimationController::STOP_SAMELAYER_DA);
					tIndex = 3;
				}
				if( g_pInput->IsKeyDown( DIK_2))
				{
					m_character->GetAnimationController()->CrossFade( "瞬发", 0.1f, IAnimationController::STOP_SAMELAYER_DA);
					tIndex = 3;
				}
				else if( g_pInput->IsKeyDown( DIK_3))
				{
					m_character->GetAnimationController()->CrossFade( "攻击3", 0.3f, IAnimationController::STOP_SAMELAYER_DA);
					tIndex = 3;
				}
			}
			break;

		case EM_JUMP:
			{
				if( tIndex != 3)
				{
					//m_character->GetAnimationController()->CrossFade( 3);

					tIndex = 3;
				}
			}
			break;

		case EM_FALL:
			{
				
			}
			break;
		}*/
	}

	// 调整新位置
	void CCharacterProxy::AdjustNextPos( Vector3 nextPos)
	{
		// 进行检测
		CollCheck( nextPos);

		switch( m_moveState)
		{
		case EM_LAND:
			{
				AdjustNextPos_Land( nextPos);
			}
			break;

		case EM_JUMP:
			{
				AdjustNextPos_Jump( nextPos);
			}
			break;

		case EM_FALL:
			{
				AdjustNextPos_Fall( nextPos);
			}
			break;
		}
	}

	// 当前位置是否应该Fall
	bool CCharacterProxy::IsShouldFall(Vector3 nextPos)
	{
		if( !m_collNum)
			return true;

		if( m_collNum == m_collFlagNum[EM_BLOCK_SURROUND] && !IsNip(nextPos))
			return true;

		return false;
	}

	// 当前状态陆地
	void CCharacterProxy::AdjustNextPos_Land( Vector3& nextPos)
	{
		// 有碰撞
		if( m_collNum)
		{
			// 是否应该变为下落状态
			if( IsShouldFall(nextPos))
			{
				// 侧移 + 下探
				Vector3 nnPos = nextPos;
				AdjustPosSurround( nnPos, EM_BLOCK_SURROUND | EM_BLOCK_DOWN, 1.0f);
				nnPos.z -= m_info->m_radius;

				// 下下个状态是否依然是Fall
				CollCheck( nnPos);
				if( IsShouldFall(nnPos))
				{
					m_moveState = EM_FALL;

					return;
				}
			}

			CollCheck( nextPos);

			// 着陆(有支撑点), 更新高度
			AdjustPosSurround(  nextPos, EM_BLOCK_SURROUND);

			if( IsNip(nextPos))
				AdjustPosUpDown( nextPos, EM_BLOCK_DOWN  | EM_BLOCK_SURROUND);
			else
				AdjustPosUpDown( nextPos, EM_BLOCK_DOWN);

			SetPos( nextPos);
		}
		else
		{
			// 下探
			Vector3 tNextPos = nextPos;
			tNextPos.z -= m_info->m_radius - (m_info->m_radius * cos(m_info->m_upDownAngle)) - 0.001f;

			CollCheck( tNextPos);
			if( m_collNum)
			{
				// 有支撑点,则不提高EM_BLOCK_SURROUND, 防止爬高
				AdjustPosUpDown( tNextPos, EM_BLOCK_DOWN  | EM_BLOCK_SURROUND);

				SetPos( tNextPos); 
			}
			else
			{
				SetPos( nextPos);
				m_moveState = EM_FALL;
			}
		}
	}

	// 跳跃
	void CCharacterProxy::AdjustNextPos_Jump( Vector3& nextPos)
	{
		// 有碰撞
		if( m_collNum)
		{
			if( IsShouldFall( nextPos))
			{
				m_jump->ClearMoveDir();

				// 状态不变, 侧移
				AdjustPosSurround( nextPos);
				SetPos( nextPos);

				//m_moveState = EM_FALL;
			}
			else
			{
				// 着陆, 更新高度
				AdjustPosSurround( nextPos, EM_BLOCK_SURROUND);
				AdjustPosUpDown( nextPos, EM_BLOCK_DOWN);

				SetPos( nextPos);

				m_moveState  = EM_LAND;
				m_timeOnLand = 0.f;
			}
		}
		else
		{
			// 无碰撞( 直接更新位置)
			SetPos( nextPos);
		}
	}

	// 下落
	void CCharacterProxy::AdjustNextPos_Fall( Vector3& nextPos)
	{
		// 有碰撞
		if( m_collNum)
		{
			if( IsShouldFall( nextPos))
			{
				// 状态不变, 侧移
				AdjustPosSurround( nextPos);
				SetPos( nextPos);
			}
			else
			{
				// 状态不变, 侧移
				AdjustPosSurround( nextPos);
				AdjustPosUpDown( nextPos, EM_BLOCK_DOWN);

				SetPos( nextPos);

				m_moveState  = EM_LAND;
				m_timeOnLand = 0.f;
			}
		}
		else
		{
			// 无碰撞( 直接更新位置)
			SetPos( nextPos);
		}
	}

	// 位置上下调整
	void CCharacterProxy::AdjustPosUpDown( Vector3& pos, DWORD flag)
	{
		float upgradeHeight = 0.f;
		for ( int i=0; i<m_collNum; i++)
		{
			if( m_collFlag[i] & flag)
			{
				// 上推
				upgradeHeight = max( upgradeHeight, m_collInfo[i].m_collNormal.z);
			}
		}

		pos.z += upgradeHeight;
	}

	// 位置左右调整(根据坡朝向)
	void CCharacterProxy::AdjustPosSurround(  Vector3& pos, DWORD flag/* = EM_BLOCK_SURROUND*/, float scale/*=1.0f*/)
	{
		float fForward = 0.f;
		float fBack	   = 0.f;	
		float fRight   = 0.f;
		float fLeft	   = 0.f;

		Vector3 vForward = GetDir( EM_FORWARD);
		Vector3 vRight	 = GetDir( EM_RIGHT);

		for ( int i=0; i<m_collNum; i++)
		{
			if( m_collFlag[i] & flag)
			{
				float fbDot = Vector3Dot( m_collInfo[i].m_collNormal, vForward);
				float rlDot = Vector3Dot( m_collInfo[i].m_collNormal, vRight);

				fForward = max( fForward, fbDot);
				fBack    = min( fBack,    fbDot);

				fRight   = max( fRight,   rlDot);
				fLeft    = min( fLeft,    rlDot);
			}	
		}

		// 侧移量
		Vector3 sideVec3 = vForward * (fForward + fBack) + vRight * ( fRight   + fLeft);

		pos += sideVec3 * scale;
	}

	// 是否被夹住
	bool CCharacterProxy::IsNip( Vector3& pos)
	{
		if( !m_collFlagNum[EM_BLOCK_SURROUND])
			return false; 

		float x0 = -1e30f;
		float x1 =  1e30f;

		for ( int i=0; i<m_collNum; i++)
		{
			if( m_collFlag[i] == EM_BLOCK_SURROUND)
			{
				Vector3 normal( m_collInfo[i].m_collNormal.x,  m_collInfo[i].m_collNormal.y, 0.f);
				normal.Normalize();

				x0 = max( x0, normal.x);
				x1 = min( x1, normal.x);
			}	
		}

		float angle = acos( x0) - acos( x1);

		if( abs( angle) > PI * 0.75f)
		{
			return true;
		}

		Vector3 interPos;
		float		dist = 0.f;
		//g_pScene->PickTerrain( pos+D3DXVECTOR3(0.f, 0.f, m_info->m_height), D3DXVECTOR3( 0.f, 0.f, -1.f), 0.f, 1000.f, interPos, &dist);

		if( dist <= (/*m_radius*0.25f+*/m_info->m_height))
			return true;

		return false;
	}

	// 设置模拟频率
	void CCharacterProxy::SetStepHZ( float hz/*=60.f*/)
	{
		if( hz < 0.1f)
			return;

		m_stepDeltaTime = 1.f / hz;
	}

	// 获取向量
	Vector3 CCharacterProxy::GetDir( MoveDir dir)
	{
		float angle = m_info->m_horizonAngle + dir * PI / 4.f;

		return Vector3( cos( angle), sin( angle), 0.f);
	}

	// 整理碰撞信息
	void CCharacterProxy::TidyCollInfo()
	{
		// 初始化
		m_collFlagNum[EM_BLOCK_DOWN]	 = 0;
		m_collFlagNum[EM_BLOCK_UP]		 = 0;
		m_collFlagNum[EM_BLOCK_SURROUND] = 0;

		// 整理碰撞信息
		for ( int i=0; i<m_collNum; i++)
		{
			// 求与上方向的角度
			float angle = acos( Vector3Dot( m_collInfo[i].m_collNormal, m_info->m_up));

			if( angle >= 0 && angle < m_info->m_upDownAngle)
			{
				// down block
				m_collFlag[i] = EM_BLOCK_DOWN;
				m_collFlagNum[EM_BLOCK_DOWN]++;
			}
			else if( angle <= PI && angle > PI - m_info->m_upDownAngle)
			{
				// up block
				m_collFlag[i] = EM_BLOCK_UP;
				m_collFlagNum[EM_BLOCK_UP]++;
			}
			else
			{
				// surround block
				m_collFlag[i] = EM_BLOCK_SURROUND;
				m_collFlagNum[EM_BLOCK_SURROUND]++;
			}
		}
	}

	// 设置位置
	void CCharacterProxy::SetPos( const Vector3& newPos)
	{	
		m_info->m_pos = newPos;
		static Vector3 virtualPos = newPos;
		Vector3 xPos = m_info->m_pos - virtualPos;
		float length = xPos.Length();
		//if( length <= 0.1f)
		//	return;

		if( IsKeyDown(OIS::KC_R))
		{
			m_info->m_pos = virtualPos = Vector3( 0.f, 0.f, 0.f);
		}

		virtualPos += ( m_info->m_pos - virtualPos) *(( length - 0.1f)/length);
		m_info->m_headerPos = virtualPos + m_info->m_height * m_info->m_up;

		Transform transform = m_character->GetTransform();
		transform.SetTrans( virtualPos);
		m_character->SetTransform( transform);

		//if( D3DXVec3LengthSq( &m_info->m_moveDir) > 1e-6f)
		//	m_characterNode->SetRotation( D3DXVECTOR3( 0, 1, 0), m_info->m_moveDir);
		if( m_showCapsule)
		{/*
			Capsule3 capsule;
			m_info->GenerateCapsule( virtualPos, capsule);

			ShowCapsule( capsule);

			// 辅助显示
			for ( int i=0; i<m_collNum; i++)
			{
				if( m_collFlag[i] == EM_BLOCK_DOWN)
				{
					m_lines[i]->SetColor( 0x00FF);

				}
				else if( m_collFlag[i] == EM_BLOCK_SURROUND)
				{
					m_lines[i]->SetColor( 0x00FF00);
				}

				m_lines[i]->SetVisible( true);
				m_lines[i]->SetParam( m_collInfo[i].m_triCollPoint, m_collInfo[i].m_triCollPoint + m_collInfo[i].m_collNormal);
			}

			for( int i=m_collNum; i<MAX_COLL; i++)
			{
				m_lines[i]->SetVisible( false);
			}*/
		}
		else
		{
			m_capsule->SetVisible( false);
		}
	}

	// 碰撞检测
	int CCharacterProxy::CollCheck( Vector3& newPos)
	{
		Capsule3 capsule;
		m_info->GenerateCapsule( newPos, capsule);

		//m_collNum = SCLogic_Map->GetScene()->GetCollMap().CheckCapsuleColl( capsule, m_collInfo);

		if( m_collNum)
		{
			// 整理
			TidyCollInfo();
		}

		return m_collNum;
	}

	// 辅助,显示capsule()
	void CCharacterProxy::ShowCapsule( Capsule3& capsule)
	{/*
		Vector3 vertexData[142];

		// 三圆环
		for ( int i=0; i<20; i++)
		{
			float radius = i * 0.1 * D3DX_PI;

			Vector3 circleDir = Vector3( cos( radius), sin(radius), 0.f) * capsule.m_radius;

			vertexData[i]    = circleDir + capsule.m_segment.m_orig;
			vertexData[i+20] = circleDir + capsule.m_segment.GetNegEnd();
			vertexData[i+40] = circleDir + capsule.m_segment.GetPosEnd();

			vertexData[i+60] = circleDir * 0.5  + capsule.m_segment.m_orig + capsule.m_segment.m_dir * ( capsule.m_segment.m_extent + capsule.m_radius * 0.86); 
			vertexData[i+80] = circleDir * 0.5  + capsule.m_segment.m_orig - capsule.m_segment.m_dir * ( capsule.m_segment.m_extent + capsule.m_radius * 0.86); 
			vertexData[i+100]= circleDir * 0.86 + capsule.m_segment.m_orig + capsule.m_segment.m_dir * ( capsule.m_segment.m_extent + capsule.m_radius * 0.5); 
			vertexData[i+120]= circleDir * 0.86 + capsule.m_segment.m_orig - capsule.m_segment.m_dir * ( capsule.m_segment.m_extent + capsule.m_radius * 0.5); 
		}

		vertexData[140] = capsule.m_segment.GetNegEnd();
		vertexData[141] = capsule.m_segment.GetPosEnd();

		// 准备顶点数据
		WORD indexData[282];

		for ( int i=0, j=19; i<20; j=i++)
		{
			indexData[i*2]   = i;
			indexData[i*2+1] = j;

			indexData[i*2+40]   = i+20;
			indexData[i*2+40+1] = j+20;

			indexData[i*2+80]   = i+40;
			indexData[i*2+80+1] = j+40;

			indexData[i*2+120] = i+60; 
			indexData[i*2+121] = j+60; 

			indexData[i*2+160] = i+80; 
			indexData[i*2+161] = j+80; 

			indexData[i*2+200] = i+100; 
			indexData[i*2+201] = j+100;

			indexData[i*2+240] = i+120; 
			indexData[i*2+241] = j+120; 
		}

		indexData[280] = 140;
		indexData[281] = 141;

		m_capsule->SetVisible( true);
		m_capsule->SetParam( 142, vertexData, D3DPT_LINELIST, 282, indexData);
		*/
	}

	// 设置朝向
	void CCharacterProxy::SetDir()
	{
		if( m_info->m_moveDir == Vector3(0, 0, 0))
		{
			Transform transform = m_character->GetTransform();
			transform.SetRotation( Vector3( 0.f, 1.f, 0.f), GetDir(EM_FORWARD));
			m_character->SetTransform( transform);

			return;
		}

		Vector3 tDir = m_info->m_moveDir;

		if( IsKeyDown( OIS::KC_A) && IsKeyDown( OIS::KC_S))
			tDir = -tDir;

		else if( IsKeyDown( OIS::KC_S) && IsKeyDown( OIS::KC_D))
			tDir = -tDir;

		else if( IsKeyDown(OIS::KC_S)) 
			tDir = -tDir;

		Transform transform = m_character->GetTransform();
		transform.SetRotation( Vector3( 0.f, 1.f, 0.f), tDir);
		m_character->SetTransform( transform);
	}

	// 摄像机响应输入
	void CCharacterProxy::CameraRespondInput( float fDeltaTime)
	{
		if( IsMRBDown() || IsMLBDown())		// 鼠标移动且右键按下
		{
			m_camera->UpdateCameraDir( SCLogic_Input->GetMouseState().Y.rel * CAMERA_RADIUS_SCALAR, SCLogic_Input->GetMouseState().X.rel * CAMERA_ROTATIONZ_SCALAR * CAMERA_ROTATIONY_SCALAR);
		}

		// 摄像机半径 "拉远" "拉近"
		static long oldlz = 0;
		long          tLZ = SCLogic_Input->GetMouseState().Z.rel;
		if( long tZ = tLZ - oldlz)
		{
		//	m_camera->ChangeDefaultRadius( tZ * -0.004f);

			oldlz = tLZ;
		}

		// 匹配 "人物朝向" 与 "摄像机朝向"
		if( IsMRBDown())
		{
			// 鼠标右键按下,人物朝向立即与摄像机匹配
			m_info->m_horizonAngle = m_camera->GetHorizonAngle();
		}
		else if( IsMLBDown())
		{

		}
		else
		{
			// 左右键均未按下, 若人物与摄像机朝向不一致,缓慢归位到与摄像机同向
			float cameraAngle = m_camera->GetHorizonAngle();

			if( cameraAngle != m_info->m_horizonAngle)
			{
				m_info->m_horizonAngle += ( cameraAngle - m_info->m_horizonAngle) * fDeltaTime;

				if( abs( cameraAngle - m_info->m_horizonAngle) < 0.01f)
					m_info->m_horizonAngle = cameraAngle;
			}
		}

		// 摄像机阻碍
		if( m_camera)
		{
			static float currRadius = m_camera->GetCurRadius();
			float defaultRadius = m_camera->GetDefaultRadius();
			currRadius += ( defaultRadius - currRadius) * fDeltaTime * 0.005f;

			if( abs( currRadius - defaultRadius) < 1e-4f)
			{
				currRadius = defaultRadius;
			}

			Vector3 dir = -m_camera->GetDirection();
			dir.Normalize();

			//D3DXVECTOR3 tt = /*m_camera->GetLookAt() + */dir * currRadius;
			//float length = D3DXVec3Length( &tt)/* - m_camera->GetNear()*/;

			float length = currRadius;

			Vector3 tt = m_camera->GetLookAt()- m_camera->GetPosition();
			float lll  = tt.Length() - m_camera->GetNear();

			Vector3 v0 = m_camera->GetFrustem().GetVertexs()[0] - dir * lll;
			Vector3 v1 = m_camera->GetFrustem().GetVertexs()[1] - dir * lll;
			Vector3 v2 = m_camera->GetFrustem().GetVertexs()[2] - dir * lll;
			Vector3 v3 = m_camera->GetFrustem().GetVertexs()[3] - dir * lll;
			
			int collNum = 0;//SCLogic_Map->GetScene()->GetCollMap().CheckOBBColl( v0, v1, v2, v3, dir, length);

			if( collNum)
			{
				m_camera->SetCurRadius( length);
			}
			else
			{
				m_camera->SetCurRadius( currRadius);
			}
		}
	}

	// 获取角色位置
	Vector3 CCharacterProxy::GetCharacterPos()
	{
		if( m_character)
			return m_character->GetTransform().GetTrans();

		return Vector3( -1024.f, -1024.f, -1024.f);
	}

	// 激活
	void CCharacterProxy::Active()
	{
		SCLogic_Camera->SetCamera( m_camera);

		SetState( CCharacterProxy::EM_FALL);
	}

	// 关闭
	void CCharacterProxy::Decative()
	{
		Scene* pScene = SCLogic_Scene;
		if( pScene)
			pScene->DelEntity( "Character");
	}
}