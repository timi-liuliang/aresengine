#include <Engine/RenderSystem/AresClothFabric.h>
#include <Engine/SceneSystem/AresScene.h>

namespace Ares
{
	// 构造函数
	ClothFabric::ClothFabric()
		: RenderEntity( ED_ClothFabric)
	{
		m_renderable = MakeSharePtr( new_ SuperRenderable( this));
	}

	// 析构函数
	ClothFabric::~ClothFabric()
	{

	}

	// 更新
	void ClothFabric::FrameMove( float fTime)
	{
		RenderEntity::FrameMove( fTime);

		m_massSpringSystem.FrameMove( fTime);

		SetLocalBounds( m_massSpringSystem.GetSettings().m_localBounds);
	}

	// 添加到场景消息
	void ClothFabric::OnAddToScene( Scene* scene)
	{
		RenderEntity::OnAddToScene( scene);

		m_massSpringSystem.SetPhysicsWorld( scene->GetPhysicsWorld());

		m_renderable->BindMaterialParameter();
	}

	// 更新Buffer
	void ClothFabric::UpdateBuffer()
	{
		A_ASSERT( m_renderable->GetVertexStreamFlag()==(SuperRenderable::VSF_Position | SuperRenderable::VSF_DiffuseUV | SuperRenderable::VSF_Normal));

		// 修改渲染数据
		struct VertexFormat
		{
			Vector3		m_position;
			Vector2		m_uv;
			Vector3		m_normal;
		};

		static vector<VertexFormat> vertices; 
		vertices.resize( m_massSpringSystem.GetNumMassPoint()); 
		for( size_t i=0; i<m_massSpringSystem.GetNumMassPoint(); i++)
		{
			MassSpringSystem::MassPoint& mp = m_massSpringSystem.GetMassPoint( i);
			vertices[i].m_position = mp.m_position;
			vertices[i].m_uv	   = mp.m_uv;
			vertices[i].m_normal   = mp.m_normal;
		}

		int vertiesBytes = sizeof(VertexFormat)* vertices.size();
		int indicesBytse = sizeof(WORD)		   * m_massSpringSystem.GetTriIndices().size();
		m_renderable->UpdateBuffer( RenderLayout::TT_TriangleList, vertices.data(), vertiesBytes, m_massSpringSystem.GetTriIndices().data(), indicesBytse);
	}

	// 提交渲染元素
	void ClothFabric::SubmitRenderElements( RenderElementMgr& renderElementMgr)
	{
		UpdateBuffer();

		// 提交渲染
		m_renderable->SubmitRenderElements( renderElementMgr);
	}
}