#pragma once

#include <string>
#include <IGame.h>
#include <vector>
#include <Engine/RenderSystem/M1/M1Loader.h>

using namespace std;
using namespace Ares;

namespace M1_IGame
{
	//---------------------------------------
	// AnimationManager 帝林 2011-07-26
	//---------------------------------------
	class CAnimationManager
	{
	public:
		// 构造函数
		CAnimationManager();

		// 添加动画
		bool AddAnimation( const string& aniName, int start, int end, CM1Animation& m1Animation);

		// 删除动画
		bool DelAnimation( int aniIndex, CM1Animation& m1Animation);

		// 修改动画名字
		bool EditAnimation( const string& newName, int index,CM1Animation& m1Animation);

		// 交换动画顺序
		bool ExchangeAnimation( int index0, int index1, CM1Animation& m1Animation);

	private:
		// 采样动画
		void SampleKeys( SM1Animation& boneAnimation, IGameNode* pGameNode, Interval interval);

		// 关键帧动画
		void BuildKeys(  SM1Animation& boneAnimation, IGameNode* pGameNode, vector<TimeValue>& keyTimes,Interval interval);

	private:
		// 查找子结点
		void FindChildNodes( IGameNode* parent, vector<IGameNode*>& out);

		// 获取所有接点
		void FindAllNodes( vector<IGameNode*>& out);

		// 获取结点
		IGameNode* GetIGameNodeByName( const char* name, vector<IGameNode*>& nodes);

		// 获取关键帧时间点
		void GetBoneAnimationSetTime( vector<TimeValue>& keyTimes, Interval interval, IGameNode* gameNode);

		// 添加控制器关键时间点
		void AddKeyTime( vector<TimeValue>& keyTimes, Control* contrl, Interval interval);

		// 添加关键时间点
		void AddKeyTime( vector<TimeValue>& keyTimes, TimeValue keyTime);

	private:
		// 判断某结点是否是骨骼
		bool IsBone( IGameNode* gameNode);

		// 判断骨骼是否导出
		bool IsNeedExport( IGameNode* gameNode);

	private:
		// 获取要导出的骨骼父结点
		void GetExportBones();

	private:
		vector<IGameNode*>			m_nodes;			// 所有结点
		vector<IGameNode*>			m_boneNodes;		// 所有骨骼结点
		vector<IGameNode*>			m_exportNodes;		// 当前选中的结点
	};
}