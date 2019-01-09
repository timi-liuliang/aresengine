#pragma once

#include <Engine/FxSystem/AresFx3DPatch.h>
#include <Engine/FxSystem/AresFxParticleSystem.h>
#include <Engine/FxSystem/AresFxEntity.h>

namespace Ares
{
	//--------------------------------------
	// 效果编辑逻辑 2012-8-20 帝林
	//--------------------------------------
	class FxEdit
	{
	public:
		FxEdit();

		// 获取当前FxEntity
		FxEntity* GetFxEntity() { return m_fxEntity.get(); }

		// 更新
		void FrameMove(  float elapsedTime);

		// 执行
		void SubmitToRenderQueue();

	public:
		// 获取路径
		string GetAfxPath() { return m_afxPath; }

		// 打开效果
		void OpenAfx( const char* file);

		// 保存效果
		void SaveAfx( const char* filePath);

		// 新建效果
		void New();

		// 重置FxEntity
		void ResetFxEntity();

		// 添加效果对象
		void AddObject( FxObjectType type);

		// 获取特效半径
		float GetFxRadius() { return 10.f;}

		// 设置选中
		void SetSelected( int idx);

		// 删除选中
		void DeleteSelected( int idx);

	private:
		// 生成唯一名称
		string GetUniqueName();

	private:
		int						m_selectedIdx;
		string					m_afxPath;			// 存储路径
		FxEntityPtr				m_fxEntity;			// 效果实体
	};
}