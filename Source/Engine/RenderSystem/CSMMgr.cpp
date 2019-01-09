#include <Engine/RenderSystem/CSMMgr.h>
#include <Engine/AresRoot.h>
#include <Physics/IntrRect3Rect3.h>

#define LIGHT_DISTANCE  50.f		// 关系到Shadowmap精度

namespace Ares
{
	// 构造函数
	CSMMgr::CSMMgr()
		: m_resolution( 0)
		, m_numSplits( 0)
		, m_splitSchemeWeight( 0.8f)
		, m_scene( NULL)
	{
		m_spab[SP_SplitPos]	     = MakeShaderParam();
		m_spab[SP_ViewCropRemap] = MakeShaderParam( Matrix44::Identity);
		m_spab[SP_DepthMap]		 = MakeShaderParam();
		m_spab[SP_NumSplits]	 = MakeShaderParam();
	}

	// 析构函数
	CSMMgr::~CSMMgr()
	{

	}

	// 设置关联场景
	void CSMMgr::SetScene( Scene* scene)
	{
		m_scene = scene;
	}

	// 设置太阳光朝向
	void CSMMgr::SetLightDirection( const Vector3& dir)
	{
		m_dir = dir;
		m_dir.Normalize();
	}

	// 设置深度纹理分辨率
	void CSMMgr::SetShadowMapResolution( size_t resolution)
	{
		resolution = std::max<size_t>( resolution, 2);
		if( resolution && m_resolution!=resolution)
		{
			// 记录分辨率
			m_resolution = resolution;

			// 计算偏移映射矩阵
			float offset = 0.5f / m_resolution;
			m_remap = Matrix44( 0.5f, 0.f, 0.f, 0.f, 0.f, -0.5f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.5f+offset, 0.5f+offset, 0.f, 1.f);

			// 分配资源
			ReAllocateRes();
		}
	}

	// 重新分配资源
	void CSMMgr::ReAllocateRes()
	{
		m_views.resize( m_numSplits);
		m_crops.resize( m_numSplits);
		m_depthBuffers.resize( m_numSplits);
		m_searchBox3s.resize( m_numSplits);
	
		if( m_numSplits)
		{
			TexturePtr depthStencil = ARenderDevice->CreateTexture2D( m_resolution, m_resolution, 1, 1, EF_D32F, 1, 0, EAH_GPURead | EAH_GPUWrite, NULL);
			m_depthTexture			= ARenderDevice->CreateTexture2D( m_resolution, m_resolution, 1, m_numSplits, EF_R32F, 1, 0, EAH_GPURead | EAH_GPUWrite, NULL);
			for( size_t i=0; i<m_numSplits; i++)
			{
				FrameBufferPtr depthBuffer	= ARenderDevice->CreateFrameBuffer();
				depthBuffer->AttachRenderView( FrameBuffer::ATT_Color0,		ARenderDevice->Create2DRenderView( m_depthTexture, i, 1, 0));
				depthBuffer->AttachRenderView( FrameBuffer::ATT_DepthStencil, ARenderDevice->Create2DDepthStencilRenderView( depthStencil, 0, 1, 0));

				m_depthBuffers[i]  = depthBuffer;
			}
		}
	}

	// 设置阴影接收者投身者区域
	void CSMMgr::SetFocusRegion( const Rect3& rect)
	{
		if( m_numSplits != 1)
		{
			m_numSplits = 1;
			ReAllocateRes();

			// make sure border values are accurate
			m_splitPos.resize( 2);
			m_splitPos[0].x			  = 0.f;
			m_splitPos[m_numSplits].x = 1000.f;
		}

		vector<Vector3> points; points.resize( 8);
		rect.GetEightPoints( points);

		SetFocusRegion( points, 0);
	}

	// 设置阴影接收者区域(点集)
	void CSMMgr::SetFocusRegion( const vector<Vector3>& points, size_t index)
	{
		Rect3 boundingBox;
		for( size_t i=0; i<points.size(); i++)
			boundingBox.UnionPoint( points[i]);

		Vector3 pos = ( boundingBox.GetMinPoint() + boundingBox.GetMaxPoint()) / 2.f - m_dir * LIGHT_DISTANCE;

		// 更新观察矩阵
		Matrix44ViewDirLH( m_views[index], pos, m_dir, Vector3::ZAxis);

		// 计算CropMatrix
		CalculateCropMatrix( m_crops[index], points, m_views[index]);

		// 更新搜寻轴向包围盒
		CalculateSearchBox3( m_searchBox3s[index], points, pos);
	}

