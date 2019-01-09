#pragma once

#include <map>
#include <string>
#include <Core/AresFileUtils.h>
#include "Engine/RenderSystem/AresModel.h"
#include "Engine/RenderSystem/AresModelIDCard.h"
#include "Engine/RenderSystem/M1Animation.h"
#include "Engine/RenderSystem/AresTerrain.h"
#include "Engine/RenderSystem/AresTextureSet.h"
#include "Engine/RenderSystem/AresPointLight.h"
#include "Engine/RenderSystem/AresDirectionalLight.h"
#include <Core/Graphics/AresElementFormat.h>
#include "Engine/ScriptSystem/AresLuaScript.h"

using namespace std;

namespace Ares
{
	// 资源路径列表
	typedef vector<string> ResPaths;

	//-----------------------------------------
	// 资源管理器 2012-05-21     帝林
	//	   只保证相同文件资源唯一，不做多线程
	//	加载处理。
	//-----------------------------------------
	class ResSystem
	{
	public:
		ResSystem( const ResPaths& paths);
		~ResSystem();

		// 添加路径
		void AddPath( const string& path);

		// 移除路径
		void RemovePath( const string& path);

		// 求全路径
		string FullPath( const string& name);

		// 加载实体组
		void LoadEntityGroup( const char* filePath) {}

		// 加载模型文件 *.mod
		MeshPtr  LoadMesh( const char* filePath, int index);

		// 加载渲染方法
		RenderMethodPtr LoadMethod( string fileName, const ShaderMacros& macros=ShaderMacros(), QWORD guid=0);

		// 加载材质
		SuperMaterialPtr LoadMaterialParameter( SuperMaterialPtr& material, const char* filePath, int index, QWORD usage);

		// 加载模型实体文件 *.m1
		ModelPtr LoadModel( const char* filePath);

		// 加载动画文件
		s_ptr<CAresAnimation> LoadAnimation( const char* filePath);

		// 加载脚本
		LuaScriptPtr	LoadLuaScript( const char* filePath);

		// 获取自动绑定shader参数
		void AutoBindParam( RenderMaterialPtr& material, const RenderEntity* renderEntity);

		// 同步加载纹理
		TexturePtr SyncLoadDDS( const char* texPath, UINT accessHint);

		// 新建纹理集
		TextureSet* CreateTextureSet( TexturePtr& texture);

		// 新建光
		LightPtr MakeLight( LightType type);

	public:
		// 文件改变
		void FileChanged( const char* path) {}

	public:
		// 同步保存纹理
		void SyncSaveTexture( const TexturePtr& texture, const char* filePath, ElementFormat destFmt=EF_UnKnown);

	public:
		// 更新
		void Step( float elapsedTime) {}

		// 清理(Garbage clear)
		UINT Clear();

	private:
		map<string, MeshPtr>					m_meshPool;				// Mesh池
		map<string, SuperMaterial::IDCardPtr>	m_materialPool;			// 源材质
		map<string, s_ptr<CAresAnimation> >		m_m1Animations;			// 动画资源
		map<size_t, RenderMethodPtr>			m_methodPool;			// 效果池
		map<string, LuaScriptPtr>				m_luaScriptPool;		// 脚本资源池
		map<string, TexturePtr>					m_texturePool;			// 纹理池
		string									m_exePath;				// 执行文件路径
		//vector<string>							m_pathes;				// 地图列表
        FileUtils*                              m_fileUtils;            // 文件工具
	};
}