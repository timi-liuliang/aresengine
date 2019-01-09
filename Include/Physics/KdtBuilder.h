#pragma once

#include "Kdt.h"
#include <deque>

using namespace std;

namespace Ares
{
	// 成员索引列表,辅助Kdt生成
	class MemberList
	{
	public:
		// 构造函数
		MemberList() : m_next(NULL) {}

		// 析构函数
		~MemberList() { SAFE_DELETE(m_next); }

		// 设置对象索引
		void SetMemIdx( size_t member) { m_memIdx = member; }

		// 获取对象
		const size_t GetMemIdx() { return m_memIdx; }

		// 设置下一个
		void SetNext( MemberList* next) { m_next=next; }

		// 获取下一个
		MemberList* GetNext() { return m_next; }

		// 获取尺寸
		int GetSize() { if(m_next) return 1+m_next->GetSize(); else return 1; }

	private:
		size_t		m_memIdx;
		MemberList*	m_next;
	};

	// MemberData
	struct MemberData
	{
		Rect3		m_aabb;
		size_t		m_idx;

		// 初始化
		void Init( const vector<Rect3>& keys, size_t idx);

		// 重建并裁剪
		void Clip( Rect3& abox);

		// 是否还有空间
		bool IsHaveSpace() { return m_aabb.GetXSize()>0.f && m_aabb.GetYSize()>0.f && m_aabb.GetZSize()>0.f;}
	};

	// 盒子, Surface Area Heuristic
	struct kdtbox
	{
		UINT	m_n[3];		// 记录下一个接点与朝向
		float   m_pos[3];	// 12

		// 下一个
		kdtbox* GetNext( int a) { return (kdtbox*)( m_n[a]&(0xffffffff - 3));}

		// 下一个
		void SetNext( int a, kdtbox* p) { m_n[a] = ( m_n[a] & 3) + (UINT)p; }

		// Side
		int GetSide( int a) { return m_n[a] & 3; }

		// Side
		void SetSide( int a, int s) { m_n[a] = ( m_n[a] & (0xffffffff - 3)) + s; }
	};

	// 盒子
	struct KdtBox 
	{ 
		enum 
		{ 
			END, 
			PLANAR, 
			DISABLE, 
			START 
		};

		// 标记
		enum 
		{ 
			LEFTLIST, 
			RIGHTLIST, 
			PROCESSED, 
			STRADDLING 
		}; 

		kdtbox			m_side[2];		// 48 
		size_t			m_memIdx;		// 4 
		unsigned int	m_flags;		// 4 
		KdtBox*			m_clone;		// 4 
		int				m_dummy;		// 4, total = 64 

		// 构造函数
		KdtBox()
		{
			std::memset( this, 0, sizeof(*this));
		}
	}; 

	// 盒子列表
	struct KdtBoxList 
	{ 
		kdtbox* m_head[3];
		kdtbox* m_tail[3];

		// 构造函数
		KdtBoxList();

		// 初始化
		void Init();

		// 排序
		void Sort( int a ); 

		// 插入
		void Insert( int a, kdtbox* n ); 

		// 移除
		void Remove( int a, kdtbox* n ); 
	}; 

	// KdtMManager
	class KdtMManager
	{
	public:
		KdtMManager();
		~KdtMManager();

		// 新建成员列表
		MemberList* NewMemberList();

		// 新建KdtBox
		KdtBox* NewKdtBox();

		// 新建KdtBoxList
		KdtBoxList* NewKdtBoxList();

	public:
		UINT		m_eFree;
		UINT		m_eReq;
		KdtBox*		m_ePtr;

		UINT		m_elFree;
		UINT		m_elReq;
		KdtBoxList* m_elPtr;

		UINT		m_mReq;
		MemberList*	m_mList;
		MemberList*	m_mPtr;

	};

	// 辅助结构体,去掉递归
	template <typename T>
	struct SAHParam
	{
		KdtNode<T>*	 m_node;		// 结点
		Rect3		 m_box;			// 包围盒
		int			 m_depth;		// 深度
		int			 m_numMembers;	// 成员数
		KdtBoxList*	 m_kdtBoxList;	// BoxList, 具体用途不详
	};
	
