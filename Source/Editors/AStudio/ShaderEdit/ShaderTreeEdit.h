#pragma once

#include <Engine/RenderSystem/SuperMaterial.h>
#include <Engine/RenderSystem/SuperMaterialShaderTree.h>

namespace Ares
{
	//-----------------------------------------
	// 超级材质编辑器 2012-12-20 帝林
	//-----------------------------------------
	class ShaderTreeEdit
	{
	public:
		ShaderTreeEdit();
		~ShaderTreeEdit();

		// 新建
		void New();

		// 添加表达式
		void AddExpression( ExpressionType type, const Vector2& pos);

		// 添加链接
		void AddConnection( const string& expression, const string& input);

		// 删除链接
		void DelConnection( const string& input);

		// 获取当前编辑ShaderTree
		MaterialShaderTree* GetShaderTree() { return m_shaderTree; }

		// 是否有存储路径
		bool IsHaveSavePath() { return !m_path.empty(); }

		// 设置存储路径
		void SetSavePath( const string& savepath) { m_path=savepath; }

		// 保存
		void Save();

		// 加载
		void Load( const string& path);

	private:
		string					m_path;				// 路径
		MaterialShaderTree*		m_shaderTree;		// shader tree
	};
}