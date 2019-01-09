#pragma once

#include <stdlib.h>
#include <string.h>
#include <vector>
#include "AresNavMap.h"

using namespace std;

namespace Ares
{
	//------------------------------------
	// A* Algorithm 2011-01-19  帝林
	//------------------------------------
	typedef UINT PolyRef;
	class CAStar
	{
		// A*接点 2011-01-19  帝林
		struct SANode
		{
			int   m_ID;				// 接点的维一标识
			float m_f, m_g, m_h;	// Fitness, goal, heuristic
			int   m_numChildren;	// 子孙数

			SANode*  m_pParent;		// 父接点
			SANode*  m_pNext;		// 用在Open表和Closed表里(看成链表)
			SANode*  m_pChild[8];

			// 构造函数
			SANode( int id)
			{
				m_ID = id;
				m_numChildren = 0;
				m_pParent   = NULL;
				m_pNext     = NULL;
				memset( m_pChild, 0, sizeof(m_pChild));
			}
		};

		// Stack for propagation
		struct SAStack
		{
			SANode*  m_pData;
			SAStack* m_pNext;
		};

		enum EFindState
		{
			EM_FAILE,		// 不存在从起始点到目的地的路径
			EM_CONTINUE,	// 继续
			EM_OK,			// 查找结束
		};

	public:
		CAStar();
		~CAStar();

		// 设置导航图
		void SetNavMap( INavMap* pNavMap);

		// 查找路过的多边形
		size_t FindPath( int startRef, int endRef, const Vector3& startPos, const Vector3& endPos, vector<PolyRef>& path);

		// 获取路径
		SANode* GetBestNode() { return m_pBest; }

	private:
		// 步进
		EFindState  Step();

		// 获取f最小结点
		SANode*  GetBest();

		// LinkChild
		void  LinkChild( SANode* pParent, SANode* pTemp);

		// 处理相连接点
		void DealWithChildren( SANode* pNode);

		// 更新父结点
		void  UpdateParents(SANode* pNode);

		// 添加结点到OpenList
		void AddToOpenList( SANode* pNodeAdd);

		// 检测列表
		SANode* CheckList( SANode* pNodeList, int id);

		// 重置
		bool  StepInitialize( int startRef, int endRef, const Vector3& vStart, const Vector3& vEnd);

		// 清除结点
		void ClearNodes();

		// Statck Fuctions  ( Push)
		void  Push( SANode* pNode);

		// 出栈
		SANode* Pop();

	private:
		int				m_iSID;
		int				m_iDID;			// 目的地ID
		Vector3			m_vStart;		// 开始点
		Vector3			m_vEnd;			// 目的地
		SANode*			m_pOpen;		// open list
		SANode*			m_pClosed;		// closed list
		SANode*			m_pBest;		// 当前最优结点
		SAStack*		m_pStack;
		INavMap*		m_pNavMap;		// 导航图
	};
}