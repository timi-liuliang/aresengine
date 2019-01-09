#include <Engine/RenderSystem/AresGaussianBlur.h>
#include <Engine/AresRoot.h>

#define KernelSize 25

namespace Ares
{
	const float BlurTexOffseter::WeightTable1x1[1] =
	{
		1.f
	};

	const float BlurTexOffseter::WeightTable3x3[9] = 
	{
		0.0625f,  0.125f, 0.0625f,
		0.125f,   0.25f,  0.125f,
		0.0625f,  0.125f, 0.0625f
	};

	// 7x7权重表
	const float BlurTexOffseter::WeightTable7x7[49] =
	{  
		0.00000067f, 0.00002292f, 0.00019117f, 0.00038771f, 0.00019117f, 0.00002292f, 0.00000067f, 
		0.00002292f, 0.00078633f, 0.00655965f, 0.01330373f, 0.00655965f, 0.00078633f, 0.00002292f, 
		0.00019117f, 0.00655965f, 0.05472157f, 0.11098164f, 0.05472157f, 0.00655965f, 0.00019117f, 
		0.00038771f, 0.01330373f, 0.11098164f, 0.22508352f, 0.11098164f, 0.01330373f, 0.00038771f, 
		0.00019117f, 0.00655965f, 0.05472157f, 0.11098164f, 0.05472157f, 0.00655965f, 0.00019117f, 
		0.00002292f, 0.00078633f, 0.00655965f, 0.01330373f, 0.00655965f, 0.00078633f, 0.00002292f, 
		0.00000067f, 0.00002292f, 0.00019117f, 0.00038771f, 0.00019117f, 0.00002292f, 0.00000067f
	};

	const float GaussianBlur::WeightTable1x49[49] = 
	{  
		0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 
		0.02f, 
		0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f 
	};

	// 构造函数
	BlurTexOffseter::BlurTexOffseter()
		: m_width( INVALID)
		, m_height( INVALID)
	{
		m_texOffset = MakeShaderParam();
	}

	// 设置参数
	void BlurTexOffseter::SetInput( UINT width, UINT height, const float* weightTable, int tableMembers)
	{
		if( m_width!=width || m_height!=height || m_kernelSize != tableMembers)
		{
			m_width = width;
			m_height= height;

			m_kernelSize   = tableMembers;
			m_kernelRadius = (int)( sqrtf( (float)(m_kernelSize)) / 2);

			vector<float4> texOffset( m_kernelSize);
			float texelW					= 1.f / static_cast<float>(m_width);
			float texelH					= 1.f / static_cast<float>(m_height);

			for( int i=-m_kernelRadius; i<=m_kernelRadius; i++)
			{
				for( int j=-m_kernelRadius; j<=m_kernelRadius; j++)
				{
					int idx     = ( j+m_kernelRadius) * ( m_kernelRadius*2+1) + (i+m_kernelRadius);
					texOffset[idx] = float4( i*texelW, j*texelH, 0.f, weightTable[idx]);
				}
			}

			*m_texOffset = texOffset;
		}
	}

	// 设置参数
	void BlurTexOffseter::SetInput( TexturePtr texture, const float* weightTable, int tableMembers)
	{
		UINT width = texture->GetWidth( 0);
		UINT height= texture->GetHeight( 0);

		SetInput( width, height, weightTable, tableMembers);
	}

