#pragma once

#include "Physics/Rect3.h"
#include <vector>
#include <boost/serialization/vector.hpp>

using namespace std;

namespace Ares
{
	// KDTreeNode 2012-11-23 帝林
	template<typename T>
	struct KdtNode
	{
		float			m_split;		// split pos
		UINT			m_data;			// leaf flag, axis face, child point, primitive list

		// 构造函数
		KdtNode() : m_split( 0.f), m_data(6) {}

		// 设置轴向
		void SetAxis( int axis) { m_data = (m_data&-4) + axis; }

		// 获取轴向
		int  GetAxis() const { return m_data & 3; }

		// 设置左结点
		void SetLeft( int leftOffset) { m_data = (leftOffset << 10) | ( m_data & 1023); }

		// 获取左子结点
		const KdtNode<T>* GetLeft( const vector<KdtNode<T> >& root) const { return (KdtNode<T>*)( &root[m_data>>10]); }

		// 获取右子结点
		const KdtNode<T>* GetRight( const vector<KdtNode<T> >& root) const { return (KdtNode<T>*)( &root[(m_data>>10)+1]); }

		// 是否为叶结点
		BOOL IsLeaf() const { return m_data & 4; }

		// 是否为非叶结点
		BOOL IsInternal() const { return !IsLeaf(); }

		// 设置结点类型
		void SetLeaf( bool isLeaf) { m_data = isLeaf ? (m_data|4) : (m_data&-5); }

		// 获取对像存储位置偏移量
		int GetMemberOffset() const { return m_data>>10; }

		// 获取对象数量
		int GetMemberCount() const { return (m_data&1016)>>3;}

		// 设置分隔位置
		void SetSplitPos( float pos) { m_split=pos; }

		// 获取分隔位置
		float GetSplitPos() const { return m_split; }

		// 设置成员列表
		void SetMemberList( int offset, int count) { m_data = (offset<<10) + (count<<3) + (m_data&7);}

		// serialization
		template <typename Archive> void serialize( Archive& ar, const unsigned int version)
		{
			ar & m_split;
			ar & m_data;
		}
	};

	//----------------------------------------
	// K-dimensional tree 2012-11-23 帝林
	// Implents a fast static bounding volume
	// tree based on axis aligned bounding boxe
	//		<<game programming gems 7>> 2.2
	//----------------------------------------
	template <typename T>
	class Kdt
	{
	public:
		Kdt(){}
		~Kdt() {}

		// 检测
		void ShapeIntersectionTest( vector<T>& results, const KdtNode<T>* root, const Shape* shape) const;

		// 获取主节点
		const KdtNode<T>* GetRoot() const { return m_root.data(); }

		// serialization
		template <typename Archive> void serialize( Archive& ar, const unsigned int version)
		{
			ar & m_root;
			ar & m_memberList;
		}

	public:
		vector<KdtNode<T> >		m_root;				// 主结点
		vector<T>				m_memberList;		// 成员列表
	};

	// 检测
	template<typename T>
	void Kdt<T>::ShapeIntersectionTest( vector<T>& results, const KdtNode<T>* root, const Shape* shape) const
	{
		results.clear();

		// check
		if( !root) return;

		// 暂使用AABB
		Rect3 intrAABB;
		shape->BuildAABB( intrAABB, Transform());

		vector<const KdtNode<T>*> stack;
		stack.reserve( 128);
		stack.push_back( root);
		while ( stack.size())
		{
			const KdtNode<T>* node = stack.back(); stack.pop_back();
			if( node->IsInternal())
			{
				int	  axis	   = node->GetAxis();
				float splitPos = node->GetSplitPos();

				// 记录左结点
				if( splitPos>intrAABB.m_min[axis])
					stack.push_back( node->GetLeft( m_root));
			
				// 记录右结点
				if( splitPos<intrAABB.m_max[axis])
					stack.push_back( node->GetRight( m_root));
			}
			else
			{
				int memOffset = node->GetMemberOffset();
				int memCount = node->GetMemberCount();
				for( int i=0; i<memCount; i++)
				{
					// 有重需结果,请解决 !!!!!
					results.push_back( m_memberList[memOffset+i]);
				}			
			}
		}
	}
}