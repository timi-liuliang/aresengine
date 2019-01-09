#include <Engine/AresRoot.h>
#include <Engine/RenderSystem/AresDeferredLighting.h>
#include <Engine/CameraSystem/AresCameraSystem.h>

namespace Ares
{
	// 构造函数
	DeferredLighting::DeferredLighting(  Scene& scene)
		: FrameFilter( scene)
	{
		CameraSystem& cameraSystem = m_scene.GetCameraSystem();

		// 初始化渲染布局
		m_lightsRenderLayout = ARenderDevice->CreateRenderLayout();
		m_lightsRenderLayout->SetTopologyType( RenderLayout::TT_TriangleList);

		GraphicBufferPtr posUVBuffer = ARenderDevice->CreateVertexBuffer( BU_Dynamic, EAH_CPUWrite | EAH_GPURead, NULL);

		m_elementType.push_back( VertexElement( VEU_Position,		0, EF_BGR32F));
		//m_elementType.push_back( VertexElement( VEU_TextureCoord,	0, EF_GR32F));
		m_elementType.push_back( VertexElement( VEU_TextureCoord,	0, EF_GR32F));
		m_elementType.push_back( VertexElement( VEU_TextureCoord,	1, EF_BGR32F));
		m_elementType.push_back( VertexElement( VEU_TextureCoord,	2, EF_BGR32F));

		m_lightsRenderLayout->BindVertexStream( posUVBuffer, m_elementType);

		m_renderable.SetRenderlayout( m_lightsRenderLayout);

		// 初始化材?
        RenderMethodPtr rMethod = AResSystem.LoadMethod("DeferredLighting.hlsl");
		m_renderable.GetMaterial()->SetMethod( rMethod);

		m_renderable.GetMaterial()->SetParamRef( "CameraView",		cameraSystem.GetSPAB( CameraSystem::SP_View));
		m_renderable.GetMaterial()->SetParamRef( "CameraViewProj",  cameraSystem.GetSPAB( CameraSystem::SP_ViewProj));
		m_renderable.GetMaterial()->SetParamRef( "CameraPos",		cameraSystem.GetSPAB( CameraSystem::SP_Pos));
		m_renderable.GetMaterial()->SetParamRef( "CameraDirection", cameraSystem.GetSPAB( CameraSystem::SP_Direction));
		//m_renderable.GetMaterial()->SetParamRef( "G_CameraNearFar", cameraSystem.GetSPAB( CameraSystem::SP_NearFar));

		//m_texOffset	   = MakeShaderParam();
		m_mrt1Texture  = MakeShaderParam();
		m_depthTexture = MakeShaderParam();
		m_renderable.GetMaterial()->SetParamRef( "TexOffset",	  m_blurTexOffseter.m_texOffset);
		m_renderable.GetMaterial()->SetParamRef( "MRT1Texture",  m_mrt1Texture);
		m_renderable.GetMaterial()->SetParamRef( "DepthTexture", m_depthTexture);

		SamplerStateDesc ssDesc;
		ssDesc.m_filter	   = TFO_MinMagMipLinear;
		ssDesc.m_addrModeU = TAM_Clamp;
		ssDesc.m_addrModeV = TAM_Clamp;
		ssDesc.m_addrModeW = TAM_Clamp;
		m_samplerState = MakeShaderParam( ARenderDevice->CreateSamplerState( ssDesc));
		m_renderable.GetMaterial()->SetParamRef( "WrapSampler", m_samplerState);

		// 点采样
		ssDesc.m_filter	   = TFO_MinMagMipPoint;
		m_pointSamplerState = MakeShaderParam( ARenderDevice->CreateSamplerState( ssDesc));
		m_renderable.GetMaterial()->SetParamRef( "PointSampler", m_samplerState);	

		// 光栅化状态
		RasterizerStateDesc   rDesc;
		rDesc.m_cullMode = CM_Back;
		RasterizerStatePtr	rasterState = ARenderDevice->CreateRasterizerState( rDesc);
		m_renderable.GetMaterial()->SetRasterizerState( rasterState);

		// 进行深度判断,但不写深度
		DepthStencilStateDesc dDesc;
		dDesc.m_depthEnable	   = true;
		dDesc.m_depthWriteMask = false;
		DepthStencilStatePtr depthStencilState = ARenderDevice->CreateDepthStencilState( dDesc);
		m_renderable.GetMaterial()->SetDepthStencilState( depthStencilState);

		// 混合状态 
		BlendStateDesc bDesc;
		bDesc.m_blendEnable.assign( true);
		bDesc.m_blendOp.assign( BOP_Add);
		bDesc.m_srcBlend.assign( ABF_One);
		bDesc.m_destBlend.assign( ABF_One);
		bDesc.m_blendOpAlpha.assign( BOP_Add);
		bDesc.m_srcBlendAlpha.assign( ABF_One);
		bDesc.m_destBlendAlpha.assign( ABF_Zero);
		bDesc.m_colorWriteMask.assign( ColorRGB::MASK_All);
		BlendStatePtr blendState = ARenderDevice->CreateBlendState( bDesc);
		m_renderable.GetMaterial()->SetBlendState( blendState, ColorRGB(0.f,0.f,0.f), 0xffffffff);

		m_vertexsCache.reserve( 1000);

		// AmbientRenderable
		{
			ShaderMacros macros;
			macros.clear(); macros.push_back( ShaderMacro( "AMBIENT_LIGHT", ""));
			RenderMethodPtr rMethod = AResSystem.LoadMethod("DeferredLighting.hlsl", macros);
			m_ambRenderable.GetMaterial()->SetMethod( rMethod);
			m_ambRenderable.GetMaterial()->SetRasterizerState( rasterState);
			m_ambRenderable.GetMaterial()->SetDepthStencilState( depthStencilState);
			m_ambRenderable.GetMaterial()->SetBlendState( blendState, ColorRGB(0.f,0.f,0.f), 0xffffffff);
			m_ambRenderable.SetRenderlayout( FrameFilter::GetScreenQuadRenderLayout());
		}
	}