	// 构造函数
	GaussianBlur::GaussianBlur( Scene& scene, bool hdrTexture)
		: FrameFilter( scene)
	{
		// 设置布局
		m_renderable.SetRenderlayout( FrameFilter::GetScreenQuadRenderLayout());

		// 初始化材质
		ShaderMacros macros;
		if( hdrTexture)
			macros.push_back( ShaderMacro( "HDR_TEXTURE", ""));
		RenderMethodPtr rMehtod = AResSystem.LoadMethod( "GaussianBlur.hlsl", macros);
        m_renderable.GetMaterial()->SetMethod( rMehtod);

		m_texOffset   = MakeShaderParam();
		m_blurTexture = MakeShaderParam();
		m_renderable.GetMaterial()->SetParamRef( "TexOffset", m_texOffset);
		m_renderable.GetMaterial()->SetParamRef( "BlurTexture", m_blurTexture);

		// 光栅化状态
		RasterizerStateDesc   rDesc;
		rDesc.m_cullMode = CM_None;
		RasterizerStatePtr	rasterState = ARenderDevice->CreateRasterizerState( rDesc);
		m_renderable.GetMaterial()->SetRasterizerState( rasterState);

		DepthStencilStateDesc dDesc;
		dDesc.m_depthEnable = false;
		dDesc.m_depthWriteMask = false;
		dDesc.m_depthFun	   = CF_Always;
		DepthStencilStatePtr depthStencilState = ARenderDevice->CreateDepthStencilState( dDesc);
		m_renderable.GetMaterial()->SetDepthStencilState( depthStencilState);
	}

	// 析构函数
	GaussianBlur::~GaussianBlur()
	{

	}

	// 执行模糊
	void GaussianBlur::Execute()
	{
		// 水平方向模糊
		*m_texOffset   = m_texOffsetX;
		*m_blurTexture = m_inputTexture;
		ARenderDevice->BindFrameBuffer( m_middleFrameBuffer);
		FrameFilter::Execute();

		// 垂直方向模糊
		*m_texOffset   = m_texOffsetY;
		*m_blurTexture = m_middleTexture;
		ARenderDevice->BindFrameBuffer( m_finalFrameBuffer);
		FrameFilter::Execute();
	}

	// 设置输入
	void GaussianBlur::SetInput( TexturePtr& oTexture, const TexturePtr& iTexture, const float* weightTable, int tableMembers)
	{
		A_ASSERT( iTexture->GetWidth( 0)== oTexture->GetWidth( 0));
		A_ASSERT( iTexture->GetHeight(0)== oTexture->GetHeight(0));

		m_inputTexture = iTexture;
		if( !m_inputTexture) return;

		UINT width = iTexture->GetWidth( 0);
		UINT height= iTexture->GetHeight( 0);
		if( !m_middleTexture || m_middleTexture->GetWidth(0)!= iTexture->GetWidth(0) || m_middleTexture->GetHeight( 0)!=iTexture->GetHeight(0))
		{
			m_middleFrameBuffer = ARenderDevice->CreateFrameBuffer();
			m_middleTexture		= ARenderDevice->CreateTexture2D( width, height, 1, 1, iTexture->GetFormat(), 1, 0, EAH_GPURead | EAH_GPUWrite, NULL);
			m_middleFrameBuffer->AttachRenderView( FrameBuffer::ATT_Color0, ARenderDevice->Create2DRenderView( m_middleTexture, 0, 1, 0));
		
			m_finalFrameBuffer  = ARenderDevice->CreateFrameBuffer();
			m_finalFrameBuffer->AttachRenderView( FrameBuffer::ATT_Color0, ARenderDevice->Create2DRenderView( oTexture, 0, 1, 0));

			int kernelSize					= tableMembers;
			int kernelRadius				= (int)( ( (float)(kernelSize)) / 2.f);
			float texelW					= 1.f / static_cast<float>(width);
			float texelH					= 1.f / static_cast<float>(height);

			m_texOffsetX.resize( kernelSize);
			m_texOffsetY.resize( kernelSize);
			for( int i=-kernelRadius; i<=kernelRadius; i++)
			{
				int baseIdx = i + kernelRadius;
				m_texOffsetX[baseIdx] = float4( (float)i * texelW, 0.f, 0.f, weightTable[baseIdx]);
				m_texOffsetY[baseIdx] = float4( 0.f, (float)i * texelH, 0.f, weightTable[baseIdx]);
			}
		}
	}
}