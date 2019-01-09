#include <Physics/KdtBuilder.h>
#include <Physics/IntrRect3Rect3.h>

namespace Ares
{
	// 初始化
	void MemberData::Init( const vector<Rect3>& keys, size_t idx)
	{
		m_idx  = idx;
		m_aabb = keys[idx];
	}

	// 重建并裁剪
	void MemberData::Clip( Rect3& abox)
	{
		IntrRect3Rect3 intrRR( m_aabb, abox);
		intrRR.Test();

		m_aabb = intrRR.m_intrRect;
	}

	// 构造函数
	KdtBoxList::KdtBoxList() 
	{ 
		std::memset( this, 0, sizeof(*this));
	} 

	// 初始化
	void KdtBoxList::Init() 
	{ 
		m_tail[0] = 0;
		m_tail[1] = 0;
		m_tail[2] = 0; 
	} 

	// 排序 mergesort by S. Tatham
	void KdtBoxList::Sort( int a)
	{
		kdtbox* p, *q, *e;
		int insize = 1, nmerges, psize, qsize, i;
		while ( true) 
		{
			p = m_head[a]; m_head[a] = m_tail[a] = 0; nmerges = 0;  
			while (p) 
			{
				nmerges++, q = p, psize = 0;
				for ( i = 0; i < insize; i++ ) 
				{ 
					psize++; 
					q = q->GetNext(a); 
					if (!q) break; 
				}

				qsize = insize;
				while (psize > 0 || ( qsize>0 && q)) 
				{
					if (psize == 0) { e = q; q=q->GetNext(a); qsize--; } 
					else if (qsize == 0 || !q) { e = p; p = p->GetNext(a); psize--; } else
					{
						float v1 = p->m_pos[a], v2 = q->m_pos[a];
						if ((v1 < v2) || ((v1 == v2) && (p->GetSide(a) < q->GetSide(a)))) { e = p; p = p->GetNext(a); psize--; } 
						else { e = q; q = q->GetNext(a); qsize--; }
					}
					if ( m_tail[a]) m_tail[a]->SetNext(a, e); else m_head[a] = e;
					m_tail[a] = e;
				}
				p = q;
			}

			m_tail[a]->SetNext( a, 0);
			if (nmerges <= 1) 
				break;

			insize *= 2;
		}
	}

	// 插入
	void KdtBoxList::Insert( int a, kdtbox* n)
	{
		kdtbox* l = m_head[a], *prev = 0;
		float pos = n->m_pos[a];
		int side = n->GetSide( a);
		while (l)
		{
			float lp = l->m_pos[a];
			int ls = l->GetSide( a );
			if ((lp > pos) || ((lp == pos) && (ls >= side)))
			{
				if (prev) { n->SetNext( a, prev->GetNext( a)); prev->SetNext( a, n); }
				else { n->SetNext( a, m_head[a] ); m_head[a] = n; }
				return;
			}
			prev = l;
			l = l->GetNext( a);
		}

		n->SetNext( a, 0);

		if (prev) 
			prev->SetNext( a, n ); 
		else 
			m_head[a] = n;
	}

	// 移除
	void KdtBoxList::Remove( int a, kdtbox* n )
	{
		kdtbox* l = m_head[a], *prev = 0;
		while (l)
		{
			if (l == n)
			{
				if (prev) 
					prev->SetNext( a, l->GetNext( a));
				else 
					m_head[a] = l->GetNext( a);

				return;
			}
			prev = l;
			l = l->GetNext( a);
		}
	}

	// 构造函数
	KdtMManager::KdtMManager()
	{
		m_eFree = 0;
		m_eReq	= 0;
		m_ePtr  =  NULL;

		m_elFree= 0;
		m_elReq = 0;
		m_elPtr = NULL;

		m_mReq  = 0;
		m_mList = NULL;
		m_mPtr  = NULL;
	}

	KdtMManager::~KdtMManager()
	{

	}

	// 新建成员列表
	MemberList* KdtMManager::NewMemberList()
	{
		MemberList* retval;
		if ( m_mList==NULL)
		{
			unsigned long addr = (unsigned long)(new_ MemberList[8200]);
			m_mList = m_mPtr = (MemberList*)((addr + 32) & (0xffffffff-31));
			MemberList* ptr = m_mList;
			for ( unsigned int i = 0; i<8192; i++, ptr++) 
				ptr->SetNext( ptr + 1 );

			ptr->SetNext( 0 );
			m_mReq += 8192;
		}

		retval = m_mList;
		m_mList = m_mList->GetNext();
		retval->SetNext( 0);
		retval->SetMemIdx( INVALID);

		return retval;
	}

	// 新建KdtBox
	KdtBox* KdtMManager::NewKdtBox()
	{
		if( m_eFree==0)
		{
			unsigned long addr = (unsigned long)(char*)(new_ KdtBox[16400]);
			m_ePtr = (KdtBox*)((addr + 63) & (0xffffffff - 63));
			std::memset( m_ePtr, 0, 16384 * 64 );
			m_eFree = 16384;
			m_eReq += 16384;
		}

		m_eFree--;
		KdtBox* retval = m_ePtr;
		m_ePtr++;
		return retval;
	}

	// 新建KdtBoxList
	KdtBoxList* KdtMManager::NewKdtBoxList()
	{
		if ( m_elFree == 0)
		{
			unsigned long addr = (unsigned long)(char*)(new_ KdtBoxList[16400]);
			m_elPtr = (KdtBoxList*)((addr + 63) & (0xffffffff - 63));
			m_elFree = 16384;
			m_elReq += 16384;
		}
		m_elFree--;
		KdtBoxList* retval = m_elPtr;
		m_elPtr++;
		return retval;
	}
}