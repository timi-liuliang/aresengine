#pragma once

#include <list>
#include <Engine/RenderSystem/AresModelIDCard.h>
#include <Core/AresMath.h>

using namespace std;

namespace Ares
{
	//----------------------------------------
	// M1 Model 2010-09-21    帝林
	//----------------------------------------
	class CAresAnimation : public CM1Animation
	{
	public:
		// 构造函数
		CAresAnimation();

		// 析构函数
		~CAresAnimation();

	public:
		// 加载资源
		//virtual bool LoadResource();

		// 保存资源
		virtual bool Save(char* fileName){ return false;}

	public:
		// 获取骨骼的所有子骨骼索引
		bool GetChildBoneIdxs( size_t parentIdx, vector<size_t>& childIdxs);

		// 获取骨骼的所有子及兄弟骨骼索引
		bool GetChildSiblingBoneIdxs( size_t parentIdx, vector<size_t>& childIdxs);

	public:
		// 获取骨骼数量
		virtual size_t GetNumBones() { return CM1Animation::m_header.m_iNumBones; }

		// 根据骨骼名获取骨骼索引
		virtual bool GetBoneIdxByName( const char* name, int& boneIdx);

		// 获取骨骼的世界矩阵
		virtual Matrix44 GetBoneWorld( int boneIdx);

		// 获取骨骼初始矩阵
		virtual bool GetInitBoneTM( Matrix44& out, size_t index);

	public:
		// 获取主骨骼
		virtual int GetRootBone();

		// 根据索引获取骨骼名
		virtual const char* GetBoneName( size_t index);

		// 获取兄弟骨骼
		virtual int GetBoneSibingIdx( size_t index);

		// 获取第一个子骨骼
		virtual int GetBoneFirstChildIdx( size_t index);

	public:
		// 获取动画数
		virtual const size_t GetNumAnimationSet() { return CM1Animation::m_header.m_iNumAnimationSet; }

		// 获取动画名
		virtual const char*  GetAnimationSetName( size_t index);

		// 暂未放到接口中
	public: 
		// 获取SM1AnimationSet结构体
		const SM1AnimationSet* GetAnimationSet(size_t index); 

		// 根据名字获取SM1AnimationSet结构体
		const SM1AnimationSet* GetAnimationSet( const char* name, size_t* pIndex=NULL);
	};
}