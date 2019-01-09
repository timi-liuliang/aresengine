#pragma once

#include <EditorKit/FollowViewerCamera.h>
#include <Engine/RenderSystem/AresModel.h>
#include <Engine/RenderSystem/AresVisualShape.h>
#include <Engine/SceneSystem/AresEntity.h>

#define  MAX_COLL	256

namespace Ares
{
	// 碰撞信息
	struct SCapsuleCollInfo
	{
		Vector3  m_triCollPoint;	// 碰撞点
		Vector3  m_collNormal;		// 碰撞法线,包含穿刺深度信息
		Vector3  m_triNormal;		// 三角形法线
	};

	//--------------------------------------
	// AresProxy for control(尾随视角摄像机)
	//					201104-29 帝林	
	//--------------------------------------
	struct SJump;
	struct SProxyInfo;
	class CCharacterProxy
	{
	public:
		enum MoveDir
		{
			EM_FORWARD = 0,
			EM_RIGHT_FORWARD,
			EM_RIGHT,
			EM_RIGHT_BACK,
			EM_BACK,
			EM_LEFT_BACK,
			EM_LEFT,
			EM_LEFT_FORWARD,
		};

		enum BlockType
		{
			EM_BLOCK_SURROUND = 0x1,
			EM_BLOCK_UP		  = 0x2,		// 上方向阻碍
			EM_BLOCK_DOWN	  = 0x4,		// 下方向阻碍
			EM_TOTAL,
		};

		enum LandMoveState
		{
			EM_LAND = 0,			// 在陆地上
			EM_FALL,				// 下落 or 下滑
			EM_JUMP,				// 跳跃
		};
	public:
		// 构造函数
		CCharacterProxy();

		// 激活
		void Active();

		// 关闭
		void Decative();

		// FrameMove
		void FrameMove( float fDeltaTime);

		// 改变运动状态
		void SwitchMoveStateBefore();

		// 根据状态与玩家操作计算新的位置
		bool ResponseInputAndCalcNextPos(  Vector3& nextPos, float fDeltaTime);

		// 调整新位置
		void AdjustNextPos( Vector3 nextPos);

		// 当前位置是否应该Fall
		bool IsShouldFall( Vector3 nextPos);

		// 设置是否显示胶囊
		void SetCapsuleVisible( bool visible) { m_showCapsule = visible; }

	private:
		// 调整nextPos 及状态改变
		void AdjustNextPos_Land( Vector3& nextPos);
		void AdjustNextPos_Jump( Vector3& nextPos);
		void AdjustNextPos_Fall( Vector3& nextPos);

		// 位置上下调整
		void AdjustPosUpDown( Vector3& pos, DWORD flag);

		// 位置左右调整(根据坡朝向)
		void AdjustPosSurround( Vector3& pos, DWORD flag = EM_BLOCK_SURROUND, float scale=1.0f);

		// 是否被夹住
		bool IsNip( Vector3& pos);

		// 移动相关函数
	public:
		// 响应输入
		void RespondInput( float fDeltaTime);

		// 摄像机响应输入
		void CameraRespondInput( float fDeltaTime);

		// 设置模拟频率
		void SetStepHZ( float hz=60.f);

		// 碰撞检测
		int CollCheck( Vector3& newPos);

		// 整理碰撞信息
		void TidyCollInfo();

		// 设置位置
		void SetPos( const Vector3& newPos);

		// 设置朝向
		void SetDir();

		// 设置状态
		void SetState( LandMoveState state) { m_moveState = state; }

	private:
		// 辅助,显示capsule()
		void ShowCapsule( Capsule3& capsule);

		// 更新角色动画
		void PlayCharacterAnimation();

		// 获取向量
		Vector3 GetDir( MoveDir dir);

	public:
		// 获取角色位置
		Vector3 GetCharacterPos();

	private:
		ModelPtr				 m_character;			// 角色

		CFollowViewerCamera*	m_camera;				// 跟随主角的摄像机	

		int						 m_collNum;				// 碰撞数量
		vector<SCapsuleCollInfo> m_collInfo;			// 碰撞信息
		char					 m_collFlag[MAX_COLL];	// 0：周围阻碍,    1, 上阻碍,    2, 下阻碍
		int						 m_collFlagNum[EM_TOTAL];// 0：周围阻碍数量 1, 上阻碍数量 2, 下阻碍数量

		SProxyInfo*				 m_info;				// 移动信息

		LandMoveState			 m_moveState;			// 移动状态
		float					 m_timeOnLand;			// 在陆地上的时间

		float					 m_stepDeltaTime;		// 步进时间(1.f / hz)

		SJump*					 m_jump;				// 跳跃管理

		// 辅助显示
		VisualSegment3*			 m_lines[MAX_COLL];
		VisualShape*			 m_capsule;
		bool					 m_showCapsule;			// 是否显示胶囊
	};
}