	//------------------------------------
	// KDTreeBuilder 2012-11-23 帝林
	// 辅助KDTree生成
	//------------------------------------
	template <typename T>
	class KdtBuilder  
	{
	public:
		KdtBuilder( const vector<Rect3>& keys, const vector<T>& members, Kdt<T>& kdt);
		~KdtBuilder();

	private:
		// 构建
		void InitBuild();

		// 获取叶子结点所记录的成员总数(Recursive)
		int CountNumLeafMembers( const KdtNode<T>* node);

		// 构建对象列表
		void BuildMemberList( KdtNode<T>* node, int depth);

		// 构建树
		void BuildTree( MemberList* memberList, KdtNode<T>* node, Rect3& rbox, int depth);

		// 分隔算法 Surface Area Heuristic
		void SubdivNewONlogN( KdtNode<T>* anode, Rect3& rbox, int depth, int numMembers, deque<SAHParam<T> >& remaining);

		// 获取成员列表
		MemberList* GetMemberList( const KdtNode<T>* node);

		// 向结点中添加成员
		void NodeAddMember( const KdtNode<T>* node, size_t memIdx);

		// 添加KdTree结点
		size_t NewKdTreeNodePair();

		typedef map<const KdtNode<T>*, MemberList*> NMLMap;
	private:
		vector<KdtNode<T> >	m_root;			// 主结点
		vector<T>			m_memberList;	// 成员列表

		const vector<Rect3>&m_keys;			// 索引数据
		const vector<T>&	m_members;		// 数据成员
		int					m_curMember;	// 当前对像
		int					m_intrCost;		// 相交消耗,分隔评估
		int					m_travCost;		// 遍历消耗
		int					m_maxDepth;		// 最大深度,在特定尝试停止分隔,以控制内存的使用和构建时间
		int					m_pPerLeaf;		// 叶结点
		KdtBoxList*			m_eList;		// 列表
		NMLMap				m_nodeMLists;	// 结点成员列表
		MemberData			m_memData;		// 成员数据
		KdtMManager			m_memMgr;		// 内存管理器
	};

	// 构造函数
	template <typename T>
	KdtBuilder<T>::KdtBuilder( const vector<Rect3>& keys, const vector<T>& members, Kdt<T>& kdt)
		: m_keys( keys)
		, m_members( members)
		, m_eList( NULL)
	{
		A_ASSERT( m_keys.size()==m_members.size());

		m_root.reserve( members.size()*4);
		m_memberList.reserve( members.size() * 4);
		m_travCost = 1;
		m_intrCost = 3;
		m_maxDepth = 20;
		m_pPerLeaf = 8;

		// 默认主结点
		m_root.push_back(KdtNode<T>());

		m_curMember = 0;
		InitBuild();
		if( !m_memberList.size())
		{
			int tReq = CountNumLeafMembers( m_root.data());
			//tReq += (tReq>>2) | 256;
			m_memberList.resize( tReq);
		}

		BuildMemberList( m_root.data(), 0);

		kdt.m_memberList = m_memberList;
		kdt.m_root		 = m_root;
	}

	// 析构函数
	template <typename T>
	KdtBuilder<T>::~KdtBuilder()
	{}

	// 获取成员列表
	template <typename T>
	MemberList* KdtBuilder<T>::GetMemberList( const KdtNode<T>* node)
	{
		typename NMLMap::iterator it=m_nodeMLists.find( node);
		if( it!=m_nodeMLists.end())
			return it->second;

		return NULL;
	}

	// 向结点中添加成员
	template <typename T>
	void KdtBuilder<T>::NodeAddMember( const KdtNode<T>* node, size_t memIdx)
	{
		MemberList* memList = m_memMgr.NewMemberList();
		memList->SetMemIdx( memIdx);
		memList->SetNext( GetMemberList( node));
		m_nodeMLists[node] = memList;
	}

