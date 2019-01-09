#include "YNDMeshFile.h"
#include "../MOLogic.h"
#include <Engine/AresRoot.h>

namespace Ares
{
	// 构造函数
	YNDMeshRender::YNDMeshRender( const std::string& filePath)
	{	
		m_worldBounds = Rect3( Vector3( -1.f, -1.f, -1.f), Vector3( 1.f, 1.f, 1.f));
		m_diffuseParams.clear();
		m_renderables.clear();

		m_spWorld = MakeShaderParam( Matrix44::Identity);

		YNDMeshFile		tYNDMeshFile;
		if( !tYNDMeshFile.ReadDataFromFile( filePath.c_str()))
		{
			ALogSystem.Warning( "%s not found!", filePath.c_str());

			return;
		}

		//取得模型体数量
		int	tMeshNodeNum = tYNDMeshFile.Fun_GetMeshNodeNum();
		for(int m = 0 ; m < tMeshNodeNum ; m++)
		{
			//取得对应模型体
			SMeshNode_Temp*	tpMeshNodeInfo	= tYNDMeshFile.Fun_GetMeshNodePtr(m);
			if( !tpMeshNodeInfo/* || tYNDMeshFile.Fun_GetTextureNum()!=tpMeshNodeInfo->m_nSubMeshNum*/)
				return; 

			for(int subIdx = 0; subIdx<tpMeshNodeInfo->m_nSubMeshNum; subIdx++)
			{
				Matrix44 subMeshMatrix = (Matrix44&)tpMeshNodeInfo->m_pSubMeshArray[subIdx].m_matSubMeshMatrix;

				// 顶点转换
				const vector<SExport_Vertex>& origVertices = tpMeshNodeInfo->m_pSubMeshArray[subIdx].m_pVertexArray;
				vector<VertexFormat> vertices; vertices.resize( origVertices.size());
				for ( size_t i=0; i<origVertices.size(); i++)
				{
					vertices[i].m_position = float3( origVertices[i].m_fPosX, origVertices[i].m_fPosZ, origVertices[i].m_fPosY);
					vertices[i].m_normal   = float3( origVertices[i].m_fNPosX, origVertices[i].m_fNPosZ, origVertices[i].m_fNPosY);
					vertices[i].m_uv	   = float2( origVertices[i].m_fU, origVertices[i].m_fV);

					// 转换顶点
					vertices[i].m_position = subMeshMatrix.TransformVector3( vertices[i].m_position);

					m_worldBounds.UnionPoint( vertices[i].m_position);
				}

				// 索引数据
				const vector<SExport_Face>& origFaces = tpMeshNodeInfo->m_pSubMeshArray[subIdx].m_pFaceArray;
				vector<WORD> indices; indices.reserve( origFaces.size()*3);
				for( size_t i=0; i<origFaces.size(); i++)
				{
					indices.push_back( static_cast<WORD>(origFaces[i].m_nVertexIndex1));
					indices.push_back( static_cast<WORD>(origFaces[i].m_nVertexIndex2));
					indices.push_back( static_cast<WORD>(origFaces[i].m_nVertexIndex3));
				}

				// 漫反射纹理
				int texIdx = tpMeshNodeInfo->m_pSubMeshArray[subIdx].m_sSubMeshInfo.m_nMaterialID;
				STexture*	tpTexture   = tYNDMeshFile.Fun_GetTexturePtr( texIdx);
				if( tpTexture)
				{
					string texturePath;
					File::ReplaceNameExt( texturePath, filePath.c_str(), tpTexture->m_szFileName);
					string texPath = texturePath;
					if( filesystem::exists( texPath))
						AddRenderable( vertices, indices, texPath);
					else
						AddRenderable( vertices, indices, tpTexture->m_szFileName);
				}
				else
				{
					ALogSystem.Warning( "YNDMesh::%s, Diffuse Texture is NULL!", filePath.c_str());
				}
			}
		}
	}

	// 刷新
	void YNDMeshRender::FrameMove()
	{
	}

	// 渲染
	void YNDMeshRender::SubmitRenderElements()
	{
		for( size_t i=0; i<m_renderables.size(); i++)
		{
			MOLogic_RenderSystem.GetRenderElementMgr().AddRenderable( m_renderables[i].m_layout, &m_renderables[i].m_material);
		}
	}