	// 更新CropMatrix
	void CSMMgr::CalculateCropMatrix( Matrix44& crop, const vector<Vector3>& points, const Matrix44& view)
	{
		Rect3 cropAABB;
		for ( size_t i=0; i<points.size(); i++)
			cropAABB.UnionPoint( view.TransformVector3( points[i]));

		// 使用光源的近平面值
		cropAABB.m_min.z = 0.f;

		// 创建矩阵
		float  scaleX,  scaleY,  scaleZ;
		float offsetX, offsetY, offsetZ;

		scaleX  = 2.f / cropAABB.GetXSize();
		scaleY  = 2.f / cropAABB.GetYSize();
		offsetX = -0.5f * ( cropAABB.m_max.x + cropAABB.m_min.x) * scaleX;
		offsetY = -0.5f * ( cropAABB.m_max.y + cropAABB.m_min.y) * scaleY;

		scaleZ  = 1.f / cropAABB.GetZSize();
		offsetZ = -cropAABB.m_min.z * scaleZ;

		crop = Matrix44( scaleX, 0.f, 0.f, 0.f, 0.f, scaleY, 0.f, 0.f, 0.f,	0.f, scaleZ, 0.f, offsetX, offsetY, offsetZ, 1.f);
	}

	// 更新搜寻轴向包围盒
	void CSMMgr::CalculateSearchBox3( Box3& searchBox, const vector<Vector3>& points, const Vector3& pos)
	{
		// 1.计算轴向
		Vector3 zAxis = m_dir;									zAxis.Normalize();		
		Vector3 xAxis = Vector3Cross( Vector3::ZAxis, zAxis);	xAxis.Normalize();
		Vector3 yAxis = Vector3Cross( zAxis, xAxis);			yAxis.Normalize();

		float   minExtent0 =  FLT_BIG;
		float	maxExtent0 = -FLT_BIG;
		float   minExtent1 =  FLT_BIG;
		float	maxExtent1 = -FLT_BIG;
		float   minExtent2 =  FLT_BIG;
		float	maxExtent2 = -FLT_BIG;
		for ( size_t i=0; i<points.size(); i++)
		{
			minExtent0 = std::min<float>( minExtent0, Vector3Dot( xAxis, points[i]));
			maxExtent0 = std::max<float>( maxExtent0, Vector3Dot( xAxis, points[i]));
			minExtent1 = std::min<float>( minExtent1, Vector3Dot( yAxis, points[i]));
			maxExtent1 = std::max<float>( maxExtent1, Vector3Dot( yAxis, points[i]));
			minExtent2 = std::min<float>( minExtent2, Vector3Dot( zAxis, points[i]));
			maxExtent2 = std::max<float>( maxExtent2, Vector3Dot( zAxis, points[i]));
		}

		minExtent2 = std::min<float>( minExtent2, pos.z / zAxis.z);
		maxExtent2 = std::max<float>( maxExtent2, pos.z / zAxis.z);

		// 沿xy方向扩展0.5单位
		float	extent0 = (maxExtent0 - minExtent0) * 0.5f + 0.5f;
		float	extent1 = (maxExtent1 - minExtent1) * 0.5f + 0.5f; 
		float	extent2 = (maxExtent2 - minExtent2) * 0.5f;
		Vector3 center	= ((maxExtent0+minExtent0) * xAxis + (maxExtent1+minExtent1) * yAxis + (maxExtent2+minExtent2) * zAxis) / 2.f;

		searchBox = Box3( center, xAxis, yAxis, zAxis, extent0, extent1, extent2);
	}

	// 更新SPAB
	void CSMMgr::UpdateSPAB()
	{
		if( m_numSplits)
		{
			vector<Matrix44>   viewCropRemaps; viewCropRemaps.resize( m_numSplits);
			for( size_t i=0; i<m_numSplits; i++)
				viewCropRemaps[i] = m_views[i] * m_crops[i] * m_remap;

			*m_spab[SP_SplitPos]      = m_splitPos;
			*m_spab[SP_ViewCropRemap] = viewCropRemaps;
			*m_spab[SP_DepthMap]	  = m_depthTexture;
			*m_spab[SP_NumSplits]	  = (float)m_numSplits;
		}
	}

