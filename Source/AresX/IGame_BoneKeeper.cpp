#include "IGame_BoneKeeper.h"
#include "AeConfig.h"
#include <algorithm>

namespace M1_IGame
{
	extern IGameScene *     g_pIGame;
	extern SExportConfig	g_config;	// 导出配置

	// 构造函数
	CBoneKeeper::CBoneKeeper()
	{

	}

	// 获取骨骼的offset matrix
	static GMatrix GetBoneOffset( IGameNode* boneNode)
	{
		GMatrix bT = boneNode->GetWorldTM( 0);

		IGameNode* parent = boneNode->GetNodeParent();

		if( parent)
		{
			GMatrix pT = parent->GetWorldTM( 0);

			return bT * pT.Inverse();
		}
	
		return bT;
	}

	// 初始化
	void CBoneKeeper::Init(  vector<IGameSkin*>& gameSkins)
	{
		// skin中所使用的骨骼
		for ( size_t i=0; i<gameSkins.size(); i++)
		{
			for( int j=0; j<gameSkins[i]->GetTotalSkinBoneCount(); j++)
			{
				IGameNode* pNode = gameSkins[i]->GetIGameBone( j);
				if( pNode)
				{
					GMatrix matInit;

					// 注：若skin未使用此骨骼,则返回false. matInit被设置为单位矩阵
					bool result = gameSkins[i]->GetInitBoneTM( pNode, matInit);

					// 骨骼初始只有旋转与位移
					Ares::Transform initTransform		 = GMatrixToAresTransform( matInit);
					Ares::Matrix44	inintMat			 = initTransform.GetMatrix();
					Matrix44Inverse( inintMat, inintMat);

					AddBoneNode( pNode, inintMat);
				}
			}
		}

		// 无骨骼 退出
		if( !m_boneNodes.size())
			return;

		// 保证第0个结点是主节点
		if( !IsRootBone( m_boneNodes[0]))
		{
			for ( size_t i=1; i<m_boneNodes.size(); i++)
				if( IsRootBone( m_boneNodes[i]))
				{
					IGameNode* tN = m_boneNodes[0];
					m_boneNodes[0] = m_boneNodes[i];
					m_boneNodes[i] = tN;

					std::swap( m_boneInitTransform[0], m_boneInitTransform[i]);

					break;
				}
		}
	}

	// 查找骨骼索引
	int  CBoneKeeper::GetBoneID( IGameNode* pGameNode)
	{
		for ( size_t i=0; i<m_boneNodes.size(); i++)
		{
			if ( pGameNode == m_boneNodes[i])
				return i;
		}
		
		// 不能出现的情况( Bug)
		return 0;
	}

	// 添加骨骼结点
	void CBoneKeeper::AddBoneNode( IGameNode* pBoneNode, Ares::Matrix44& matTransform)
	{
		for ( size_t i=0; i<m_boneNodes.size(); i++)
			if ( pBoneNode == m_boneNodes[i])
			{
				// 保证正确的初始化矩阵
				if( m_boneInitTransform[i].IsIdentity())
					m_boneInitTransform[i] = matTransform;

				return;
			}

		m_boneNodes.push_back( pBoneNode);
		m_boneInitTransform.push_back( matTransform);
	}

	// 构建骨骼结构
	void CBoneKeeper::BuildBoneStruct( CM1Animation& m1Animation)
	{
		m1Animation.m_header.m_iNumBones = m_boneNodes.size();

		if ( !m_boneNodes.size())
			return;

		m1Animation.m_pRootBone = new SM1Bone[ m_boneNodes.size()];

		vector<size_t> rootBones;

		// 复制名字
		for ( size_t i=0; i<m_boneNodes.size(); i++)
		{
			strcpy( m1Animation.m_pRootBone[i].m_cName, m_boneNodes[i]->GetName());

			// 设置第一child
			m1Animation.m_pRootBone[i].m_iFirstChildIndex = GetFirstChildIndex( m_boneNodes[i]);

			// 设置兄弟结点
			m1Animation.m_pRootBone[i].m_iSiblingIndex = GetSiblingIndex( m_boneNodes[i]);

			// InitMatrix
			//Transform tfT = m_boneInitTransform[i];
			//if( IsRootBone( m_boneNodes[i]))
				//tfT.m_rotation.y = -tfT.m_rotation.y;
			//tfT.m_origin.x *= g_config.m_scale.x;
			//tfT.m_origin.y *= g_config.m_scale.y;
			//tfT.m_origin.z *= g_config.m_scale.z;
			std::memcpy( m1Animation.m_pRootBone[i].m_boneOffset, &m_boneInitTransform[i], sizeof(Ares::Matrix44));

			// 记录主骨骼
			if ( IsRootBone( m_boneNodes[i]))
			{
				rootBones.push_back(i);
			}
		}

		// 连接所有主结点
		for ( size_t i=0; i<rootBones.size()-1; i++)
		{
			m1Animation.m_pRootBone[rootBones[i]].m_iSiblingIndex = rootBones[i+1];
		}

		// 验证结果的正确性
		vector<SM1Bone> boneStructs;
		for ( size_t i=0; i<m_boneNodes.size(); i++)
		{
			boneStructs.push_back( m1Animation.m_pRootBone[i]);
		}
		int a = 10;
	}

