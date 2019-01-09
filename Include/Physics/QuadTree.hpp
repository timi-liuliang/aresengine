#pragma once

#include "quadtreenode.hpp"
#include <vector>

using namespace std;

namespace Ares
{
	//----------------------------------------
	// 四元数 2012-11-13 帝林
	//----------------------------------------
	template<typename T>
	class QuadTree
	{
	public:
		enum
		{
			EM_TREE_DEPTH_MIN = 1,
			EM_TREE_DEPTH_MAX = 17  // A value between 1 and 17
		};

		// 构造函数
		QuadTree();

		// 析构函数
		~QuadTree();

		// 指定空间大小 指定深度
		void create( quad_tree_rect& worldBoundingBox, int _depth);

		// 根据矩形或平截头体来建立搜索
		size_t search(const quad_tree_rect& worldRect, vector<T>& pResult);

		// 添加或更新场景结点
		void add_member(quad_tree_member<T>* newMember);

	private:
		// 通这指定深度与xy 获取树结点
		quad_tree_node<T>* get_node_from_level_xy( int iLevel, int iX, int iY);

		// 未知作用
		void  build_byte_rect(const quad_tree_rect& worldRect, quad_tree_byte_rect& worldByteRect);

		// ...
		void find_tree_node_info(quad_tree_byte_rect& worldByteRect, int& level, int& levelX, int& levelY);

		// 查找合适的树结点
		quad_tree_node<T>* find_tree_node(quad_tree_byte_rect& worldByteRect);

		// 清除
		void clear();

	private:
		quad_tree_node<T>*	m_levelNodes[EM_TREE_DEPTH_MAX];
		Vector3				m_vWorldExtents;
		Vector3				m_vWorldScle;						// 世界缩放
		Vector3				m_vWorldOffset;						// 区域偏移
		int					depth;								// 深度
		int					m_iMemorySize;						// 内存大小
	};

	// 构造函数
	template<typename T>
	QuadTree<T>::QuadTree()
	{
		depth      = 0;
		m_iMemorySize = 0;
		memset( m_levelNodes, 0, sizeof( m_levelNodes));
	}

	// 析构函数
	template<typename T>
	QuadTree<T>::~QuadTree() 
	{ 
		clear(); 
	}

	// 查找树结点
	template<typename T>
	quad_tree_node<T>* QuadTree<T>::find_tree_node(quad_tree_byte_rect& worldByteRect)
	{
		int level, levelX, levelY;

		find_tree_node_info( worldByteRect, level, levelX, levelY);

		return get_node_from_level_xy( level, levelX, levelY);
	}

	// 未知作用
	template<typename T>
	void QuadTree<T>::build_byte_rect(const quad_tree_rect& worldRect, quad_tree_byte_rect& worldByteRect)
	{
		// 矩形区域转换
		worldByteRect.convert( worldRect, m_vWorldOffset, m_vWorldScle);
	}

	// 查找信息( 根据包围盒找出结点信息)
	template<typename T>
	void QuadTree<T>::find_tree_node_info(quad_tree_byte_rect& worldByteRect, int& level, int& levelX, int& levelY)
	{
		int xPattern = worldByteRect.x0 ^ worldByteRect.x1;
		int yPattern = worldByteRect.y0 ^ worldByteRect.y1;

		int bitPattern = (std::max)(xPattern, yPattern);
		int highBit    = bitPattern ? highest_bit_set(bitPattern)+1 : 0;

		level = EM_TREE_DEPTH_MAX - highBit - 1;
		level = (std::min)( level, depth - 1);

		int shift = EM_TREE_DEPTH_MAX - level - 1;

		levelX = worldByteRect.x1 >> shift;
		levelY = worldByteRect.y1 >> shift;
	}

	// 向四叉树中添加被渲染对象
	template<typename T>
	void QuadTree<T>::add_member(quad_tree_member<T>* newMember)
	{
		quad_tree_byte_rect byteRect;
		build_byte_rect(newMember->key, byteRect);

		// 查找该被渲染对象所属的四叉树结点
		quad_tree_node<T>* node = find_tree_node(byteRect);

		if (node)
			node->add_member( newMember, byteRect);
	}

	// 通过级别,x,y查找四叉树结点
	template<typename T>
	inline quad_tree_node<T>* QuadTree<T>::get_node_from_level_xy(int iLevel, int iX, int iY)
	{
		if ( iLevel>=0 && iLevel<depth)
		{
			return &m_levelNodes[iLevel][(iY<<iLevel)+iX];
		}

		return NULL;
	}

