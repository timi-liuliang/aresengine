#include "SCLogic.h"
#include <vector>
#include <Physics/Intersect.h>
#include <Physics/IntrRect3Rect3.h>
#include <Physics/KdtTriangleMesh.h>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <Engine/Foundation/AresSerialization.h>
#include <Engine/FxSystem/AresFxDecal.h>
#include <Engine/PhysicsSystem/AresPhysicsEntity.h>

using namespace std;

namespace Ares
{
	#define VECTOR3_INVALID Vector3( -1e30f, -1e30f, -1e30f)

	// 更换顶点数据
	static void SwitchVertex( int* data, int vertexNum, int idx1, int idx2)
	{
		if( idx1 == idx2)
			return;

		for( int i=0; i<vertexNum; i++)
		{
			int tmp      = data[i*3+idx1];
			data[i*3+idx1] = data[i*3+idx2];
			data[i*3+idx2] = tmp;
		}
	}

	// 更换顶点数据
	static void SwitchVertex( Vector3* data, int vertexNum, int idx1, int idx2)
	{
		if( idx1 == idx2)
			return;

		for( int i=0; i<vertexNum; i++)
		{
			float tmp     = data[i][idx1];
			data[i][idx1] = data[i][idx2];
			data[i][idx2] = tmp;
		}
	}

	// 构造函数
	CAILogic::CAILogic()
	{
		m_decal					= NULL;
		m_triMesh				= SELogic_VisualShape.CreateVisualShape();
		m_polyMeshBoundaries	= SELogic_VisualShape.CreateVisualShape();
		m_polyMeshTriangle		= SELogic_VisualShape.CreateVisualShape();
		m_pPathLine				= SELogic_VisualShape.CreateVisualShape();
		m_pApexPoints			= SELogic_VisualShape.CreateVisualShape();

		m_polyMeshTriangle->SetVisible( false);

		m_pStartCone= SELogic_VisualShape.CreateCone();
		m_pEndCone  = SELogic_VisualShape.CreateCone();

		m_pStartCone->SetRadiusAndHeight( 0.03f, 0.05f);
		m_pStartCone->SetFace( Vector3( 0.f, 0.f, 1.f));
		m_pStartCone->SetColor( ColorRGB( 0.2f, 1.f, 0.2f, 1.f));

		m_pEndCone->SetRadiusAndHeight( 0.03f, 0.05f);
		m_pEndCone->SetFace( Vector3( 0.f, 0.f, 1.f));
		m_pEndCone->SetColor( ColorRGB( 1.f, 0.2f, 0.2f, 1.f));

		for (int i=0; i<7; i++)
		{		
			m_PolyShow[i] = SELogic_VisualShape.CreateVisualShape();
			m_PolyShow[i]->SetColor( ColorRGB::White);
		}

		m_PolyShow[6]->SetColor( ColorRGB::Black);

		// 导航网格生
		m_pStaticNavMesh = new CStaticNavMesh;
		m_waterZoneMgr	 = NULL;

		m_startPos = VECTOR3_INVALID;
		m_endPos   = VECTOR3_INVALID;
	}

