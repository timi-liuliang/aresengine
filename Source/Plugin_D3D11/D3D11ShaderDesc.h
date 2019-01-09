#pragma once

#include <string>

using namespace std;

namespace Ares {
namespace HLSL
{
	// Variable desc
	struct VariableDesc
	{
		string		m_name;						// 变量名称
		UINT		m_startOffset;				// 偏移量
		UINT		m_size;						// 大小
		BYTE		m_type;						// 类型
		BYTE		m_rows;						// 行数
		BYTE		m_columns;					// 列数
		WORD		m_elements;
	};

	// BoundResourceDesc
	struct BoundResourceDesc
	{
		string		m_name;
		BYTE		m_type;
		BYTE		m_dimension;
		WORD		m_bindpoint;
	};

	// Constant buffer desc
	struct ConstantBufferDesc
	{
		UINT					m_size;			// 数据大小
		vector<VariableDesc>	m_varDesc;		// 变量数组
		vector<BYTE>			m_memorys;		// 内存数据
	};

	// Shader描述
	struct ShaderDesc
	{
		vector<ConstantBufferDesc>	m_cbDesc;		// constand buffer desc
		vector<BoundResourceDesc>	m_sampDesc;		// bound resource desc
		vector<BoundResourceDesc>	m_srvDesc;		// bound resource desc

		// 构造函数
		ShaderDesc()
		{}
	};
}}