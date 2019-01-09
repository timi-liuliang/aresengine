#pragma once

#include <Core/AresCore.h>
#include "D3D11FrameBuffer.h"
#include "D3D11Typedefs.h"
#include "D3D11Adapter.h"

using namespace std;
using namespace boost;

namespace Ares
{
	//---------------------------------------------
	// D3D11RenderWindow 2012-3-3 帝林
	//---------------------------------------------
	class D3D11RenderDevice;
	class D3D11RenderWindow : public RenderWindow
	{
	public:
		// constructor
		D3D11RenderWindow( IDXGIFactory1Ptr giFactory, D3D11Adapter* adapter, const RenderSettings& settings);

		// 获取类型
		virtual const char* GetType() { return "D3D11RenderWindow"; }

		// 获取FrameBuffer
		virtual FrameBufferPtr GetFrameBuffer() { return m_frameBuffer; }

		// 更改窗口大小
		virtual void Resize( size_t width, size_t height);

		// 设置是否全屏
		virtual void SetFullScreen( bool fullScreen);

		// 获取背景缓冲纹理
		virtual TexturePtr GetBackbufferTextureCopy();

		// Present the information rendered to the back buffer to the front buffer(the screen)
		virtual void SwapBuffers();

	private:
		// 更新SurfacePtrs
		void UpdateSurfacePtrs();

		// 重置FrameBuffer
		void ResetFrameBuffer();

	private:
		string						m_title;				// 标题
		bool						m_external;				// 是否外部创建
		UINT						m_syncIntervel;			// 同步间隔

		bool						m_ready;				// Is ready ie. available for update
		bool						m_closed;				// is closed

		D3D11FrameBufferPtr			m_frameBuffer;			// default framebuffer

		D3D11Adapter*				m_adapter;				// 所使用的适配器
		IDXGIFactory1Ptr			m_giFactory;			// Pointer to the 3D device specific for this window
		ID3D11DevicePtr				m_d3dDevice;			// D3D11Device
		ID3D11DeviceContextPtr		m_immediateContex;		// immediate context
		DXGI_SWAP_CHAIN_DESC		m_swapChainDesc;		// 
		IDXGISwapChainPtr			m_swapChain;			// swap chain pointer
		bool						m_mainWnd;				// mainWnd

		ID3D11Texture2DPtr			m_backbuffer;			// 背景缓冲
		ID3D11Texture2DPtr			m_depthStencil;			// 深度缓冲
		ID3D11RenderTargetViewPtr	m_renderTargetView;		// 背景RenderTargetView
		ID3D11DepthStencilViewPtr	m_depthStencilView;		// 深度View
		D3D11Texture2DPtr			m_backBufferTexture;	// 背景缓冲纹理
		D3D11Texture2DPtr			m_backBufferTextureCopy;// 背景缓冲纹理拷贝

		DXGI_FORMAT					m_backBufferFormat;		// 背景缓冲格式
		DXGI_FORMAT					m_depthStencilFormat;	// 深度模板缓冲格式

#ifdef ARES_PLATFORM_WINDOWS
		HWND						m_hwnd;					// 窗口句柄
#endif
	};
}