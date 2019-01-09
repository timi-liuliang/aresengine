#pragma once

#include <string>
#include <Core/AresTransform.h>
#include <Physics/Rect3.h>

using namespace std;

namespace Ares
{
	//------------------------------------------
	// 效果对象类型 2012-8-25 帝林
	//------------------------------------------
	enum FxObjectType
	{
		FOT_2DPatch=0,		// 2D单片
		FOT_3DPatch,		// 3D单片
		FOT_2DGridPatch,	// 2D网格单片
		FOT_3DGridPatch,	// 3D网格单片
		FOT_ParticleSystem,	// 粒子系统
		FOT_Decal,			// 贴花
		FOT_Cloth,			// 布料
		FOT_Sound,			// 声效
		FOT_Max,
	};

	//-----------------------------------------
	// 效果对象 2012-8-25 帝林
	//-----------------------------------------
	class FxEntity;
	class RenderElementMgr;
	class FxObject
	{
	public:
		// 构造函数
		FxObject( FxObjectType type)
			: m_type( type)
			, m_entity( NULL)
		{}

		// 虚析构函数
		virtual ~FxObject() 
		{}

		// 返回类型
		FxObjectType GetType() { return m_type; }

		// 获取名称
		const char* GetName() const { return m_name.c_str(); }

		// 获取实体
		FxEntity* GetEntity() { return m_entity; }

		// 获取转换
		Transform* GetTransform() { return &m_transform; }

		// 获取世界转换
		Transform GetWorldTransform();

		// 获取本地包围盒
		const Rect3& GetLocalBounds() const { return m_localBounds; }

		// 设置名称
		void SetName( const char* name) { m_name = name; } 

		// 设置实体
		void SetEntity( FxEntity* enity) { m_entity=enity; }

		// 更新
		virtual void FrameMove( float elpasedTime, FxEntity* fxEntity)=0;

		// 执行
		virtual void SubmitRenderElements( RenderElementMgr& renderElementMgr)=0;

		// 序列化
		template <typename Archive> void serialize( Archive& ar, const unsigned int version)
		{
			ar & m_type;
			ar & m_name;
			ar & m_transform;
		}

	protected:
		FxObjectType	m_type;			// 类型
		string			m_name;			// 名称
		Transform		m_transform;	// 转换工具
		Rect3			m_localBounds;	// 本地包围盒
		FxEntity*		m_entity;		// 实体
	};
}