#include "OGLRenderDevice.h"
#include "OGLGraphicsBuffer.h"
#include "OGLRenderLayout.h"
#include "OGLRenderView.h"
#include "OGLTexture.h"
#include "OGLFrameBuffer.h"
#include "OGLRenderState.h"
#include "OGLRenderMethod.h"
#include "OGLPlatform.h"

namespace Ares
{
	template<> OGLRenderDevice* Singleton<OGLRenderDevice>::m_singleton = NULL;

	// 构造函数
	OGLRenderDevice::OGLRenderDevice()
		: m_renderWindow( NULL)
		, m_preLayout( NULL)
		, m_preMaterial( NULL)
	{

	}

	// 析构函数
	OGLRenderDevice::~OGLRenderDevice()
	{

	}

	// 获取名字
	const char* OGLRenderDevice::GetName() const
	{
		return "OGL";
	}

	// 开始帧
	void OGLRenderDevice::BeginFrame()
	{
		//ResetRenderStates();
	}

	// 结束帧
	void OGLRenderDevice::EndFrame()
	{

	}

	// 创建渲染窗口
	RenderWindow* OGLRenderDevice::CreateRenderWindow( const RenderSettings& settings)
	{
		RenderDevice::CreateRenderWindow( settings);
		if( !m_renderWindow)
		{
			// 填充设备能力
			FillRenderDeviceCaps();

			m_renderWindow = new_ OGLRenderWindow( settings);

			// 重置渲染状态
			//ResetRenderStates();

			// 绑定FrameBuffer
			BindFrameBuffer( m_renderWindow->GetFrameBuffer());
		}

		return m_renderWindow;
	}

	// 创建RenderLayout
	RenderLayoutPtr OGLRenderDevice::CreateRenderLayout()
	{
		return RenderLayoutPtr( new_ OGLRenderLayout());
	}

	// 创建效果文件 "*.hlsl"
	RenderMethodPtr OGLRenderDevice::CreateRenderMethod()
	{
		return RenderMethodPtr( new_ OGLRenderMethod());
	}

	// 创建索引缓冲区
	GraphicBufferPtr OGLRenderDevice::CreateIndexBuffer( BufferUsage usage, UINT accessHint, const ElementInitData* initData, ElementFormat format)
	{
		return GraphicBufferPtr( new_ OGLGraphicsBuffer( usage, accessHint, GL_ELEMENT_ARRAY_BUFFER, initData));
	}

	// 创建顶点缓冲区
	GraphicBufferPtr OGLRenderDevice::CreateVertexBuffer( BufferUsage usage, UINT accessHint,const ElementInitData* initData, ElementFormat format)
	{
		return GraphicBufferPtr( new_ OGLGraphicsBuffer( usage, accessHint, GL_ARRAY_BUFFER, initData));
	}

	// 创建1D纹理
	TexturePtr OGLRenderDevice::CreateTexture1D( UINT widht, UINT numMipMaps, UINT arraySize, ElementFormat format, UINT sampleCount, UINT sampleQuality, UINT accessHine, const ElementInitData* initData)
	{
		return TexturePtr();
	}

	// 创建2D纹理
	TexturePtr OGLRenderDevice::CreateTexture2D( UINT width, UINT height, UINT numMipMaps, UINT arraySize, ElementFormat format, UINT sampleCount, UINT sampleQuality, UINT accessHint, ElementInitData* const initData)
	{
		return TexturePtr( new_ OGLTexture2D( width, height, numMipMaps, arraySize, format, sampleCount, sampleQuality, accessHint, initData));
	}

	// 创建FrameBuffer
	FrameBufferPtr OGLRenderDevice::CreateFrameBuffer()
	{
		return FrameBufferPtr( new_ OGLFrameBuffer( this));
	}

	// 创建纹理采样状态
	SamplerStatePtr OGLRenderDevice::CreateSamplerState( const SamplerStateDesc& desc)
	{
		SamplerStatePool::iterator it = m_ssPool.find( desc);
		if( it==m_ssPool.end())
		{
			SamplerStatePtr samplerState = SamplerStatePtr( new_ OGLSamplerState( desc));
			if( samplerState)
				m_ssPool[desc] = samplerState;

			return samplerState;
		}

		return it->second;
	}

