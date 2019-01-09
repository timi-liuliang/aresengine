#pragma once

#include <Core/Graphics/AresTexture.h>
#include "Engine/RenderSystem/AresRenderable.h"
#include <Core/AresSerialization.h>

namespace Ares
{
	//---------------------------------
	// FxParticleRender 2013-1-23 帝林
	//---------------------------------
	class FxRenderable : public Renderable
	{
	public:
		// 顶点格式
		struct VertexFormat
		{
			float3   m_position;		// 顶点位置
			ColorRGB m_color;			// 顶点色
			float2   m_texcoord;		// 贴图坐标
		};

	public:
		FxRenderable();

		// 更新渲染缓冲
		void UpdateBuffer( RenderLayout::TopologyType topologyType,const VertexFormat* vertices, int vertexSizeInByte, WORD* indices, int indicesSizeInByte);

		// 设置为公告板
		void SetBillBoard( bool isBillBoard) { m_isBillBoard=isBillBoard; }

		// 设置为空间扭曲
		void SetRenderQueue( BITFIELD renderQueue);

		// 设置纹理
		void SetTexture( const char* texturePath);

		// 获取当前纹理
		const char* GetTexture() { return m_texture.c_str(); }

		// 是否为公告板
		bool IsBillBoard() const { return m_isBillBoard; }

		// 更新
		void FrameMove( float elpasedTime,  FxEntity* fxEntity, FxObject* fxObject);

		// 序列化
		BOOST_SERIALIZATION_SPLIT_MEMBER()
		template< typename Archive> void save( Archive& ar, const unsigned int version) const
		{
			BITFIELD renderQueue = m_material->GetRenderQueue();

			ar & m_texture;
			ar & m_isBillBoard;
			ar & renderQueue;
		}
		template< typename Archive> void load( Archive& ar, const unsigned int version)
		{
			string   texture;
			BITFIELD renderQueue;

			ar & texture;
			ar & m_isBillBoard;
			ar & renderQueue;

			SetTexture( texture.c_str());
			SetRenderQueue( renderQueue);
		}

	private:
		VertexElementType	m_elementType;			// 类型

		string				m_texture;				// 纹理路径
		ShaderParamPtr		m_spabWVP;				// 世界矩阵

		bool				m_isBillBoard;			// 是否为公告板
	};
	typedef s_ptr<FxRenderable>	FxRenderablePtr;
	typedef FxRenderable::VertexFormat FxVertexFormat;
}