	// 打开三角形文件
	void CAILogic::OpenTri( const char* fileName)
	{
		m_triPath  = fileName;
		m_apcsPath = fileName;
		m_apcsPath.replace_extension( ".apcs");

		Logic_Cfg->AddToRecentlyFile( fileName);

		SAFE_DELETE_ARRAY( m_buildParam.m_pVerts);
		SAFE_DELETE_ARRAY( m_buildParam.m_pTris);

		FILE* fileHandle = fopen( fileName, "rb");
		if( fileHandle)
		{
			fread( &m_buildParam.m_iVerts, sizeof( int), 1, fileHandle);
			fread( &m_buildParam.m_iTris,  sizeof( int), 1, fileHandle);

			m_buildParam.m_pVerts = new Vector3[m_buildParam.m_iVerts];
			m_buildParam.m_pTris  = new int[ 3 * m_buildParam.m_iTris];

			fread( m_buildParam.m_pVerts, sizeof( Vector3) * m_buildParam.m_iVerts, 1, fileHandle);
			fread( m_buildParam.m_pTris,  sizeof( int) * 3 * m_buildParam.m_iTris, 1, fileHandle);

			fflush( fileHandle);
			fclose( fileHandle);

			// 交换顶点
			SwitchVertex( m_buildParam.m_pVerts, m_buildParam.m_iVerts, 1, 2);

			// 计算法线
			vector<Vector3>	normals(m_buildParam.m_iVerts);
			TBNComputeNormal( normals.data(), m_buildParam.m_pVerts, m_buildParam.m_iVerts, m_buildParam.m_pTris, m_buildParam.m_iTris);

			int vertexBytes = sizeof( Vector3) * m_buildParam.m_iVerts;
			int normalBytes = sizeof( Vector3) * m_buildParam.m_iVerts;
			int indicesBytes= sizeof( UINT)    * m_buildParam.m_iTris * 3;

			// 显示
			if( m_buildParam.m_iVerts && m_buildParam.m_iTris)
			{
				m_triMesh->Set( RenderLayout::TT_TriangleList, m_buildParam.m_pVerts, vertexBytes,(void*)m_buildParam.m_pTris, indicesBytes, EF_R32UI, normals.data());
				m_triMesh->SetColor( ColorRGB::White);
				m_triMesh->SetVisible( true);
			}

			// 隐藏polyMesh 及 polyMeshDetail
			m_polyMeshBoundaries->SetVisible( false);
			m_polyMeshTriangle->SetVisible( false);

			// 设置碰撞图
			SetPhysicsCollmap();

			// 设置水区域
			string waterZoneFile;
			FileUtils::ReplaceExt( waterZoneFile, fileName, ".wzi");
			if( boost::filesystem::exists( waterZoneFile))
			{
				WaterZoneMgr* waterZone = new_ WaterZoneMgr;
				waterZone->LoadWaterFlags( waterZoneFile.c_str());

				// 区域
				RcConvexVolume volume;
				volume.m_area  = RC_WATER_AREA;
				volume.m_shape = MakeSharePtr( waterZone);

				m_buildParam.m_convexVolumes.push_back( volume);
			}

			// 主路区域
			string roadZoneFile;
			FileUtils::ReplaceExt( roadZoneFile, fileName, ".PathFind_Pos");
			if( boost::filesystem::exists( roadZoneFile))
			{
				vector<Vector3> vertices;

				FILE* fileHandle = fopen( roadZoneFile.c_str(), "rb");
				if( fileHandle)
				{
					int vertexNum;
					fread( &vertexNum, sizeof( int), 1, fileHandle);
					vertices.resize( vertexNum);

					fread( vertices.data(),  sizeof( Vector3)*vertices.size(), 1, fileHandle);

					SwitchVertex( vertices.data(), vertices.size(), 1, 2);

					fflush( fileHandle);
					fclose( fileHandle);
				}

				for( size_t i=0; i<vertices.size()/2; i++)
				{
					Box3* roadZone = new_ Box3;

					Vector3 end0	= vertices[i*2];
					Vector3 end1	= vertices[i*2+1];
					Vector3 forward	= (end1-end0); forward.Normalize();
					float	lenght	= (end1-end0).Length()*0.5f;
					Vector3 right	= Vector3Cross( Vector3::ZAxis, forward);	right.Normalize();
					Vector3 up		= Vector3Cross( forward, right);			up.Normalize();

					roadZone->Set( (end0+end1)*.5f, forward, right, up, lenght, 0.6f, 2.f);

					// 区域
					RcConvexVolume volume;
					volume.m_area  = RC_ROAD_AREA;
					volume.m_shape = MakeSharePtr( roadZone);

					m_buildParam.m_convexVolumes.push_back( volume);
				}
			}

			Rect3 rect;
			for ( int i=0; i<m_buildParam.m_iVerts; i++)
				rect.UnionPoint( m_buildParam.m_pVerts[i]);

#ifdef ARES_EDITOR_MODE
			// 检测 并警告
			if( rect.GetZSize() > 2000.f)
				ALogSystem.Warning( "Triangle File May be too big! min:%.4f,%.4f,%.4f; max:%.4f,%.4f,%.4f;", rect.m_min.x, rect.m_min.y, rect.m_min.z, rect.m_max.x, rect.m_max.y, rect.m_max.z);

			if( m_buildParam.m_iVerts > 3*m_buildParam.m_iTris)
				ALogSystem.Warning( "too many vertices : m_buildParam.m_iVerts > 3*m_buildParam.m_iTris");
#endif
		}
	}

	// 网络填充模式
	void CAILogic::SwitchToWireFrameFillMode()
	{
		if( m_triMesh)
		{
			// 设置光栅化状态
			RasterizerStateDesc desc;
			desc.m_fillMode = FM_WireFrame;
			RasterizerStatePtr rasterizerState = ARenderDevice->CreateRasterizerState( desc);
			m_triMesh->GetMaterial()->SetRasterizerState( rasterizerState);
		}
	}

	// 正常填充模式
	void CAILogic::SwitchToNormalFillMode()
	{
		if( m_triMesh)
			m_triMesh->GetMaterial()->SetRasterizerState( RasterizerStatePtr());
	}

