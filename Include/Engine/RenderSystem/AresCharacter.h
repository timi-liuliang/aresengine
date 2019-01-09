#pragma once

#include "Engine/RenderSystem/AresRenderEntity.h"

namespace Ares
{
	//------------------------------------
	// 身体部件 2011-08-12 帝林
	//------------------------------------
	class IBodyPart
	{

	};

	//------------------------------------
	// 角色实体 2011-08-09 帝林
	//------------------------------------
	class ICharacterEntity : public RenderEntity
	{
	public:
		// 添加主体部件
		virtual bool AddBodyPart( const char* partName)=0;

		// 设置主体部件mesh
		virtual void SetBodyPartModel( const char* partName, const char* modPath, int meshIdx, int subsetIdx=0)=0;

		// 设置主体部件
		virtual void SetBodyPartModel( const char* partName, SPtr<IAresModel> modPath, int meshIdx, int subsetIdx=0)=0;

		// 设置主体部件材质
		virtual void SetBodyPartMaterial( const char* partName, const char* matPath, int matIdx)=0;

		// 设置主体部件材质
		virtual void SetBodyPartMaterial( const char* partName, SPtr<ISurfaceMaterial> pMaterial, int matIdx)=0;

		// 删除主体部件
		virtual void DelBodyPart( const char* partName)=0;

	public:
		// 获取动画控制器,必有值
		//virtual IAnimationController* GetAnimationController()=0;

	public:
		// 初始化(.cha格式)
		virtual void Load( const char *resPath)=0;

		// 保存
		virtual void Save( const char* resPath)=0;
	};
}