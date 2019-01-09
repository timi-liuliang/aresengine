#pragma once

#include <vector>
//#include "MtlKeeper.h"
#include "Function.h"
#include "IGame.h"
#include "IGameModifier.h"
#include <Engine/RenderSystem/M1/M1Loader.h>
#include <Core/AresMath.h>

using namespace std;
using namespace Ares;

namespace M1_IGame
{
	//----------------------------------------------------
	// MaxScene bone管理 BoneKeeper 帝林 2010-11-08 15:16
	//----------------------------------------------------
	class CBoneKeeper
	{
	public:
		// 构造函数
		CBoneKeeper();

		// 初始化
		void Init( vector<IGameSkin*>& gameSkins);

		// 构建骨骼结构
		void BuildBoneStruct( CM1Animation& m1Animation);

		// 构建动画数据(一次会导出)
		void BuildAnimationSets( CM1Animation& m1Animation);

		// 查找骨骼索引
		int  GetBoneID( IGameNode* pGameNode);

	private:
		// 添加骨骼结点
		void AddBoneNode( IGameNode* pBoneNode, Matrix44& matInit);

		// 查找第一子结点索引
		int  GetFirstChildIndex( IGameNode* pNode);

		// 查找结点的兄弟骨骼
		int  GetSiblingIndex( IGameNode* pNode);

		// 该结点是否为joint
		bool  IsBone( IGameNode* pNode);

		// 是否是主骨骼
		bool  IsRootBone( IGameNode* pGameNode);

		// 构建TransKeys
		//void  BuildKeys( SM1Animation& boneAnimation,IGameNode* pGameNode, Control* pControl, Interval interval);

		// 采样动画
		//void  SampleKeys( SM1Animation& boneAnimation, IGameNode* pGameNode, Interval interval);

		// 获取关键帧时间
		//TimeValue GetKeyTime(Control* control, int i);

	private:
		bool					m_bScale;				// 是否有缩放
		vector<IGameNode*>		m_boneNodes;			// 骨骼结点( 务必保证第一个结点是主结点) 
		vector<Matrix44>		m_boneInitTransform;	// 骨骼初始转换( 把顶点从世界空间转换到骨骼空间中)
	};
}