	// 开始阴影渲染图阶段
	void CSMMgr::RefreshShadowMaps( const Rect3& rect)
	{
		SetFocusRegion( rect);

		// 设置DepthMap为当前FrameBuffer
		ARenderDevice->BindFrameBuffer( m_depthBuffers[0]);
		m_depthBuffers[0]->Clear( ColorRGB( 1.f, 1.f, 1.f, 1.f), 1.f, 0);

		if( m_scene)
			m_scene->RenderDepth( m_searchBox3s[0], m_views[0]*m_crops[0]);
		else
			ALogSystem.Warning( "Cascadedshadowmaps have no depth write function");

		UpdateSPAB();
	}

	// 设置阴影接收者投身者区域(用于动态平行分隔阴影)
	void CSMMgr::SetFocusRegion( const Frustum3& frustum, size_t numSplits)
	{
		Rect3 entityWorldBounds;
		if( m_scene)
		{
			vector<Entity*> entitys;
			m_scene->BuildSearchResults( entitys, &frustum);
			foreach_( Entity* entity, entitys)
			{
				if( entity->GetType() == ET_Render)
				{
					Rect3 eWB = entity->GetWorldBounds();
					entityWorldBounds.UnionPoint( eWB.GetMaxPoint());
					entityWorldBounds.UnionPoint( eWB.GetMinPoint());
				}
			}
		}

		if( m_numSplits != numSplits)
		{
			m_numSplits = numSplits;
			ReAllocateRes();

			// 计算拆分位置
			// Practical split scheme
			// CLi = n * ( f/n) ^ ( i/numSplits)
			// CUi = n + ( f - n) * ( i/numSplits)
			// Ci  = CLi * ( lambda) + CUi * ( 1-lambda)
			// 
			// lambda scales between logarithmic and uniform
			m_splitPos.resize( m_numSplits+1);
			for ( size_t i=0; i<m_numSplits; i++)
			{
				float fIDM = (float)i / (float)m_numSplits;
				float fLog = frustum.GetNear() * powf( frustum.GetFar() / frustum.GetNear(), fIDM);
				float fUnitform = frustum.GetNear() + ( frustum.GetFar() - frustum.GetNear()) * fIDM;
				m_splitPos[i].x = fLog * m_splitSchemeWeight + fUnitform * ( 1.f - m_splitSchemeWeight);
			}

			// make sure border values are accurate
			m_splitPos[0].x			  = frustum.GetNear();
			m_splitPos[m_numSplits].x = frustum.GetFar();
		}

		// 分隔平截头体
		for( size_t i=0; i<m_numSplits; i++)
		{
			Frustum3 splitFrustum = frustum;
			splitFrustum.SetNear( m_splitPos[i].x);
			splitFrustum.SetFar( m_splitPos[i+1].x);

			// 计算层级区域
			Rect3 frustumRegion;
			const Vector3* vertices = splitFrustum.GetVertexs();
			for( size_t j=0; j<8; j++)
				frustumRegion.UnionPoint( vertices[j]);

			// 求交
			IntrRect3Rect3 intrRR( frustumRegion, entityWorldBounds);
			intrRR.Test();

			vector<Vector3> points; points.resize( 8);
			intrRR.m_intrRect.GetEightPoints( points);
			SetFocusRegion( points, i);
		}
	}

	// 开始阴影渲染图阶段
	void CSMMgr::RefreshShadowMaps( Frustum3& frustum, size_t numSplits)
	{
		numSplits =std::min<size_t>( numSplits, 8);

		SetFocusRegion( frustum, numSplits);

		for( size_t i=0; i<m_numSplits; i++)
		{
			// 设置DepthMap为当前FrameBuffer
			ARenderDevice->BindFrameBuffer( m_depthBuffers[i]);
			m_depthBuffers[i]->Clear( ColorRGB( 1.f, 1.f, 1.f, 1.f), 1.f, 0);

			if( m_scene)
				m_scene->RenderDepth( m_searchBox3s[i], m_views[i]*m_crops[i]);
			else
				ALogSystem.Warning( "Cascadedshadowmaps have no depth write function");
		}

		UpdateSPAB();
	}
}