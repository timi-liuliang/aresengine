#include "OGLESPlatform.h"
#include <Core/AresCore.h>
#include "OGLESRenderWindow.h"
#include "OGLESRenderLayout.h"

namespace Ares
{
	//-----------------------------------
	// OGLESRenderDevice 2013-02-05 帝林
	//-----------------------------------
	class OGLESRenderDevice : public RenderDevice, public Singleton<OGLESRenderDevice>
	{
		typedef map<SamplerStateDesc,    SamplerStatePtr>	 SamplerStatePool;
		typedef map<RasterizerStateDesc, RasterizerStatePtr> RasterizerStatePool;
		typedef map<BlendStateDesc,		 BlendStatePtr>		 BlendStatePool;
	public:
		OGLESRenderDevice();
		~OGLESRenderDevice();

	public:
		// 开始帧
		virtual void BeginFrame();

		// 结束帧
		virtual void EndFrame();

		// 创建渲染窗口
		virtual RenderWindow* CreateRenderWindow( const RenderSettings& settings);

		// 获取渲染窗口
		virtual RenderWindow* GetRenderWindow() { return m_renderWindow; }

	public:
		// 获取名字
		virtual const char* GetName() const;

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
		virtual TexturePtr CreateTexture2D( UINT width, UINT height, UINT numMipMaps, UINT arraySize, ElementFormat format, UINT sampleCount, UINT sampleQuality, UINT accessHint, ElementInitData* const initData);

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
		virtual RenderViewPtr Create2DDepthStencilRenderView( UINT widht, UINT height, ElementFormat format, UINT sampleCount, UINT sampleQuality);

		// 创建深度View
		virtual RenderViewPtr Create2DDepthStencilRenderView( TexturePtr& texture, int firstArrayIndex, int arraySize, int level);

		// 渲染Renderable
		virtual void Render(  RenderLayout* layout, RenderMaterial* material);

		// OPENGL 相关
	public:
		// 绑定缓冲
		void BindBuffer( GLenum target, GLuint buffer);

		// 删除缓冲
		void DeleteBuffers( GLsizei n, const GLuint* buffers);

		// 绑定Fbo
		void BindFrameBufferObject( GLuint fbo, bool force=false);

		// 获取当前帧缓存对象
		GLuint GetCurFrameBufferObject() { return m_curFbo; }

	private:
		OGLESRenderWindow*		m_renderWindow;			// 渲染窗口

		SamplerStatePool		m_ssPool;				// 光栅化状态池
		RasterizerStatePool		m_rsPool;				// 纹理采样状态池

		map<GLenum, GLuint>		m_bindedBuffer;			// 缓冲
		bool					m_fbSRGBCache;			// 帧缓存对象SRGB
		GLuint					m_curFbo;				// 当前帧缓存对象

		RenderLayout*			m_preLayout; 
		RenderMaterial*			m_preMaterial;
	};

	#define PDRenderDevice OGLESRenderDevice::GetSingleton()
	#define PDLogSystem	   PDRenderDevice.GetLogSystem()
}