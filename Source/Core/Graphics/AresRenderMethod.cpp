#include <stdio.h>
#include "Core/Graphics/EffectLoader.h"
#include "Core/Graphics/CgTokenizer.h"
#include "Core/Graphics/AresRenderMethod.h"

namespace Ares
{
	// 解析词法分析器
	static void EffectParseTokenizer( CgTokenizer& tokenizer, RenderMethod& effect)
	{

	}

	// 加载函数 *.cgfx
	//bool RenderMethod::Load( const char* fileName)
	//{
	//	CgTokenizer tokenizer;
	//	if( tokenizer.TokenizeFile( fileName))
	//	{
	//		EffectParseTokenizer( tokenizer, *this);

	//		return true;
	//	}

	//	return false;
	//}

	// 根据参数名获取参数索引
	bool RenderMethod::GetParamIdx( size_t& idx, const string& paramName)
	{
		for ( size_t i=0; i<m_paramDescs.size(); i++)
		{
			if( m_paramDescs[i].m_name == paramName)
			{
				idx = i;

				return true;
			}
		}

		return false;
	}

	// constructor
	ShaderParam::ShaderParam()
		: m_type( SPT_Invalid),
		  m_bytes( 0),
		  m_elems( 0),
		  m_data( NULL)
	{}

	ShaderParam::~ShaderParam()
	{
		SAFE_DELETE_ARRAY( m_data);
	}

	// 重载运算符 "="
	void ShaderParam::operator = ( float val)
	{
		if( m_bytes == sizeof( float))
		{
			m_type = SPT_FloatArray;
			m_elems= 1;

			std::memcpy( m_dataFloat, &val, m_bytes);
		}
		else
		{
			SAFE_DELETE_ARRAY( m_data);

			m_type = SPT_FloatArray;
			m_bytes= sizeof( float);
			m_elems= 1;
			m_data = new_ float[1];

			std::memcpy( m_dataFloat, &val, m_bytes);
		}
	}

	// 重载运算符 "="
	void ShaderParam::operator = ( const ShaderParam& v)
	{
		if( m_bytes != v.m_bytes)
		{
			SAFE_DELETE_ARRAY(m_data);

			m_data = new_ float[v.m_elems];
		}

		if( v.m_bytes)
			memcpy( m_data, v.m_data, v.m_bytes);

		m_type  = v.m_type;
		m_elems = v.m_elems;
		m_bytes = v.m_bytes;
	}

	// 重载运算符 "="
	void ShaderParam::operator = ( const float2& val)
	{
		if( m_bytes == sizeof( float2))
		{
			m_type = SPT_FloatArray;
			m_elems= 2;
		}
		else
		{
			SAFE_DELETE_ARRAY( m_data);

			m_type = SPT_FloatArray;
			m_bytes= sizeof( float2);
			m_elems= 2;
			m_data = new_ float[2];
		}

		memcpy( m_dataFloat, &val, m_bytes);
	}

	// 重载运算符 "="
	void ShaderParam::operator = ( const float3& val)
	{
		if( m_bytes == sizeof( float3))
		{
			m_type = SPT_FloatArray;
			m_elems= 3;
		}
		else
		{
			SAFE_DELETE_ARRAY( m_data);

			m_type = SPT_FloatArray;
			m_bytes= sizeof( float3);
			m_elems= 3;
			m_data = new_ float[3];
		}

		memcpy( m_dataFloat, &val, m_bytes);
	}

	// 重载运算符 "="
	void ShaderParam::operator = ( const float4& val)
	{
		if( m_bytes == sizeof( float4))
		{
			m_type = SPT_FloatArray;
			m_elems= 4;
		}
		else
		{
			SAFE_DELETE_ARRAY( m_data);

			m_type = SPT_FloatArray;
			m_bytes= sizeof( float4);
			m_elems= 4;
			m_data = new_ float[4];
		}

		memcpy( m_dataFloat, &val, m_bytes);
	}

	// 重载运算符 "="
	void ShaderParam::operator = ( const vector<float>& val)
	{
		// 矩阵默认转置
		if( m_bytes == sizeof( float)*val.size())
		{
			m_type = SPT_FloatArray;
			m_elems= val.size();
		}
		else
		{
			SAFE_DELETE_ARRAY(m_data);

			m_type = SPT_FloatArray;
			m_bytes= sizeof( float) * val.size();
			m_elems= val.size();
			m_data = new_ float[m_elems];
		}

		std::memcpy( m_dataFloat, val.data(), m_bytes);
	}

