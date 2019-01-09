#include "OGLESRenderDevice.h"
#include "OGLESGraphicsBuffer.h"
#include "OGLESRenderLayout.h"
#include "OGLESRenderView.h"
#include "OGLESTexture.h"
#include "OGLESFrameBuffer.h"
#include "OGLESRenderState.h"
#include "OGLESRenderMethod.h"
#include "OGLESPlatform.h"

namespace Ares
{
	template<> OGLESRenderDevice* Singleton<OGLESRenderDevice>::m_singleton = NULL;

	// 构造函数
	OGLESRenderDevice::OGLESRenderDevice()
		: m_renderWindow( NULL)
		, m_preLayout( NULL)
		, m_preMaterial( NULL)
	{

	}

	// 析构函数
	OGLESRenderDevice::~OGLESRenderDevice()
	{

	}

	// 获取名字
	const char* OGLESRenderDevice::GetName() const
	{
		return "OGL";
	}

	// 开始帧
	void OGLESRenderDevice::BeginFrame()
	{
		//ResetRenderStates();
	}

	// 结束帧
	void OGLESRenderDevice::EndFrame()
	{
	}

	// 创建渲染窗口
	RenderWindow* OGLESRenderDevice::CreateRenderWindow( const RenderSettings& settings)
	{
		RenderDevice::CreateRenderWindow( settings);

		if( !m_renderWindow)
		{
			m_renderWindow = new_ OGLESRenderWindow( settings);

			// 重置渲染状态
			//ResetRenderStates();

			// 绑定FrameBuffer
			BindFrameBuffer( m_renderWindow->GetFrameBuffer());
		}

		return m_renderWindow;
	}

	// 创建RenderLayout
	RenderLayoutPtr OGLESRenderDevice::CreateRenderLayout()
	{
		return RenderLayoutPtr( new_ OGLESRenderLayout());
	}

	// 创建效果文件 "*.hlsl"
	RenderMethodPtr OGLESRenderDevice::CreateRenderMethod()
	{
		return RenderMethodPtr( new_ OGLESRenderMethod());
	}

	// 创建索引缓冲区
	GraphicBufferPtr OGLESRenderDevice::CreateIndexBuffer( BufferUsage usage, UINT accessHint, const ElementInitData* initData, ElementFormat format)
	{
		return GraphicBufferPtr( new_ OGLESGraphicsBuffer( usage, accessHint, GL_ELEMENT_ARRAY_BUFFER, initData));
	}

	// 创建顶点缓冲区
	GraphicBufferPtr OGLESRenderDevice::CreateVertexBuffer( BufferUsage usage, UINT accessHint,const ElementInitData* initData, ElementFormat format)
	{
		return GraphicBufferPtr( new_ OGLESGraphicsBuffer( usage, accessHint, GL_ARRAY_BUFFER, initData));
	}

	// 创建1D纹理
	TexturePtr OGLESRenderDevice::CreateTexture1D( UINT widht, UINT numMipMaps, UINT arraySize, ElementFormat format, UINT sampleCount, UINT sampleQuality, UINT accessHine, const ElementInitData* initData)
	{
		return TexturePtr();
	}

	// 创建2D纹理
	TexturePtr OGLESRenderDevice::CreateTexture2D( UINT width, UINT height, UINT numMipMaps, UINT arraySize, ElementFormat format, UINT sampleCount, UINT sampleQuality, UINT accessHint, ElementInitData* const initData)
	{
		return TexturePtr( new_ OGLESTexture2D( width, height, numMipMaps, arraySize, format, sampleCount, sampleQuality, accessHint, initData));
	}

	// 创建FrameBuffer
	FrameBufferPtr OGLESRenderDevice::CreateFrameBuffer()
	{
		return FrameBufferPtr( new_ OGLESFrameBuffer( this));
	}

