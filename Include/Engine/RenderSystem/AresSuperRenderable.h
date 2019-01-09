#ifndef ARES_SUPER_RENDERABLE_H
#define ARES_SUPER_RENDERABLE_H

#include "AresRenderable.h"
#include "SuperMaterial.h"
#include "AresRenderEntity.h"

namespace Ares
{
	//-------------------------------------------
	// SuperRenderable 2013-07-12 帝林
	//-------------------------------------------
	class SuperRenderable : public Renderable
	{
	public:
		// VertexStreamFlag
		enum VertexStreamFlag
		{
			VSF_Position		= 1<<0,		// POSITION0	位置
			VSF_DiffuseUV		= 1<<1,		// TEXCOORD0	漫反射贴图uv
			VSF_LightmapUV		= 1<<2,		// TEXCOORD1	光照图uv
			VSF_Normal			= 1<<3,		// NORMAL0		法线
			VSF_TangentBinormal	= 1<<4,		// TANGENT0		切线	 BINORMAL0	副线		
			VSF_Color			= 1<<5,		// COLOR0		顶点色
			VSF_BoneInfulence	= 1<<6,		// BLENDWEIGHT0	骨骼权重 BLENDINDICES0骨骼索引
			VSF_Total			= 7,	
		};

	public:
		SuperRenderable( RenderEntity* entity);
		virtual ~SuperRenderable();

		// 设置RenderLayout
		void UpdateBuffer(RenderLayout::TopologyType topologyType, void* vertices, int vertexSizeInByte, const void* indices, int indicesSizeInByte);

		// 获取顶点标记
		UINT GetVertexStreamFlag() { return m_superMaterial->GetVertexStreamFlag(); }

		// 绑定材质参数
		void BindMaterialParameter();

	private:
		// 映射VertexElementType
		VertexElementType Mapping( UINT vertexDataFlag);

	public:
		RenderEntity*			m_entity;					// 关联实体				
		SuperMaterialPtr		m_superMaterial;			// 超级材质

		VertexElementType		m_elementType;				// 顶点格式
		GraphicBufferPtr		m_vertexBuffer;				// 顶点流
		GraphicBufferPtr		m_indexBuffer;				// 索引流	
	};
	typedef s_ptr<SuperRenderable> SuperRenderablePtr;
}

#endif