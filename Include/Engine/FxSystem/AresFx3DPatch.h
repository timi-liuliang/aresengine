#pragma once

#include <string>
#include "AresFxObject.h"
#include "AresFxRenderable.h"
#include <Core/AresSerialization.h>
#include <Core/Graphics/AresTexture.h>
#include <Core/Graphics/AresRenderlayout.h>

using namespace std;

namespace Ares
{
	//------------------------------------
	// 单片 2012-8-25 帝林
	//------------------------------------
	class Fx3DPatch : public FxObject
	{
	public:
		Fx3DPatch();
		virtual ~Fx3DPatch();

		// 设置颜色
		void SetColor( const ColorRGB& color);

		// 获取3D片颜色
		const ColorRGB& GetColor() const { return m_color; }

		// 获取转换
		Transform& GetTransform() { return m_transform; }

		// 获取Renderable
		FxRenderablePtr& GetRenderable() { return m_renderable; }

		// 更新
		virtual void FrameMove( float elpasedTime,  FxEntity* fxEntity);

		// 执行
		virtual void SubmitRenderElements( RenderElementMgr& renderElementMgr);

		// 序列化
		BOOST_SERIALIZATION_SPLIT_MEMBER()
		template<typename Archive> void save( Archive& ar, const unsigned int version) const
		{
			FxRenderable* renderable = m_renderable.get();

			ar & boost::serialization::base_object<FxObject>(*this);
			ar & m_color;
			ar & renderable;
		}
		template<typename Archive> void load( Archive& ar, const unsigned int version)
		{
			string			texture;
			FxRenderable*	renderable;

			ar & boost::serialization::base_object<FxObject>(*this);
			ar & m_color;
			ar & renderable;

			m_renderable = MakeSharePtr( renderable);

			UpdateBuffer();
		}

	private:
		// 更新Buffer
		void UpdateBuffer();

	private:
		ColorRGB			m_color;			// 颜色
		FxRenderablePtr		m_renderable;		// 3DPatch定制
	};
}