	// 是否是主骨骼
	bool  CBoneKeeper::IsRootBone( IGameNode* pGameNode)
	{
		IGameNode* pGameParent = pGameNode->GetNodeParent();
		if( !pGameParent)
			return true;

		if( !IsBone( pGameParent))
			return true;

		return false;
	}

	// 构建动画数据(一次会导出)
	void CBoneKeeper::BuildAnimationSets( CM1Animation& m1Animation)
	{
		if ( !m_boneNodes.size())
			return;

		Interval interval;
		interval.SetStart( g_pIGame->GetSceneStartTime());
		interval.SetEnd( g_pIGame->GetSceneEndTime());
		float    fTime = TicksToSec( interval.End() - interval.Start());

		// 修改header
		m1Animation.m_header.m_iNumAnimationSet = 1;

		m1Animation.m_pAnimationSets = new SM1AnimationSet[ m1Animation.m_header.m_iNumAnimationSet];

		strcpy( m1Animation.m_pAnimationSets[0].m_header.m_cName, "default");
		m1Animation.m_pAnimationSets[0].m_header.m_fLenght = fTime;						// 动画时长
		m1Animation.m_pAnimationSets[0].m_header.m_iNumBoneUses = m_boneNodes.size();
		m1Animation.m_pAnimationSets[0].m_pAnimations  = new SM1Animation[m_boneNodes.size()];

		// cycle through every bone( 应该可以优化,某些骨骼在整个过程中未发生变化,无需保存)
		for ( size_t i=0; i<m_boneNodes.size(); i++)
		{
			SM1Animation& boneAnimation = m1Animation.m_pAnimationSets[0].m_pAnimations[i];

			// 骨骼索引
			boneAnimation.m_header.m_iBoneIndex = i;

			Control* pControl = m_boneNodes[i]->GetMaxNode()->GetTMController();
			IGameControl* pGameControl = m_boneNodes[i]->GetIGameControl();
			if ( pControl)
			{
			}
			else
			{
				int a = 10;
			}
		}
	}

	// 查找第一子结点索引
	int  CBoneKeeper::GetFirstChildIndex( IGameNode* pNode)
	{
		for ( int i=0; i<pNode->GetChildCount(); i++)
		{
			if ( IsBone( pNode->GetNodeChild(i)))
			{
				return GetBoneID( pNode->GetNodeChild(i));
			}
		}

		return -1;
	}

	// 查找结点的兄弟骨骼
	int  CBoneKeeper::GetSiblingIndex( IGameNode* pNode)
	{
		IGameNode* pParentNode = pNode->GetNodeParent();
		if ( pParentNode)
			for ( int i=0; i<pParentNode->GetChildCount(); i++)
				if ( pNode == pParentNode->GetNodeChild(i))
				{
					int tIndex = i+1;
					while( tIndex < pParentNode->GetChildCount())
					{
						if ( IsBone( pParentNode->GetNodeChild( tIndex)))
						{
							return GetBoneID( pParentNode->GetNodeChild( tIndex));
						}

						tIndex++;
					}
				}

		return -1;
	}

	// 该结点是否为joint
	bool  CBoneKeeper::IsBone( IGameNode* pNode)
	{
		vector<IGameNode*>::iterator pos = find( m_boneNodes.begin(), m_boneNodes.end(), pNode);
		if( pos == m_boneNodes.end())
			return false;

		return true;
	}
}