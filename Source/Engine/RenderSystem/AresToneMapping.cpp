#include <Engine/RenderSystem/AresToneMapping.h>
#include <Engine/AresRoot.h>

namespace Ares
{
	// 构造函数
	ToneMapping::ToneMapping( Scene& scene)
		: FrameFilter( scene)
		, m_renderAutoExposure( m_renderable)
	{
		m_spabTextureFullSize = MakeShaderParam();
		m_lumianceTable		  = MakeShaderParam( float3( 0.27f, 0.67f, 0.06f));

		// 初始化纹理 FrameBuffer
		Int2 sizes[] = { Int2(),Int2( 256, 256), Int2( 64, 64), Int2( 16, 16), Int2(4, 4), Int2( 1, 1), Int2( 1, 1), Int2( 1, 1) };
		for ( int i = FBT_DownSampled256x256; i<=FBT_LuminanceTemp; i++)
		{
			m_textures[i]	  = ARenderDevice->CreateTexture2D( static_cast<UINT>( sizes[i].x), static_cast<UINT>( sizes[i].y), 1, 1, EF_ABGR16F, 1, 0, EAH_GPURead | EAH_GPUWrite, NULL);
			m_spabTextures[i] = MakeShaderParam( m_textures[i]);
			m_frameBuffers[i] = ARenderDevice->CreateFrameBuffer();
			m_frameBuffers[i]->AttachRenderView( FrameBuffer::ATT_Color0,	   ARenderDevice->Create2DRenderView( m_textures[i], 0, 1, 0));
		}

		// PointSamplerState
		SamplerStateDesc ssDesc;
		ssDesc.m_filter	= TFO_MinMagMipPoint;
		ShaderParamPtr pointSamplerState = MakeShaderParam( ARenderDevice->CreateSamplerState( ssDesc));

		// 初始化亮度Renderable
		ShaderMacros macros;
		macros.clear(); macros.push_back( ShaderMacro( "LUMINANCE", ""));
		RenderMethodPtr rMethod = AResSystem.LoadMethod( "ToneMapping", macros);
		m_luminanceRenderable.GetMaterial()->SetMethod( rMethod);
		m_luminanceRenderable.GetMaterial()->SetParamRef( "G_LumianceTable", m_lumianceTable);
		m_luminanceRenderable.GetMaterial()->SetParamRef( "G_DiffuseTex", m_spabTextureFullSize);
		m_luminanceRenderable.SetRenderlayout( FrameFilter::GetScreenQuadRenderLayout());

		// DownSampledRenderable
		macros.clear(); macros.push_back( ShaderMacro( "AVERAGESAMPLES", ""));
		rMethod = AResSystem.LoadMethod( "ToneMapping", macros);
		m_renderableDownSampled.GetMaterial()->SetMethod( rMethod);
		m_renderableDownSampled.GetMaterial()->SetParamRef( "G_PointerSampler", pointSamplerState);
		m_renderableDownSampled.SetRenderlayout( FrameFilter::GetScreenQuadRenderLayout());

		vector<float4> texOffset256x256(16);
		vector<float4> texOffset64x64(16);
		vector<float4> texOffset16x16(16);
		vector<float4> texOffset4x4(16);
		int index = 0;
		for( int y=0; y<4; y++)
		{
			for ( int x=0; x<4; x++)
			{
				texOffset256x256[index] = float4( x/256.f, y/256.f, 0.f, 0.f);
				texOffset64x64[index]   = float4( x/64.f,  y/64.f,  0.f, 0.f);
				texOffset16x16[index]   = float4( x/16.f,  y/16.f,  0.f, 0.f);
				texOffset4x4[index]     = float4( x/4.f,   y/4.f,   0.f, 0.f);
				index++;
			}
		}
		m_downSampledOffset[0] = MakeShaderParam( texOffset256x256);
		m_downSampledOffset[1] = MakeShaderParam( texOffset64x64);
		m_downSampledOffset[2] = MakeShaderParam( texOffset16x16);
		m_downSampledOffset[3] = MakeShaderParam( texOffset4x4);

		// Initialize renderable exp luminance
		macros.clear(); macros.push_back( ShaderMacro( "EXPLUMINANCE", ""));
		rMethod = AResSystem.LoadMethod( "ToneMapping", macros);
		m_renderableExpLuminance.GetMaterial()->SetMethod( rMethod);
		m_renderableExpLuminance.GetMaterial()->SetParamRef( "G_PointerSampler", pointSamplerState);
		m_renderableExpLuminance.SetRenderlayout( FrameFilter::GetScreenQuadRenderLayout());

		// Initialize LuminanceAdaptive renderable
		macros.clear(); macros.push_back( ShaderMacro( "ADAPTIVELUMINANCE", ""));
		rMethod = AResSystem.LoadMethod( "ToneMapping", macros);
		m_renderableLuminanceAdaptive.GetMaterial()->SetMethod( rMethod);

		m_adaptiveSpeed = MakeShaderParam( 0.02f);
		m_renderableLuminanceAdaptive.GetMaterial()->SetParamRef( "G_AdaptiveSpeed", m_adaptiveSpeed);
		m_renderableLuminanceAdaptive.GetMaterial()->SetParamRef( "G_PointerSampler", pointSamplerState);
		m_renderableLuminanceAdaptive.SetRenderlayout( FrameFilter::GetScreenQuadRenderLayout());

		// Initialize renderable auto exposure
		macros.clear(); macros.push_back( ShaderMacro( "AUTOEXPOSURE", ""));
		rMethod = AResSystem.LoadMethod( "ToneMapping", macros);
		m_renderAutoExposure.GetMaterial()->SetMethod( rMethod);
		m_renderAutoExposure.GetMaterial()->SetParamRef( "G_LumianceTable", m_lumianceTable);
		m_renderAutoExposure.GetMaterial()->SetParamRef( "G_DiffuseTex", m_spabTextureFullSize);
		m_multiplierClamp = MakeShaderParam( float4( 0.2f, 3.f, 0.f, 0.f));
		m_renderAutoExposure.GetMaterial()->SetParamRef( "G_MultiplierClamp", m_multiplierClamp);
		m_middleGray	  = MakeShaderParam( 0.5f);
		m_renderAutoExposure.GetMaterial()->SetParamRef( "G_MiddleGray", m_middleGray);
		m_renderAutoExposure.SetRenderlayout( FrameFilter::GetScreenQuadRenderLayout());
	}

