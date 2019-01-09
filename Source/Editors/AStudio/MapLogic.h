#pragma once

#include <Engine/AresRoot.h>
#include <Engine/SceneSystem/AresScene.h>
#include <Engine/RenderSystem/AresVisualShape.h>
#include <EditorKit/AresWorldEK.h>
#include <Physics/MassSpringSystem.h>
#include <Engine/RenderSystem/AresClothFabricBuilder.h>
#include <Physics/Bullet/btBulletDynamicsCommon.h>
#include <EditorKit/BulletDebugDrawer.h>

namespace Ares
{
	//-------------------------------------
	// 世界地图逻辑  2011-08-16  帝林
	//-------------------------------------
	class CMapLogic
	{
	public:
		// 地图更新
		void FrameMove( float fTime);

		// 鼠标检选
		Entity* SelectEntity();

	public:
		// 构造函数
		CMapLogic();
		~CMapLogic();

		// 恢复上次关闭时状态
		void Restore();

		// 创建地图
		void CreateScene( const char* name, const char* location);

		// 打开地图
		void OpenScene( const char* filename);

		// 保存地图
		void SaveMap( const char* filename);

		// 另存为
		void SaveMapAs();

	private:
		// 新建区域
		void SetSceneSize( size_t posX, size_t posY, int width, int height);

	public:
		// 设置Window(中心点x,y; 半径radius)
		void SetNavWindow( float x, float y, float radius);

		// 辅助网格
		void CreateWhiteGrid();

		// 创建红色中线
		void CreateRedCross();

		// 区域地形
	public:
		// 设置地形UV缩放
		void SetUVScale(int index, float value);

		// 获取UV坐标值
		float GetUVScale(int index);

		// 设置地形用纹理
		void SetTerrainTexture( int index, const char* resPath);

		// 设置地形用纹理
		const char* GetTerrainTexture( int index);

		// 向当前地形导入高度图
		void ImportTerrainHeight( const char* heightPath);

		// 向当前地形导入纹理混合度
		void ImportTerrainTexBlend( int idx, const char* texPath);

		// 编辑地形或纹理
		void EditTerrainMeshOrTexture( float fTime);

		// 获取地形编辑数据(虽然不喜欢这种方式, 编辑器无所谓了)
		STerrainMeshEditorData& GetTerrainMeshEditorData() { return m_meshEditorData; }

		// 测试用
	public:
		// 初始化物理
		void InitPhysics();

	public:
		// Recent files changed signals
		signal<void()> Signal_OnSceneChanged;

	private:
		STerrainMeshEditorData	m_meshEditorData;	// 地形编辑数
		VisualShape*			m_whiteGrid;		// 辅助网格
		VisualShape*			m_redCross;			// 中心红十字

		MassSpringSystem*		m_massSpringSystem;	// 质点弹簧系统
		VisualShape*			m_cloth;			// 布料渲染
		VisualShape*			m_springRender;		// 弹簧调试

		ClothFabricPtr			m_clothFabric;


		PhysicsWorldPtr			m_physicsWorld;		// 物理世界

		btBroadphaseInterface*					m_broadphase;				// 粗阶段
		btCollisionDispatcher*					m_dispatcher;				// 碰撞
		btConstraintSolver*						m_solver;					// 解约束
		btDefaultCollisionConfiguration*		m_collisionConfiguration;	// 碰撞配置
		btDynamicsWorld*						m_dynamicsWorld;			// 动态世界
		btAlignedObjectArray<btCollisionShape*>	m_collisionShapes;			// 碰撞体
		BulletDebugDrawer*						m_bulletDebugDrawer;		// 渲染
	};
}