	// 重载运算符 "="
	void ShaderParam::operator = ( const vector<float4>& val)
	{
		// 矩阵默认转置
		if( m_bytes == sizeof( float4)*val.size())
		{
			m_type = SPT_FloatArray;
			m_elems= 4 * val.size();
		}
		else
		{
			SAFE_DELETE_ARRAY(m_data);

			m_type = SPT_FloatArray;
			m_bytes= sizeof( float4) * val.size();
			m_elems= 4 * val.size();
			m_data = new_ float[m_elems];
		}

		std::memcpy( m_dataFloat, val.data(), m_bytes);
	}

	// 重载运算符 "="
	void ShaderParam::operator = (const ColorRGB& val)
	{
		if( m_bytes == sizeof( ColorRGB))
		{
			m_type = SPT_FloatArray;
			m_elems= 4;
		}
		else
		{
			SAFE_DELETE_ARRAY( m_data);

			m_type = SPT_FloatArray;
			m_bytes= sizeof( ColorRGB);
			m_elems= 16;
			m_data = new_ float[4];
		}

		memcpy( m_dataFloat, &val, m_bytes);
	}

	// 重载运算符 "="
	void ShaderParam::operator = (const float4x4& val)
	{
		// 矩阵默认转置
		if( m_bytes == sizeof( float4x4))
		{
			m_type = SPT_FloatArray;
			m_elems= 16;
		}
		else
		{
			SAFE_DELETE_ARRAY(m_data);

			m_type = SPT_FloatArray;
			m_bytes= sizeof( float4x4);
			m_elems= 16;
			m_data = new_ float[16];
		}

        float4x4 transpose = val.GetTranspose();
		memcpy( m_dataFloat, &transpose, m_bytes);
	}

	// 重载运算符 "="
	void ShaderParam::operator = (const vector<float4x4>& val)
	{
		// 矩阵默认转置
		if( m_bytes == sizeof( float4x4)*val.size())
		{
			m_type = SPT_FloatArray;
			m_elems= 16 * val.size();
		}
		else
		{
			SAFE_DELETE_ARRAY(m_data);

			m_type = SPT_FloatArray;
			m_bytes= sizeof( float4x4) * val.size();
			m_elems= 16 * val.size();
			m_data = new_ float[val.size()*16];
		}

		vector<float4x4> tVal = val;
		for( size_t i=0; i<tVal.size(); i++)
			tVal[i] = tVal[i].GetTranspose();

		std::memcpy( m_dataFloat, tVal.data(), m_bytes);
	}

	// 重载运算符 "="
	void ShaderParam::operator = ( const TexturePtr& val)
	{
		if( m_type==SPT_Texture)
		{
			m_elems= 1;

			*m_texture = val;
		}
		else
		{
			SAFE_DELETE_ARRAY( m_data);
			m_type = SPT_Texture;
			m_bytes = sizeof(TexturePtr);
			m_elems= 1;
			m_data = new_ TexturePtr(val);
		}
	}

	// 重载运算符 "="
	void ShaderParam::operator = ( const SamplerStatePtr& val)
	{
		if( m_type == SPT_SamplerState)
		{
			m_elems= 1;

			*m_samplerState = val;
		}
		else
		{
			SAFE_DELETE_ARRAY( m_data);
			m_type = SPT_SamplerState;
			m_bytes = sizeof(SamplerStatePtr);
			m_elems= 1;

			m_data = new_ SamplerStatePtr(val);
		}
	}

	// 重载运算符 "!="
	UBOOL ShaderParam::operator != ( const ShaderParam& v) const
	{
		if( m_type!=v.m_type || m_elems!=v.m_elems)
			return TRUE;

		if( m_type == SPT_Texture)
		{
			if( *m_texture != *v.m_texture)
				return TRUE;
		}
		else if( m_type==SPT_SamplerState)
		{
			if( m_samplerState != v.m_samplerState)
				return TRUE;
		}
		else
		{
			for( int i=0; i<m_elems; i++)
			{
				if( m_dataFloat[i] != v.m_dataFloat[i])
					return TRUE;
			}
		}

		return FALSE;
	}

	// 构造函数
	RenderMethod::RenderMethod()
		: m_isEnabled( false)
	{}

	// 加载函数 *.cgfx
	bool RenderMethod::Load(  const char* fileName, const ShaderMacros& macros) 
	{ 
		m_fileName = fileName;
		m_macros   = macros;
		
		return true; 
	}
}