#pragma once

#include "AresRenderState.h"
#include "AresRenderAPITypedefs.h"

using namespace std;

namespace Ares
{
	// 参数类型枚举
	enum ShaderParamType
	{
		SPT_Invalid = 0,
		SPT_FloatArray,
		SPT_Texture,
		SPT_SamplerState,
	};

	//----------------------------------
	// ShaderParamDesc 2012-4-11 帝林
	//----------------------------------
	struct ShaderParamDesc
	{
		std::string		m_name;
		ShaderParamType	m_type;

		// 重载比较运算符
		FORCEINLINE UBOOL operator == ( const ShaderParamDesc& spd) const
		{
			return m_name==spd.m_name && m_type==spd.m_type;
		}
	};
	typedef vector<ShaderParamDesc> ShaderParamDescs;

	//------------------------------------
	// RM::Variant 2012/4/27 帝林
	//------------------------------------
	class ShaderParam
	{
	public:
		// constructor
		ShaderParam();
		~ShaderParam();

		// 重载运算符 "="
		void operator = ( float val);

		// 重载运算符 "="
		void operator = ( const vector<float>& val);

		// 重载运算符 "="
		void operator = ( const float2& val);

		// 重载运算符 "="
		void operator = ( const float3& val);

		// 重载运算符 "="
		void operator = ( const float4& val);

		// 重载运算符 "="
		void operator = ( const vector<float4>& val);

		// 重载运算符 "="
		void operator = ( const ColorRGB& val);

		// 重载运算符 "="
		void operator = ( const float4x4& val);

		// 重载运算符 "="
		void operator = ( const vector<float4x4>& val);

		// 重载运算符 "="
		void operator = ( const TexturePtr& val);

		// 重载运算符 "="
		void operator = ( const SamplerStatePtr& val);

		// 重载运算符 "="
		void operator = ( const ShaderParam& v);

		// 重载运算符 "!="
		UBOOL operator != ( const ShaderParam& v) const;

		// 是否可用
		bool IsValid() const { return m_type!=SPT_Invalid; }

		// 判断类型
		bool IsType( ShaderParamType type) const { return static_cast<bool>(m_type==type); }

	public:
		ShaderParamType		m_type;			// 类型
		int					m_bytes;		// 字节数
		int					m_elems;		// 元素个数
		union
		{
			void*			m_data;			// 数据
			bool*			m_dataBool;		// 以bool格式进行查看
			int*			m_dataInt;		// 以Int格式进行查看
			float*			m_dataFloat;	// 以float格式进行查看
			TexturePtr*		m_texture;		// 纹理
			SamplerStatePtr*m_samplerState;	// 采样状态
		};
	};

	// Make VisualParam (Factory pattern)
	template<typename T>
	INLINE ShaderParamPtr MakeShaderParam(const T& val)
	{
		ShaderParamPtr vp = ShaderParamPtr( new_ ShaderParam);
		*vp = val;

		return vp;
	}

	// MakeShaderParam
	INLINE ShaderParamPtr MakeShaderParam()
	{
		return ShaderParamPtr( new_ ShaderParam);
	}

	//---------------------------------
	// ShaderType 2012-9-1 帝林
	//---------------------------------
	enum ShaderType
	{
		ST_VertexShader  = 0,
		ST_PixelShader,
		ST_GeometryShader,
		ST_ComputeShader,
		ST_HullShader,
		ST_DomainShader,

		ST_NumShaderTypes,
	};

	// 宏定义
	struct ShaderMacro
	{
		string  m_name;			// 宏名称
		string	m_definition;	// 宏定义

		// 构造函数
		ShaderMacro( const char* name, const char* definition)
			: m_name( name), m_definition( definition)
		{}
	};
	typedef vector<ShaderMacro> ShaderMacros;

	//----------------------------------
	// RenderMethod 2012-4-5 帝林
	//----------------------------------
	class RenderMaterial;
	struct RenderMethod
	{
		friend class RenderMaterial;
		typedef std::vector<ShaderParamPtr>			  VisualParamPtrs;
		typedef boost::array<bool, ST_NumShaderTypes> ShaderValidateArray;

	public:
		// 构造函数
		RenderMethod();

		// 虚析构函数
		virtual ~RenderMethod() {}

		// 加载函数 *.cgfx
		virtual bool Load(  const char* fileName, const ShaderMacros& macros);

		// 从内存加载
		virtual bool LoadFromMemory( const void* code, size_t length)=0;

		// 绑定
		virtual void Bind() {};

		// 应用参数
		virtual void SetParams(const RenderMaterial& mat)=0;

		// 保存ByteCode
		virtual void SaveByteCode( const char* fileName, ShaderType type) {}

	public:
		// 是否可用
		bool IsEnabled() { return m_isEnabled; }

		// 获取默认参数数量
		size_t GetParamNum() { return m_curParamValues.size(); }

		// 获取参数描述
		const ShaderParamDesc& GetParamDesc( int idx) { return m_paramDescs[idx]; }

		// 获取默认参数
		const ShaderParamPtr& GetVisualParamPtr( size_t i) { return m_curParamValues[i]; }

		// 根据参数名获取参数索引
		bool GetParamIdx( size_t& idx, const string& paramName);

	protected:
		string					m_fileName;			// 资源名称
		bool					m_isEnabled;			// 是否可用
		ShaderMacros			m_macros;			// 宏
		ShaderParamDescs		m_paramDescs;		// param descs
		VisualParamPtrs			m_curParamValues;	// current param values

		bool					m_hasDiscard;
		bool					m_hasTessellation;
		ShaderValidateArray		m_isShaderValidate;

		RasterizerStatePtr		m_rasterState;		// 光栅化状态
		DepthStencilStatePtr	m_depthStencilState;// 深度状态
		BlendStatePtr			m_blendState;		// 混合状态		
	};
}