	// 渲染深度
	void YNDMeshRender::RenderDepth( const Matrix44& viewProj)
	{
		for( size_t i=0; i<m_zRenderables.size(); i++)
		{
			m_zRenderables[i].Render( viewProj);
		}
	}

	// 初始化RenderLayout
	void YNDMeshRender::AddRenderable( const vector<VertexFormat>& vertexs, const vector<WORD>& indices, const string& texturePath)
	{
		string _texturePath = texturePath;
		boost::filesystem::path texpath( texturePath);
		if( texpath.extension().string()!=".dds")
		{
			boost::filesystem::path ddspath = texpath; 
			ddspath.replace_extension( "dds");
			_texturePath = ddspath.string();

			ALogSystem.Warning( "the texture %s not supported!", texpath.filename());
		}

		YNDRenderable renderable;

		// 拓扑类型
		RenderLayoutPtr layout = ARenderDevice.CreateRenderLayout();
		layout->SetTopologyType( RenderLayout::TT_TriangleList);

		// 顶点数据
		ElementInitData initVertexData;
		initVertexData.m_rowPitch   = sizeof(VertexFormat) * vertexs.size();
		initVertexData.m_slicePitch = 0;
		initVertexData.m_data		= vertexs.data();
		GraphicBufferPtr vertexPosBuffer = ARenderDevice.CreateVertexBuffer( BU_Static, EAH_GPURead | EAH_Immutable, &initVertexData);
		VertexElementType elementType;
		elementType.push_back( VertexElement(VEU_Position,	   0, EF_BGR32F));
		elementType.push_back( VertexElement(VEU_Normal,       0, EF_BGR32F));
		elementType.push_back( VertexElement(VEU_TextureCoord, 0, EF_GR32F));
		layout->BindVertexStream( vertexPosBuffer, elementType);

		// 索引数据
		ElementInitData initIndexData;
		initIndexData.m_rowPitch   = sizeof(WORD) * indices.size();
		initIndexData.m_slicePitch = 0;
		initIndexData.m_data	   = indices.data();
		GraphicBufferPtr indexBuffer = ARenderDevice.CreateIndexBuffer( BU_Dynamic, EAH_CPUWrite | EAH_GPURead, &initIndexData);
		layout->BindIndexStream( indexBuffer, EF_R16UI);

		// 设置渲染布局
		renderable.m_layout = layout;

		// 设置材质
		renderable.m_material.SetUsage( SMU_OutputDiffuse | SMU_OutputNormal);
		renderable.m_material.Compile();
		//renderable.GetMaterial().SetMethod( AResSystem.LoadMethod("visualmesh.hlsl"));

		// 位置参数
		renderable.m_material.SetParamRef( "CameraPos", MOLogic_CameraSystem.GetSPAB( CameraSystem::SP_Pos));
		renderable.m_material.SetParamRef( "ViewProj",  MOLogic_CameraSystem.GetSPAB( CameraSystem::SP_ViewProj));
		renderable.m_material.SetParamRef( "World",     m_spWorld);
		renderable.m_material.SetParamRef( "View",		MOLogic_CameraSystem.GetSPAB( CameraSystem::SP_View));
		renderable.m_material.SetRenderQueue( RQ_Opacity);

		// 纹理参数
		TexturePtr texture = AResSystem.SyncLoadDDS( _texturePath.c_str(), EAH_GPURead | EAH_Immutable);
		ShaderParamPtr difftex = MakeShaderParam( texture);
		SamplerStatePtr sampState = ARenderDevice.CreateSamplerState( SamplerStateDesc());
		//ShaderParamPtr wrapSampler = MakeShaderParam( sampState);
		renderable.m_material.SetParamRef( "DiffuseTex", difftex);
		//renderable.GetMaterial().SetParamRef( "WrapSampler",wrapSampler);

		m_renderables.push_back( renderable);
		m_diffuseParams.push_back( difftex);
		//m_diffuseParams.push_back( wrapSampler);

		// 设置状态
		RasterizerStateDesc rDesc;
		rDesc.m_cullMode = CM_None;
		renderable.m_material.SetRasterizerState( ARenderDevice.CreateRasterizerState( rDesc));

		// ZRenderable
		ZRenderable zRenderable;
		zRenderable.SetRenderlayout( layout);

		m_zRenderables.push_back( zRenderable);
	}

	// 另存为AresModel
	void YNDMeshRender::SaveAsAresModel( const char* file)
	{
		for( size_t i=0; i<m_renderables.size(); i++)
		{

		}
	}
}