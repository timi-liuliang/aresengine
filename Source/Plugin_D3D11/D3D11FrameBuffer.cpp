#include "D3D11RenderView.h"
#include "D3D11FrameBuffer.h"
#include "D3D11RenderDevice.h"
#include "D3D11Mapping.h"

namespace Ares
{
	// 构造函数
	D3D11FrameBuffer::D3D11FrameBuffer( RenderDevice* device)
		: FrameBuffer(device)
	{
		m_left = 0;
		m_top  = 0;
		
		m_viewPort.m_left = m_left;
		m_viewPort.m_top  = m_top;

		m_d3dViewPort.MinDepth = 0.1f;
		m_d3dViewPort.MaxDepth = 1.f;
		m_d3dViewPort.TopLeftX = 0;
		m_d3dViewPort.TopLeftY = 0;
	}

	// 析构函数
	D3D11FrameBuffer::~D3D11FrameBuffer()
	{
	}

	// 获取RenderTargetView
	ID3D11RenderTargetViewPtr D3D11FrameBuffer::GetD3D11RenderTargetView( UINT i) const
	{
		if( i < m_clrViews.size() && m_clrViews[i])
		{
			const D3D11RenderTargetRenderView* d3dView = dynamic_cast<D3D11RenderTargetRenderView*>(m_clrViews[i].get());

			return d3dView->GetD3DRenderTargetView();
		}

		return ID3D11RenderTargetViewPtr();
	}

	// 获取深度模板View
	ID3D11DepthStencilViewPtr D3D11FrameBuffer::GetD3D11DepthStencilView() const
	{
		if( m_dsView)
		{
			const D3D11DepthStencilRenderView* d3dView = dynamic_cast<D3D11DepthStencilRenderView*>( m_dsView.get());

			return d3dView->GetD3DDepthStencilView();
		}

		// return NULL;
		return ID3D11DepthStencilViewPtr();
	}

	// Clear
	void D3D11FrameBuffer::Clear( const ColorRGB& color, float depth, UINT stencil, UINT flags)
	{
		D3D11RenderDevice* pRenderDevice = D3D11RenderDevice::GetSingletonPtr();
		const ID3D11DeviceContextPtr& d3dImmCtx = pRenderDevice->GetD3DDeviceImmContext();

		if( flags & CBM_Color)
		{
			if( m_clrViews.size())
			{
				ID3D11RenderTargetViewPtr renderTargetView = GetD3D11RenderTargetView( 0);
				if( renderTargetView)
				{				
					float tColor[4] = { color.r, color.g, color.b, color.a };
					d3dImmCtx->ClearRenderTargetView( renderTargetView.get(), tColor);
				}
			}
		}
		
		UINT clearFlags = D3D11Mapping::MappingDepthStencilClearMask( flags);
		if( clearFlags)
		{
			const ID3D11DepthStencilViewPtr& dsView = GetD3D11DepthStencilView();
			if( dsView)
				d3dImmCtx->ClearDepthStencilView( dsView.get(), clearFlags, depth, static_cast<BYTE>(stencil));
		}
	}

	// Clear
	void D3D11FrameBuffer::ClearMRTColor( const ColorRGB& color, Attachment attachment)
	{
		D3D11RenderDevice* pRenderDevice = D3D11RenderDevice::GetSingletonPtr();
		const ID3D11DeviceContextPtr& d3dImmCtx = pRenderDevice->GetD3DDeviceImmContext();
		{
			ID3D11RenderTargetViewPtr renderTargetView = GetD3D11RenderTargetView( attachment);
			if( renderTargetView)
			{				
				float tColor[4] = { color.r, color.g, color.b, color.a };
				d3dImmCtx->ClearRenderTargetView( renderTargetView.get(), tColor);
			}
		}
	}

	// 绑定
	void D3D11FrameBuffer::Bind()
	{
		const ID3D11DeviceContextPtr& d3dImmCtx = PDRenderDevice.GetD3DDeviceImmContext();

		vector<void*> rtSrc;			// Rendertarget source
		vector<UINT>  rtFirstSubRes;	// Sub Resource
		vector<UINT>  rtNumSubres;		// SubResource num
		vector<ID3D11RenderTargetView*> rtViews( m_clrViews.size());
		for ( size_t i=0; i<m_clrViews.size(); i++)
		{
			if( m_clrViews[i])
			{
				D3D11RenderTargetRenderView* pRV = dynamic_cast<D3D11RenderTargetRenderView*>(m_clrViews[i].get());
				rtSrc.push_back( pRV->GetRTSrc());
				rtFirstSubRes.push_back( pRV->GetRTFirstSubRes());
				rtNumSubres.push_back( pRV->GetRTNumSubRes());
				rtViews[i] = GetD3D11RenderTargetView( i).get();
			}
			else
				rtViews[i] = NULL;
		}

		if( m_dsView)
		{
			D3D11DepthStencilRenderView* p = dynamic_cast<D3D11DepthStencilRenderView*>(m_dsView.get());
			rtSrc.push_back( p->GetRTSrc());
			rtFirstSubRes.push_back( p->GetRTFirstSubRes());
			rtNumSubres.push_back( p->GetRTNumSubRes());
		}

		// 分离RenderTarget
		for( size_t i=0; i<rtSrc.size(); i++)
			PDRenderDevice.DetachSRV( rtSrc[i], rtFirstSubRes[i], rtNumSubres[i]);
		
		// 设置RenderTargets( RenderTargetView and DepthStencilView)
		d3dImmCtx->OMSetRenderTargets( static_cast<UINT>(rtViews.size()), rtViews.data(), GetD3D11DepthStencilView().get());

		m_d3dViewPort.Width = static_cast<float>( m_viewPort.m_width);
		m_d3dViewPort.Height= static_cast<float>( m_viewPort.m_height);

		PDRenderDevice.RSSetViewports( 1, &m_d3dViewPort);
	}

	// 解绑
	void D3D11FrameBuffer::Unbind()
	{
		const ID3D11DeviceContextPtr& d3dImmCtx = PDRenderDevice.GetD3DDeviceImmContext();

		vector<void*> rtSrc;			// Rendertarget source
		vector<UINT>  rtFirstSubRes;	// Sub Resource
		vector<UINT>  rtNumSubres;		// SubResource num
		for ( size_t i=0; i<m_clrViews.size(); i++)
		{
			if( m_clrViews[i])
			{
				D3D11RenderTargetRenderView* pRV = dynamic_cast<D3D11RenderTargetRenderView*>(m_clrViews[i].get());
				rtSrc.push_back( pRV->GetRTSrc());
				rtFirstSubRes.push_back( pRV->GetRTFirstSubRes());
				rtNumSubres.push_back( pRV->GetRTNumSubRes());
			}
		}

		if( m_dsView)
		{
			D3D11DepthStencilRenderView* p = dynamic_cast<D3D11DepthStencilRenderView*>(m_dsView.get());
			rtSrc.push_back( p->GetRTSrc());
			rtFirstSubRes.push_back( p->GetRTFirstSubRes());
			rtNumSubres.push_back( p->GetRTNumSubRes());
		}

		// 分离RenderTarget
		for( size_t i=0; i<rtSrc.size(); i++)
			PDRenderDevice.DetachSRV( rtSrc[i], rtFirstSubRes[i], rtNumSubres[i]);
	}
}