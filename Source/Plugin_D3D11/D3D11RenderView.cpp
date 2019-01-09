#include "D3D11RenderView.h"
#include "D3D11Texture.h"
#include "D3D11Mapping.h"
#include <Core/AresPtr.h>
#include <Core/AresException.h>

namespace Ares
{
	// 构造函数
	D3D11RenderView::D3D11RenderView()
	{
		D3D11RenderDevice& renderFactory = D3D11RenderDevice::GetSingleton();

		m_d3dDevice = renderFactory.GetD3DDevice();
		m_d3dImmCtx = renderFactory.GetD3DDeviceImmContext();
	}

	// 析构函数
	D3D11RenderView::~D3D11RenderView()
	{
	}

	// 构造函数
	D3D11RenderTargetRenderView::D3D11RenderTargetRenderView( TexturePtr texture_1d_2d_cube, int firstArrayIndex, int arraySize, int level)
		: m_rtSrc( &texture_1d_2d_cube), 
		  m_rtFirstSubres( firstArrayIndex*texture_1d_2d_cube->GetNumMipMaps()+level), 
		  m_rtNumSubres(1)
	{
		m_texture = texture_1d_2d_cube;
		m_rtView  = dynamic_pointer_cast<D3D11Texture>(texture_1d_2d_cube)->RetriveD3DRenderTargetView( firstArrayIndex, arraySize, level);

		m_width	  = texture_1d_2d_cube->GetWidth( level);
		m_height  = texture_1d_2d_cube->GetHeight( level);
		m_format  = texture_1d_2d_cube->GetFormat();
	}

	// 构造函数
	D3D11RenderTargetRenderView::D3D11RenderTargetRenderView( const ID3D11RenderTargetViewPtr& view, UINT width, UINT height, ElementFormat format)
		: m_rtView( view),
		  m_rtSrc( NULL),
		  m_rtFirstSubres( 0),
		  m_rtNumSubres( 0)
	{
		m_width = width;
		m_height= height;
		m_format = format;


	}

	// 当附加FrameBuffer
	void D3D11RenderTargetRenderView::OnAttachedFrameBuffer( FrameBuffer& frameBuffer, FrameBuffer::Attachment att)
	{

	}

	// 构造函数
	D3D11DepthStencilRenderView::D3D11DepthStencilRenderView( const ID3D11DepthStencilViewPtr& view, UINT width, UINT height, ElementFormat format)
		: m_dsView( view),
		  m_rtSrc( NULL),
		  m_rtFirstSubres( 0),
		  m_rtNumSubres( 0)
	{
		m_width = width;
		m_height= height;
		m_format = format;
	}

	// 构造函数
	D3D11DepthStencilRenderView::D3D11DepthStencilRenderView( UINT width, UINT height, ElementFormat format, UINT sampleCount, UINT sampleQuality)
		: m_rtSrc( NULL),
		  m_rtFirstSubres( 0),
		  m_rtNumSubres( 0)
	{
		A_ASSERT( IsDepthFormat( format));

		D3D11_TEXTURE2D_DESC desc;
		desc.Width	   = width;
		desc.Height	   = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format	   = D3D11Mapping::MappingFormat( format);
		desc.SampleDesc.Count = sampleCount;
		desc.SampleDesc.Quality = sampleQuality;
		desc.Usage		= D3D11_USAGE_DEFAULT;
		desc.BindFlags  = D3D11_BIND_DEPTH_STENCIL;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		ID3D11Texture2D* depthTex;
		TIF( m_d3dDevice->CreateTexture2D( &desc, NULL, &depthTex));
		m_textured3d  = MakeCOMPtr( depthTex);

		D3D11_DEPTH_STENCIL_VIEW_DESC viewDesc;
		viewDesc.Format = desc.Format;

		if( sampleCount > 1)
			viewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		else
			viewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		viewDesc.Flags				= 0;
		viewDesc.Texture2D.MipSlice = 0;

		ID3D11DepthStencilView* dsView;
		TIF( m_d3dDevice->CreateDepthStencilView( m_textured3d.get(), &viewDesc, &dsView));
		m_dsView = MakeCOMPtr( dsView);

		m_width  = width;
		m_height = height;
		m_format = format;
	}

	// 构造函数
	D3D11DepthStencilRenderView::D3D11DepthStencilRenderView( TexturePtr& texture1d2dCube, int firstArrayIndex, int arraySize, int level)
		: m_rtSrc( NULL)
	{
		m_texture = texture1d2dCube;

		m_dsView = dynamic_pointer_cast<D3D11Texture2D>( texture1d2dCube)->RetriveD3DDepthStencilView( firstArrayIndex, arraySize, level);

		m_width = texture1d2dCube->GetWidth( level);
		m_height= texture1d2dCube->GetHeight( level);
		m_format= texture1d2dCube->GetFormat();
	}

	// 当附加FrameBuffer
	void D3D11DepthStencilRenderView::OnAttachedFrameBuffer( FrameBuffer& frameBuffer, FrameBuffer::Attachment att)
	{
		A_ASSERT( FrameBuffer::ATT_DepthStencil == att);
	}
}