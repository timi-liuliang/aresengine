#include <Engine/AresRoot.h>
#include <Engine/RenderSystem/AresAnimationController.h>
#include <Physics/QuadTreeMember.hpp>

#define BONE_NOTUSE -1		// 骨骼无动画

namespace Ares
{
	// 状态控制构造函数
	AnimationController::SAnimationInfo::SAnimationInfo()
	{		
		m_index			  = -1;
		m_weight		  = 1.f;
		m_elapseTime = 0.f;
		m_totalTime    = 0.f;

		m_desc		   = NULL;
		m_animation = NULL;
	}

	// 构造函数
	AnimationController::AnimationController()
	{
	}

	// 析构函数
	AnimationController::~AnimationController()
	{
		m_attachments.clear();
		m_layerController.clear();
	}

	// 设置动画
	void AnimationController::SetAnimation( const char* aniPath)
	{
		//IResource::LoadListener listener( OnResourceLoaded, this);

		m_animation = AResSystem.LoadAnimation( aniPath);
		//m_animation->AddLoadListener( listener);

		OnAnimationLoaded();
	}

	// 设置动画
	void AnimationController::SetAnimation( s_ptr<CAresAnimation> animation)
	{
		//IResource::LoadListener listener( OnResourceLoaded, this);

		m_animation = animation;
		//if( m_animation)
		//{
		//	m_animation->AddLoadListener( listener);
		//}
	}

	// 动画加载完成回调
	void AnimationController::OnAnimationLoaded()
	{
		size_t numAniSet = m_animation->GetNumAnimationSet();
		size_t numBones  = m_animation->GetNumBones();

		// 动画数量 
		if( numAniSet)
			m_animationDesc.resize( numAniSet);

		if( numBones)
		{
			// 为骨骼计算分配内存, 并初始化为单位矩阵
			m_resultMatrixs.resize( numBones);
			m_customMatrixs.resize( numBones);
			m_resultTransforms.resize( numBones);
			m_customBone.resize( numBones, false);

			for ( size_t i=0; i<numBones; i++)
			{
				m_resultMatrixs[i].SetIdentity();
				m_customMatrixs[i].SetIdentity();
			}
		}

		// 清除状态
		m_layerController.clear();
	}

	// 播放动画
	void AnimationController::PlayAnimation( size_t setIndex, float fSetTotalTime)
	{
		if( setIndex >= m_animationDesc.size())
			return;

		SBlendController& blendController = m_layerController[ m_animationDesc[setIndex].m_layer];

		blendController.m_elapseTime = 0.f;
		blendController.m_state	= BLENDED;

		blendController.m_currAnimInfo.m_index      = setIndex;
		blendController.m_currAnimInfo.m_animation  = &m_animation->m_pAnimationSets[setIndex];
		blendController.m_currAnimInfo.m_desc	    = &m_animationDesc[setIndex];
		blendController.m_currAnimInfo.m_totalTime  = fSetTotalTime;
	}

	// 融合
	void AnimationController::Fade( size_t index, float duration, EPlayMode playModel)
	{
		Fade( index, duration, playModel, FADE);
	}

	// 交叉融合
	void AnimationController::CrossFade( size_t index, float duration/*=0.3f*/, EPlayMode playmodel/*=STOP_SAMELAYER_DA*/)
	{
		Fade( index, duration, playmodel, CROSS_FADE);
	}

	// 交叉融合
	void AnimationController::CrossFade( const char* name, float duration/*=0.3f*/, EPlayMode playmodel/*=STOP_SAMELAYER_DA*/)
	{
		if( !m_animation)
			return;

		// 对应名称索引
		size_t index;
		if( m_animation->GetAnimationSet( name, &index))
		{
			Fade( index, duration, playmodel, CROSS_FADE);
		}
	}