	// 添加KdTree结点
	template <typename T>
	size_t KdtBuilder<T>::NewKdTreeNodePair()
	{
		A_ASSERT( m_root.size()+2 < m_root.capacity());

		m_root.push_back( KdtNode<T>());
		m_root.push_back( KdtNode<T>());

		return m_root.size() - 2;
	}

	// 获取叶子结点所记录的成员总数(Recursive)
	template <typename T>
	int KdtBuilder<T>::CountNumLeafMembers( const KdtNode<T>* node)
	{
		int numMembers = 0;
		if( node->IsLeaf())
		{
			// 当前结点成员数
			MemberList* memberList = GetMemberList( node);
			while( memberList)
			{
				numMembers++;
				memberList = memberList->GetNext();
			}
		}
		else
		{
			// 左右接点成员数
			const KdtNode<T>* left = node->GetLeft( m_root);
			const KdtNode<T>* right= node->GetRight( m_root);
			if( left)
				numMembers += CountNumLeafMembers( left);
			if( right)
				numMembers += CountNumLeafMembers( right);
		}

		return numMembers;
	}

	// 构建对象列表
	template <typename T>
	void KdtBuilder<T>::BuildMemberList( KdtNode<T>* node, int depth)
	{
		if( node->IsLeaf())
		{
			// Convert primitives in leaves to triAccels
			int first = m_curMember;
			int count = 0;
			MemberList* memberList = GetMemberList( node);
			while( memberList)
			{
				m_memberList[m_curMember++] = m_members[memberList->GetMemIdx()];
				memberList = memberList->GetNext();

				count++;
			}

			if( count>127)
			{
				count = 127;
				//A_ASSERT( FALSE);
			}

			// 记录起始位置与数量
			node->SetMemberList( first, count);
		}
		else
		{
			KdtNode<T>* left = (KdtNode<T>*)node->GetLeft( m_root);
			KdtNode<T>* right= (KdtNode<T>*)node->GetRight( m_root);
			if( left) BuildMemberList( left, depth+1);
			if( right)BuildMemberList( right,depth+1);
		}
	}

	// 构建
	template<typename T>
	void KdtBuilder<T>::InitBuild()
	{
		MemberList* mlist=NULL, *tail=NULL;
		for ( size_t i=0; i<m_members.size(); i++)
		{
			MemberList* newNode = m_memMgr.NewMemberList();
			newNode->SetMemIdx( i);
			newNode->SetNext( 0);
			if( !tail)
			{
				tail = newNode;
				mlist= newNode;
			}
			else
			{
				tail->SetNext( newNode);
				tail = newNode;
			}
		}

		Rect3 sceneBox;
		for ( size_t i=0; i<m_keys.size(); i++)
		{
			sceneBox.UnionPoint( m_keys[i].GetMinPoint());
			sceneBox.UnionPoint( m_keys[i].GetMaxPoint());
		}

		BuildTree( mlist, m_root.data(), sceneBox, 0);
	}

