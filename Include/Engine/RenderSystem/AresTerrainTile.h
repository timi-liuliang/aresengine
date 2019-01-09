#pragma once

#include <boost/tuple/tuple.hpp>
#include "Engine/RenderSystem/AresRenderEntity.h"

using namespace boost;

namespace Ares
{
	//------------------------------------------------------------
	// 地形区块 2010-03-28 17:54 -- 未知   帝林
	//------------------------------------------------------------
	class Terrain;
	class TerrainTile
	{
	public:
		// 顶点数据标记
		enum VertexStreamFlag
		{
			VSF_Position		= 0,		// POSITION0	位置
			VSF_Normal,						// NORMAL0	    TEXCOORD0
			VSF_Blend,
			VSF_LightmapUV,
			VSF_Total,	
		};
		typedef map<VertexStreamFlag, GraphicBufferPtr> VertexBufferMap;

	public:
		// 构造函数
		TerrainTile( Terrain* terrain);

		// 析构函数
		~TerrainTile();

		// 添加Renderable
		void AddRenderable( const Renderable& renderable);

		// 获取Renderable
		Renderable* GetRenderable();

		// 获取包围盒
		Rect3 GetLocalBounds() { return m_localBounds; }

		// 设置实体包围盒
		void  SetLocalBounds( const Rect3& rect) { m_localBounds = rect; }

		// 设置标记
		void SetDirty( bool isDirty) { m_dirtyFlag = isDirty; }

		// 提交到渲染队列( 使用特效 队列索引)
		virtual void SubmitRenderElements( RenderElementMgr& renderElementMgr);

	public:
		// 映射VertexElementType
		static VertexElementType Mapping( VertexStreamFlag vertexStreamFlag);

		// 获取顶点缓冲
		GraphicBufferPtr GetVertexGraphicsBuffer( VertexStreamFlag flag);

		// 获取索引缓冲
		GraphicBufferPtr GetIndexGraphicsBuffer() { return m_indexBuffer; }

		// 设置顶点缓冲
		void SetVertexGraphicBuffer( VertexStreamFlag flag, GraphicBufferPtr& buffer);

		// 设置索引缓冲
		void SetIndexGraphicsBuffer( GraphicBufferPtr& buffer) { m_indexBuffer = buffer; }

	public:
		// 绑定材质自动参数
		void AutoBindParam();

	public:
		bool				m_dirtyFlag;			// 脏标记(需要更新)
		WORD				m_xIndex;				// 是x方向的第几区块
		WORD				m_yIndex;				// 是y方向的第几区块 
		WORD				m_xVerts;				// X轴方向顶点数(本区块)
		WORD				m_yVerts;				// Y轴方向顶点数(本区块)
		WORD				m_xHBase;				// 在地图上X轴第几个顶点
		WORD				m_yHBase;				// 在地图上Y轴第几个顶点

		vector<Renderable>	m_renderables;			// 可渲染物集

		VertexBufferMap		m_vertexBuffers;		// 顶点Buffer
		GraphicBufferPtr	m_indexBuffer;			// 索引Buffer

		Rect3				m_localBounds;			// 本地包围盒
		Terrain*			m_terrain;				// 所属地形
	};
	typedef s_ptr<TerrainTile> TerrainTilePtr;
}