	// 创建纹理采样状态
	SamplerStatePtr OGLESRenderDevice::CreateSamplerState( const SamplerStateDesc& desc)
	{
		SamplerStatePool::iterator it = m_ssPool.find( desc);
		if( it==m_ssPool.end())
		{
			SamplerStatePtr samplerState = SamplerStatePtr( new_ OGLESSamplerState( desc));
			if( samplerState)
				m_ssPool[desc] = samplerState;

			return samplerState;
		}

		return it->second;
	}

	// 创建光栅化状态
	RasterizerStatePtr& OGLESRenderDevice::CreateRasterizerState( const RasterizerStateDesc& desc)
	{
		RasterizerStatePool::iterator it = m_rsPool.find( desc);
		if( it==m_rsPool.end())
		{		
			m_rsPool[desc] = RasterizerStatePtr( new_ OGLESRasterizerState( desc));

			return m_rsPool[desc];
		}

		return it->second;
	}

	// 创建深度模板状态
	DepthStencilStatePtr OGLESRenderDevice::CreateDepthStencilState( const DepthStencilStateDesc& desc)
	{
		return DepthStencilStatePtr( new_ OGLESDepthStencilState( desc));
	}

	// 创建混合状态 
	BlendStatePtr OGLESRenderDevice::CreateBlendState( const BlendStateDesc& desc)
	{
		return BlendStatePtr( new_ OGLESBlendState( desc));
	}

	// 创建RenderView
	RenderViewPtr OGLESRenderDevice::Create2DRenderView( TexturePtr texture, int firstArrayIndex, int arraySize, int levle)
	{
		return RenderViewPtr( new_ OGLTexture2DRenderView( texture, firstArrayIndex, levle));
	}

	// 创建深度View
	RenderViewPtr OGLESRenderDevice::Create2DDepthStencilRenderView( UINT widht, UINT height, ElementFormat format, UINT sampleCount, UINT sampleQuality)
	{
		return RenderViewPtr( new_ OGLESDepthStencilRenderView( widht, height, format, sampleCount, sampleQuality));
	}

	// 创建深度View
	RenderViewPtr OGLESRenderDevice::Create2DDepthStencilRenderView( TexturePtr& texture, int firstArrayIndex, int arraySize, int level)
	{
		return RenderViewPtr();
	}

	// 渲染Renderable
	void OGLESRenderDevice::Render(  RenderLayout* layout, RenderMaterial* material)
	{
		OGLESRenderLayout* preRenderLayout = dynamic_cast<OGLESRenderLayout*>( m_preLayout);
		OGLESRenderLayout* curRenderLayout = dynamic_cast<OGLESRenderLayout*>( layout);
		OGLESRenderMethod* glslMethod	   = dynamic_cast<OGLESRenderMethod*>( material->GetMethod().get());

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
	void OGLESRenderDevice::BindBuffer( GLenum target, GLuint buffer)
	{
		map<GLenum, GLuint>::iterator it = m_bindedBuffer.find( target);
		if( (it==m_bindedBuffer.end()) || (it->second!=buffer))
		{
			OGLESDebug( glBindBuffer( target, buffer));

			m_bindedBuffer[target] = buffer;
		}
	}

	// 删除缓冲
	void OGLESRenderDevice::DeleteBuffers( GLsizei n, const GLuint* buffers)
	{
		for ( GLsizei i=0; i<n; i++)
		{
			map<GLenum, GLuint>::iterator it = m_bindedBuffer.find( buffers[i]);
			if( it!=m_bindedBuffer.end())
				m_bindedBuffer.erase( it);
		}

		OGLESDebug( glDeleteBuffers( n, buffers));
	}

	// 绑定Fbo
	void OGLESRenderDevice::BindFrameBufferObject( GLuint fbo, bool force)
	{
		if( force || (m_curFbo!=fbo))
		{
			OGLESDebug( glBindFramebuffer( GL_FRAMEBUFFER, fbo));

			m_curFbo = fbo;
		}
	}
}