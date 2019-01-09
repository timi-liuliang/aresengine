#include <Engine/RenderSystem/AresOcean.h>
#include <Engine/AresRoot.h>

namespace Ares
{
	// 构造函数
	Ocean::Ocean()
		: RenderEntity( ED_Ocean)
	{
		// 设置渲染方法
		RenderMethodPtr rMethod = AResSystem.LoadMethod("Ocean.hlsl");
		m_renderable.GetMaterial()->SetMethod( rMethod);

		// Lod因子
		m_lodFactor = MakeShaderParam( 0.98f);
		m_renderable.GetMaterial()->SetParamRef( "G_LodFactor", m_lodFactor);

		// 参数0
		m_oceanParams0 = MakeShaderParam( Vector4( 1.f, 4.f, 5.f, 1.5f));
		m_renderable.GetMaterial()->SetParamRef( "G_OceanParams0", m_oceanParams0);
		
		// 参数1
		m_oceanParams1 = MakeShaderParam( Vector4( 0.75f, -1.f, -1.f, 0.f));
		m_renderable.GetMaterial()->SetParamRef( "G_OceanParams1", m_oceanParams1);

		// 水颜色
		m_waterShallowColor = MakeShaderParam( Vector3( 0.f, 0.1f, 0.15f));
		m_renderable.GetMaterial()->SetParamRef( "G_WaterShallowColor", m_waterShallowColor);

		// 深水色
		m_waterDeepColor = MakeShaderParam( Vector3( 0.f, 0.1f, 0.125f));
		m_renderable.GetMaterial()->SetParamRef( "G_WaterDeepColor", m_waterDeepColor);

		// 折射因子
		m_fracStrength = MakeShaderParam( 0.03f);
		m_renderable.GetMaterial()->SetParamRef( "G_FracStrenght", m_fracStrength);

		// 泡沫
		m_texFoam	= MakeShaderParam( AResSystem.SyncLoadDDS( "foam.dds", EAH_GPURead | EAH_Immutable));
		m_renderable.GetMaterial()->SetParamRef( "G_TexFoam", m_texFoam);

		// 水面波形
		m_texWaveBump   = MakeShaderParam( AResSystem.SyncLoadDDS( "wavesbump.dds", EAH_GPURead | EAH_Immutable));
		m_renderable.GetMaterial()->SetParamRef( "G_TexWaveBump", m_texWaveBump);

		// 反射贴图
		m_texReflect	= MakeShaderParam( AResSystem.SyncLoadDDS( "reflect.dds", EAH_GPURead | EAH_Immutable));
		m_renderable.GetMaterial()->SetParamRef( "G_TexReflect", m_texReflect);

		// 渲染队列
		m_renderable.GetMaterial()->SetRenderQueue( RQ_Water);

		// 设置默认渲染Buffer
		SetScreenSpaceGridSize( 100);

		//// 测试用 设置光栅化状态
		//RasterizerStateDesc desc;
		//desc.m_fillMode = FM_WireFrame;
		//RasterizerStatePtr rasterizerState = ARenderDevice->CreateRasterizerState( desc);
		//m_renderable.GetMaterial()->SetRasterizerState( rasterizerState);
	}

	// 析构函数
	Ocean::~Ocean()
	{

	}