	// 创建光栅化状态
	RasterizerStatePtr& OGLRenderDevice::CreateRasterizerState( const RasterizerStateDesc& desc)
	{
		RasterizerStatePool::iterator it = m_rsPool.find( desc);
		if( it==m_rsPool.end())
		{		
			m_rsPool[desc] = RasterizerStatePtr( new_ OGLRasterizerState( desc));

			return m_rsPool[desc];
		}

		return it->second;
	}

	// 创建深度模板状态
	DepthStencilStatePtr OGLRenderDevice::CreateDepthStencilState( const DepthStencilStateDesc& desc)
	{
		return DepthStencilStatePtr( new_ OGLDepthStencilState( desc));
	}

	// 创建混合状态 
	BlendStatePtr OGLRenderDevice::CreateBlendState( const BlendStateDesc& desc)
	{
		return BlendStatePtr( new_ OGLBlendState( desc));
	}

	// 创建RenderView
	RenderViewPtr OGLRenderDevice::Create2DRenderView( TexturePtr texture, int firstArrayIndex, int arraySize, int levle)
	{
		return RenderViewPtr( new_ OGLTexture2DRenderView( texture, firstArrayIndex, levle));
	}

	// 创建深度View
	RenderViewPtr OGLRenderDevice::Create2DDepthStencilRenderView( UINT widht, UINT height, ElementFormat format, UINT sampleCount, UINT sampleQuality)
	{
		return RenderViewPtr( new_ OGLDepthStencilRenderView( widht, height, format, sampleCount, sampleQuality));
	}

	// 创建深度View
	RenderViewPtr OGLRenderDevice::Create2DDepthStencilRenderView( TexturePtr& texture, int firstArrayIndex, int arraySize, int level)
	{
		return RenderViewPtr();
	}

	// 渲染Renderable
	void OGLRenderDevice::Render(  RenderLayout* layout, RenderMaterial* material)
	{
		OGLRenderLayout* preRenderLayout = dynamic_cast<OGLRenderLayout*>( m_preLayout);
		OGLRenderLayout* curRenderLayout = dynamic_cast<OGLRenderLayout*>( layout);
		OGLRenderMethod* glslMethod	     = dynamic_cast<OGLRenderMethod*>( material->GetMethod().get());

		A_ASSERT( glslMethod);

		if( glslMethod->IsEnabled() && curRenderLayout)
		{
			// 设置材质(BindProgram, 设置参数状态)
			material->SetActive();

			// 绘制几何体
			curRenderLayout->Render( preRenderLayout, glslMethod);

			m_preLayout   = layout;
			m_preMaterial = material;
		}
	}

	// 绑定缓冲
	void OGLRenderDevice::BindBuffer( GLenum target, GLuint buffer)
	{
		map<GLenum, GLuint>::iterator it = m_bindedBuffer.find( target);
		if( (it==m_bindedBuffer.end()) || (it->second!=buffer))
		{
			OGLDebug( glBindBuffer( target, buffer));

			m_bindedBuffer[target] = buffer;
		}
	}

	// 删除缓冲
	void OGLRenderDevice::DeleteBuffers( GLsizei n, const GLuint* buffers)
	{
		for ( GLsizei i=0; i<n; i++)
		{
			map<GLenum, GLuint>::iterator it = m_bindedBuffer.find( buffers[i]);
			if( it!=m_bindedBuffer.end())
				m_bindedBuffer.erase( it);
		}

		OGLDebug( glDeleteBuffers( n, buffers));
	}

	// 颜色
	void OGLRenderDevice::EnableFrameBufferSRGB( bool srgb)
	{
		if( m_fbSRGBCache != srgb)
		{
			if( glloader_GL_ARB_framebuffer_sRGB)
			{
				if( srgb)
					glEnable( GL_FRAMEBUFFER_SRGB);
				else
					glEnable( GL_FRAMEBUFFER_SRGB);
			}

			m_fbSRGBCache = srgb;
		}
	}

	// 绑定Fbo
	void OGLRenderDevice::BindFrameBufferObject( GLuint fbo, bool force)
	{
		if( force || (m_curFbo!=fbo))
		{
			glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, fbo);

			m_curFbo = fbo;
		}
	}

	// 获取设备能力
	void OGLRenderDevice::FillRenderDeviceCaps()
	{
		GLint temp;

		glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, &temp);
		m_deviceCaps.m_maxVertexStreams = static_cast<BYTE>( temp);
	}
}