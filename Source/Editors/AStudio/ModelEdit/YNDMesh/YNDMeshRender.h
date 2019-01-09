#pragma once

#include <string>
#include <Engine/RenderSystem/ZRenderable.h>
#include <Engine/RenderSystem/AresRenderable.h>
#include <Physics/Rect3.h>

namespace Ares
{
	// YNDRenderable
	struct YNDRenderable
	{
		RenderLayoutPtr m_layout;			// 布局
		SuperMaterial	m_material;			// 材质
	};

	//-------------------------------------------
	// 辅助YNDMesh渲染 2012-9-26 帝林
	//-------------------------------------------
	class YNDMeshRender
	{
	public:
		// 顶点格式
		struct VertexFormat
		{
			float3 m_position;
			float3 m_normal;
			float2 m_uv;
		};

		// 构造函数
		YNDMeshRender( const std::string& filePath);

		// 刷新
		void FrameMove();

		// 渲染
		void SubmitRenderElements();

		// 渲染深度
		void RenderDepth( const Matrix44& viewProj);

		// 返回包围盒大小
		const Rect3& GetWorldBounds() { return m_worldBounds; }

		// 另存为AresModel
		void SaveAsAresModel( const char* file);

	private:
		// 初始化RenderLayout
		void AddRenderable( const vector<VertexFormat>& vertexs, const vector<WORD>& indices, const string& texturePath);

	private:
		Rect3					m_worldBounds;
		vector<YNDRenderable>	m_renderables;
		vector<ZRenderable>		m_zRenderables;		// 深度渲染
		vector<ShaderParamPtr>	m_diffuseParams;
		ShaderParamPtr			m_spWorld;
	};
	typedef s_ptr<YNDMeshRender> YNDMeshRenderPtr;
}