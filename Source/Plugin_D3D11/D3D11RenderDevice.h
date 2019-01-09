#pragma once

#include <Core/AresCore.h>
#include "D3D11AdapterList.h"
#include "D3D11RenderWindow.h"
#include "D3D11GraphicsBuffer.h"

using namespace std;

namespace Ares
{
	//------------------------------------
	// D3D11RenderDevice 2012-02-11 帝林
	//------------------------------------
	class D3D11RenderDevice : public RenderDevice, public Singleton<D3D11RenderDevice>
	{
		typedef map<ElementFormat,vector<pair<UINT, UINT>>> RenderTargetFormat;
	public:
		D3D11RenderDevice();
		~D3D11RenderDevice();

	public:
		// 开始帧
		virtual void BeginFrame();

		// 结束帧
		virtual void EndFrame();

		// 渲染Renderable
		virtual void Render( RenderLayout* layout, RenderMaterial* material);

		// 创建渲染窗口
		virtual RenderWindow* CreateRenderWindow( const RenderSettings& settings);

	public:
		// 获取名字
		virtual const char* GetName() const;

		// 获取ActiveAdapter
		D3D11Adapter* GetActiveAdapter() const;

		// 获取D3DDevice
		ID3D11DevicePtr& GetD3DDevice() { return m_d3d11Device; }

		// 获取immediateContext
		ID3D11DeviceContextPtr& GetD3DDeviceImmContext() { return m_d3d11ImmContex; }

		// 获取FeatureLevel
		D3D_FEATURE_LEVEL GetDeviceFeatureLevel() const { return m_d3dFeatureLevel; }

	public:
		// 重置渲染状态
		void ResetRenderStates();

		// 设置D3D11Device
		void SetDevice( ID3D11DevicePtr& device, ID3D11DeviceContextPtr& immCtx, D3D_FEATURE_LEVEL featureLevel);

	public:
		// 创建RenderLayout
		virtual RenderLayoutPtr CreateRenderLayout();

		// 创建效果文件 "*.hlsl"
		virtual RenderMethodPtr CreateRenderMethod();

		// 创建索引缓冲区
		virtual GraphicBufferPtr CreateIndexBuffer( BufferUsage usage, UINT accessHint, const ElementInitData* initData, ElementFormat format=EF_UnKnown);

		// 创建顶点缓冲区
		virtual GraphicBufferPtr CreateVertexBuffer( BufferUsage usage, UINT accessHint,const ElementInitData* initData, ElementFormat format=EF_UnKnown);

		// 创建1D纹理
		virtual TexturePtr CreateTexture1D( UINT widht, UINT numMipMaps, UINT arraySize, ElementFormat format, UINT sampleCount, UINT sampleQuality, UINT accessHine, const ElementInitData* initData);

		// 创建2D纹理
		virtual TexturePtr CreateTexture2D(UINT width, UINT height, UINT numMipMaps, UINT arraySize, ElementFormat format, UINT sampleCount, UINT sampleQuality, UINT accessHint, ElementInitData* const initData);

		// 创建FrameBuffer
		virtual FrameBufferPtr CreateFrameBuffer();

		// 创建纹理采样状态
		virtual SamplerStatePtr CreateSamplerState( const SamplerStateDesc& desc);

		// 创建光栅化状态
		virtual RasterizerStatePtr& CreateRasterizerState( const RasterizerStateDesc& desc);

		// 创建深度模板状态
		virtual DepthStencilStatePtr CreateDepthStencilState( const DepthStencilStateDesc& desc);

		// 创建混合状态 
		virtual BlendStatePtr CreateBlendState( const BlendStateDesc& desc);

		// 创建RenderView
		virtual RenderViewPtr Create2DRenderView( TexturePtr texture, int firstArrayIndex, int arraySize, int levle);

		// 创建深度View
		virtual RenderViewPtr Create2DDepthStencilRenderView( UINT width, UINT height, ElementFormat format, UINT sampleCount, UINT sampleQuality);

		// 创建深度View
		virtual RenderViewPtr Create2DDepthStencilRenderView( TexturePtr& texture, int firstArrayIndex, int arraySize, int level);

	public:
		// 设置光栅化状态
		void RSSetState( const ID3D11RasterizerStatePtr& ras);

