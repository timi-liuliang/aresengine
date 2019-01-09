#pragma once

#include <map>
#include <list>
#include "Engine/RenderSystem/M1Animation.h"
#include "Engine/RenderSystem/AresAnimationController.h"
#include "Engine/RenderSystem/AresRenderEntity.h"
#include "Engine/RenderSystem/SuperMaterial.h"
#include "Engine/RenderSystem/AresMesh.h"

using namespace std;

#define BONE_MAX 64

namespace Ares
{
	//-------------------------------------
	// MeshRenderable 2013-4-27 帝林
	//-------------------------------------
	class MeshRenderable : public Renderable
	{
	public:
		// 构造函数
		MeshRenderable();

		// 获取几何体数量
		int GetPrimitiveCount() const;

	public:
		string				m_materialPath;		// 材质路径	
		int					m_materialIdx;		// 材质索引
		string				m_meshFilePath;		// 网格路径
		int					m_meshIdx;			// 网格索引
		MeshPtr				m_mesh;				// 渲染网格
		SuperMaterialPtr	m_superMaterial;	// 当前材质
	};

	//---------------------------------------
	// M1Entity  2010-09-25   帝林
	//---------------------------------------
	class Model : public RenderEntity
	{
	public:
		//------------------------------
		// IDCardModel 2013-4-26 帝林
		//------------------------------
		struct IDCard
		{
		public:
			// 渲染元素
			struct RenderInfo
			{
				string	m_materialPath;			// 材质路径	
				int		m_materialIdx;			// 材质索引
				string	m_meshFilePath;			// 网格路径
				int		m_meshIdx;				// 网格索引

				// 序列化
				template <typename Archive> void serialize( Archive& ar, const unsigned int version)
				{
					ar & m_materialPath & m_materialIdx & m_meshFilePath & m_meshIdx;
				}
			};

			// 重建模型数据
			void RebuildModel( Model& model);

			// 序列化
			template <typename Archive> void serialize( Archive& ar, const unsigned int version)
			{
				ar & m_aniPath;
				ar & m_renderInfos;
			}

		public:
			string				m_aniPath;		// 动画路径(唯一)
			vector<RenderInfo>	m_renderInfos;	// 渲染信息
		};
		typedef s_ptr<IDCard> IDCardPtr;
		friend struct IDCard;

	public:
		Model();
		~Model();

		// 设置资源路径
		void SetFilePath( const char* filePath) { m_filePath = filePath; }

		// 获取资源路径
		const char* GetFilePath() const { return m_filePath.c_str(); }

		// 获取M1Renderable数量
		size_t GetMeshRenderableNum() { return m_renderables.size(); }

		// 获取M1Renderable
		MeshRenderable& GetMeshRenderable( size_t idx);

		// 使用光照图
		bool ApplyLightmap( TexturePtr lightmapTex, const Vector4& lightmapViewport, int meshIdx);

		// 获取动画控制器
		AnimationController* GetAnimationController() { return &m_animationController; }

		// 更新
		virtual void FrameMove( float fTime);

		// 提交到渲染队列( 使用特效 队列索引)
		virtual void SubmitRenderElements( RenderElementMgr& renderElementMgr);

	private:
		// 绑定材质参数
		void AutoBindMaterialParameter( MeshRenderable& mr);

		// 绑定RenderLayout
		void AutoBindRenderLayout( MeshRenderable& mr);

		// 添加到场景消息
		virtual void OnAddToScene( Scene* scene);

	protected:
		IDCardPtr				m_idcard;				// 身份信息
		string					m_filePath;				// 模型路径
		AnimationController		m_animationController;	// 动画控制器
		vector<MeshRenderable>	m_renderables;			// 可渲染物集
		ShaderParamPtr			m_boneTransform;		// 骨骼转换
		ShaderParamPtr			m_lightViewCrop;		// 光源空间
	};
	typedef s_ptr<Model>  ModelPtr;
}