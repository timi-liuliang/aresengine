#pragma once

#include <boost/filesystem.hpp>
#include <Physics/AresAStar.h>
#include <Physics/AresStaticNavMesh.h>
#include <Physics/AresStaticMeshTiledBuilder.h>
#include <Physics/PhysicsWorld.h>
#include <Engine/RenderSystem/AresVisualShape.h>
#include <Physics/WaterZoneMgr.h>
#include <Engine/AresRoot.h>
#include <Engine/FxSystem/AresFxDecal.h>

namespace Ares
{
	//---------------------------------------
	// AI 逻辑(当前主为寻路) 2011-01-19 帝林
	//---------------------------------------
	class  CAILogic
	{
	public:
		CAILogic();

		// 打开三角形文件
		void OpenTri( const char* fileName);

		// 打开Nav文件
		void OpenNav( const char* fileName);

		// 构建导航网格
		void BuildNavMesh( CStaticMeshTiledBuilder::SBuildParam& buildParam);

		// 根据区域构建导航网格
		void BuildNavMeshByRegion( CStaticMeshTiledBuilder::SBuildParam& buildParam);

		// 一步构建导航网格(工具用)
		static void BuildNavMesh( const char* triFile, const char* navFile, const char* waterZone, const char* roadZoneFile);

		// 路径查找
		void PathFind( const Vector3& startPos, const Vector3& endPos);

		// 保存导航网格
		void SaveNavMesh( const char* fileName);

		// 鼠标按下 逻辑处理
		void FrameMove();

	public:
		// 绘制PolyMesh(调试用)
		void  DebugDraw();

		// 绘制路径点
		void DebugDrawPathLine( const vector<CStaticNavMesh::SPathNode>& pathData);

		// DebugDrawPolyMesh
		void DebugDrawPolyMesh();

		// 绘制详细mesh
		void DebugDrawPolyMeshDetail();

		// Show
		void NavDebugShow( bool bShowOrig, bool bSHowNavMesh, bool bShowDetailNavMesh, bool bShowPath);

		// 显示多边形及邻接多边形
		void ShowPolyAndNearPoly( const Vector3& pos);

		// 绘制Poly
		void DebugDrawPoly( PolyRef ref, VisualShape* pGeometry);

		// 绘制顶端点
		void DebugDrawApexPoint();

		// 网络填充模式
		void SwitchToWireFrameFillMode();

		// 正常填充模式
		void SwitchToNormalFillMode();

	private:
		// 设置开始点
		void SetStartPos();

		// 设置结束点
		void SetEndPos();

		// 设置PhysicsLogic CollMap
		void SetPhysicsCollmap();

		// 获取鼠标点选射线到三角形文件最近点
		bool GetPickPoint( SegmentDetectResult& detectResult);

	private:
		filesystem::path				m_triPath;				// 路径
		filesystem::path				m_apcsPath;				// CollisionShape Path
		CStaticMeshTiledBuilder::SBuildParam m_buildParam;		// 构建导航网格参数
		CStaticNavMesh*					m_pStaticNavMesh;		// 导入模型
		CStaticMeshTiledBuilder			m_meshBuilder;			// NavMeshBuilder( Debug 用)

		Vector3							m_startPos;				// 寻路起始点
		Vector3							m_endPos;				// 寻路结束点

		VisualShape*					m_triMesh;				// 三角形Mesh
		VisualShape*					m_polyMeshBoundaries;	// 绘制辅助多边形边界
		VisualShape*					m_polyMeshTriangle;		// 绘制辅助多边形三角形
		VisualShape*					m_pPathLine;
		VisualShape*					m_PolyShow[7];			// 显示
		VisualCone3*					m_pStartCone;			// 起始点
		VisualCone3*					m_pEndCone;				// 结束点
		VisualShape*					m_pApexPoints;			// 外围点

		WaterZoneMgr*					m_waterZoneMgr;			// 水区域管理器(无限世界)

		FxDecal*						m_decal;				// 测试用
	};
}