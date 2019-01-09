#pragma once

#include "Core/AresMath.h"
#include "AresViewPort.h"
#include "AresRenderAPITypedefs.h"

using namespace std;

namespace Ares
{
	// 清除标记
	enum ClearBufferMask
	{
		CBM_Depth  = 1ul << 0,		// 清除深度
		CBM_Stencil= 1ul << 1,		// 清除模板
		CBM_Color  = 1ul << 2,		// 清除颜色
	};

	//------------------------------
	// FrameBuffer 2012-3-3 帝林
	//------------------------------
	class RenderDevice;
	class CORE_EXPORT FrameBuffer
	{
	public:
		// 附加
		enum Attachment
		{
			ATT_Color0 = 0,
			ATT_Color1,
			ATT_Color2,
			ATT_Color3,
			ATT_Color4,
			ATT_Color5,
			ATT_Color6,
			ATT_Color7,
			ATT_DepthStencil
		};

	public:
		FrameBuffer( RenderDevice* renderDevice);
		virtual ~FrameBuffer();

		// rendertarget's left point
		UINT GetLeft() const { return m_left; }

		// rendertarget's top point
		UINT GetTop() const { return m_top; }

		// render target's width
		UINT GetWidth() const { return m_width; }

		// render target's height
		UINT GetHeight() const { return m_height; }

		// get viewport
		const ViewPort& GetViewport() const { return m_viewPort; }

		// set viewport
		void SetViewport( const ViewPort& viewPort) { m_viewPort = viewPort; }

		// set active
		void SetActive( bool active) { m_active = active; }

		// Is views dirty
		bool IsDirty() const { return m_viewsDirty; }

		// Attach RenderView
		virtual void AttachRenderView( Attachment att, const RenderViewPtr& renderView);

		// Detach RenderView
		virtual void DetachRenderView( Attachment att);

		// Clear
		virtual void Clear( const ColorRGB& color, float depth, UINT stencil, UINT flags=CBM_Depth|CBM_Stencil|CBM_Color)=0;

		// Clear
		virtual void ClearMRTColor( const ColorRGB& color, Attachment attachment)=0;

		// GetRenderView
		RenderViewPtr GetRenderView( Attachment att);

	public:
		// 绑定
		virtual void Bind();

		// 解绑
		virtual void Unbind();

	protected:
		UINT					m_left;			
		UINT					m_top;			// 左上位置
		UINT					m_width;		// 宽
		UINT					m_height;		// 高
		BOOL					m_active;		// Is active

		ViewPort				m_viewPort;		// 视口
		bool					m_viewsDirty;	// views dirty
		RenderViewPtr			m_dsView;		// DepthstencilRenderview
		vector<RenderViewPtr>	m_clrViews;		// 清除views
		RenderDevice*			m_renderDevice; // 渲染设备
	};
}