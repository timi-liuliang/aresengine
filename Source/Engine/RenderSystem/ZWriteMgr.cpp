#include <Engine/RenderSystem/ZWriteMgr.h>
#include <Engine/AresRoot.h>

namespace Ares
{
	// 写深度
	void ZWriteMgr::Write( vector<Entity*>& entitys, const Matrix44& viewProj)
	{
		// 构建ZRenderable
		RefreshRenderableZWrite( entitys);

		// 执行ZWrite
		ExecZWrite( entitys, viewProj);
	}

	// 刷新Renderable
	void ZWriteMgr::RefreshRenderableZWrite( vector<Entity*>& entitys)
	{
		// 查找ZRenderable
		for( size_t i=0; i<entitys.size(); i++)
		{
			if( entitys[i]->GetType() == ET_Render /*&& entitys[i]->IsStatic()*/)
			{
				// 新建ZPreRenderables
				if( entitys[i]->GetTypeDetail() == ED_Model)
				{
					Model* model = dynamic_cast<Model*>(entitys[i]);
					if( model)
						RefreshRenderableZWrite( model);
				}
				else if( entitys[i]->GetTypeDetail() == ED_Terrain)
				{
					Terrain* terrain = dynamic_cast<Terrain*>( entitys[i]);
					if( terrain)
						RefreshRenderableZWrite( terrain);
				}
			}
		}
	}

	// 刷新模型Renderable
	void ZWriteMgr::RefreshRenderableZWrite( Model* model)
	{
		if( m_zRenderables.find( model) != m_zRenderables.end())
			return;

		size_t renderableNum = model->GetMeshRenderableNum();
		for ( size_t i=0; i<renderableNum; i++)
		{
			MeshRenderable& m1Renderable = model->GetMeshRenderable(i);

			if( true)
			{
				UINT vsf = Mesh::VSF_Position | Mesh::VSF_DiffuseUV;
				RenderLayoutPtr layout = m1Renderable.m_mesh->BuildRenderLayout( vsf);

				ZRenderable* zRenderable = new_ ZRenderable( true);
				zRenderable->SetRenderlayout( layout);
				zRenderable->SetMaskTexture( m1Renderable.m_superMaterial->GetZTestMaskTexture());

				m_zRenderables[model][i] =zRenderable;
			}
			else
			{
				UINT vsf = Mesh::VSF_Position;
				RenderLayoutPtr layout = m1Renderable.m_mesh->BuildRenderLayout( vsf);

				ZRenderable* zRenderable = new_ ZRenderable;
				zRenderable->SetRenderlayout( layout);

				m_zRenderables[model][i] =zRenderable;
			}

		}
	}

	// 更新地形ZWriteRenderable
	void ZWriteMgr::RefreshRenderableZWrite( Terrain* terrain)
	{/*
		const Terrain::TileList& tileList = terrain->GetTileList();
		for( size_t i=0; i<tileList.size(); i++)
		{
			RenderLayoutPtr layout = ARenderDevice->CreateRenderLayout();
			layout->SetTopologyType( RenderLayout::TT_TriangleStrip);

			layout->BindVertexStream( tileList[i]->GetVertexGraphicsBuffer( TerrainTile::VSF_Position), TerrainTile::Mapping( TerrainTile::VSF_Position));
			layout->BindIndexStream( tileList[i]->GetIndexGraphicsBuffer(), EF_R16UI);

			ZRenderable* zRenderable = new_ ZRenderable;
			zRenderable->SetRenderlayout( layout);

			m_zRenderables[terrain][i] = zRenderable;
		}*/
	}

	// 执行写深度
	void ZWriteMgr::ExecZWrite( vector<Entity*>& entitys, const Matrix44& viewProj)
	{
		Matrix44 worldViewProj;
		for( size_t i=0; i<entitys.size(); i++)
		{
			worldViewProj = entitys[i]->GetTransform().GetMatrix() * viewProj;

			// 执行渲染
			MultimapZR::iterator it= m_zRenderables.find( entitys[i]);
			if( it!=m_zRenderables.end())
			{
				for( map<size_t, ZRenderable*>::iterator it1 = it->second.begin(); it1!=it->second.end(); it1++)
				{
					it1->second->Render( worldViewProj);
				}
			}
		}
	}
}