	// 析构函数
	DeferredLighting::~DeferredLighting()
	{

	}

	// 执行滤镜
	void DeferredLighting::Execute()
	{
		if( m_ambientLight)
		{
			m_ambRenderable.GetMaterial()->SetParamRef( "G_AmbientColor", m_ambientLight->GetSPABColor());	
			m_ambRenderable.Render();
		}

		if( m_lights.size() || m_dominantLight)
		{
			m_renderable.Render();
		}
	}

	// 设置输入
	void DeferredLighting::SetInput( const TexturePtr& mrt1, const TexturePtr& zDistance, vector<Light*> lights,  const LightPtr& dominantLight)
	{
		m_ambientLight = NULL;
		foreach_( Light* light, lights)
		{
			if( light->GetTypeDetail()==ED_LightAmbient)
				m_ambientLight = dynamic_cast<AmbientLight*>(light);
		}


		*m_mrt1Texture = mrt1;
		*m_depthTexture= zDistance;
		m_lights	   = lights;
		m_dominantLight= dynamic_pointer_cast<DirectionalLight>( dominantLight);

		OgranizeRenderLayoutByLights();

		if( dominantLight)
		{
			A_ASSERT( dominantLight->GetLightType()==LT_Direction);

			// 设置主光源参数到Shader
			m_renderable.GetMaterial()->SetParamRef( "SplitPos",					m_dominantLight->GetCSMMgr()->GetSPAB( CSMMgr::SP_SplitPos));
			m_renderable.GetMaterial()->SetParamRef( "DomainantLightViewProj",		m_dominantLight->GetCSMMgr()->GetSPAB( CSMMgr::SP_ViewCropRemap));
			m_renderable.GetMaterial()->SetParamRef( "DomainantLightDepthTexture",	m_dominantLight->GetCSMMgr()->GetSPAB( CSMMgr::SP_DepthMap));
			m_renderable.GetMaterial()->SetParamRef( "NumSplits",					m_dominantLight->GetCSMMgr()->GetSPAB( CSMMgr::SP_NumSplits));

			// 计算模糊参数
			m_blurTexOffseter.SetInput( m_dominantLight->GetCSMMgr()->GetDepthTexture(), BlurTexOffseter::WeightTable3x3, 9);
		}
	}

