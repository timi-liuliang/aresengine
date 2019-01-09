#pragma once

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>
#include <Core/AresCore.h>
#include <Core/Graphics/AresTexture.h>

using namespace std;

namespace Ares
{
	// 声明
	class MaterialCompiler;
	class MaterialExpression;

	// UI坐标系
	struct UICoordinate
	{
		Vector2		m_pos;		// 在界面中位置,辅助界面显示

		// 构造函数
		UICoordinate() : m_pos( 0.f, 0.f)
		{}

		// 设置位置
		void Set( int x, int y)
		{
			m_pos.x = static_cast<float>(x);
			m_pos.y = static_cast<float>(y);
		}

		// 序列化
		template <typename Archive> void serialize( Archive& ar, const unsigned int version)
		{
			ar & m_pos;
		}
	};

	// 表达式输入
	struct ExpressionInput
	{
		string				m_id;			// 标识
		MaterialExpression*	m_expression;	// 表达式
		bool				m_mask;
		bool				m_maskR;
		bool				m_maskG;
		bool				m_maskB;
		bool				m_maskA;

		// 构造函数
		ExpressionInput();

		// 析构函数
		virtual ~ExpressionInput();

		// 编译
		INT Compile( MaterialCompiler& compiler);

		// 序列化
		template <typename Archive> void serialize( Archive& ar, const unsigned int version)
		{
			ar & m_id;
			ar & m_mask;
			ar & m_maskR;
			ar & m_maskG;
			ar & m_maskB;
			ar & m_maskA;
		}
	};

	// ColorMaterialInput


	// ExpressionOutput
	struct ExpressionOutput
	{
		int			m_numComponents;	// 数量
		float4		m_value;			// 数据

		// 构造函数
		ExpressionOutput() 
		{}

		// 构造函数
		ExpressionOutput( int numComponents)
			: m_numComponents( numComponents)
		{};

		// 设置参数
		void Set( int numComponents)
		{
			m_numComponents = numComponents;
		}

		// 序列化
		template <typename Archive> void serialize( Archive& ar, const unsigned int version)
		{
			ar & m_numComponents;
			ar & m_value;
		}
	};

	// 表达式类型
	enum ExpressionType
	{
		ET_ConstantVector,
		ET_ConstantColor,
		ET_TextureSample,
	};

	//-------------------------------------------
	// MaterialExpression 2012-12-20 帝林
	//-------------------------------------------
	class MaterialExpression
	{
	public:
		MaterialExpression() {}
		MaterialExpression( ExpressionType type);
        virtual ~MaterialExpression() {}

		// 获取输出
		const ExpressionOutput& GetOutput() const { return m_output; }

		// 获取输入
		virtual void GetInputs( vector<ExpressionInput*>& inputs) const=0;

		// 编译 返回ChunkIndex
		virtual INT Compile( MaterialCompiler& compiler) { return INVALID; }

		// 标题说明
		virtual string GetCaption() const { return "Expression"; }

		// 获取ID
		const string& GetID() const { return m_id; }

		// 获取类型
		const ExpressionType GetType() { return m_type; }

		// 获取位置
		UICoordinate& GetUICoordinate() { return m_uiCoordinate; }

		// 序列化
		template <typename Archive> void serialize( Archive& ar, const unsigned int version)
		{
			ar & m_id;
			ar & m_type;
			ar & m_uiCoordinate;
			ar & m_output;
		}

	public:
		std::string			m_id;				// 标识
		ExpressionType		m_type;				// 表达式类型
		UICoordinate		m_uiCoordinate;		// 在界面中位置,辅助界面显示

		ExpressionOutput	m_output;			// 每个表达式只有一个输出(参照GameStart3D, 摈弃Unreal做法)
	};

	//---------------------------------
	// 向量 2013-3-12 帝林
	//---------------------------------
	class MaterialExpressionConstantVector : public MaterialExpression
	{
	public:
		MaterialExpressionConstantVector() {}
		MaterialExpressionConstantVector( int numComponents);

		// 获取输入
		virtual void GetInputs( vector<ExpressionInput*>& inputs) const {}

		// 编译
		virtual INT Compile( MaterialCompiler& compile);

		// 序列化
		template <typename Archive> void serialize( Archive& ar, const unsigned int version)
		{
#ifdef ARES_PLATFORM_WINDOWS
			ar & boost::serialization::base_object<MaterialExpression>(*this);
#endif
		}
	};

	//---------------------------------
	// 常量颜色 2013-3-25 帝林
	//---------------------------------
	class MaterialExpressionConstantColor : public MaterialExpression
	{
	public:
		MaterialExpressionConstantColor();

		// 获取输入
		virtual void GetInputs( vector<ExpressionInput*>& inputs) const {}

		// 编译
		virtual INT Compile( MaterialCompiler& compile);

		// 设置颜色
		void SetValue( const ColorRGB& color);

		// 序列化
		template <typename Archive> void serialize( Archive& ar, const unsigned int version)
		{
#ifdef ARES_PLATFORM_WINDOWS
			ar & boost::serialization::base_object<MaterialExpression>(*this);
#endif
		}
	};

	//---------------------------------
	// TextureSample 2013-3-11 帝林
	//---------------------------------
	class MaterialExpressionTextureSample : public MaterialExpression
	{
	public:
		// 编译
		virtual INT  Compile( MaterialCompiler& compile);

		// 标题说明
		virtual string GetCaption() const { return "Texture Sample"; }

		// 序列化
		template <typename Archive> void serialize( Archive& ar, const unsigned int version)
		{
#ifdef ARES_PLATFORM_WINDOWS
			ar & boost::serialization::base_object<MaterialExpression>(*this);
#endif
		}

	public:
		TexturePtr			m_texture;			// 使用纹理
		ExpressionInput		m_coordinates;		// uv coordinates stream
												// 纹理
												// 采样方式
	};

	//----------------------------------------------
	// MaterialExpressionConnection 2013-3-13 帝林
	//----------------------------------------------
	class MaterialExpressionConnection
	{
	public:
		MaterialExpressionConnection() {}
		MaterialExpressionConnection( const string& expression, const string& input)
			: m_expression( expression), m_input( input)
		{}
	
		// 析构函数
		virtual ~MaterialExpressionConnection() {}

		// 序列化
		template <typename Archive> void serialize( Archive& ar, const unsigned int version)
		{
			ar & m_input;
			ar & m_expression;
		}

	public:
		string		m_input;
		string		m_expression;
	};

	//---------------------------------
	// 材质槽 2013-3-25 帝林
	//---------------------------------
	class MaterialExpressionSlot
	{
	public:
		MaterialExpressionSlot();

		// 获取位置
		UICoordinate& GetUICoordinate() { return m_uiCoordinate; }

		// 序列化
		template <typename Archive> void serialize( Archive& ar, const unsigned int version)
		{
			ar & m_id;
			ar & m_uiCoordinate;
			ar & m_diffuseColor;
			ar & m_normal;
		}

	public:
		string				m_id;				// 标识
		UICoordinate		m_uiCoordinate;		// 在界面中位置,辅助界面显示
		//ExpressionInput	m_varialbe;			// 变量
		ExpressionInput		m_diffuseColor;		// 漫反射
		ExpressionInput		m_normal;			// 法线
	};
}
