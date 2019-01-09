#pragma once

#include <map>
#include <queue>
#include "M1Animation.h"
#include "Attachment.h"
#include <Core/AresMath.h>
#include "Engine/Foundation/Ares3DEngine_h.h"

using namespace std;
using namespace boost;

namespace Ares
{
	//-----------------------------------
	// 动画控制器 2011-08-10  帝林
	//-----------------------------------
	class CORE_EXPORT AnimationController
	{
		typedef map<string, SAttachment> Attachments; 

	public:
		// playmode
		enum EPlayMode
		{
			STOP_SAMELAYER_DA,	// stop animation that were started in the samelayer and different animation
			STOP_SAMELAYER,		// stop all animations that were started in the samelayer
			STOP_ALL,			// stop all animations that were started with this component before playing
		};

		// queuemode
		enum EQueueMode
		{
			COMPLETE_OTHERS,// this animation will only start once all other animations have stopped playing	
			PLAY_NOW,		// this animation will start playing immediately on a duplicated animation state
		};

		// 动画描述
		struct SAnimationDesc
		{
			bool		m_loop;			// 是否循环播放
			float		m_speed;		// 播放速率
			float		m_weight;		// 权重( 0 - 1)
			int			m_layer;		// 层级,优先级

			SAnimationDesc()
			{
				m_loop	  = false;
				m_speed	  = 1.f;
				m_weight  = 1.f;
				m_layer	  = 0;
			}
		};

		// 动画状态
		struct SAnimationInfo
		{
			int						m_index;		// 索引
			const SM1AnimationSet*	m_animation;	// 当前播放动画
			const SAnimationDesc*	m_desc;			// 动画描述
			float					m_elapseTime;	// 已播放时间
			float					m_totalTime;	// 该动画总时长
			float					m_weight;		// 融合权重
			//vector<bool>			m_boneIdxs;		// 使用的骨骼(为空使用全部)

			// 构造函数
			SAnimationInfo();
		};

		// 融入信息
		struct SFadeInfo
		{
			size_t		m_index;		// 动画索引	
			float		m_duration;		// 融入时长
			EPlayMode	m_playmodel;	// 播放模式
		};

		enum BlendState
		{
			BLENDING,		// 融合中
			BLENDED,		// 融合结束
		};

		enum BlendMode
		{
			FADE,			// 整合到第一帧
			CROSS_FADE,		// 交叉融合
		};

		// 融合控制器
		struct SBlendController
		{
			float				m_elapseTime;		// 已用时间
			float				m_duration;			// 融入时间
			BlendMode			m_blendModel;		// 融入方式
			vector<Transform>	m_sourceTransforms;	// 源数据
			vector<Transform>	m_currTransforms;			// 当前数据
			SAnimationInfo		m_currAnimInfo;		// 融入动画信息
			queue<SFadeInfo>	m_crossFadeQueued;	// 交叉融入队列

			BlendState			m_state;		// 状态

			// 更新
			void Update( float elapsedTime, AnimationController& owner);

		private:
			// 融合
			void Blend( vector<Transform>& out, const vector<Transform>& source, const vector<Transform>& target, float elapseTime, float totalTime);
		};

		typedef map<int, SBlendController, std::greater<int> >  LayerController;

	public:
		// 构造函数
		AnimationController();

		// 析构函数
		~AnimationController();

		// 每帧更新
		void FrameMove( float fTime, const Matrix44& matWorld);

	public:
		// 设置动画
		virtual void SetAnimation( const char* aniPath);

		// 设置动画
		virtual void SetAnimation( s_ptr<CAresAnimation> animation);

		// 获取动画
		virtual s_ptr<CAresAnimation> GetAnimation() { return m_animation; }

	public:
		// 获取动画描述
		virtual SAnimationDesc* GetAnimationDesc( size_t index);

		// 根据名称获取动画描述
		virtual SAnimationDesc* GetAnimationDesc( const char* name);

	public:
		// 添加附属
		void AddAttachment( const char* attachName);

		// 设置附属
		void SetAttachment( const char* attachName, const char* boneName);

		// 删除附属
		void DelAttachment( const char* attachName);

	public:
		// 混合动画


	public:
		// 动画名 是否循环 播放速度(正常速度的百分比) 
		void PlayAnimation( const char* name, bool loop = false, float speed = 1.0f, float blend = 1.0f);

		// 动画索引 是否循环 播放速度(正常速度的百分比) 
		void PlayAnimationByIndex( size_t index, bool loop = false, float speed = 1.0f, float blend = 1.0f);

		// 播放动画
		void PlayAnimation( size_t setIndex, float fSetTotalTime);

	public:
		// 融合
		void Fade( size_t index, float duration=0.3f, EPlayMode playModel=STOP_SAMELAYER_DA);

		// 交叉融合
		void CrossFade( size_t index, float duration=0.3f, EPlayMode playmodel=STOP_SAMELAYER_DA);

		// 交叉融合
		void CrossFade( const char* name, float duration=0.3f, EPlayMode playmodel=STOP_SAMELAYER_DA);

		// 交叉队列融合
		void CrossFadeQueued( size_t index, float duration=0.3f, EQueueMode queueModel=COMPLETE_OTHERS, EPlayMode playmodel=STOP_SAMELAYER_DA);

	private:
		// 融合
		void Fade( size_t index, float duration=0.3f, EPlayMode playmodel=STOP_SAMELAYER_DA, BlendMode blendMode=CROSS_FADE);

	public:
		// 骨骼自定义
		void SetCustomBone( size_t index, const Matrix44& mat);

		// 获取自定义
		bool GetCustomBone( Matrix44& bone, size_t index);

	public:
		// 获取动画数据 
		size_t GetCurrBone( Matrix44* meshBone, const vector<int>& boneIdxs);

		// 获取骨骼的世界矩阵
		const Matrix44& GetCurrBone( int boneIdx);

	private:
		// 附加boneOffset
		void AttachBoneOffset();

		// 更新架构
		void UpdateHierarchy( vector<Transform>& out, const vector<Transform>& source, Transform* fartherData, int index);

		// 层级融合
		void LayerBlend( float elapseTime);

		// 根据融合权重获取最终结果
		void CalcResultByBlendWeight( size_t boneIdx);

		// 计算指定index动画在指定时间t的矩阵
		bool ComputeAnimationTransforms( vector<Transform>& oTransforms, const SM1AnimationSet& animSet, float time, const vector<bool>& boneIdxs=vector<bool>(0));

		// 计算指定index在指定动画的t的矩阵
		bool ComputeAnimationTransform( vector<Transform>& oTransforms, const SM1AnimationSet& animSet, float time, size_t boneIdx);

		// 附加骨骼自定义数据
		void AttachCustomBoneData( Transform& oTransform, size_t boneIdx);

	public:
		// 动画加载完成回调
		void OnAnimationLoaded();

	private:
		Attachments							m_attachments;			// 附属
		s_ptr<CAresAnimation>				m_animation;			// 动画数据
		vector<SAnimationDesc>				m_animationDesc;		// 动画描述	
		LayerController						m_layerController;		// 融合分层控制

		vector<bool>						m_customBone;			// 骨骼是否自定义	
		vector<Matrix44>					m_customMatrixs;		// 自定义矩阵
		vector<Matrix44>					m_resultMatrixs;		// 计算结果
		vector<Transform>					m_resultTransforms;		// 转换

		ShaderParamPtr						m_spabBone;				// 骨骼数据
	};
}