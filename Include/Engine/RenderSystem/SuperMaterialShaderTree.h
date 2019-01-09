#pragma once

#include <map>
#include <Core/AresSerialization.h>
#include "SuperMaterialD3D11Compiler.h"

using namespace std;

namespace Ares
{
	//-------------------------------------------
	// SuperMaterialShaderTree 2013-3-12 帝林 
	//-------------------------------------------
	class MaterialShaderTree
	{
	public:
		MaterialShaderTree();
		~MaterialShaderTree();

		// 编译
		INT Compile( string& oCode, const string& iCode);

		// 添加表达式
		void AddExpression( MaterialExpression* expression);

		// 添加链接
		void AddConnection( MaterialExpressionConnection* conection);

		// 删除链接
		void DelConnection( const string& input);

		// 获取材质槽
		MaterialExpressionSlot& GetExpressionSlot() { return m_slot; }

		// 获取表达式输入
		ExpressionInput* GetExpressionInput( const string& id);

		// 获取表达式
		MaterialExpression* GetMaterialExpression( const string& id);

		// 获取表达式
		const vector<MaterialExpression*>& GetMaterialExpression() { return m_expressions; }

		// 获取链接
		MaterialExpressionConnection* GetMaterialExpressionConnection( const string& expression, const string& input);

		// 获取链接
		const vector<MaterialExpressionConnection*>& GetMaterialExpressionConnection() { return m_connections; }

		// 序列化
		BOOST_SERIALIZATION_SPLIT_MEMBER()
		template<typename Archive> void save( Archive& ar, const unsigned int version) const
		{
			ar & m_idSeed;
			ar & m_slot;
			ar & m_expressions;
			ar & m_connections;
		}
		template<typename Archive> void load( Archive& ar, const unsigned int version)
		{
			ar & m_idSeed;
			ar & m_slot;
			ar & m_expressions;
			ar & m_connections;

			RegisterInputs();
			ReConnection();
		}

	private:
		// 重新链接
		void ReConnection();

		// 获取唯一ID
		void BuildUniqueID( string& oID);

		// 注册inputs
		void RegisterInputs();

	private:
		D3D11MaterialCompiler					m_compiler;		// D3D11MaterialCompiler

		int										m_idSeed;		// id种子
		MaterialExpressionSlot					m_slot;			// 槽
		vector<ExpressionInput*>				m_inputs;		// 输入
		vector<MaterialExpression*>				m_expressions;	// 表达式
		vector<MaterialExpressionConnection*>	m_connections;	// 链接
	};
}