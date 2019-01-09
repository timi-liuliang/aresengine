#pragma once

#include <Core/AresMath.h>
#include <Core/AresTRect.hpp>
#include <Core/Graphics/AresTexture.h>
#include "Engine/Foundation/Ares3DEngine_h.h"

namespace Ares
{
	//------------------------------------------------------
	// 纹理包-由小纹理组成的大纹理 2012-7-20 帝林
	// 装箱算法:
	// http://www.blackpawn.com/texts/lightmaps/default.html
	//-------------------------------------------------------
	class ARES_EXPORT TextureSet
	{
		typedef TRect<int> IRect;
	public:
		// 接点
		struct Node
		{
			int			m_id;				// ID>=0标识该结点已使用
			int			m_child[2];			// 记录子结点ID
			IRect		m_rc;

			// 构造函数
			Node();

			// 是否为叶结点
			bool IsLeaf() const;
		};

	public:
		// 构造函数
		TextureSet();

		// 构造函数,箱子纹理
		TextureSet( TexturePtr& texture);

		// 插入纹理 INVALID 无效索引
		int Insert( TexturePtr& subTex);

		// 覆写纹理
		int OverWrite( int nodeIdx, TexturePtr& subTex);

		// 获取当前管理纹理
		const TexturePtr& GetTexture() const { return m_texture; }

		// 获取块纹理Viewport
		const Vector4 GetViewport( int nodeIdx) const;  

		// 获取结点信息
		const Node& GetNode( int nodeIdx) const { return m_nodes[nodeIdx]; }

		// 获取宽
		int GetWidth() const { return m_width; }

		// 获取高
		int GetHeight() const { return m_height; }

		// 加载
		void Load( const char* filePath);

		// 保存
		void Save( const char* filePath);

	private:
		// 初始化
		void Init( TexturePtr& texture);

		// 插入纹理
		int Insert(  int nodeIdx, const TexturePtr& subTex);

	private:
		int					m_width;			// 纹理宽
		int					m_height;			// 纹理高
		TexturePtr			m_texture;			// 所管理的纹理
		vector<Node>		m_nodes;			// 主节点(索引0为主节点)
	};
}