	// 设置屏幕空间格式大小
	void Ocean::SetScreenSpaceGridSize( int gridNum)
	{
		A_ASSERT( gridNum * gridNum < 65535);

		m_scrGridNum = gridNum;

		// define elementType
		VertexElementType elementTypePos;
		elementTypePos.push_back( VertexElement(VEU_Position, 0, EF_BGR32F));

		// 1.设置拓扑类型
		RenderLayoutPtr layout = ARenderDevice->CreateRenderLayout();
		layout->SetTopologyType( RenderLayout::TT_TriangleList);

		vector<Vector3> positions; positions.resize( m_scrGridNum*m_scrGridNum);
		vector<WORD>	indices;   indices.reserve( m_scrGridNum*m_scrGridNum * 6);

		// 计算顶点
		float rcpSrcGridSize = 1.f / (float)m_scrGridNum;
		for( int i=0; i<m_scrGridNum; i++)
		{
			for ( int j=0; j<m_scrGridNum; j++)
			{
				UINT  index = i * m_scrGridNum + j;
				positions[index].x = (float)j * rcpSrcGridSize + rcpSrcGridSize;
				positions[index].y = (float)i * rcpSrcGridSize + rcpSrcGridSize;

				// store edges information
				float fx = fabs( positions[index].x * 2.f-1.f);
				float fy = fabs( positions[index].y * 2.f-1.f);

				float fEdgeDisplace = max( fx, fy);
				positions[index].z  =fEdgeDisplace;
			}
		}

		// 计算索引
		for( int i=0; i<m_scrGridNum-1; i++)
		{
			for ( int j=0; j<m_scrGridNum-1; j++)
			{
				WORD index = static_cast<WORD>( i * m_scrGridNum + j);

				indices.push_back( index);
				indices.push_back( index + m_scrGridNum + 1);
				indices.push_back( index + m_scrGridNum);

				indices.push_back( index);
				indices.push_back( index + 1);
				indices.push_back( index + m_scrGridNum + 1);
			}
		}

		// 2.设置顶点
		ElementInitData initVertexData;
		initVertexData.m_rowPitch   = sizeof(Vector3)*positions.size();
		initVertexData.m_slicePitch = 0;
		initVertexData.m_data		= positions.data();
		GraphicBufferPtr vertexPosBuffer = ARenderDevice->CreateVertexBuffer( BU_Dynamic, EAH_GPURead | EAH_Immutable, &initVertexData);
		layout->BindVertexStream( vertexPosBuffer, elementTypePos);

		ElementInitData initIndexData;
		initIndexData.m_rowPitch   = sizeof(WORD)*indices.size();
		initIndexData.m_slicePitch = 0;
		initIndexData.m_data	   = indices.data();
		GraphicBufferPtr indexBuffer = ARenderDevice->CreateIndexBuffer( BU_Dynamic, EAH_GPURead | EAH_Immutable, &initIndexData);
		layout->BindIndexStream( indexBuffer, EF_R16UI);

		m_renderable.SetRenderlayout( layout);
	}

	// 更新
	void Ocean::FrameMove( float fTime)
	{
		RenderEntity::FrameMove( fTime);
	}

	// 添加到场景消息
	void Ocean::OnAddToScene( Scene* scene)
	{
		RenderEntity::OnAddToScene( scene);

		CameraSystem& cameraSystem = GetCameraSystem();

		// 自动绑定参数
		m_renderable.GetMaterial()->SetParamRef( "G_Time",			scene->GetSPABElapsedTime());
		m_renderable.GetMaterial()->SetParamRef( "G_CameraPos",		cameraSystem.GetSPAB( CameraSystem::SP_Pos));
		m_renderable.GetMaterial()->SetParamRef( "G_CameraUp",		cameraSystem.GetSPAB( CameraSystem::SP_Up));
		m_renderable.GetMaterial()->SetParamRef( "G_CameraRight",	cameraSystem.GetSPAB( CameraSystem::SP_Right));
		m_renderable.GetMaterial()->SetParamRef( "G_CameraFront",	cameraSystem.GetSPAB( CameraSystem::SP_Front));
		m_renderable.GetMaterial()->SetParamRef( "G_CameraNearFar", cameraSystem.GetSPAB( CameraSystem::SP_NearFar));
		m_renderable.GetMaterial()->SetParamRef( "G_ViewProj",		cameraSystem.GetSPAB( CameraSystem::SP_ViewProj));
		m_renderable.GetMaterial()->SetParamRef( "G_View",			cameraSystem.GetSPAB( CameraSystem::SP_View));
		m_renderable.GetMaterial()->SetParamRef( "G_TexSceneDepth", GetScene()->GetRenderSystem().GetDeferredRenderingMgr()->GetSPABOpaqueZDistance());
		m_renderable.GetMaterial()->SetParamRef( "G_TexRefraction", GetScene()->GetRenderSystem().GetSPABWaterRefraction());
		m_renderable.GetMaterial()->SetParamRef( "G_SunColor",		GetScene()->GetSPABSunColor());
		m_renderable.GetMaterial()->SetParamRef( "G_SunDir",		GetScene()->GetSPABSunDir());
	}

	// 提交渲染元素
	void Ocean::SubmitRenderElements( RenderElementMgr& renderElementMgr)
	{
		// 提交渲染
		m_renderable.SubmitRenderElements( renderElementMgr);
	}
}