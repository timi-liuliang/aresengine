#pragma once

#include <map>
#include <set>
#include <Engine/RenderSystem/AresRenderEntity.h>
#include <Engine/RenderSystem/AresLight.h>
#include <Engine/RenderSystem/AresDirectionalLight.h>
#include <Engine/RenderSystem/AresModel.h>
#include <Engine/SceneSystem/AresScene.h>
#include <Engine/RenderSystem/AresHDREncode.h>
#include <Engine/RenderSystem/AresGrayscale.h>

using namespace std;

namespace Ares
{
	//-------------------------------------
	// 光照图硬件烘焙管理器 2012-6-20 帝林
	//-------------------------------------
	class GpuLightmapBakerMgr
	{
		typedef set<RenderEntity*>			  TaskSet;
		typedef multimap<Entity*, Renderable> MultimapRB;
	public:
		// 构造函数
		GpuLightmapBakerMgr();

		// 添加任务
		void AddTask( RenderEntity* entity);

	public:
		// 渲染Lightmap
		void Render();

		// 构造函数
		void SetLightmapSize( int size);

		// 烘焙RenderEntity
		void Baking( RenderEntity* renderEntity, Scene* scene);

		// 烘焙方向光
		void DirectionalBaking( RenderEntity* renderEntity, DirectionalLight* dirLight, Scene* scene);

		// 更新方向光深度图(只有静态投射者)
		void UpdateDirLightDepthMapForBake( DirectionalLight* dirLight, Scene* scene);

		// 执行烘焙Lightmap
		void BakeLightmap(  DirectionalLight* dirLight, RenderEntity* renderEntity);

	private:
		// 添加光照图到场景中
		void AddLightmapToScene( const char* name);

		// 更换材质
		void ModifyMaterial( RenderEntity* renderEntity);

		// 构建ZPre Renderable
		bool BuildRenderableZPre( TerrainTile* terrainTile);

		// 烘焙结束后修改地形块所用材质
		bool ModifyMaterialUseLightmap( TerrainTile* terrainTile);

		// 构建ZPre Renderable for model
		bool BuildRenderableZPre( Model* model);

		// 烘焙结束后修改模型所用材质
		void ModifyMaterialUseLightmap( Model* model);

		// 搜寻渲染深度Renderable
		void SearchRenderableZPre( vector<pair< Entity*, Renderable*>>& results, Scene* scene, const Box3& r3);

		// 构建Bake Renderable
		bool BuildRenderableBake( TerrainTile* terrainTile);

		// 构建Bake Renderable
		bool BuildRenderableBake( Model* model);

		// 搜寻光照烘焙Renderable
		void SearchRenderableBake(  vector<pair< Entity*, Renderable*>>& results, RenderEntity* renderEntity);

		// 计算光照图大小
		int GetLightmapSize( RenderEntity* renderEntity, int meshIdx=0);

	private:
		MultimapRB		m_zPreRenderables;		// Z深度辅助(编辑器使用)
		MultimapRB		m_bakeRenaderables;		// 执行烘焙

		FrameBufferPtr	m_lightmapFrame;		// 光照图FrameBuffer
		TexturePtr		m_lightmapTexture;		// 辅助纹理

		ShaderParamPtr	m_lightWorldViewPorj;	// 光源空间世界观察投影矩阵

		TaskSet			m_tasks;				// 烘焙任务集

		int				m_lightmapResolution;	// 光照图分辨率
	};
}