	// 融合
	void AnimationController::Fade( size_t index, float duration, EPlayMode playmodel, BlendMode blendMode)
	{
		if( index >= m_animationDesc.size())
			return;

		SBlendController& blendController = m_layerController[ m_animationDesc[index].m_layer];
		SAnimationInfo& animInfo = blendController.m_currAnimInfo;
		if( playmodel == STOP_SAMELAYER_DA)
		{
			if( animInfo.m_index == index)
				return;
		}

		size_t tNumBones = m_animation->GetNumBones();

		blendController.m_sourceTransforms.resize( tNumBones);
		blendController.m_currTransforms.resize( tNumBones);

		blendController.m_blendModel = blendMode;
		blendController.m_duration	 = duration;
		blendController.m_elapseTime = 0.f;
		blendController.m_state	= BLENDING;

		// 不是全骨骼
		if( !animInfo.m_animation)
			memset( blendController.m_sourceTransforms.data(), 0, sizeof( Transform) * blendController.m_sourceTransforms.size());
		else
			ComputeAnimationTransforms( blendController.m_sourceTransforms, *animInfo.m_animation, animInfo.m_elapseTime/*, animInfo.m_boneIdxs*/);

		animInfo.m_index     = index;
		animInfo.m_animation = &m_animation->m_pAnimationSets[index];
		animInfo.m_desc	     = &m_animationDesc[index];
		animInfo.m_totalTime = blendController.m_currAnimInfo.m_animation->m_header.m_fLenght;
	}

	// 交叉队列融合
	void AnimationController::CrossFadeQueued( size_t index, float duration, EQueueMode queueModel, EPlayMode playmodel)
	{
		if( index >= m_animationDesc.size())
			return;

		// 若无需排队,直接融合
		if( m_layerController.find(  m_animationDesc[index].m_layer) == m_layerController.end())
		{
			CrossFade( index, duration, playmodel);
		}
		else
		{
			SFadeInfo tFadeInfo;
			tFadeInfo.m_index    = index;
			tFadeInfo.m_duration = duration;
			tFadeInfo.m_playmodel= playmodel;

			SBlendController& blendController = m_layerController[ m_animationDesc[index].m_layer];
			blendController.m_crossFadeQueued.push( tFadeInfo);
		}
	}

	// 每帧更新
	void AnimationController::FrameMove( float fTime, const Matrix44& matWorld)
	{
		if( !m_animation)
			return;

		if( !m_layerController.empty())
		{
			LayerBlend( fTime);

			if( !m_layerController.empty())
			{
				// 更新框架数据
				UpdateHierarchy( m_resultTransforms, m_resultTransforms, NULL, 0);

				// 附加骨骼初始矩阵( 转换到骨骼空间)
				AttachBoneOffset();
			}
		}

		// 更新骨骼绑定点物体
		for ( Attachments::iterator it = m_attachments.begin(); it != m_attachments.end(); it++)
		{
			Matrix44 tBoneWorld = GetCurrBone( it->second.m_boneIdx);
			Matrix44 tResult	  = matWorld * tBoneWorld;

			//it->second.m_sceneNode->SetWorldMatrix( tResult);
		}
	}

	// 更新架构
	void AnimationController::UpdateHierarchy( vector<Transform>& out, const vector<Transform>& source, Transform* fartherData, int index)
	{
		if ( !fartherData)
		{
			Transform tfIdentity;

			fartherData = &tfIdentity;		// 默认使用单位矩阵
		}

		out[index] = (*fartherData) * source[index];

		if( m_animation->m_pRootBone[index].m_iFirstChildIndex != -1)	 
			UpdateHierarchy( out , source, &out[index], m_animation->m_pRootBone[index].m_iFirstChildIndex);

		if(  m_animation->m_pRootBone[index].m_iSiblingIndex != -1)
			UpdateHierarchy( out, source, fartherData, m_animation->m_pRootBone[index].m_iSiblingIndex);
	}

	// 附加boneOffset
	void AnimationController::AttachBoneOffset()
	{
		for( size_t i=0; i<m_resultTransforms.size(); i++)
			m_resultMatrixs[i] = m_resultTransforms[i].GetMatrix();

		for( size_t i=0; i<m_resultMatrixs.size(); i++)
		{
			Matrix44 inverseOffset = (Matrix44&)m_animation->m_pRootBone[i].m_boneOffset;

			m_resultMatrixs[i] = inverseOffset * m_resultMatrixs[i];
		}
	}

