#pragma once

#include <vector>
#include <Physics/Rect3.h>
#include <Physics/Collide.h>

using namespace std;

namespace Ares
{
	typedef Rect3 DbvtVolume;

	// DbvtNode 2012-11-19 帝林
	template<typename T>
	struct DbvtNode
	{
		DbvtVolume		m_volume;		// 包围盒
		DbvtNode<T>*	m_parent;		// 父结点
		DbvtNode<T>*	m_childs[2];	// 子结点
		T				m_data;			// 用户数据

		// 是否为叶子结点
		bool IsLeaf() const { return !m_childs[1]; }

		// 是否为非叶结点
		bool IsInternal() const { return !IsLeaf(); }
	};

	//------------------------------------------
	// Dynamics bounding volume tree 2012-11-19
	// Implents a fast dynamic bounding volume
	// tree based on axis aligned bounding boxes,
	// It has a fast insert, remove and update of
	// nodes.
	//------------------------------------------
	template <typename T>
	class Dbvt
	{
	public:
		// Stack element
		struct StkNN
		{
			const DbvtNode<T>*	m_a;
			const DbvtNode<T>*	m_b;

			StkNN() {}
			StkNN( const DbvtNode<T>* na, const DbvtNode<T>* nb) : m_a(na), m_b(nb) {}
		};

	public:
		Dbvt();
		~Dbvt() {}

		// 插入
		DbvtNode<T>* Insert( const DbvtVolume& volume, T& data);

		// 更新
		void Update( DbvtNode<T>* leaf, const DbvtVolume& volume);

		// 检测
		void ShapeIntersectionTest( const DbvtNode<T>* root, const Shape* shape, vector<T>& results);

		// 获取主节点
		const DbvtNode<T>* GetRoot() const { return m_root; }

		// 是否为空
		bool IsEmpty() const { return !m_root; }

		// 移除结点
		void Remove( DbvtNode<T>* leaf);

		// 清空
		void Clear();

	private:
		// 递归删除结点
		void RecursiveDeleteNode( DbvtNode<T>* node);

		// 创建结点
		DbvtNode<T>* CreateNode( DbvtNode<T>* parent, T& data);

		// 创建结点
		DbvtNode<T>* CreateNode( DbvtNode<T>* parent, const DbvtVolume& volume, T& data);

		// 创建结点
		DbvtNode<T>* CreateNode( DbvtNode<T>* parent, const DbvtVolume& volume0, const DbvtVolume& volume1, T& data);

		// 删除结点
		void DeleteNode( DbvtNode<T>* node);

		// 插入节点
		void InsertLeaf( DbvtNode<T>* root, DbvtNode<T>* leaf);

		// 移除叶结点
		DbvtNode<T>* RemoveLeaf( DbvtNode<T>* leaf);

		// 获取索引
		int Indexof( const DbvtNode<T>* node);

		// Proximity
		float Proximity( const DbvtVolume& a, const DbvtVolume& b);

		// Select
		int Select( const DbvtVolume& o, const DbvtVolume& a, const DbvtVolume& b);

	private:
		DbvtNode<T>*	m_root;		// 主节点
		DbvtNode<T>*	m_free;		// 空闲节点
		int				m_lkhd;
		int				m_leaves;	// 叶节点数
		UINT			m_opath;

		vector<StkNN>				m_stkStack;
		vector<const DbvtNode<T>*>	m_shapeTestStack;
	};

	// 构造函数
	template <typename T>
	Dbvt<T>::Dbvt()
	 : m_root( NULL)
	 , m_free( NULL)
	 , m_lkhd( -1)
	 , m_leaves( 0)
	 , m_opath( 0)
	{}

	// 清空
	template <typename T>
	FORCEINLINE void Dbvt<T>::Clear()
	{
		if( m_root)
			RecursiveDeleteNode( m_root);

		m_lkhd = -1;
		m_opath = 0;
		m_stkStack.clear();
	}

	// 插入
	template <typename T>
	FORCEINLINE DbvtNode<T>* Dbvt<T>::Insert( const DbvtVolume& volume, T& data)
	{
		DbvtNode<T>* leaf = CreateNode( NULL, volume, data);

		InsertLeaf( m_root, leaf);

		m_leaves++;

		return leaf;
	}

	// 更新
	template <typename T>
	void Dbvt<T>::Update( DbvtNode<T>* leaf, const DbvtVolume& volume)
	{
		DbvtNode<T>* root = RemoveLeaf( leaf);
		if( root)
		{
			if( m_lkhd>=0)
			{
				for( int i=0; (i<m_lkhd)&&root->m_parent; ++i)
					root = root->m_parent;
			}
			else
				root = m_root;
		}

		leaf->m_volume = volume;
		InsertLeaf( root, leaf);
	}

	// 创建结点
	template <typename T>
	FORCEINLINE DbvtNode<T>* Dbvt<T>::CreateNode( DbvtNode<T>* parent, T& data)
	{
		DbvtNode<T>* node;
		if( m_free)
		{
			node   = m_free;
			m_free = NULL;
		}
		else
		{
			// 重新分配
			node = new_ DbvtNode<T>;
		}
		
		node->m_parent	  = parent;
		node->m_data	  = data;
		node->m_childs[1] = NULL;

		return node;
	}

	// 创建结点
	template <typename T>
	FORCEINLINE DbvtNode<T>* Dbvt<T>::CreateNode( DbvtNode<T>* parent, const DbvtVolume& volume, T& data)
	{
		DbvtNode<T>* node = CreateNode( parent, data);
		node->m_volume = volume;

		return node;
	}