	// 设置PhysicsLogic CollMap
	void CAILogic::SetPhysicsCollmap()
	{/*
		Scene* scene = SCLogic_Map->GetScene();
		if( scene)
		{
			// 测试
			KdtTriangleMesh* kdtMesh = new_ KdtTriangleMesh;
			if( filesystem::exists( m_apcsPath) && filesystem::last_write_time( m_apcsPath) > filesystem::last_write_time( m_triPath))
			{
				Serialization::UnSerializeFromBinaryFile( m_apcsPath.string().c_str(), *kdtMesh);
			}
			else
			{
				kdtMesh->Init( m_buildParam.m_iVerts, m_buildParam.m_pVerts, m_buildParam.m_iTris, m_buildParam.m_pTris);

				Serialization::SerializeToBinaryFile( m_apcsPath.string().c_str(), *kdtMesh);
			}

			PhysicsEntityPtr physicsEntity = MakeSharePtr( new_ PhysicsEntity());
			physicsEntity->SetCollisionShape( kdtMesh);
			scene->AddEntity( "AI Physics", physicsEntity);

			// 测试用贴花
			{	
				m_decal = new_ FxDecal;
				m_decal->GetRenderable().SetTexture("area_1.dds");
				FxEntityPtr fxEntity = MakeSharePtr( new_ FxEntity);
				fxEntity->AddObject( m_decal);
				scene->AddEntity( "text_decal", fxEntity);		
			}
		}*/
	}

	// 打开Nav文件
	void CAILogic::OpenNav( const char* fileName)
	{
		Logic_Cfg->AddToRecentlyFile( fileName);

		SAFE_DELETE( m_pStaticNavMesh);

		m_pStaticNavMesh = new_ CStaticNavMesh;
		if( m_pStaticNavMesh->Load( fileName))
		{
			DebugDrawPolyMesh();
			DebugDrawPolyMeshDetail();
		}
	}

	// 一步构建导航网格
	void CAILogic::BuildNavMesh( const char* triFile, const char* navFile, const char* waterZone, const char* roadZoneFile)
	{
		CStaticMeshTiledBuilder::SBuildParam	tBuildParam;			// 构建导航网格参数
		CStaticNavMesh							tStaticNavMesh;

		FILE* fileHandle = fopen( triFile, "rb");
		if( fileHandle)
		{
			fread( &tBuildParam.m_iVerts, sizeof( int), 1, fileHandle);
			fread( &tBuildParam.m_iTris,  sizeof( int), 1, fileHandle);

			tBuildParam.m_pVerts = new Vector3[tBuildParam.m_iVerts];
			tBuildParam.m_pTris  = new int[ 3 * tBuildParam.m_iTris];

			fread( tBuildParam.m_pVerts, sizeof( float) * 3 * tBuildParam.m_iVerts, 1, fileHandle);
			fread( tBuildParam.m_pTris,  sizeof( int) * 3 * tBuildParam.m_iTris, 1, fileHandle);

			fflush( fileHandle);
			fclose( fileHandle);
		}

		// 顶点转换
		SwitchVertex( tBuildParam.m_pVerts, tBuildParam.m_iVerts, 1, 2);

		Rect3 rect;
		for ( int i=0; i<tBuildParam.m_iVerts; i++)
			rect.UnionPoint( tBuildParam.m_pVerts[i]);

		tBuildParam.m_vMin = Vector3Max( rect.GetMinPoint(), Vector3(    0.f,    0.f, -2000.f));
		tBuildParam.m_vMax = Vector3Min( rect.GetMaxPoint(), Vector3( 4096.f, 4096.f,  2000.f));

		// 水区域
		WaterZoneMgr* waterZoneMgr = new_ WaterZoneMgr;
		if( waterZoneMgr->LoadWaterFlags( waterZone))
		{
			RcConvexVolume volume;
			volume.m_area  = RC_WATER_AREA;
			volume.m_shape = MakeSharePtr(waterZoneMgr);

			tBuildParam.m_convexVolumes.push_back( volume);
		}

		// 主路区域
		if( boost::filesystem::exists( roadZoneFile))
		{
			vector<Vector3> vertices;

			FILE* fileHandle = fopen( roadZoneFile, "rb");
			if( fileHandle)
			{
				int vertexNum;
				fread( &vertexNum, sizeof( int), 1, fileHandle);
				vertices.resize( vertexNum);

				fread( vertices.data(),  sizeof( Vector3)*vertices.size(), 1, fileHandle);

				SwitchVertex( vertices.data(), vertices.size(), 1, 2);

				fflush( fileHandle);
				fclose( fileHandle);
			}

			for( size_t i=0; i<vertices.size()/2; i++)
			{
				Box3* roadZone = new_ Box3;

				Vector3 end0	= vertices[i*2];
				Vector3 end1	= vertices[i*2+1];
				Vector3 forward	= (end1-end0); forward.Normalize();
				float	lenght	= (end1-end0).Length()*0.5f;
				Vector3 right	= Vector3Cross( Vector3::ZAxis, forward);	right.Normalize();
				Vector3 up		= Vector3Cross( forward, right);			up.Normalize();

				roadZone->Set( (end0+end1)*.5f, forward, right, up, lenght, 0.6f, 2.f);

				// 区域
				RcConvexVolume volume;
				volume.m_area  = RC_ROAD_AREA;
				volume.m_shape = MakeSharePtr( roadZone);

				tBuildParam.m_convexVolumes.push_back( volume);
			}
		}

		// 导航网格生
		CStaticMeshTiledBuilder staticMeshTiledBuilder;
		staticMeshTiledBuilder.HandleBuild( tStaticNavMesh, tBuildParam);

		// 保存
		tStaticNavMesh.SetUpDirIdx( 1);
		tStaticNavMesh.Save( navFile);
		tStaticNavMesh.SetUpDirIdx( 2);
	}