	// 生成Rect3面数据
	void DeferredLighting::GenerateRect3Surface( vector<Vector3>& surface, const Rect3 rect)
	{
		surface.clear();
		surface.resize( 36);

		vector<Vector3> vertices; vertices.resize( 8);
		rect.GetEightPoints( vertices);

		surface.push_back( vertices[0]); surface.push_back( vertices[3]); surface.push_back( vertices[1]);
		surface.push_back( vertices[3]); surface.push_back( vertices[2]); surface.push_back( vertices[1]);

		surface.push_back( vertices[0]); surface.push_back( vertices[4]); surface.push_back( vertices[1]);
		surface.push_back( vertices[4]); surface.push_back( vertices[5]); surface.push_back( vertices[1]);

		surface.push_back( vertices[1]); surface.push_back( vertices[5]); surface.push_back( vertices[2]);
		surface.push_back( vertices[5]); surface.push_back( vertices[6]); surface.push_back( vertices[2]);

		surface.push_back( vertices[2]); surface.push_back( vertices[6]); surface.push_back( vertices[3]);
		surface.push_back( vertices[6]); surface.push_back( vertices[7]); surface.push_back( vertices[3]);

		surface.push_back( vertices[0]); surface.push_back( vertices[4]); surface.push_back( vertices[3]);
		surface.push_back( vertices[4]); surface.push_back( vertices[7]); surface.push_back( vertices[3]);

		surface.push_back( vertices[4]); surface.push_back( vertices[7]); surface.push_back( vertices[5]);
		surface.push_back( vertices[7]); surface.push_back( vertices[6]); surface.push_back( vertices[5]);
	}