	// 创建结点
	template <typename T>
	DbvtNode<T>* Dbvt<T>::CreateNode( DbvtNode<T>* parent, const DbvtVolume& volume0, const DbvtVolume& volume1, T& data)
	{
		DbvtNode<T>* node = CreateNode( parent, data);
		Rect3::Merge( node->m_volume, volume0, volume1);

		return node;
	}

	// 获取索引
	template <typename T>
	FORCEINLINE int Dbvt<T>::Indexof( const DbvtNode<T>* node)
	{
		return static_cast<int>( node->m_parent->m_childs[1]==node);
	}

	// 插入节点
	template <typename T>
	FORCEINLINE void Dbvt<T>::InsertLeaf( DbvtNode<T>* root, DbvtNode<T>* leaf)
	{
		if( !m_root)
		{
			m_root		   = leaf;
			leaf->m_parent = NULL;
		}
		else
		{
			if( !root->IsLeaf())
			{
				do 
				{
					root = root->m_childs[ Select( leaf->m_volume, root->m_childs[0]->m_volume, root->m_childs[1]->m_volume)];
				} while ( !root->IsLeaf());
			}

			T			 nullT;
			DbvtNode<T>* prev = root->m_parent;
			DbvtNode<T>* node = CreateNode( prev, leaf->m_volume, root->m_volume, nullT);
			if( prev)
			{
				prev->m_childs[Indexof( root)] = node;
				node->m_childs[0]			   = root; root->m_parent = node;
				node->m_childs[1]			   = leaf; leaf->m_parent = node;
				do 
				{
					if( !prev->m_volume.IsContain( node->m_volume))
						Rect3::Merge( prev->m_volume, prev->m_childs[0]->m_volume, prev->m_childs[1]->m_volume);
					else
						break;

					node = prev;
				} while ( prev=node->m_parent);
			}
			else
			{
				node->m_childs[0] = root; root->m_parent = node;
				node->m_childs[1] = leaf; leaf->m_parent = node;
				m_root			  = node;
			}
		}
	}

	// 递归删除结点
	template <typename T>
	void Dbvt<T>::RecursiveDeleteNode( DbvtNode<T>* node)
	{
		if( !node->IsLeaf())
		{
			RecursiveDeleteNode( node->m_childs[0]);
			RecursiveDeleteNode( node->m_childs[1]);
		}

		if( node==m_root)
			m_root = NULL;

		DeleteNode( node);
	}

	// 删除结点
	template <typename T>
	void Dbvt<T>::DeleteNode( DbvtNode<T>* node)
	{
		SAFE_DELETE( m_free);

		m_free = node;
	}

	// 移除叶结点
	template <typename T>
	DbvtNode<T>* Dbvt<T>::RemoveLeaf( DbvtNode<T>* leaf)
	{
		if( leaf==m_root)
		{
			m_root = NULL;
			return NULL;
		}
		else
		{
			DbvtNode<T>* parent = leaf->m_parent;
			DbvtNode<T>* prev	= parent->m_parent;
			DbvtNode<T>* sibling= parent->m_childs[1-Indexof(leaf)];
			if( prev)
			{
				prev->m_childs[Indexof(parent)] = sibling;
				sibling->m_parent = prev;
				DeleteNode( parent);
				while( prev)
				{
					const DbvtVolume pd = prev->m_volume;
					Rect3::Merge( prev->m_volume, prev->m_childs[0]->m_volume, prev->m_childs[1]->m_volume);
					if( pd!=prev->m_volume)
						prev = prev->m_parent;
					else
						break;
				}
				return prev ? prev : m_root;
			}
			else
			{
				m_root = sibling;
				sibling->m_parent = NULL;
				DeleteNode( parent);
				
				return m_root;
			}
		}
	}

	// 检测
	template<typename T>
	void Dbvt<T>::ShapeIntersectionTest( const DbvtNode<T>* root, const Shape* shape, vector<T>& results)
	{
		results.clear();

		// check
		if( !root) return;

		vector<const DbvtNode<T>*>& stack = m_shapeTestStack;
		stack.reserve( 128);
		stack.push_back( root);
		while ( stack.size()) 
		{
			const DbvtNode<T>* node = stack.back(); stack.pop_back();
			if( Collide::IntersectionTest( shape, &node->m_volume))
			{
				if( node->IsInternal())
				{
					stack.push_back( node->m_childs[0]);
					stack.push_back( node->m_childs[1]);
				}
				else
				{
					// 是否有重复结果 !!!!!
					results.push_back( node->m_data);
				}
			}
		}
	}

	// 移除结点
	template <typename T>
	void Dbvt<T>::Remove( DbvtNode<T>* leaf)
	{
		RemoveLeaf( leaf);
		DeleteNode( leaf);
		m_leaves--;
	}

	// Select
	template <typename T>
	int Dbvt<T>::Select( const DbvtVolume& o, const DbvtVolume& a, const DbvtVolume& b)
	{
		return Proximity( o, a) < Proximity( o, b) ? 0 : 1;
	}

	// Proximity
	template <typename T>
	float Dbvt<T>::Proximity( const DbvtVolume& a, const DbvtVolume& b)
	{
		const Vector3 d = (a.m_min+a.m_max) - (b.m_min+b.m_max);

		return std::abs( d.x) + std::abs( d.y) + std::abs( d.z);
	}
}