	// 构建导航网格
	void  CAILogic::BuildNavMesh( CStaticMeshTiledBuilder::SBuildParam& buildParam)
	{
		// 参数复制
		m_buildParam.m_fCellSize   = buildParam.m_fCellSize;
		m_buildParam.m_fCellHeight = buildParam.m_fCellHeight;

		m_buildParam.m_fAgentHeight   = buildParam.m_fAgentHeight;
		m_buildParam.m_fAgentRadius   = buildParam.m_fAgentRadius;
		m_buildParam.m_fAgentClimb    = buildParam.m_fAgentClimb;
		m_buildParam.m_fAgentMaxSlope = buildParam.m_fAgentMaxSlope;

		m_buildParam.m_fRegionMinSize   = buildParam.m_fRegionMinSize;
		m_buildParam.m_fRegionMergeSize = buildParam.m_fRegionMergeSize;

		m_buildParam.m_fEdgeMaxLen    = buildParam.m_fEdgeMaxLen;
		m_buildParam.m_fEdgeMaxError  = buildParam.m_fEdgeMaxError;
		m_buildParam.m_iVertesPerPoly = buildParam.m_iVertesPerPoly;

		m_buildParam.m_fDetailSampleDist     = buildParam.m_fDetailSampleDist;
		m_buildParam.m_fDetailSampleMaxError = buildParam.m_fDetailSampleMaxError;

		//m_buildParam.m_waterZone			 = m_waterZoneMgr;

		Rect3 rect;
		for ( int i=0; i<m_buildParam.m_iVerts; i++)
			rect.UnionPoint( m_buildParam.m_pVerts[i]);

		m_buildParam.m_vMin = Vector3Max( rect.GetMinPoint(), Vector3(    0.f,    0.f, -2000.f));
		m_buildParam.m_vMax = Vector3Min( rect.GetMaxPoint(), Vector3( 4096.f, 4096.f,  2000.f));
		
#ifdef ARES_EDITOR_MODE
		// 检测 并警告
		if( rect.GetZSize() > 2000.f)
			ALogSystem.Warning( "Triangle File May be too big! min:%.4f,%.4f,%.4f; max:%.4f,%.4f,%.4f;", rect.m_min.x, rect.m_min.y, rect.m_min.z, rect.m_max.x, rect.m_max.y, rect.m_max.z);
#endif

		// 导航网格生
		CStaticMeshTiledBuilder staticMeshTiledBuilder;
		staticMeshTiledBuilder.HandleBuild( *m_pStaticNavMesh, m_buildParam);

		m_polyMeshBoundaries->SetVisible( true);

		// 绘制PolyMesh
		DebugDrawPolyMesh();

		// 显示外围点
		DebugDrawApexPoint();

		DebugDrawPolyMeshDetail();
	}

