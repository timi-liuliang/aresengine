#include <Engine/RenderSystem/AresModelIDCard.h>
#include <Engine/RenderSystem/M1Animation.h>

namespace Ares
{
	// 构造函数
	CAresAnimation::CAresAnimation()
	{
	}

	// 析构函数
	CAresAnimation::~CAresAnimation()
	{
	}

	// 加载资源
	//bool CAresAnimation::LoadResource()
	//{
		//if ( GetResPath().empty() || IsResourceLoaded())	
		//	return false;

		//if( !this->Load( GetResPath().c_str()))
		//	return false;

		// 标记已加载 可用
		//NotifyLoaded();
		//NotifyEnabled();

	//	return true;
	//}

	// 获取骨骼的所有子及兄弟骨骼索引
	bool CAresAnimation::GetChildSiblingBoneIdxs( size_t parentIdx, vector<size_t>& childIdxs)
	{
		// 记录子结点,兄弟结弟
		int childIdx = m_pRootBone[parentIdx].m_iFirstChildIndex;
		int sibIdx	 = m_pRootBone[parentIdx].m_iSiblingIndex;

		if( childIdx != -1)
		{
			childIdxs.push_back( childIdx);
			GetChildSiblingBoneIdxs( childIdx, childIdxs);
		}

		if(  sibIdx != -1)
		{
			childIdxs.push_back( sibIdx);
			GetChildSiblingBoneIdxs( sibIdx, childIdxs);
		}

		return false;
	}

	// 获取骨骼的所有子骨骼索引
	bool CAresAnimation::GetChildBoneIdxs( size_t parentIdx, vector<size_t>& childIdxs)
	{
		if( m_pRootBone[parentIdx].m_iFirstChildIndex != -1)	
			return GetChildSiblingBoneIdxs( parentIdx, childIdxs);

		return false;
	}

	// 获取SM1AnimationSet结构体
	const SM1AnimationSet* CAresAnimation::GetAnimationSet(size_t index)
	{
		if ( index >= CM1Animation::m_header.m_iNumAnimationSet)
			return NULL;

		return  &m_pAnimationSets[index];

		return NULL;
	}

	// 根据名字获取SM1AnimationSet结构体
	const SM1AnimationSet* CAresAnimation::GetAnimationSet(  const char* name,  size_t* pIndex/*=NULL*/)
	{
		for ( int i=CM1Animation::m_header.m_iNumAnimationSet-1; i>=0; i--)
			if ( strcmp( name, m_pAnimationSets[i].m_header.m_cName) == 0)
			{
				if( pIndex)
					*pIndex = i;

				return &m_pAnimationSets[i];
			}

			return NULL;
	}

	// 根据骨骼名获取骨骼索引
	bool CAresAnimation::GetBoneIdxByName( const char* name, int& boneIdx)
	{
		for ( unsigned int i=0; i<CM1Animation::m_header.m_iNumBones; i++)
		{
			if( strcmp( m_pRootBone[i].m_cName, name) == 0)
			{
				boneIdx = (int)i;

				return true;
			}
		}

		return false;
	}

	// 获取骨骼的世界矩阵
	Matrix44 CAresAnimation::GetBoneWorld( int boneIdx)
	{
		Matrix44 tReturn;

		//D3DXMatrixTranspose( &tReturn, &(D3DXMATRIX&)m_boneMatrixs[boneIdx]);

		return tReturn;
	}

	// 获取动画名
	const char* CAresAnimation::GetAnimationSetName( size_t index)
	{
		if( index < GetNumAnimationSet())
		{
			return m_pAnimationSets[index].m_header.m_cName;
		}

		return NULL;
	}

	// 获取主骨骼
	int CAresAnimation::GetRootBone()
	{
		return 0;
	}

	// 根据索引获取骨骼名
	const char* CAresAnimation::GetBoneName( size_t index)
	{
		return m_pRootBone[index].m_cName;
	}

	// 获取兄弟骨骼
	int CAresAnimation::GetBoneSibingIdx( size_t index)
	{
		return m_pRootBone[index].m_iSiblingIndex;
	}

	// 获取第一个子骨骼
	int CAresAnimation::GetBoneFirstChildIdx( size_t index)
	{
		return m_pRootBone[index].m_iFirstChildIndex;
	}

	// 获取骨骼初始矩阵
	bool CAresAnimation::GetInitBoneTM( Matrix44& out, size_t index)
	{
		if( index < CM1Animation::m_header.m_iNumBones)
		{
			Matrix44Inverse( out, (Matrix44&)m_pRootBone[index].m_boneOffset);

			return true;
		}

		return false;
	}
}