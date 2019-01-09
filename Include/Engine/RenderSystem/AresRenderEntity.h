#pragma once

#include "AresRenderElementMgr.h"
#include "Engine/SceneSystem/AresEntity.h"

namespace Ares
{
	//--------------------------------------------------------
	// 渲染实体: 2010-08-05  -- 未知      帝林
	//--------------------------------------------------------
	class RenderEntity : public Entity
	{
	public:
		// shader param type
		enum SPType
		{
			SP_World			= 0,
			SP_WorldView,	
			SP_WorldViewProj,
		};

	public:
		// 构造函数
		RenderEntity( EntityTypeDetail typeDetail);

		// 更新
		virtual void  FrameMove( float fTime);

		// 获取shader绑定参数
		ShaderParamPtr GetSPAB( SPType type) const;

		// 获取是否可见
		bool IsVisible() { return m_visible; }

		// 设置是否显示
		void SetVisible(  bool visible) { m_visible = visible; }

	private:
		bool							m_visible;			// 是否显示
		boost::array<ShaderParamPtr,3>	m_spab;				// ShaderParamAutoBind
	};

	typedef s_ptr<RenderEntity> RenderEntityPtr;
}