	// 根据区域更新导航网格
	void CAILogic::BuildNavMeshByRegion(  CStaticMeshTiledBuilder::SBuildParam& buildParam)
	{
		if( m_startPos==VECTOR3_INVALID || m_endPos==VECTOR3_INVALID)
			return;

		// 参数复制
		m_buildParam.m_fCellSize   = buildParam.m_fCellSize;
		m_buildParam.m_fCellHeight = buildParam.m_fCellHeight;

		m_buildParam.m_fAgentHeight   = buildParam.m_fAgentHeight;
		m_buildParam.m_fAgentRadius   = buildParam.m_fAgentRadius;
		m_buildParam.m_fAgentClimb    = buildParam.m_fAgentClimb;
		m_buildParam.m_fAgentMaxSlope = buildParam.m_fAgentMaxSlope;

		m_buildParam.m_fRegionMinSize   = buildParam.m_fRegionMinSize;
		m_buildParam.m_fRegionMergeSize = buildParam.m_fRegionMergeSize;

		m_buildParam.m_fEdgeMaxLen    = buildParam.m_fEdgeMaxLen;
		m_buildParam.m_fEdgeMaxError  = buildParam.m_fEdgeMaxError;
		m_buildParam.m_iVertesPerPoly = buildParam.m_iVertesPerPoly;

		m_buildParam.m_fDetailSampleDist     = buildParam.m_fDetailSampleDist;
		m_buildParam.m_fDetailSampleMaxError = buildParam.m_fDetailSampleMaxError;

		Rect3 rect;
		for ( int i=0; i<m_buildParam.m_iVerts; i++)
			rect.UnionPoint( m_buildParam.m_pVerts[i]);

		Rect3 buildRegion;
		buildRegion.UnionPoint( Vector3( m_startPos.x, m_startPos.y, -1000.f));
		buildRegion.UnionPoint( Vector3( m_endPos.x,   m_endPos.y,    1000.f));

		// 相交区域
		IntrRect3Rect3 intrRR( rect, buildRegion);
		intrRR.Test();
		rect = intrRR.m_intrRect;

		m_buildParam.m_vMin = rect.GetMinPoint();
		m_buildParam.m_vMax = rect.GetMaxPoint();

#ifdef ARES_EDITOR_MODE
		// 检测 并警告
		if( rect.GetZSize() > 2000.f)
			ALogSystem.Warning( "Triangle File May be too big! min:%.4f,%.4f,%.4f; max:%.4f,%.4f,%.4f;", rect.m_min.x, rect.m_min.y, rect.m_min.z, rect.m_max.x, rect.m_max.y, rect.m_max.z);
#endif

		// 导航网格生
		CStaticMeshTiledBuilder staticMeshTiledBuilder;
		staticMeshTiledBuilder.HandleBuild( *m_pStaticNavMesh, m_buildParam);

		m_polyMeshBoundaries->SetVisible( true);

		// 绘制PolyMesh
		DebugDrawPolyMesh();

		// 显示外围点
		DebugDrawApexPoint();

		DebugDrawPolyMeshDetail();
	}

	//! 绘制路径点
	void CAILogic::DebugDrawPathLine( const vector<CStaticNavMesh::SPathNode>& pathData)
	{
		int numPoint = pathData.size();

		vector<Vector3> positions( numPoint);
		for ( int i=0; i<numPoint; i++)
		{
			positions[i] = pathData[i].m_vPos + Vector3( 0.f, 0.f, 0.2f);
		}

		vector<WORD> indices(numPoint*2-2);
		for ( int i=0; i<numPoint-1; i++)
		{
			indices[i*2+0] = i;
			indices[i*2+1] = i+1;
		}

		int vertexBytes = sizeof( Vector3) * positions.size();
		int indicesBytes= NumFormatBytes(EF_R16UI)*indices.size();
		m_pPathLine->Set( RenderLayout::TT_LineList,  positions.data(), vertexBytes, indices.data(), indicesBytes, EF_R16UI);
		m_pPathLine->SetColor( ColorRGB::Black);
	}

	// 绘制PolyMesh(调试用)
	void  CAILogic::DebugDraw()
	{
		// 现在只绘制PolyMesh
		DebugDrawPolyMesh();
		DebugDrawPolyMeshDetail();

		DebugDrawApexPoint();
	}