	// 清除
	template<typename T>
	inline void  QuadTree<T>::clear()
	{
		for ( int i=0; i<depth; i++)
		{
			if( m_levelNodes[i])
				delete[] m_levelNodes[i];
		}
	}

	template<typename T>
	inline void QuadTree<T>::create( quad_tree_rect& worldBoundingBox, int _depth)
	{
		depth = _depth;
		m_vWorldExtents[0] = worldBoundingBox.x1 - worldBoundingBox.x0;
		m_vWorldExtents[1] = worldBoundingBox.y1 - worldBoundingBox.y0;

		m_vWorldOffset[0]  = -worldBoundingBox.x0;
		m_vWorldOffset[1]  = -worldBoundingBox.y0;

		m_vWorldScle[0] = 65536.0f / m_vWorldExtents[0];
		m_vWorldScle[1] = 65536.0f / m_vWorldExtents[1];

		// 分配结点
		m_iMemorySize = 0;

		// 清理
		clear();

		for (int i=0; i<depth; i++)
		{
			// 每个深度的结点数( 例： 八叉树 nodeCount = (1<<(i+1) * (1<<(i+1))))
			int nodeCount = (1<<i) * (1<<i);
			int size = sizeof(quad_tree_node<T>) * nodeCount;

			m_levelNodes[i] = (quad_tree_node<T>*)new_ quad_tree_node<T>[nodeCount];

			m_iMemorySize += size;
		}

		// Setup each node (设置每个结点)
		for (int i=0; i<depth; i++)
		{
			int levelDimension = (1<<i);
			int levelIndex = 0;

			for (int y=0; y<levelDimension; y++)
			{
				for (int x=0; x<levelDimension; x++)
				{
					m_levelNodes[i][levelIndex].setup(
						get_node_from_level_xy(i-1, (x>>1), (y>>1)),
						get_node_from_level_xy(i+1, (x<<1), (y<<1)),
						get_node_from_level_xy(i+1, (x<<1)+1, (y<<1)),
						get_node_from_level_xy(i+1, (x<<1), (y<<1)+1),
						get_node_from_level_xy(i+1, (x<<1)+1, (y<<1)+1)
						);

					levelIndex++;
				}
			}
		}
	}

	// 根据矩形或平截头体来建立搜索
	template<typename T>
	inline size_t QuadTree<T>::search(const quad_tree_rect& worldRect, vector<T>& pResult)
	{
		quad_tree_member<T>* resultListStart = NULL;
		quad_tree_member<T>* resultListEnd = NULL;

		quad_tree_byte_rect byteRect;
		build_byte_rect(worldRect, byteRect);

		bool continue_search = true;
		int  level = 0;

		// 按深度搜索
		while (level < depth && continue_search)
		{
			int shift_count = EM_TREE_DEPTH_MAX - 1 - level;
			quad_tree_byte_rect localRect( byteRect.x0 >> shift_count,
				byteRect.x1 >> shift_count,
				byteRect.y0 >> shift_count,
				byteRect.y1 >> shift_count
				);

			// do not continue unless a populated node is found
			continue_search = false;

			for (int y = localRect.y0; y <= localRect.y1; y++)
			{
				for (int x= localRect.x0; x <= localRect.x1; x++)
				{
					quad_tree_node<T>* node = get_node_from_level_xy(level, x, y);

					// a populated node has been found
					continue_search = true;

					// search all the edge cells with the full world rectangle, all non-edge cells are
					// contained within the search rect and be called with just the z extents
					if (y == localRect.y0 || y == localRect.y1 || x == localRect.x0 || x == localRect.x1)
					{
						// Test all members of this node against the world rect
						node->test_local_members_for_search_results( &resultListStart, &resultListEnd, worldRect);
					}
					else
					{
						// Test all members of this node against the world z extents only
						node->test_local_members_for_search_results(&resultListStart, &resultListEnd);
					}
				}
			}

			// set up to the next level of the tree
			level++;
		}

		quad_tree_member<T>* pCurrMem = resultListStart;
		while( pCurrMem)
		{
			pResult.push_back( pCurrMem->value);

			pCurrMem = pCurrMem->rear_search_link();
		}

		return pResult.size(); 
	}
}
