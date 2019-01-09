#include <Engine/AresRoot.h>
#include <Engine/RenderSystem/AresModel.h>
#include <Engine/RenderSystem/AresModelIDCard.h>

namespace Ares
{
	// 构造函数
	MeshRenderable::MeshRenderable() 
		: Renderable()
	{
		m_superMaterial = MakeSharePtr( new_ SuperMaterial);
		SetMaterial( m_superMaterial);
	}

	// 获取几何体数量
	int MeshRenderable::GetPrimitiveCount() const
	{
		return m_mesh->GetTriNum();
	}

	// 构造函数
	Model::Model()
		: RenderEntity( ED_Model)
	{
		m_boneTransform	= MakeShaderParam();
		m_lightViewCrop = MakeShaderParam();
	}

	// 析构函数
	Model::~Model()
	{
	}

	// 重建模型数据
	void Model::IDCard::RebuildModel( Model& model)
	{
		model.m_idcard = IDCardPtr( this);

		// 动画数据
		if( m_aniPath.size())
			model.m_animationController.SetAnimation( m_aniPath.c_str());
	
		model.m_renderables.resize( m_renderInfos.size());
		for ( size_t i=0; i<m_renderInfos.size(); i++)
		{
			MeshRenderable& renderable		= model.m_renderables[i];
			renderable.m_meshFilePath		= m_renderInfos[i].m_meshFilePath;
			renderable.m_meshIdx			= m_renderInfos[i].m_meshIdx;
			renderable.m_mesh				= AResSystem.LoadMesh( m_renderInfos[i].m_meshFilePath.c_str(), m_renderInfos[i].m_meshIdx);

			// 默认拥有漫反射,法线
			QWORD usage						= SMU_OutputDiffuse | SMU_OutputNormal | (model.m_animationController.GetAnimation()?SMU_SkeletalMesh:0);
			renderable.m_materialPath		= m_renderInfos[i].m_materialPath.c_str();
			renderable.m_materialIdx		= m_renderInfos[i].m_materialIdx;
			AResSystem.LoadMaterialParameter( renderable.m_superMaterial, renderable.m_materialPath.c_str(), renderable.m_materialIdx, usage);

			model.AutoBindRenderLayout( renderable);

			// 包围盒
			Rect3 localAABB = model.GetLocalBounds();
			Rect3 meshAABB	= renderable.m_mesh->GetBoundingBox();	
			localAABB.UnionPoint( meshAABB.GetMinPoint());
			localAABB.UnionPoint( meshAABB.GetMaxPoint());
			model.SetLocalBounds( localAABB);
		}
	}

	// 更新
	void Model::FrameMove( float fTime)
	{
		RenderEntity::FrameMove( fTime);

		m_animationController.FrameMove( fTime, GetTransform().GetMatrix());

		// 更新骨骼动画
		vector<Matrix44> boneTransform( 60);
		for( size_t i=0; i<m_renderables.size(); i++)
			m_animationController.GetCurrBone( boneTransform.data(), m_renderables[i].m_mesh->GetBoneIdxs());

		*m_boneTransform = boneTransform;
	}

	// 提交到渲染队列( 使用特效 队列索引)
	void Model::SubmitRenderElements( RenderElementMgr& renderElementMgr)
	{
		for( size_t i=0; i<m_renderables.size(); i++)
			m_renderables[i].SubmitRenderElements( renderElementMgr);
	}

	// 获取M1Renderable
	MeshRenderable& Model::GetMeshRenderable( size_t idx)
	{
		A_ASSERT( idx < m_renderables.size());

		return m_renderables[idx];
	}

	// 使用光照图
	bool Model::ApplyLightmap( TexturePtr lightmapTex, const Vector4& lightmapViewport, int meshIdx)
	{
		MeshRenderable& m1Renderable = GetMeshRenderable(meshIdx);
		if( m1Renderable.m_mesh)
		{
			// 2.设置材质静态参数
			m1Renderable.m_superMaterial->SetParamRef( "LightmapTex" ,	   MakeShaderParam( lightmapTex));
			m1Renderable.m_superMaterial->SetParamRef( "LightmapViewport", MakeShaderParam( lightmapViewport));

			// 1.重新生成Shader
			QWORD usage	= SMU_OutputDiffuse | SMU_OutputNormal | SMU_StaticLighting | (m_animationController.GetAnimation()?SMU_SkeletalMesh:0);
			AResSystem.LoadMaterialParameter( m1Renderable.m_superMaterial, m1Renderable.m_materialPath.c_str(), m1Renderable.m_materialIdx, usage);

			// 3.指向新的Renderlayout
			AutoBindMaterialParameter( m1Renderable);
			AutoBindRenderLayout( m1Renderable);
		}
		
		return true;
	}

	// 绑定材质参数
	void Model::AutoBindMaterialParameter( MeshRenderable& mr)
	{
		CameraSystem& cameraSystem = GetCameraSystem();

		// 加载材质静态参数
		if( m_animationController.GetAnimation())
		{				
			mr.m_superMaterial->SetParamRef( "BoneTransforms", m_boneTransform);
		}

		// 自动绑定参数
		for( size_t i=0; i< mr.m_superMaterial->GetParamNum(); i++)
		{
			const string autoVaule =  mr.m_superMaterial->GetParamAutoBind( i);
			if( autoVaule.substr( 0, 5)== "SPAB_")
			{
				if( autoVaule == "SPAB_WorldViewProj")
					 mr.m_superMaterial->SetParamRef( i, GetSPAB( RenderEntity::SP_WorldViewProj));
				else if( autoVaule == "SPAB_World")
					 mr.m_superMaterial->SetParamRef( i, GetSPAB( RenderEntity::SP_World));
				else if( autoVaule == "SPAB_View")
					 mr.m_superMaterial->SetParamRef( i, cameraSystem.GetSPAB( CameraSystem::SP_View));
				else if( autoVaule == "SPAB_ViewProj")
					 mr.m_superMaterial->SetParamRef( i, cameraSystem.GetSPAB( CameraSystem::SP_ViewProj));
				else if( autoVaule == "SPAB_SkyViewProj")
					 mr.m_superMaterial->SetParamRef( i, cameraSystem.GetSPAB( CameraSystem::SP_SkyViewProj));
			}
		}
	}

	// 绑定RenderLayout
	void Model::AutoBindRenderLayout( MeshRenderable& mr)
	{
		// renderlayout
		RenderLayoutPtr layout = mr.m_mesh->BuildRenderLayout( mr.m_superMaterial->GetVertexStreamFlag());

		mr.SetRenderlayout( layout);
	}

	// 添加到场景消息
	void Model::OnAddToScene( Scene* scene)
	{
		RenderEntity::OnAddToScene( scene);

		// 自动绑定材质参数
		foreach_( MeshRenderable& renderable, m_renderables)
			AutoBindMaterialParameter( renderable);
	}
}