	// 析构函数
	ToneMapping::~ToneMapping()
	{

	}

	// 设置源纹理
	bool ToneMapping::SetSrcTexture( TexturePtr& src, FrameBufferPtr& oFrameBuffer)
	{
		m_output = &oFrameBuffer;
		*m_spabTextureFullSize = src;

		return true;
	}

	// 转换为亮度
	void ToneMapping::ConvertToLogLuminance()
	{
		ARenderDevice->BindFrameBuffer( m_frameBuffers[FBT_DownSampled256x256]);

		m_luminanceRenderable.Render();
	}

	// 求平均亮度
	void ToneMapping::AverageLuminance()
	{
		// 256x256 -> 64x64
		ARenderDevice->BindFrameBuffer( m_frameBuffers[FBT_DownSampled64x64]);
		m_renderableDownSampled.GetMaterial()->SetParamRef( "G_DiffuseTex", m_spabTextures[FBT_DownSampled256x256]);
		m_renderableDownSampled.GetMaterial()->SetParamRef( "G_TexOffset", m_downSampledOffset[0]);
		m_renderableDownSampled.Render();

		// 64x64 -> 16x16
		ARenderDevice->BindFrameBuffer( m_frameBuffers[FBT_DownSampled16x16]);
		m_renderableDownSampled.GetMaterial()->SetParamRef( "G_DiffuseTex", m_spabTextures[FBT_DownSampled64x64]);
		m_renderableDownSampled.GetMaterial()->SetParamRef( "G_TexOffset", m_downSampledOffset[1]);
		m_renderableDownSampled.Render();

		// 16x16-> 4x4
		ARenderDevice->BindFrameBuffer( m_frameBuffers[FBT_DownSampled4x4]);
		m_renderableDownSampled.GetMaterial()->SetParamRef( "G_DiffuseTex", m_spabTextures[FBT_DownSampled16x16]);
		m_renderableDownSampled.GetMaterial()->SetParamRef( "G_TexOffset", m_downSampledOffset[2]);
		m_renderableDownSampled.Render();

		// 4x4->1x1
		ARenderDevice->BindFrameBuffer( m_frameBuffers[FBT_LuminanceCurrent]);
		m_renderableDownSampled.GetMaterial()->SetParamRef( "G_DiffuseTex", m_spabTextures[FBT_DownSampled4x4]);
		m_renderableDownSampled.GetMaterial()->SetParamRef( "G_TexOffset", m_downSampledOffset[3]);
		m_renderableDownSampled.Render();
	}

	// 交换亮度结果
	void ToneMapping::SwapLuminanceFrameBuffer( FrameBufferType type0, FrameBufferType type1)
	{
		std::swap( m_textures[type0],	  m_textures[type1]);
		std::swap( m_frameBuffers[type0], m_frameBuffers[type1]);
		std::swap( m_spabTextures[type0], m_spabTextures[type1]);
	}

	// 调整亮度
	void ToneMapping::AdaptiveLuminance()
	{
		ARenderDevice->BindFrameBuffer( m_frameBuffers[FBT_LuminanceTemp]);
		m_renderableExpLuminance.GetMaterial()->SetParamRef( "G_DiffuseTex", m_spabTextures[FBT_LuminanceCurrent]);
		m_renderableExpLuminance.Render();
		SwapLuminanceFrameBuffer( FBT_LuminanceCurrent, FBT_LuminanceTemp);

		ARenderDevice->BindFrameBuffer( m_frameBuffers[FBT_LuminanceTemp]);
		m_renderableLuminanceAdaptive.GetMaterial()->SetParamRef( "G_PreLuminanceTex", m_spabTextures[FBT_LuminancePrevious]);
		m_renderableLuminanceAdaptive.GetMaterial()->SetParamRef( "G_CurLuminanceTex", m_spabTextures[FBT_LuminanceCurrent]);
		m_renderableLuminanceAdaptive.Render();
	}

	// 自动暴光
	void ToneMapping::AutoExposure()
	{
		ARenderDevice->BindFrameBuffer( *m_output);
		m_renderAutoExposure.GetMaterial()->SetParamRef( "G_AvgLuminanceTex", m_spabTextures[FBT_LuminanceTemp]);
		m_renderAutoExposure.Render();

		// 存储
		SwapLuminanceFrameBuffer( FBT_LuminancePrevious, FBT_LuminanceTemp);
	}

	// 执行滤镜
	void ToneMapping::Execute()
	{
		// 1.转换成亮度
		ConvertToLogLuminance();

		// 2.求平均亮度
		AverageLuminance();

		// 3.调整亮度
		AdaptiveLuminance();

		// 4.自动暴光
		AutoExposure();
	}
}