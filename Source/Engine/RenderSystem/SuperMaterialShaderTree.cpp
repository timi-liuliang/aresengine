#include <Engine/RenderSystem/SuperMaterialShaderTree.h>
#include <Engine/AresRoot.h>
#include <boost/algorithm/string.hpp>

namespace Ares
{
	// 构造函数
	MaterialShaderTree::MaterialShaderTree()
		: m_idSeed( 0)
	{
		m_inputs.reserve( 512);	

		m_inputs.push_back( &m_slot.m_diffuseColor);
		m_inputs.push_back( &m_slot.m_normal);
	}

	// 析构函数
	MaterialShaderTree::~MaterialShaderTree()
	{
		foreach_( MaterialExpression* expression, m_expressions)
			SAFE_DELETE( expression);

		foreach_( MaterialExpressionConnection* connection, m_connections)
			SAFE_DELETE( connection);

		m_expressions.clear();
		m_connections.clear();
	}

	// 编译
	INT MaterialShaderTree::Compile( string& oCode, const string& iCode)
	{
		// 获取块索引
		INT diffuseChunkIdx = m_slot.m_diffuseColor.Compile( m_compiler);

		oCode = iCode;
		boost::replace_first( oCode, "SLOT_VARIABLES", "");
		boost::replace_first( oCode, "SLOT_DIFFUSE",   m_compiler.GetChunkCode( diffuseChunkIdx));

		return INVALID;
	}

	// 添加表达式
	void MaterialShaderTree::AddExpression( MaterialExpression* expression)
	{
		if( expression)
		{
			// 添加公式
			m_expressions.push_back( expression);
			BuildUniqueID( expression->m_id);

			// 添加输入
			vector<ExpressionInput*> inputs; 
			expression->GetInputs( inputs);
			foreach_( ExpressionInput* input, inputs)
			{
				BuildUniqueID( input->m_id);
				m_inputs.push_back( input);
			}
		}
	}

	// 注册inputs
	void MaterialShaderTree::RegisterInputs()
	{
		foreach_( MaterialExpression* expression, m_expressions)
		{
			vector<ExpressionInput*> inputs; 
			expression->GetInputs( inputs);
			foreach_( ExpressionInput* input, inputs)
			{
				m_inputs.push_back( input);
			}
		}	
	}

	// 添加链接
	void MaterialShaderTree::AddConnection( MaterialExpressionConnection* conection)
	{
		if( conection)
		{
			// 1.去除冲突的链接
			DelConnection( conection->m_input);

			// 2.添加新的链接数据
			m_connections.push_back( conection);

			// 3.重新组织链接
			ReConnection();
		}
	}

	// 删除链接
	void MaterialShaderTree::DelConnection( const string& input)
	{
		// 1.去除冲突的链接
		for( vector<MaterialExpressionConnection*>::iterator it=m_connections.begin(); it!=m_connections.end(); )
		{
			if( (*it)->m_input == input)
				it = m_connections.erase( it);
			else
				it++;
		}
	}

	// 获取ExpressionInput
	ExpressionInput* MaterialShaderTree::GetExpressionInput( const string& id)
	{
		for( size_t i=0; i<m_inputs.size(); i++)
		{
			if( m_inputs[i]->m_id == id)
				return m_inputs[i];
		}

		return NULL;
	}

	// 获取表达式
	MaterialExpression* MaterialShaderTree::GetMaterialExpression( const string& id)
	{
		for( size_t i=0; i<m_expressions.size(); i++)
		{
			if( m_expressions[i]->m_id == id)
				return m_expressions[i];
		}

		return NULL;
	}

	// 获取链接
	MaterialExpressionConnection* MaterialShaderTree::GetMaterialExpressionConnection( const string& expression, const string& input)
	{
		for( size_t i=0; i<m_connections.size(); i++)
		{
			if( m_connections[i]->m_expression==expression && m_connections[i]->m_input==input)
				return m_connections[i];
		}

		return NULL;
	}

	// 重新链接,(加载时将使用此函数)
	void MaterialShaderTree::ReConnection()
	{
		// 断开所有链接
		for( size_t i=0; i<m_inputs.size(); i++)
			m_inputs[i]->m_expression = NULL;

		// 重新链接
		for( size_t i=0; i<m_connections.size(); i++)
		{
			ExpressionInput*	input      = GetExpressionInput( m_connections[i]->m_input);
			MaterialExpression* expression = GetMaterialExpression( m_connections[i]->m_expression);
			if( input && expression)
				input->m_expression = expression;
			else
				ALogSystem.Warning( "Expression or connected missed! can't connect");
		}
	}

	// 获取唯一ID
	void MaterialShaderTree::BuildUniqueID( string& oID)
	{
		char id[512];
#ifdef ARES_PLATFORM_WINDOWS
		sprintf_s( id, 512, "%d", m_idSeed++);
#else
        sprintf( id, "%d", m_idSeed++);
#endif

		oID = id;
	}
}