	// DebugDrawPolyMesh
	void CAILogic::DebugDrawPolyMesh()
	{
		if( !m_pStaticNavMesh || !m_pStaticNavMesh->m_pHeader)
			return;

		CStaticNavMesh::SStaticNavMeshHeader& meshHeader = *m_pStaticNavMesh->m_pHeader;
		Vector3* vertexData = meshHeader.m_pVerts;

		// 索引数据
		UINT32	  indexNum	 = 0;

		// 先计算需要索引数量
		for ( size_t i=0; i<meshHeader.m_iNumPolys; i++)
		{
			CStaticNavMesh::SStaticPoly* poly = &meshHeader.m_pPolys[i];

			indexNum += poly->m_iNumVerts;
		}

		int vertexBytes = meshHeader.m_iNumVerts * sizeof( Vector3);
		int indiceBytes = indexNum * sizeof(UINT32) * 2;

		vector<UINT32> indexData(indexNum*2);

		indexNum = 0;
		for ( size_t i=0; i<meshHeader.m_iNumPolys; i++)
		{
			CStaticNavMesh::SStaticPoly* poly = &meshHeader.m_pPolys[i];
			for ( int j=0; j<poly->m_iNumVerts; j++)
			{
				UINT32 vi[2];

				vi[0] = poly->m_indices[j];
				if( j+1 >= poly->m_iNumVerts)
					vi[1] = poly->m_indices[0];
				else
					vi[1] = poly->m_indices[j+1];

				indexData[indexNum*2]   = vi[0];
				indexData[indexNum*2+1] = vi[1];

				indexNum++;
			}
		}

		m_polyMeshBoundaries->Set( RenderLayout::TT_LineList, vertexData, vertexBytes, indexData.data(), indiceBytes, EF_R32UI);
		m_polyMeshBoundaries->SetColor( ColorRGB( 0, 48, 64, 32));
		m_polyMeshBoundaries->SetVisible( true);

		// 设置渲染状态
		DepthStencilStateDesc dDesc;
		dDesc.m_depthWriteMask = false;
		DepthStencilStatePtr depthStencilState = ARenderDevice->CreateDepthStencilState( dDesc);
		m_polyMeshBoundaries->GetMaterial()->SetDepthStencilState( depthStencilState);

		BlendStateDesc bDesc;
		bDesc.m_blendEnable.assign( true);
		bDesc.m_blendOp.assign( BOP_Add);
		bDesc.m_srcBlend.assign( ABF_SrcAlpha);
		bDesc.m_destBlend.assign( ABF_InvSrcAlpha);
		bDesc.m_blendOpAlpha.assign( BOP_Add);
		bDesc.m_srcBlendAlpha.assign( ABF_One);
		bDesc.m_destBlendAlpha.assign( ABF_Zero);
		bDesc.m_colorWriteMask.assign( ColorRGB::MASK_All);
		BlendStatePtr blendState = ARenderDevice->CreateBlendState( bDesc);
		m_polyMeshBoundaries->GetMaterial()->SetBlendState( blendState, ColorRGB(0.f,0.f,0.f), 0xffffffff);
	}

	// 绘制详细mesh
	void CAILogic::DebugDrawPolyMeshDetail()
	{
		if( !m_pStaticNavMesh || !m_pStaticNavMesh->m_pHeader)
			return;

		CStaticNavMesh::SStaticNavMeshHeader& meshHeader = *m_pStaticNavMesh->m_pHeader;

		// 顶点数量
		vector<Vector3> vertices;
		vector<DWORD>	colors;
		for ( size_t i=0; i<meshHeader.m_iNumPolys; i++)
		{
			BYTE									 areaID    = m_pStaticNavMesh->GetPolyAreaIDByRef( i);
			const CStaticNavMesh::SStaticPoly*		 poly      = m_pStaticNavMesh->GetPolyByRef( i);
			const CStaticNavMesh::SStaticPolyDetail* polyDetail= m_pStaticNavMesh->GetPolyDetailByRef( i);

			for ( int i=0; i<polyDetail->m_iNumTris; i++)
			{
				const unsigned char* t = m_pStaticNavMesh->GetDetailTri( polyDetail->m_iTriBase + i);

				// 三角形的三个顶点
				Vector3 v[3];
				for ( int j=0; j<3; j++)
				{
					if( t[j] < poly->m_iNumVerts)
						v[j] = m_pStaticNavMesh->GetVertex( poly->m_indices[t[j]]);
					else
						v[j] = m_pStaticNavMesh->GetDetailVertex( polyDetail->m_iVertBase + (t[j] - poly->m_iNumVerts));
				}

				vertices.push_back( v[0]/* + Vector3( 0.f, 0.f, 0.5f)*/);
				vertices.push_back( v[1]/* + Vector3( 0.f, 0.f, 0.5f)*/);
				vertices.push_back( v[2]/* + Vector3( 0.f, 0.f, 0.5f)*/);

				DWORD color;
				switch( areaID)
				{
				case RC_WALK_AREA:	color = ColorRGB( 0,192,255,64).ToDWORD();		break;
				case RC_ROAD_AREA:	color = ColorRGB( 255, 0, 0, 200).ToDWORD();	break;
				case RC_WATER_AREA: color = ColorRGB( 51,176, 45, 64).ToDWORD();	break;
				default:			color = ColorRGB( 255, 0, 0,64).ToDWORD();		break;
				}

				colors.push_back( color);
				colors.push_back( color);
				colors.push_back( color);

				Triangle3 testTri( v[0], v[1], v[2]);
				if( (v[0]-v[1]).Length()>100.f || (v[0]-v[2]).Length()>100.f || (v[1]-v[2]).Length()>100.f)
					printf( "\nBugTriangle:\n\t Vertex0 %10.2f,%10.2f,%10.2f;\n\t Vertex1 %10.2f,%10.2f,%10.2f;\n\t Vertex1 %10.2f,%10.2f,%10.2f;", v[0].x, v[0].y, v[0].z, v[1].x, v[1].y, v[1].z, v[2].x, v[2].y, v[2].z);
			} 
		}

		m_polyMeshTriangle->Set( RenderLayout::TT_TriangleList, vertices.data(), sizeof(Vector3)*vertices.size(), NULL, 0, EF_R32UI, NULL, colors.data());
		m_polyMeshTriangle->SetColor( ColorRGB::White);
		m_polyMeshTriangle->SetVisible( true);

		// 设置渲染状态
		DepthStencilStateDesc dDesc;
		dDesc.m_depthWriteMask = false;
		DepthStencilStatePtr depthStencilState = ARenderDevice->CreateDepthStencilState( dDesc);
		m_polyMeshTriangle->GetMaterial()->SetDepthStencilState( depthStencilState);

		BlendStateDesc bDesc;
		bDesc.m_blendEnable.assign( true);
		bDesc.m_blendOp.assign( BOP_Add);
		bDesc.m_srcBlend.assign( ABF_SrcAlpha);
		bDesc.m_destBlend.assign( ABF_InvSrcAlpha);
		bDesc.m_blendOpAlpha.assign( BOP_Add);
		bDesc.m_srcBlendAlpha.assign( ABF_One);
		bDesc.m_destBlendAlpha.assign( ABF_Zero);
		bDesc.m_colorWriteMask.assign( ColorRGB::MASK_All);
		BlendStatePtr blendState = ARenderDevice->CreateBlendState( bDesc);
		m_polyMeshTriangle->GetMaterial()->SetBlendState( blendState, ColorRGB(0.f,0.f,0.f), 0xffffffff);
	}

