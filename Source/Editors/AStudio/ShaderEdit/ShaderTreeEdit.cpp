#include "ShaderTreeEdit.h"

namespace Ares
{
	// 构造函数
	ShaderTreeEdit::ShaderTreeEdit()
		: m_shaderTree( NULL)
	{
		//string templateCode;
		//string modifyedCode;
		//ifstream templateIfs( AResSystem.FullPath( "SuperMaterialTemplate.hlsl"));
		//if( templateIfs.is_open())
		//{
		//	std::ostringstream oss; oss<<templateIfs.rdbuf();
		//	templateCode = oss.str();
		//}

		//MaterialShaderTree shaderTree;

		//MaterialExpressionConstantVector* eConstant = new_ MaterialExpressionConstantVector( "Color0", 4);
		//shaderTree.AddExpression( eConstant);

		//MaterialExpressionConnection* eConnection = new_ MaterialExpressionConnection( "oDiffuseColor", eConstant->m_name);
		//shaderTree.AddConnection( eConnection);

		//shaderTree.Compile( modifyedCode, templateCode);
	}

	// 新建
	void ShaderTreeEdit::New()
	{
		SAFE_DELETE( m_shaderTree);

		m_shaderTree = new_ MaterialShaderTree;
		m_shaderTree->GetExpressionSlot().GetUICoordinate().Set( 100, 250);
	}

	// 添加表达式
	void ShaderTreeEdit::AddExpression( ExpressionType type, const Vector2& pos)
	{
		A_ASSERT( m_shaderTree);

		switch( type)
		{
		case ET_ConstantColor:
			{
				MaterialExpressionConstantColor* constantColor = new_ MaterialExpressionConstantColor();
				if( constantColor)
				{
					constantColor->GetUICoordinate().Set( (int)pos.x, (int)pos.y);
					m_shaderTree->AddExpression( constantColor);
				}
			}
			break;
		}
	}

	// 添加链接
	void ShaderTreeEdit::AddConnection( const string& expression, const string& input)
	{
		MaterialExpressionConnection* eConnection = new_ MaterialExpressionConnection( expression, input);
		m_shaderTree->AddConnection( eConnection);
	}

	// 删除链接
	void ShaderTreeEdit::DelConnection( const string& input)
	{
		m_shaderTree->DelConnection( input);
	}

	// 加载
	void ShaderTreeEdit::Load( const string& path)
	{
		SAFE_DELETE( m_shaderTree);

		m_path = path;
		ifstream ifs( m_path.c_str());
		if( ifs.is_open())
		{
			boost::archive::binary_iarchive biarchive( ifs);
			biarchive >> m_shaderTree;
		}
	}

	// 保存
	void ShaderTreeEdit::Save()
	{
		ofstream ofs( m_path.c_str());
		if( ofs.is_open())
		{
			boost::archive::binary_oarchive boarchive( ofs);
			boarchive << m_shaderTree;
		}
	}
}