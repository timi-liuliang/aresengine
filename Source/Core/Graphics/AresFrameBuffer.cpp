#include "Core/Graphics/AresRenderView.h"
#include "Core/Graphics/AresFrameBuffer.h"
#include "Core/Graphics/AresRenderDevice.h"
#include "Core/AresException.h"

namespace Ares
{
	// constructor
	FrameBuffer::FrameBuffer( RenderDevice* renderDevice)
		: m_left( 0)
		, m_top( 0)
		, m_width( 0)
		, m_height( 0)
		, m_active( FALSE)
		, m_renderDevice( renderDevice)
	{

	}

	// destructor
	FrameBuffer::~FrameBuffer()
	{
	}

	// 绑定
	void FrameBuffer::Bind()
	{
		if( m_dsView)
			m_dsView->OnBind( *this, ATT_DepthStencil);

		for ( size_t i=0; i<m_clrViews.size(); i++)
		{
			if( m_clrViews[i])
				m_clrViews[i]->OnBind( *this, ATT_Color0 + i);
		}

		m_viewsDirty = false;
	}

	// 解绑
	void FrameBuffer::Unbind()
	{
		if( m_dsView)
			m_dsView->OnUnbind( *this, ATT_DepthStencil);

		for ( size_t i=0; i<m_clrViews.size(); i++)
		{
			if( m_clrViews[i])
				m_clrViews[i]->OnUnbind( *this, ATT_Color0 + i);
		}
	}

	// Attach RenderView
	void FrameBuffer::AttachRenderView( Attachment att, const RenderViewPtr& renderView)
	{
		switch( att)
		{
		case ATT_DepthStencil:
			{
				if( m_dsView)
					DetachRenderView( att);

				m_dsView = renderView;
			}
			break;

		default:
			{
				A_ASSERT(att >= ATT_Color0);

				if( att >= (ATT_Color0 + m_renderDevice->GetDeviceCaps().m_maxSimulataneousRts))
				{
					//THR;		
				}

				size_t clrID = att - ATT_Color0;
				if( clrID < m_clrViews.size() && m_clrViews[clrID])
					DetachRenderView( att);

				if( m_clrViews.size() < clrID + 1)
					m_clrViews.resize( clrID+1);

				m_clrViews[clrID] = renderView;
				size_t minClrIdx = clrID;
				for ( size_t i=0; i<clrID; i++)
				{
					if( m_clrViews[i])
						minClrIdx = i;
				}

				if( minClrIdx == clrID)
				{
					m_width  = renderView->GetWidth();
					m_height = renderView->GetHeight();

					// 记录视口参数
					m_viewPort.m_left = 0;
					m_viewPort.m_top  = 0;
					m_viewPort.m_width = m_width;
					m_viewPort.m_height= m_height;
				}
			}
			break;
		}

		if( renderView)
			renderView->OnAttachedFrameBuffer( *this, att);

		m_active     = true;
		m_viewsDirty = true;
	}

	// Detach RenderView
	void FrameBuffer::DetachRenderView( Attachment att)
	{
		switch( att)
		{
			// 深度模板
		case ATT_DepthStencil:
				m_dsView.reset();
			break;

			// RenderTarget
		default:
			{
				if( att>(ATT_Color0 + m_renderDevice->GetDeviceCaps().m_maxSimulataneousRts))
				{
					//THR;
				}

				size_t clrID = att - ATT_Color0;
				if( m_clrViews.size() < clrID+1)
					m_clrViews.resize( clrID+1);

				if( m_clrViews[clrID])
				{
					m_clrViews[clrID]->OnDetachedFrameBuffer( *this, att);
					m_clrViews[clrID].reset();
				}
			}
			break;
		}

		m_viewsDirty = true;
	}

	// GetRenderView
	RenderViewPtr FrameBuffer::GetRenderView( Attachment att)
	{
		if( ATT_DepthStencil == att)
			return m_dsView;

		return m_clrViews[att - ATT_Color0];
	}
}