	// Show
	void CAILogic::NavDebugShow( bool bShowOrig, bool bSHowNavMesh,  bool bShowDetailNavMesh, bool bShowPath)
	{
		m_triMesh->SetVisible( bShowOrig);				// 三角形Mesh
		m_polyMeshBoundaries->SetVisible( bSHowNavMesh);			// 绘制辅助
		m_polyMeshTriangle->SetVisible( bShowDetailNavMesh);
		m_pPathLine->SetVisible( bShowPath);
	}

	// 保存导航网格
	void CAILogic::SaveNavMesh( const char* fileName)
	{
		m_pStaticNavMesh->SetUpDirIdx( 1);

		m_pStaticNavMesh->Save( fileName);

		m_pStaticNavMesh->SetUpDirIdx( 2);
	}

	// 路径查找
	void CAILogic::PathFind( const Vector3& startPos, const Vector3& endPos)
	{
		NavMeshFilter filter;
		//filter.SetAreaPathable( RC_ROAD_AREA, false);
		filter.SetCost( RC_WALK_AREA, 10.f);
		filter.SetCost( RC_ROAD_AREA, 1.f);

		vector<CStaticNavMesh::SPathNode> Path;
		Vector3 extend( 5.f);
		if( int numPoly = m_pStaticNavMesh->FindPath( Path, filter, startPos, endPos,extend, extend))
		{
			// 显示路径点
			DebugDrawPathLine( Path);
		}
	}

	// 鼠标按下 逻辑处理
	void CAILogic::FrameMove()
	{
		// 设置寻路开始点, 结束点
		if( SCLogic_Input->IsMouseDown(OIS::MB_Left))
		{
			if( SCLogic_Input->IsKeyDown( OIS::KC_LCONTROL))
				SetEndPos();
			else
				SetStartPos();
		}
	}

	// 获取鼠标点选射线到三角形文件最近点
	bool CAILogic::GetPickPoint( SegmentDetectResult& detectResult)
	{
		Vector3 rayOrig, rayDir;
		Logic_Physics->GetPickRay( rayOrig, rayDir);

		Segment3 segment( rayOrig, rayOrig + rayDir * 100.f);

		if( SCLogic_Scene->GetPhysicsWorld()->SegmentDetect( segment, SegmentDetectConfig(), detectResult))
			return true;

		return false;
	}

