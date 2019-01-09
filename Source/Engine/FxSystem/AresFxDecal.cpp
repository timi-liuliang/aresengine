#include <Physics/AresCSG.h>
#include <Engine/FxSystem/AresFxDecal.h>
#include <Engine/FxSystem/AresFxEntity.h>

namespace Ares
{
	// 构造函数
	FxDecal::FxDecal()
		: FxObject( FOT_Decal)
	{
		m_renderable.SetBillBoard( false);
	}

	// 析构函数
	FxDecal::~FxDecal()
	{

	}

	// 设置参数
	void FxDecal::Set( const Vector3& center, const Vector3& normal, const Vector3& tangent, float width, float height, float depth, const Vector3& upDir)
	{
		m_center = center;
		m_normal = normal; m_normal.Normalize();

		// 计算副线
		Vector3 _binormal = Vector3Cross( upDir, tangent);
		Vector3 _tangent  = Vector3Cross( normal, _binormal);

		Box3 box3( center, normal, _binormal, _tangent, depth/2.f, height/2.f, width/2.f);
		if( m_physicsWorld)
		{
			m_vertices.clear();
			m_indices.clear();

			Rect3 localAABB, worldAABB;
			box3.BuildAABB( localAABB, *GetTransform());
			box3.BuildAABB( worldAABB, GetWorldTransform());

			// 设置包围盒
			m_localBounds = localAABB;
			if( m_entity)
				m_entity->UpdateLocalBounds();

			// 三角形筛选
			vector<Triangle3> triangles;
			m_physicsWorld->RegionQueryTriangles( triangles, worldAABB);

			vector<Vector3>	  vertices;
			vector<WORD>	  indices;
			for ( size_t i=0; i<triangles.size(); i++)
			{
				CSGBox3Triangle3::Intersection( vertices, indices, box3, triangles[i]);

				// 添加索引
				for( size_t j=0; j<indices.size(); j++)
					m_indices.push_back( indices[j]+static_cast<WORD>( m_vertices.size()));

				// 顶点数据
				for ( size_t j=0; j<vertices.size(); j++)
				{
					FxVertexFormat format; 
					format.m_position = vertices[j]+m_normal*0.1f;

					m_vertices.push_back( format);
				}
			
			}
		}

		// 顶点色
		float oneOverD = 2.f / depth;
		for( size_t i=0; i<m_vertices.size(); i++)
		{
			Vector3 v = m_vertices[i].m_position - m_center;

			m_vertices[i].m_color = ColorRGB( 1.f, 1.f, 1.f, 1.f - (abs( Vector3Dot( v, normal)) * oneOverD));
		}

		// 纹理坐标
		float oneOverW = 1.f / width;
		float oneOverH = 1.f / height;
		for( size_t i=0; i<m_vertices.size(); i++)
		{
			 Vector3 v = m_vertices[i].m_position - m_center;

			 m_vertices[i].m_texcoord.x = Vector3Dot( v, _tangent)  * oneOverW + 0.5f;
			 m_vertices[i].m_texcoord.y = Vector3Dot( v, _binormal) * oneOverH + 0.5f; 
		}

		UpdateBuffer();
	}

	// 更新Buffer
	void FxDecal::UpdateBuffer()
	{
		m_renderable.UpdateBuffer( RenderLayout::TT_TriangleList, m_vertices.data(), sizeof(FxVertexFormat)*m_vertices.size(), m_indices.data(), sizeof(WORD)*m_indices.size());
	}

	// 更新
	void FxDecal::FrameMove( float elpasedTime, FxEntity* fxEntity)
	{
		m_renderable.FrameMove( elpasedTime, fxEntity, this);
	}

	// 执行
	void FxDecal::SubmitRenderElements( RenderElementMgr& renderElementMgr)
	{
		m_renderable.SubmitRenderElements( renderElementMgr);
	}
}