	// 根据光源数据组织渲染Buffer
	void DeferredLighting::OgranizeRenderLayoutByLights()
	{
		if( !m_lights.size() && !m_dominantLight)
			return;

		CameraSystem& cameraSystem = m_scene.GetCameraSystem();
		Frustum3*	  cameraFrustum= cameraSystem.GetFrustum();

		// 近截面顶点
		vector<Vector3> plane( 4);
		cameraSystem.GetFrustum()->BuildPlane( plane, cameraSystem.GetFrustum()->GetNear()+0.001f);

		m_vertexsCache.clear();
		foreach_( Light* light, m_lights)
		{
			if( light->GetLightType() == LT_Direction)
			{
				ColorRGB tcolor		= light->GetColor();
				float2	 info		= float2( static_cast<float>(light->GetLightType()), 0.f);
				float3	 color		= float3( tcolor.r, tcolor.g, tcolor.b) * tcolor.a;
				float3	 direction  = light->GetDirection();

				// 1.组织渲染Buffer
				VertexFormat quad[6];
				quad[0].m_position = plane[0]; quad[0].m_info = info; quad[0].m_color=color; quad[0].m_direction=direction;
				quad[1].m_position = plane[3]; quad[1].m_info = info; quad[1].m_color=color; quad[1].m_direction=direction;
				quad[2].m_position = plane[1]; quad[2].m_info = info; quad[2].m_color=color; quad[2].m_direction=direction;
				quad[3].m_position = plane[3]; quad[3].m_info = info; quad[3].m_color=color; quad[3].m_direction=direction;
				quad[4].m_position = plane[2]; quad[4].m_info = info; quad[4].m_color=color; quad[4].m_direction=direction;
				quad[5].m_position = plane[1]; quad[5].m_info = info; quad[5].m_color=color; quad[5].m_direction=direction;

				foreach_( const VertexFormat& vertex, quad)
					m_vertexsCache.push_back( vertex);
			}
			else if( light->GetLightType() == LT_Point)
			{
				ColorRGB tcolor		= light->GetColor();
				float2	 info		= float2( static_cast<float>(light->GetLightType()), 0.f);
				float3	 color		= float3( tcolor.r, tcolor.g, tcolor.b) * tcolor.a;
				float3	 position   = light->GetTransform().GetTrans();
				float    plRadius;
				Vector3  plCenter;
				light->GetWorldBounds().BuildBoundingSphere( plCenter, plRadius);

				Vector3	 nr  = cameraFrustum->GetRight() * plRadius;
				Vector3	 nu  = cameraFrustum->GetUp() * plRadius;

				vector<Vector3> plPlane( 4);

				plPlane[0] = plCenter - nr - nu;
				plPlane[1] = plCenter + nr - nu;
				plPlane[2] = plCenter + nr + nu;
				plPlane[3] = plCenter - nr + nu;

				// 1.组织渲染Buffer
				VertexFormat quad[6];
				quad[0].m_position = plPlane[0]; quad[0].m_info = info; quad[0].m_color=color; quad[0].m_direction=position;
				quad[1].m_position = plPlane[3]; quad[1].m_info = info; quad[1].m_color=color; quad[1].m_direction=position;
				quad[2].m_position = plPlane[1]; quad[2].m_info = info; quad[2].m_color=color; quad[2].m_direction=position;
				quad[3].m_position = plPlane[3]; quad[3].m_info = info; quad[3].m_color=color; quad[3].m_direction=position;
				quad[4].m_position = plPlane[2]; quad[4].m_info = info; quad[4].m_color=color; quad[4].m_direction=position;
				quad[5].m_position = plPlane[1]; quad[5].m_info = info; quad[5].m_color=color; quad[5].m_direction=position;

				foreach_( const VertexFormat& vertex, quad)
					m_vertexsCache.push_back( vertex);
			}
		}

		if( m_dominantLight)
		{
			// 与上面的方向参数不同,故未提成函数
			ColorRGB tcolor		= m_dominantLight->GetColor();
			float3	 color		= float3( tcolor.r, tcolor.g, tcolor.b) * tcolor.a;
			float2	 info		= float2( static_cast<float>(m_dominantLight->GetLightType()), 1.f);
			float3	 direction  = m_dominantLight->GetDirection();

			// 1.组织渲染Buffer
			vector<VertexFormat> quad(6);
			quad[0].m_position = plane[0]; quad[0].m_info = info; quad[0].m_color=color; quad[0].m_direction=direction;
			quad[1].m_position = plane[3]; quad[1].m_info = info; quad[1].m_color=color; quad[1].m_direction=direction;
			quad[2].m_position = plane[1]; quad[2].m_info = info; quad[2].m_color=color; quad[2].m_direction=direction;
			quad[3].m_position = plane[3]; quad[3].m_info = info; quad[3].m_color=color; quad[3].m_direction=direction;
			quad[4].m_position = plane[2]; quad[4].m_info = info; quad[4].m_color=color; quad[4].m_direction=direction;
			quad[5].m_position = plane[1]; quad[5].m_info = info; quad[5].m_color=color; quad[5].m_direction=direction;

			foreach_( const VertexFormat& vertex, quad)
				m_vertexsCache.push_back( vertex);
		}

		// 2.更新顶点数据
		GraphicBufferPtr lightingBuffer = m_lightsRenderLayout->GetVertexStream( m_elementType);
		if( lightingBuffer)
		{
			UINT sizeInByte = sizeof(VertexFormat)*m_vertexsCache.size();

			lightingBuffer->Resize( sizeInByte);
			GraphicsBuffer::Modifier tModifier( lightingBuffer, BA_WriteOnly);

			tModifier.Set( m_vertexsCache.data(), sizeInByte);
		}
	}
}