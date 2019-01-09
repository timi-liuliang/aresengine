#pragma once

#include <set>
#include <string>
#include "AresTerrainTile.h"
#include <Physics/Shapes.h>
#include <Core/Graphics/AresRenderMaterial.h>
#include <Core/Graphics/AresGraphicsBuffer.h>

#define TERRAIN_TESSELLATION

using namespace std;

namespace Ares
{
	class Terrain : public RenderEntity
	{
	public:
		friend class TerrainTile;

		typedef DWORD STexBlend;
		typedef vector<TerrainTilePtr> TileList;

	public:
		Terrain();
		~Terrain();

		// 地形大小,区块大小
		bool Build( int width, int height, BYTE shift=6);

	public:
		// 获取高度图
		const HeightField& GetHeightField() { return m_heightField; }

		// 获取渲染方法 
		const RenderMaterialPtr& GetDefaultMaterial() { return m_material; }

		// 获取包围盒
		Rect3& GetWorldExtens() { return m_worldExtends; }

		// 设置渲染方法
		void  SetMaterial( const RenderMaterialPtr& material) { m_material = material; }			

		// 添加新地形高度到修改队列
		bool SetHeight( WORD _x, WORD _y, float height);

		// 设置纹理混合度
		bool SetTexBlend( WORD _x, WORD _y,const STexBlend& texBlend);

		// 获取索引
		UINT32 TableIndex(WORD mapX, WORD mapY);

		// 获取区块行数
		int GetTileNumX() { return m_numTileX; }

		// 获取区块列数
		int GetTileNumY() { return m_numTileY; }

		// 获取块
		TileList& GetTileList() { return m_tileList; }

		// 设置地形是否可见
		void SetVisible( bool visible) { m_visible = visible; }

		// 返回shift
		WORD  GetTileShift() { return m_tileShift; }

		// 设置是否可编辑(不可编辑提高渲染速度)
		void SetEditable( bool editEnable){}

		// 获取指定点高度
		virtual float Height(float x, float y);

		// 获取指定点高度
		virtual float GetHeight( WORD _x, WORD _y, float defaultValue=0.f);

		// 获取指定点纹理混合度
		DWORD GetTexBlend( WORD _x, WORD _y);

		// 获取指定点法线
		virtual void  Normal(Vector3& outNoraml, float x, float y);

		// 分配区块
		TerrainTilePtr AllocateTile( size_t xIdx, size_t yIdx);

		// 更新高度显示
		void  UpdateHeightAndNormal( WORD _x,WORD _y, float* height=NULL);

		// 销毁区块
		void DestroyTile( const s_ptr<TerrainTile>& tile);

		// 更新显示
		void UpdateDisplay();

		// 提交渲染元素
		virtual void SubmitRenderElements( RenderElementMgr& renderElementMgr);

		// 添加到场景消息
		virtual void OnAddToScene( Scene* scene);

	protected:
		// 构建索引Buffer
		bool BuildIndexBuffer();

		// 通过高度图创建地形
		bool  BuildHeightAndNormalTables();

	public:
		// 序列化
		BOOST_SERIALIZATION_SPLIT_MEMBER()
		template<typename Archive> void save( Archive& ar, const unsigned int version) const
		{
			ar & m_version;
			ar & m_visible;
			ar & m_worldExtends;
			ar & m_vWorldSize;
			ar & m_vMapScale;
			ar & m_numTileX;
			ar & m_numTileY;
			ar & m_tileSize;
			ar & m_tileShift;
			ar & m_iSectorUnits;
			ar & m_tileVerts;
			ar & m_iTableWidth;
			ar & m_iTableHeight;
			ar & m_blendTable;
			ar & m_heightField;
		}
		template<typename Archive> void load( Archive& ar, const unsigned int version)
		{
			ar & m_version;
			ar & m_visible;
			ar & m_worldExtends;
			ar & m_vWorldSize;
			ar & m_vMapScale;
			ar & m_numTileX;
			ar & m_numTileY;
			ar & m_tileSize;
			ar & m_tileShift;
			ar & m_iSectorUnits;
			ar & m_tileVerts;
			ar & m_iTableWidth;
			ar & m_iTableHeight;
			ar & m_blendTable;
			ar & m_heightField;

			SetLocalBounds( m_worldExtends);

			// 构建索引
			BuildIndexBuffer();

			// 分配区块数据
			m_tileList.resize( m_numTileX * m_numTileY, s_ptr<TerrainTile>());
			for ( int x=0; x<GetTileNumX(); x++)
			{
				for ( int y=0; y<GetTileNumY(); y++)
					AllocateTile( x, y);
			}
		}

	protected:
		int						m_version;					// 当前版本
		bool					m_visible;					// 地形是否可见
		Rect3					m_worldExtends;				// 地形包围盒
		Vector3					m_vWorldSize;				// 世界大小
		Vector3					m_vMapScale;				// 地图缩放
		size_t					m_numTileX;				
		size_t					m_numTileY;					// 区块数量

		Vector2					m_tileSize;					// 每区块大小

		WORD					m_tileShift;				// 
		WORD					m_iSectorUnits;				// 组成区块的小格子数
		WORD					m_tileVerts;				// 区块顶点数

		WORD					m_iTableWidth;				// 行数
		WORD					m_iTableHeight;				// 列数

		GraphicBufferPtr		m_indexBuffer;				// 索引缓冲()
		RenderMaterialPtr		m_material;					// 材质

		vector<STexBlend>		m_blendTable;				// 纹理混合数据
		HeightField				m_heightField;				// 高度图
		TileList				m_tileList;					// 区块集合
	};
	typedef s_ptr<Terrain> TerrainPtr;
}