	// 动画索引 是否循环 播放速度(正常速度的百分比) 
	void AnimationController::PlayAnimationByIndex( size_t index, bool loop /*= false*/, float speed /*= 1.0f*/, float blend /*= 1.0f*/)
	{
		if( !m_animation)
			return;

		const SM1AnimationSet* pAnimSet = m_animation->GetAnimationSet( index);	

		if( !pAnimSet) 
			return;

		// 删除动画状态列表中元素 
		PlayAnimation( index, pAnimSet->m_header.m_fLenght);
	}

	// 设置播放动画状态
	void AnimationController::PlayAnimation( const char *name, bool loop, float speed, float blend)
	{
		if(!name)	return;

		// 查找AnimationSet
		size_t index;
		const SM1AnimationSet* pAnimSet = m_animation->GetAnimationSet( name, &index);	

		if( !pAnimSet) 
			return;

		PlayAnimation( index, pAnimSet->m_header.m_fLenght);
	}

	// 获取动画数据 
	size_t AnimationController::GetCurrBone( Matrix44* meshBone, const vector<int>& boneIdxs)
	{
		if( !m_resultMatrixs.size())
			return false;

		for ( size_t i=0; i<boneIdxs.size(); i++)
		{
			meshBone[i] = m_resultMatrixs[boneIdxs[i]];
		}

		return boneIdxs.size();
	}

	// 获取骨骼的世界矩阵
	const Matrix44& AnimationController::GetCurrBone( int boneIdx)
	{
		return m_resultMatrixs[boneIdx];
	}

	// 获取动画描述
	AnimationController::SAnimationDesc* AnimationController::GetAnimationDesc( size_t index)
	{
		if( index < m_animationDesc.size())
		{
			return &m_animationDesc[index];
		}

		return NULL;
	}

	// 根据名称获取动画描述
	AnimationController::SAnimationDesc* AnimationController::GetAnimationDesc( const char* name)
	{
		// 对应名称索引
		size_t index;
		if( m_animation->GetAnimationSet( name, &index))
		{
			if( index < m_animationDesc.size())
			{
				return &m_animationDesc[index];
			}
		}

		return NULL;
	}

	// 添加附属
	void AnimationController::AddAttachment( const char* attachName)
	{

	}

	// 设置附属
	void AnimationController::SetAttachment( const char* attachName, const char* boneName)
	{

	}

	// 删除附属
	void AnimationController::DelAttachment( const char* attachName)
	{

	}

	// 骨骼自定义
	void AnimationController::SetCustomBone( size_t index, const Matrix44& mat)
	{
		if( mat.IsIdentity())
		{
			m_customBone[index]    = false;
			m_customMatrixs[index] = mat;
		}
		else
		{
			m_customBone[index]	   = true;
			m_customMatrixs[index] = mat;
		}
	}

	// 获取自定义
	bool AnimationController::GetCustomBone( Matrix44& bone, size_t index)
	{
		if( index < m_customMatrixs.size())
		{
			bone = m_customMatrixs[index];

			return true;
		}

		return false;
	}


	struct SMatFactor 
	{
		const Transform*	m_mat;		// 矩阵引用
		float				m_factor;	// 因子

		// 构造函数
		SMatFactor( const Transform& mat, float factor)
		{
			m_mat	 = &mat;
			m_factor = factor;
		}
	};