	// 设置开始点
	void CAILogic::SetStartPos()
	{
		SegmentDetectResult detectResult;
		if( GetPickPoint( detectResult))
		{
			m_startPos = detectResult.m_closetHitPoint;

			if( m_endPos != VECTOR3_INVALID)
				PathFind( m_startPos, m_endPos);

			m_pStartCone->GetTransform()->SetTrans( m_startPos.x, m_startPos.y, m_startPos.z);

			m_decal->Set( m_startPos, detectResult.m_closetHitNormal, Vector3::YAxis, 5.f, 5.f, 2.5f, Vector3::ZAxis);

			//// 界面更新
			////CGUI::g_pAIUI->SetStartPos( m_startPos);

			// 显示多边形及领接多边形
			//ShowPolyAndNearPoly( m_startPos);
		}
	}

	// 设置结束点
	void CAILogic::SetEndPos()
	{
		SegmentDetectResult detectResult;
		if( GetPickPoint( detectResult))
		{
			m_endPos = detectResult.m_closetHitPoint;

			if( m_startPos != VECTOR3_INVALID)
				PathFind( m_startPos, m_endPos);

			m_pEndCone->GetTransform()->SetTrans( m_endPos.x, m_endPos.y, m_endPos.z);

			// 界面更新
			//CGUI::g_pAIUI->SetEndPos( m_endPos);

			// 显示多边形及邻接多边形
			//ShowPolyAndNearPoly( m_endPos);
		}	
	}

	// 显示多边形及邻接多边形
	void CAILogic::ShowPolyAndNearPoly( const Vector3& pos)
	{
		for (int i=0; i<7; i++)
		{
			m_PolyShow[i]->SetVisible( false);
		}

		Vector3 extendBox( 2.f, 2.f, 10.f);

		PolyRef nearestPoly;
		if( m_pStaticNavMesh->FindNearstPoly( nearestPoly, pos, extendBox))
		{
			//CGUI::g_pAIUI->SetPolyRef( nearestPoly, 0);

			DebugDrawPoly( nearestPoly, m_PolyShow[6]);

			int child[6];
			int numChild;
			m_pStaticNavMesh->GetPassAbleNeighbor( NavMeshFilter(), child, numChild, nearestPoly);

			for ( int i=0; i<numChild; i++)
			{
				DebugDrawPoly( child[i], m_PolyShow[i]);
			}
		}
	}

	// 绘制Poly
	void CAILogic::DebugDrawPoly( PolyRef ref, VisualShape* pGeometry)
	{
		pGeometry->SetVisible( true);

		const CStaticNavMesh::SStaticPoly* poly = m_pStaticNavMesh->GetPolyByRef( ref);

		if( !poly)
			return; 

		int numPoint = poly->m_iNumVerts;
		vector<Vector3> vertexData( numPoint);

		for ( int i=0; i<numPoint; i++)
			vertexData[i] = m_pStaticNavMesh->m_pHeader->m_pVerts[poly->m_indices[i]] + Vector3::ZAxis*0.1f;

		vector<WORD> indexData(numPoint*2);
		for ( int i=0, j=numPoint-1; i<numPoint; j = i++)
		{
			indexData[i*2+0] = j;
			indexData[i*2+1] = i;
		}

		pGeometry->Set( RenderLayout::TT_LineList, vertexData.data(), sizeof(Vector3)*vertexData.size(), indexData.data(), sizeof(WORD)*indexData.size(), EF_R16UI);
	}

	// 绘制顶端点
	void CAILogic::DebugDrawApexPoint()
	{/*
		if( !m_pStaticNavMesh || !m_pStaticNavMesh->m_pHeader)
			return;

		CStaticNavMesh::SStaticNavMeshHeader& meshHeader = *m_pStaticNavMesh->m_pHeader;
		Vector3* vertexData = meshHeader.m_pVerts;

		vector<Vector3> apexVertes; apexVertes.reserve( 10000);

		// 先计算需要索引数量
		for ( int i=0; i<meshHeader.m_iNumVerts; i++)
		{
			if( meshHeader.m_pVertFlags[i] & CStaticNavMesh::VF_APEX)
			{
				apexVertes.push_back( meshHeader.m_pVerts[i]);
				apexVertes.push_back( meshHeader.m_pVerts[i]+Vector3::ZAxis);
			}
		}

		int vertexBytes = apexVertes.size() * sizeof(Vector3);
		m_pApexPoints->Set( RenderLayout::TT_LineList, apexVertes.data(), vertexBytes, NULL, 0);
		m_pApexPoints->SetColor( ColorRGB( 0.f, 1.f, 0.f));*/
	}
}