	// 构建树
	template <typename T>
	void KdtBuilder<T>::BuildTree( MemberList* memberList, KdtNode<T>* node, Rect3& rbox, int depth)
	{
		m_eList = m_memMgr.NewKdtBoxList();
		m_eList->Init();

		MemberList* ml  = memberList;
		int		    events = 0;
		float		pos[2];
		KdtBox*		box;
		KdtBox*		next  = m_memMgr.NewKdtBox();
		KdtBox*		first = next;
		// 根据成员列表生成KdtBox
		while( ml)
		{
			m_memData.Init( m_keys, ml->GetMemIdx());
			box			 = next;
			next		 = m_memMgr.NewKdtBox();
			box->m_memIdx= ml->GetMemIdx();
			box->m_flags = KdtBox::STRADDLING;
			for ( int a=0; a<3; a++)
			{
				pos[0] = m_memData.m_aabb.GetMinPoint()[a];
				pos[1] = m_memData.m_aabb.GetMaxPoint()[a];
				box->m_side[0].m_pos[a] = pos[0];
				box->m_side[0].SetNext( a, &box->m_side[1]);
				box->m_side[0].SetSide( a, KdtBox::START);
				box->m_side[1].m_pos[a] = pos[1];
				box->m_side[1].SetNext( a, &next->m_side[0]);
				box->m_side[1].SetSide( a, KdtBox::END);
				if ( pos[0] == pos[1]) 
				{
					box->m_side[0].SetSide( a, KdtBox::PLANAR);
					box->m_side[1].SetSide( a, KdtBox::PLANAR);
					box->m_side[0].SetNext( a, box->m_side[1].GetNext( a));
				}
			}

			ml = ml->GetNext();
		}

		for ( int a=0; a<3; a++)
		{
			m_eList->m_head[a] = &first->m_side[0];
			box->m_side[1].SetNext( a, 0);
			if (box->m_side[1].GetSide(a) == KdtBox::PLANAR) 
				box->m_side[0].SetNext( a, 0);

			m_eList->Sort( a);
		}

		// 执行分隔计算
		SAHParam<T>		rootParam;
		deque<SAHParam<T> > remaining;
		rootParam.m_node	   = node;
		rootParam.m_box		   = rbox;
		rootParam.m_depth	   = 0;
		rootParam.m_numMembers = m_members.size();
		rootParam.m_kdtBoxList = m_eList;
		remaining.push_back( rootParam);
		while( remaining.size())
		{
			SAHParam<T> sahParam = remaining.front(); 
			remaining.pop_front();
			m_eList = sahParam.m_kdtBoxList;

			SubdivNewONlogN( sahParam.m_node, sahParam.m_box, sahParam.m_depth, sahParam.m_numMembers, remaining);
		}
	}