	void AnimationController::SBlendController::Update( float fTime,  AnimationController& owner)
	{
		// 更新进度
		if( m_state == BLENDING)
		{
			m_elapseTime += fTime * m_currAnimInfo.m_desc->m_speed;
			m_currAnimInfo.m_weight = clamp(m_elapseTime / m_duration * m_currAnimInfo.m_desc->m_weight, 0.f, 1.f);

			if( m_elapseTime > m_duration)
			{
				m_state = BLENDED;

				//if( m_blendModel == CROSS_FADE)
				//	m_currAnim.m_fElapseTime = m_elapseTime;
				//else
				//	m_currAnim.m_fElapseTime = m_elapseTime - m_duration;
			}
		}
		else if( m_state == BLENDED)
		{
			m_currAnimInfo.m_elapseTime += fTime * m_currAnimInfo.m_desc->m_speed;

			// 播放结束,融合回去
			if( !m_currAnimInfo.m_desc->m_loop /*&& m_crossFadeQueued.empty()*/)
			{
				if( m_currAnimInfo.m_elapseTime > m_currAnimInfo.m_totalTime-m_duration)
				{
					m_currAnimInfo.m_weight = clamp((m_currAnimInfo.m_totalTime - m_currAnimInfo.m_elapseTime) / m_duration  * m_currAnimInfo.m_desc->m_weight, 0.f, 1.f);
				}
			}

			if( m_currAnimInfo.m_elapseTime > m_currAnimInfo.m_totalTime)
			{
				if( m_currAnimInfo.m_desc->m_loop)
					m_currAnimInfo.m_elapseTime = fmodf( m_currAnimInfo.m_elapseTime, m_currAnimInfo.m_totalTime);
				else
				{
					if( m_crossFadeQueued.empty())
						m_currAnimInfo.m_animation = NULL;
					else
					{
						SFadeInfo& fadeInfo = m_crossFadeQueued.front();

						owner.CrossFade( fadeInfo.m_index, fadeInfo.m_duration, fadeInfo.m_playmodel);
						m_crossFadeQueued.pop();
					}
				}
			}
		}

		// 开始融合
		if( m_state == BLENDING)
		{
			// 计算目标骨骼数据
			if( m_blendModel == CROSS_FADE)
				m_currAnimInfo.m_elapseTime = m_elapseTime;

			// 初始化当前矩阵
			owner.ComputeAnimationTransforms( m_currTransforms, *m_currAnimInfo.m_animation, m_currAnimInfo.m_elapseTime/*, m_currAnimInfo.m_boneIdxs*/);

			// 融合
			Blend( m_currTransforms, m_sourceTransforms, m_currTransforms, m_elapseTime, m_duration);
		}
		else if( m_state == BLENDED && m_currAnimInfo.m_animation)
		{
			owner.ComputeAnimationTransforms( m_currTransforms, *m_currAnimInfo.m_animation, m_currAnimInfo.m_elapseTime/*, m_currAnimInfo.m_boneIdxs*/);
		}
	}

	// 融合
	void AnimationController::SBlendController::Blend( vector<Transform>& out, const vector<Transform>& source, const vector<Transform>& target, float elapseTime, float totalTime)
	{
		float factor = clamp( elapseTime / totalTime, 0.f, 1.f);

		for ( size_t i=0; i<out.size(); i++)
			out[i] = source[i] + ( target[i]-source[i]) * factor;
	}

	// 层级融合
	void AnimationController::LayerBlend( float elapseTime)
	{
		// 每层更新
		for ( LayerController::iterator it=m_layerController.begin(); it!=m_layerController.end(); it++)
		{
			it->second.Update( elapseTime, *this);
		}

		// 清除无用层
		for ( LayerController::iterator it=m_layerController.begin(); it!=m_layerController.end();)
		{
			if( !it->second.m_currAnimInfo.m_animation)
            {
#ifdef ARES_PLATFORM_WINDOWS
				it = m_layerController.erase( it);
#endif
                A_ASSERT(false);
            }
			else
				it++;
		}

		// 各层之间再融合,获取最终结果
		if( m_layerController.empty())
			return;

		if( m_layerController.size() == 2)
			int a = 10;

		// 计算结果矩阵
		for ( size_t i=0; i<m_resultMatrixs.size(); i++)
		{
			CalcResultByBlendWeight( i);
		}
	}