		// 设置深度模板缓冲状态
		void OMSetDepthStencilState( const ID3D11DepthStencilStatePtr& ds, UINT stencilRef);

		// 设置混合状态
		void OMSetBlendState( const ID3D11BlendStatePtr& bs, const ColorRGB blendFactor, UINT sampleMask);

		// 分离SRV
		void DetachSRV( void* rtvSrc, UINT rtFirstSubres, UINT rtNumsubRes);

		// 设置视口
		void RSSetViewports( UINT numViewPorts, const D3D11_VIEWPORT* viewPorts);

		// 设置顶点着色器
		void VSSetShader( const ID3D11VertexShaderPtr& shader);

		// 设置像像素着色器
		void PSSetShader( const ID3D11PixelShaderPtr& shader);

		// 设置几何着色器
		void GSSetShader( const ID3D11GeometryShaderPtr& shader);

		// 设置计算着色器
		void CSSetShader( const ID3D11ComputeShaderPtr& shader);

		// 设置Hull着色器
		void HSSetShader( const ID3D11HullShaderPtr& shader);

		// 设置Domain着色器
		void DSSetShader( const ID3D11DomainShaderPtr& shader);

		// 设置常量Buffer
		void SetConstantBuffers( const vector<ID3D11BufferPtr>& cbs, ShaderType shaderType);

		// 设置资源
		void SetShaderResources( const vector<ID3D11ShaderResourceViewPtr>& res, ShaderType shaderType);

		// 设置采样状态
		void SetSamplers( const vector<ID3D11SamplerStatePtr>& samplers, ShaderType shaderType);

		// 设置拓扑类型
		void SetTopologyType( D3D11_PRIMITIVE_TOPOLOGY topologyType);

	private:
		// 填充渲染设备能力
		void FillRenderDeviceCaps();

		// 是否顶点格式支持
		bool IsVertexFormatSupport( ElementFormat eleformat);

		// 是否支持纹理格式
		bool IsTextureFormatSupport( ElementFormat eleformat);

		// 是否支持RenderTarget格式
		bool IsRenderTargetFormatSupport( ElementFormat eleformat, UINT sampleCount, UINT sampleQuality);

	private:
		UINT						m_numBuffers;				// 当前Buffer总数
		D3D11AdapterList			m_adapterList;				// 适配器列表

		IDXGIFactory1Ptr			m_giFactory;				// IDXGIFactory1
		ID3D11DevicePtr				m_d3d11Device;				// D3D11Device
		ID3D11DeviceContextPtr		m_d3d11ImmContex;			// D3D11ImmContex
		D3D_FEATURE_LEVEL			m_d3dFeatureLevel;			// D3DFeatureLevel

		D3D11_PRIMITIVE_TOPOLOGY	m_topologyTypeCache;		// 拓扑类型
		ID3D11RasterizerStatePtr	m_rasterizerStateCache;		// 光栅状态缓存
		ID3D11DepthStencilStatePtr  m_depthStencilStateCache;	// 深度模板状态缓存
		UINT						m_stencilRefCache;			
		ID3D11BlendStatePtr			m_blendStateCache;
		ColorRGB					m_blendFactorCache;
		UINT						m_sampleMaskCache;
		ID3D11VertexShaderPtr		m_vertexShaderCache;		// 顶点着色器缓冲
		ID3D11PixelShaderPtr		m_pixelShaderCache;			// 像素着色器缓冲
		ID3D11GeometryShaderPtr		m_geometryShaderCache;		// 几何着色器缓冲
		ID3D11ComputeShaderPtr		m_compueShaderCache;		// 计算着色器缓冲
		ID3D11HullShaderPtr			m_hullShaderCache;			// Hull着色器缓冲
		ID3D11DomainShaderPtr		m_domainShderCache;			// Domain着色器缓冲

		set<ElementFormat>			m_vertexFormats;			// 支持的格式
		set<ElementFormat>			m_textureFormats;			// 支持的纹理格式
		RenderTargetFormat			m_renderTargetFormats;		// 渲染目标支持的纹理格式

		vector<D3D11RenderWindow*>	m_renderWindows;			// 渲染窗口
		RenderLayout*				m_preLayout; 
		RenderMaterial*				m_preMaterial;
	};

	#define PDRenderDevice D3D11RenderDevice::GetSingleton()
	#define PDLogSystem	   PDRenderDevice.GetLogSystem()
}