	// Surface Area Heuristic
	template <typename T>
	void KdtBuilder<T>::SubdivNewONlogN(  KdtNode<T>* anode, Rect3& rbox, int depth, int numMembers, deque<SAHParam<T> >& remaining)
	{
		// best cost calculation
		const float NA = rbox.GetXSize() * rbox.GetZSize() + rbox.GetXSize() * rbox.GetYSize() + rbox.GetZSize() * rbox.GetYSize();
		const float iNA = m_intrCost / NA, minf = 0.1f + 0.01f * depth;
		float bestcost = m_intrCost * (float)numMembers, bestpos = -1.0f;
		int bestaxis = -1, bestside = -1;
		int bestTL, bestTR, bestTP;
		for ( int a = 0; a < 3; a++ )
		{
			if ((rbox.GetMaxPoint()[a] - rbox.GetMinPoint()[a]) > ZERO_TOLERANCE)
			{
				const float w = 1.0f / ( rbox.GetMaxPoint()[a] - rbox.GetMinPoint()[a]);
				int TL = 0, TR = numMembers, TP = 0;
				Vector3 lsize = rbox.GetSize();
				Vector3 rsize = lsize;
				kdtbox* el = m_eList->m_head[a];
				while (el)
				{
					const float pos = el->m_pos[a];
					int pl = 0, pr = 0;
					while ((el) && (el->m_pos[a] == pos) && (el->GetSide(a) == KdtBox::END)) { pl++; el = el->GetNext(a); }
					while ((el) && (el->m_pos[a] == pos) && (el->GetSide(a) == KdtBox::PLANAR)) { TP++; el = el->GetNext(a); }
					while ((el) && (el->m_pos[a] == pos) && (el->GetSide(a) == KdtBox::START)) { pr++; el = el->GetNext(a); }
					TR -= (TP + pl);
					lsize[a] = pos - rbox.GetMinPoint()[a];
					rsize[a] = rbox.GetMaxPoint()[a] - pos;
					const float LA = lsize.x * lsize.y + lsize.x * lsize.z + lsize.y * lsize.z;
					const float RA = rsize.x * rsize.y + rsize.x * rsize.z + rsize.y * rsize.z;
					const float f1 = (pos - rbox.GetMinPoint()[a]) * w;
					const float f2 = (rbox.GetMaxPoint()[a] - pos) * w;
					float bonus1 = 1.0f, bonus2 = 1.0f, bound = 0.1f;
					const float cost1 = bonus1 * ( m_travCost + iNA * (LA * (TL + TP) + RA * TR));
					const float cost2 = bonus2 * ( m_travCost + iNA * (LA * TL + RA * (TR + TP)));
					if (cost1 < bestcost)
					{
						bestcost = cost1, bestpos = pos;
						bestside = 0, bestaxis = a;
						bestTL = TL, bestTR = TR, bestTP = TP;
					}
					if (cost2 < bestcost)
					{
						bestcost = cost2, bestpos = pos;
						bestside = 1, bestaxis = a;
						bestTL = TL, bestTR = TR, bestTP = TP;
					}
					TL += (pr + TP);
					TP = 0;
				}
			}
		}
		
		if (bestside == -1)
		{
			// store primitives in leaf
			kdtbox* el = m_eList->m_head[0];
			while (el)
			{
				KdtBox* eb = (KdtBox*)(((UINT)el) & (0xffffffff - 63));
				if (eb->m_flags != KdtBox::PROCESSED)
				{
					NodeAddMember( anode, eb->m_memIdx);
					eb->m_flags = KdtBox::PROCESSED;
				}
				el = el->GetNext( 0);
			}
			return;
		}
		// best cost calculated; build child nodes
		int count[2] = { bestTL + ((bestside == 0)?bestTP:0), bestTR + ((bestside == 1)?bestTP:0) };
		Rect3 box[2] = { rbox, rbox};
		box[0].GetMaxPoint()[bestaxis] = bestpos;
		box[1].GetMinPoint()[bestaxis] = bestpos;
		size_t nodeIdxs[2];
		nodeIdxs[0] = NewKdTreeNodePair();
		nodeIdxs[1] = nodeIdxs[0] + 1;
		// classify EBoxes (all set to straddling by final pass)
		kdtbox* el = m_eList->m_head[bestaxis];
		while (el)
		{
			const int side = el->GetSide(bestaxis);
			KdtBox* eb = (KdtBox*)(((UINT)el) & (0xffffffff - 63));
			if ((side == KdtBox::END) && (el->m_pos[bestaxis] <= bestpos))
				eb->m_flags = KdtBox::LEFTLIST;
			else if ((side == KdtBox::START) && (el->m_pos[bestaxis] >= bestpos))
				eb->m_flags = KdtBox::RIGHTLIST;
			else if (side == KdtBox::PLANAR)
			{
				if (bestside == 0)
					eb->m_flags = (el->m_pos[bestaxis] <= bestpos)? KdtBox::LEFTLIST : KdtBox::RIGHTLIST;
				else
					eb->m_flags = (el->m_pos[bestaxis] >= bestpos)? KdtBox::RIGHTLIST : KdtBox::LEFTLIST;
			}
			el = el->GetNext( bestaxis );
		}
		el = m_eList->m_head[bestaxis];

		// Split elist to cl[0] and cl[1]
		KdtBoxList* cl[2];
		cl[0] = m_memMgr.NewKdtBoxList();
		cl[1] = m_memMgr.NewKdtBoxList();
		for ( int a = 0; a < 3; a++)
		{
			kdtbox* el = m_eList->m_head[a], *llp = 0, *rlp = 0;
			while (el)
			{
				kdtbox* next = el->GetNext( a);
				KdtBox* eb = (KdtBox*)((( UINT)el) & (0xffffffff - 63));
				if (eb->m_flags == KdtBox::LEFTLIST)
				{
					if (llp) 
						llp->SetNext( a, el ); 
					else 
					{ 
						llp = el; 
						cl[0]->m_head[a] = el; 
					}

					el->SetNext( a, 0);
					llp = el;
				}
				else if (eb->m_flags == KdtBox::RIGHTLIST)
				{
					if (rlp) 
						rlp->SetNext( a, el ); 
					else 
					{ 
						rlp = el; 
						cl[1]->m_head[a] = el; 
					}

					el->SetNext( a, 0);
					rlp = el;
				}
				else
				{
					eb->m_flags = KdtBox::PROCESSED;
					if (!eb->m_clone)
					{
						eb->m_clone = m_memMgr.NewKdtBox();
						std::memcpy( eb->m_clone, eb, 64);
					}
					const int oside = (el->GetSide( a) == KdtBox::END)?1:0;
					kdtbox* clel = &eb->m_clone->m_side[oside];
					if (llp) 
						llp->SetNext( a, el ); 
					else 
					{ 
						llp = el; cl[0]->m_head[a] = el; 
					}

					el->SetNext( a, 0 );
					llp = el;

					if (rlp) 
						rlp->SetNext( a, clel); 
					else 
					{ 
						rlp = clel; cl[1]->m_head[a] = clel; 
					}

					clel->SetNext( a, 0 );
					rlp = clel;
				}

				el = next;
			}
		}
		// remove invalid primitives from ll and rl
		for ( int i = 0; i < 2; i++ )
		{
			el = cl[i]->m_head[0];
			bool needsort = false, needclean = false;
			// clip
			while (el)
			{
				KdtBox* eb = (KdtBox*)(((unsigned long)el) & (0xffffffff - 63));
				if (eb->m_flags == KdtBox::PROCESSED)
				{
					eb->m_flags = KdtBox::LEFTLIST + i;
					eb->m_clone = 0;
					m_memData.Init( m_keys, eb->m_memIdx);
					m_memData.Clip( box[i]);
					if ( !m_memData.IsHaveSpace())
					{
						for ( int a = 0; a < 3; a++)
						{
							eb->m_side[0].SetSide( a, KdtBox::DISABLE);
							eb->m_side[1].SetSide( a, KdtBox::DISABLE);
						}
						count[i]--;
						needclean = true;
					}
					else 
					{
						for ( int a = 0; a < 3; a++ )
						{
							eb->m_side[0].m_pos[a] = m_memData.m_aabb.GetMinPoint()[a];
							eb->m_side[1].m_pos[a] = m_memData.m_aabb.GetMaxPoint()[a];
						}
						needsort = true;
					}
				}

				eb->m_flags = KdtBox::STRADDLING;
				el = el->GetNext( 0 );
			}
			// clean and resort
			for ( int a = 0; a < 3; a++ )
			{
				if (needclean)
				{
					el = cl[i]->m_head[a];
					kdtbox* prev = 0;
					while (el)
					{
						if (el->GetSide( a) == KdtBox::DISABLE)
						{
							if (!prev) 
								cl[i]->m_head[a] = el->GetNext( a);
							else 
								prev->SetNext( a, el->GetNext( a));
						}
						else 
							prev = el;

						el = el->GetNext( a);
					}
				}

				if ((needsort) && (cl[i]->m_head[a])) 
					cl[i]->Sort( a);
			}
		}
		// recursive
		if ( depth < m_maxDepth)
		{
			anode->SetSplitPos( bestpos );
			anode->SetAxis( bestaxis );
			anode->SetLeft( nodeIdxs[0] );
			anode->SetLeaf( false );
			for ( int i=0; i<2; i++)
			{
				if ( count[i]>m_pPerLeaf) 
				{
					SAHParam<T>	sahParam;
					sahParam.m_node		  = &m_root[nodeIdxs[i]];
					sahParam.m_box		  = box[i];
					sahParam.m_depth	  = depth + 1;
					sahParam.m_numMembers = count[i];
					sahParam.m_kdtBoxList = cl[i];
					remaining.push_back( sahParam);
				}
				else
				{
					// store primitives in leaf
					kdtbox* el = cl[i]->m_head[0];
					while (el)
					{
						KdtBox* eb = (KdtBox*)(((UINT)el) & (0xffffffff - 63));
						if (eb->m_flags != KdtBox::PROCESSED)
						{
							NodeAddMember( &m_root[nodeIdxs[i]], eb->m_memIdx);
							eb->m_flags = KdtBox::PROCESSED;
						}

						el = el->GetNext( 0);
					}
				}
			}
		}
	}
}