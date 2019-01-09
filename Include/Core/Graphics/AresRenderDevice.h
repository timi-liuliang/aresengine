#pragma once

#include "AresTexture.h"
#include "AresRenderState.h"
#include "AresRenderWindow.h"
#include "AresFrameBuffer.h"
#include "AresRenderSettings.h"
#include "AresRenderDeviceCaps.h"
#include "AresRenderAPITypedefs.h"
#include "AresGraphicsBuffer.h"
#include "AresRenderMaterial.h"
#include "Core/AresLogSystem.h"

using namespace std;

namespace Ares
{
	//--------------------------------
	// RenderDevice 2012-02-11 帝林
	//--------------------------------
	class RenderDevice
	{
		typedef map<SamplerStateDesc,    SamplerStatePtr>	 SamplerStatePool;
		typedef map<RasterizerStateDesc, RasterizerStatePtr> RasterizerStatePool;
		typedef map<BlendStateDesc,		 BlendStatePtr>		 BlendStatePool;
	public:
		RenderDevice( RenderDeviceType api);
		virtual ~RenderDevice();

		// 开始帧
		virtual void BeginFrame()=0;

		// 结束帧
		virtual void EndFrame()=0;

		// 设置日志系统
		void SetLogSystem( LogSystem* logSystem);

		// 获取日志系统
		LogSystem& GetLogSystem();

		// 获取名字
		virtual const char* GetName() const=0;

		// 获取当前FrameBuffer
		FrameBufferPtr GetCurFrameBuffer() const;

		// 获取当前光栅化状态
		const RasterizerStatePtr GetCurRasterState() const { return m_curRasterState; } 

		// 获取当前深度状态
		const DepthStencilStatePtr GetCurDepthStencilState() const { return m_curDepthState; }

		// 绑定FrameBuffer
		void BindFrameBuffer( FrameBufferPtr frameBuffer);

		// 解绑当前FrameBuffer
		void ResetFrameBuffer();

	public:
		// 创建渲染窗口
		virtual RenderWindow* CreateRenderWindow( const RenderSettings& settings);

		// 设置状态对象
		void SetStateObjects();

	public:
		// 创建效果文件 "*.glsl"
		virtual RenderMethodPtr CreateRenderMethod()=0;

		// 创建RenderLayout
		virtual RenderLayoutPtr CreateRenderLayout()=0;

		// 创建索引缓冲区
		virtual GraphicBufferPtr CreateIndexBuffer( BufferUsage usage, UINT accessHint, const ElementInitData* initData, ElementFormat format=EF_UnKnown)=0;

		// 创建顶点缓冲区
		virtual GraphicBufferPtr CreateVertexBuffer( BufferUsage usage, UINT accessHint,const ElementInitData* initData, ElementFormat format=EF_UnKnown)=0;

		// 创建1D纹理
		virtual TexturePtr CreateTexture1D( UINT widht, UINT numMipMaps, UINT arraySize, ElementFormat format, UINT sampleCount, UINT sampleQuality, UINT accessHine, const ElementInitData* initData)=0;

		// 创建2D纹理
		virtual TexturePtr CreateTexture2D( UINT width, UINT height, UINT numMipMaps, UINT arraySize, ElementFormat format, UINT sampleCount, UINT sampleQuality, UINT accessHint, ElementInitData* const initData)=0;

		// 创建FrameBuffer
		virtual FrameBufferPtr CreateFrameBuffer()=0;

		// 创建纹理采样状态
		virtual SamplerStatePtr CreateSamplerState( const SamplerStateDesc& desc)=0;

		// 创建光栅化状态
		virtual RasterizerStatePtr& CreateRasterizerState( const RasterizerStateDesc& desc)=0;

		// 创建深度模板状态
		virtual DepthStencilStatePtr CreateDepthStencilState( const DepthStencilStateDesc& desc)=0;

		// 创建混合状态 
		virtual BlendStatePtr CreateBlendState( const BlendStateDesc& desc)=0;

		// 创建RenderView
		virtual RenderViewPtr Create2DRenderView( TexturePtr texture, int firstArrayIndex, int arraySize, int levle)=0;

		// 创建深度View
		virtual RenderViewPtr Create2DDepthStencilRenderView( UINT widht, UINT height, ElementFormat format, UINT sampleCount, UINT sampleQuality)=0;

		// 创建深度View
		virtual RenderViewPtr Create2DDepthStencilRenderView( TexturePtr& texture, int firstArrayIndex, int arraySize, int level)=0;

	public:
		// 获取设备类型
		RenderDeviceType GetType() const { return m_api; }

		// 获取设备能力
		const RenderDeviceCaps& GetDeviceCaps() const;

		// 渲染Renderable
		virtual void Render(  RenderLayout* layout, RenderMaterial* material)=0;

	protected:
		RenderDeviceType		m_api;					// 渲染设备类型	
		RenderDeviceCaps		m_deviceCaps;			// 设备能力
		FrameBufferPtr			m_curFrameBuffer;		// 当前FrameBuffer
		RasterizerStatePtr		m_curRasterState;		// 当前光栅化状态
		DepthStencilStatePtr	m_curDepthState;		// 光前深度状态

		SamplerStatePool		m_ssPool;				// 光栅化状态池
		RasterizerStatePool		m_rsPool;				// 纹理采样状态池
		BlendStatePool			m_bsPool;				// 混合状态池
		LogSystem*				m_logSystem;			// 日志系统		
	};
    
#ifdef ARES_PLATFORM_IOS
    // 启动插件
    void  DllStartRenderFactory( RenderDevice*& ptr);
#endif
}