	// 根据融合权重获取最终结果
	void AnimationController::CalcResultByBlendWeight(  size_t boneIdx)
	{
		// 计算层间权重
		float tFactor   = 1.f;
		float curFactor = 0.f;
		vector<SMatFactor> factors;
		LayerController::iterator itNext;
		for ( LayerController::iterator it=m_layerController.begin(); it!=m_layerController.end(); it++)
		{
			if( tFactor > 0.f)
			{
				itNext = it; 
				if( ++itNext !=m_layerController.end())
				{		
					if( it->second.m_currAnimInfo.m_animation->m_boneUsed[boneIdx])
					{
						curFactor =  tFactor * it->second.m_currAnimInfo.m_weight;
						tFactor   -= curFactor;
						factors.push_back( SMatFactor(it->second.m_currTransforms[boneIdx], curFactor));
					}
				}
				else
				{
					// 最后一个必有混合因子
					factors.push_back( SMatFactor(it->second.m_currTransforms[boneIdx], tFactor));
				}			
			}
		}

		// 计算结果
		Transform result = (*factors[0].m_mat) * factors[0].m_factor;
		for ( size_t i=1; i<factors.size(); ++i)
		{
			result += (*factors[i].m_mat) * factors[i].m_factor;
		}

		m_resultTransforms[boneIdx] = result;
	}

	// 计算指定index动画在指定时间t的矩阵
	bool AnimationController::ComputeAnimationTransforms( vector<Transform>& oTransforms, const SM1AnimationSet& animSet, float time, const vector<bool>& boneIdxs)
	{
		size_t tAnimBoneUses = animSet.m_header.m_iNumBoneUses;

		if( !boneIdxs.empty())
		{
			// 部分骨骼
			for ( size_t i=0; i<tAnimBoneUses; i++)
			{
				if( boneIdxs[i])
					ComputeAnimationTransform( oTransforms, animSet, time, boneIdxs[i]);
			}
		}
		else
		{
			// 使用所有骨骼
			for ( size_t i=0; i<tAnimBoneUses; i++)
			{
				ComputeAnimationTransform( oTransforms, animSet, time, i);
			}
		}

		return true;
	}

