#include <string>
#include <algorithm>
#include <Engine/RenderSystem/AresTerrain.h>
#include "Engine/RenderSystem/AresTerrainTile.h"
#include "Engine/AresRoot.h"

using namespace std;

namespace Ares
{
	// 构造函数
	TerrainTile::TerrainTile( Terrain* terrain)
		: m_terrain( terrain)
		, m_dirtyFlag( false)
	{
	}

	// 析构函数
	TerrainTile::~TerrainTile()
	{
		m_renderables.clear();
	}

	// 添加Renderable
	void TerrainTile::AddRenderable( const Renderable& renderable)
	{
		m_renderables.push_back( renderable);
	}

	// 绑定材质自动参数
	void TerrainTile::AutoBindParam()
	{
		CameraSystem& cameraSystem = m_terrain->GetCameraSystem();

		for( size_t i=0; i<m_renderables.size(); i++)
		{
			RenderMaterialPtr& material = m_renderables[i].GetMaterial();
			for ( size_t i=0; i<material->GetParamNum(); i++)
			{
				const string& spabDesc = material->GetParamAutoBind( i);
				if( spabDesc.substr( 0, 5) == "SPAB_")
				{
					if( spabDesc == "SPAB_WorldViewProj")
						material->SetParamRef( i, m_terrain->GetSPAB( RenderEntity::SP_WorldViewProj));
					else if( spabDesc == "SPAB_WorldView")
						material->SetParamRef( i, m_terrain->GetSPAB( RenderEntity::SP_WorldView));
					else if( spabDesc == "SPAB_World")
						material->SetParamRef( i, m_terrain->GetSPAB( RenderEntity::SP_World));
					else if( spabDesc == "SPAB_CameraPos")
						material->SetParamRef( i, m_terrain->GetCameraSystem().GetSPAB( CameraSystem::SP_Pos));
				}
			}
		}
	}

	// 获取Renderable
	Renderable* TerrainTile::GetRenderable()
	{
		if( m_renderables.empty())
			return NULL;

		return &m_renderables[0];
	}

	// 提交到渲染组
	void TerrainTile::SubmitRenderElements( RenderElementMgr& renderElementMgr)
	{
		for( size_t i=0; i<m_renderables.size(); i++)
			m_renderables[i].SubmitRenderElements( renderElementMgr);
	}

	// 映射VertexElementType
	VertexElementType TerrainTile::Mapping( VertexStreamFlag vertexStreamFlag)
	{
		VertexElementType result;

		switch( vertexStreamFlag)
		{
		case VSF_Position:
			{
				result.push_back( VertexElement(VEU_Position,     0, EF_BGR32F));
			}
			break;

		case VSF_Normal:
			{
				result.push_back( VertexElement(VEU_Normal,      0, EF_BGR32F));
			}
			break;

		case VSF_Blend:
			{
				result.push_back( VertexElement(VEU_BlendWeight, 0, EF_ARGB8));
			}
			break;

		case VSF_LightmapUV:
			{
				result.push_back( VertexElement(VEU_TextureCoord, 1, EF_GR32F));
			}
			break;
		}

		return result;
	}

	// 获取顶点缓冲
	GraphicBufferPtr TerrainTile::GetVertexGraphicsBuffer( VertexStreamFlag flag)
	{
		VertexBufferMap::const_iterator it = m_vertexBuffers.find( flag);
		if( it != m_vertexBuffers.end())
			return it->second;

		return GraphicBufferPtr(); 
	}

	// 设置顶点缓冲
	void TerrainTile::SetVertexGraphicBuffer( VertexStreamFlag flag, GraphicBufferPtr& buffer)
	{
		m_vertexBuffers[flag] = buffer;
	}
}