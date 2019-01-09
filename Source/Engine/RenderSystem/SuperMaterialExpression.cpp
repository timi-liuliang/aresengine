#include <Engine/RenderSystem/SuperMaterialCompiler.h>
#include <Engine/AresRoot.h>

BOOST_CLASS_EXPORT( Ares::MaterialExpressionConstantVector)
BOOST_CLASS_EXPORT( Ares::MaterialExpressionConstantColor)

namespace Ares
{
	// ExpressionGuard
	struct ExpressionGuard
	{
		MaterialExpression*		m_expression;
		MaterialCompiler*		m_shaderTree;
	};

	// 构造函数
	ExpressionInput::ExpressionInput()
		: m_expression( NULL)
		, m_mask( false)
		, m_maskR( false)
		, m_maskG( false)
		, m_maskB( false)
		, m_maskA( false)
	{
	}

	// 析构函数
	ExpressionInput::~ExpressionInput()
	{

	}

	// 编译
	INT ExpressionInput::Compile( MaterialCompiler& compiler)
	{
		if( m_expression)
		{
			if( m_mask)
				return compiler.ComponentMask( m_expression->Compile( compiler), m_maskR, m_maskG, m_maskB, m_maskA);
			else
				return m_expression->Compile( compiler);
		}

		return INVALID;
	}

	// 构造函数
	MaterialExpression::MaterialExpression( ExpressionType type)
		: m_type( type)
	{}

	// 构造函数
	MaterialExpressionConstantVector::MaterialExpressionConstantVector( int numComponents)
		: MaterialExpression( ET_ConstantVector)
	{
		m_output.Set( numComponents);
	}

	// 编译
	INT MaterialExpressionConstantVector::Compile( MaterialCompiler& compile)
	{
		return compile.Constant( m_output.m_value, m_output.m_numComponents);
	}

	MaterialExpressionConstantColor::MaterialExpressionConstantColor()
		: MaterialExpression( ET_ConstantColor)
	{
		m_output.Set( 4);
	}

	// 编译
	INT MaterialExpressionConstantColor::Compile( MaterialCompiler& compile)
	{
		return compile.Constant( m_output.m_value, m_output.m_numComponents);
	}

	// 设置颜色
	void MaterialExpressionConstantColor::SetValue( const ColorRGB& color)
	{
		m_output.m_value = float4( color.r, color.g, color.g, color.a);
	}

	// 编译
	INT MaterialExpressionTextureSample::Compile( MaterialCompiler& compile)
	{
		if( m_texture)
		{
			//return compile.TextureSample();
		}
		else
		{
			ALogSystem.Warning( "Missing TextureSample input texture");
		}

		return INVALID;
	}

	// 构造函数
	MaterialExpressionSlot::MaterialExpressionSlot()
		: m_id( "Slot")
	{
		m_diffuseColor.m_id = "Slot_DiffuseColor";	
		m_normal.m_id	    = "Slot_Normal";
	}
}