	// 计算指定index在指定动画的t的矩阵
	bool AnimationController::ComputeAnimationTransform( vector<Transform>& oTransforms, const SM1AnimationSet& animSet, float time, size_t boneIdx)
	{
		const SM1Animation&       currAnim       = animSet.m_pAnimations[boneIdx];
		const SM1AnimationHeader& currAnimHeader = currAnim.m_header;

		// 当前动画未使用此骨骼
		if( currAnimHeader.m_iBoneIndex == BONE_NOTUSE)
			return false;

		// 单位化
		Transform& transform = oTransforms[currAnimHeader.m_iBoneIndex];

		//// 应用缩放帧
		//if ( currAnimHeader.m_iNumScaleKeys)
		//{
		//	size_t key1 = 0, key2 = 0;

		//	// 待优化 快速插值
		//	for ( size_t i=0; i<currAnimHeader.m_iNumScaleKeys; i++)
		//	{
		//		if( time >= currAnim.m_pScaleKeys[i].m_fTime)
		//			key1 = i;
		//		else
		//			break;
		//	}

		//	key2 = key1 > ( currAnimHeader.m_iNumScaleKeys - 1)? key1 : key1 + 1;

		//	float timeDiff = currAnim.m_pScaleKeys[key2].m_fTime - currAnim.m_pScaleKeys[key1].m_fTime;

		//	if( timeDiff == 0.f)
		//		timeDiff = 1.f;

		//	float scalar = ( time - currAnim.m_pScaleKeys[key1].m_fTime) / timeDiff;

		//	Vector3 vScale = ( (Vector3&)currAnim.m_pScaleKeys[key2].m_vecKey - (Vector3&)currAnim.m_pScaleKeys[key1].m_vecKey) * scalar;
		//	vScale += (Vector3&)currAnim.m_pScaleKeys[key1].m_vecKey;

		//	Matrix44 matScale;
		//	Matrix44Scaling( matScale, vScale.x, vScale.y, vScale.z);
		//}

		// 应用旋转帧
		if ( currAnimHeader.m_iNumRotationKeys)
		{
			size_t key1 = 0, key2 = 0;

			// 待优化 快速插值
			for ( size_t i=0; i<currAnimHeader.m_iNumRotationKeys; i++)
			{
				if( time >= currAnim.m_pRotationKeys[i].m_fTime)
					key1 = i;
				else
					break;
			}

			key2 = key1 > ( currAnimHeader.m_iNumRotationKeys - 1)? key1 : key1 + 1;

			float timeDiff = currAnim.m_pRotationKeys[key2].m_fTime - currAnim.m_pRotationKeys[key1].m_fTime;
			float scalar = ( time - currAnim.m_pRotationKeys[key1].m_fTime) / timeDiff;

			// 四元数插值
			Quat quatSD[2];
			quatSD[0] = (Quat&)currAnim.m_pRotationKeys[key1].m_quatKey;
			quatSD[1] = (Quat&)currAnim.m_pRotationKeys[key2].m_quatKey;

			transform.m_rotation = QuatSlerp( quatSD[0], quatSD[1], scalar);
		}

		// 应用平移帧
		if ( currAnimHeader.m_iNumTransKeys)
		{
			size_t key1 = 0, key2 = 0;

			// 待优化 快速插值
			for ( size_t i=0; i<currAnimHeader.m_iNumTransKeys; i++)
			{
				if( time >= currAnim.m_pTransKeys[i].m_fTime)
					key1 = i;
				else
					break;
			}

			key2 = key1 > ( currAnimHeader.m_iNumTransKeys - 1)? key1 : key1 + 1;

			float timeDiff = currAnim.m_pTransKeys[key2].m_fTime - currAnim.m_pTransKeys[key1].m_fTime;

			if( timeDiff == 0.f)
				timeDiff = 1.f;

			float scalar = ( time - currAnim.m_pTransKeys[key1].m_fTime) / timeDiff;

			Vector3 tTrans = ( (Vector3&)currAnim.m_pTransKeys[key2].m_vecKey - (Vector3&)currAnim.m_pTransKeys[key1].m_vecKey) * scalar;
			tTrans += (Vector3&)currAnim.m_pTransKeys[key1].m_vecKey;

			transform.m_origin = tTrans;
		}

		//// 矩阵处理
		//if ( currAnimHeader.m_iNumMatKeys)
		//{
		//	size_t key1 = 0, key2 = 0;

		//	// 待优化 快速插值
		//	for ( size_t i=0; i<currAnimHeader.m_iNumMatKeys; i++)
		//	{
		//		if( time >= currAnim.m_pMatKeys[i].m_fTime)
		//			key1 = i;
		//		else
		//			break;
		//	}

		//	key2 = key1 > ( currAnimHeader.m_iNumMatKeys - 1)? key1 : key1 + 1;

		//	// 两帧间时长间隔
		//	float timeDiff = currAnim.m_pMatKeys[key2].m_fTime - currAnim.m_pMatKeys[key1].m_fTime;
		//	if( timeDiff == 0.f)
		//		timeDiff = 1.f;

		//	float scalar = ( time - currAnim.m_pMatKeys[key1].m_fTime) / timeDiff;

		//	Matrix44 matResult; 
		//	matResult  = ((Matrix44&)currAnim.m_pMatKeys[key2].m_matKey - (Matrix44&)currAnim.m_pMatKeys[key1].m_matKey) * scalar;
		//	matResult += ( Matrix44&)currAnim.m_pMatKeys[key1].m_matKey;

		//	currBone *= matResult;
		//}

		// 附加自定义数据

		AttachCustomBoneData( transform, boneIdx);

		return true;
	}

	// 附加骨骼自定义数据
	void AnimationController::AttachCustomBoneData( Transform& oTransform, size_t boneIdx)
	{
		if( !m_customBone[boneIdx])
			return;

		// 分解旋转缩放平移
		//Vector3    tScale;
		//D3DXQUATERNION tRotate;
		//Vector3	   tTrans;
		//D3DXMatrixDecompose( &tScale, &tRotate, &tTrans, &out);

		//// 分解旋转缩放平移
		//Vector3    tScale1;
		//D3DXQUATERNION tRotate1;
		//Vector3	   tTrans1;
		//D3DXMatrixDecompose( &tScale1, &tRotate1, &tTrans1, &m_customMatrixs[boneIdx]);

		//tScale.x = tScale.x * tScale1.x;
		//tScale.y = tScale.y * tScale1.y;
		//tScale.z = tScale.z * tScale1.z;

		//tTrans = tTrans + tTrans1;

		//D3DXMatrixTransformation( &out, NULL, NULL, &tScale, NULL, &tRotate, &tTrans);
	}
}