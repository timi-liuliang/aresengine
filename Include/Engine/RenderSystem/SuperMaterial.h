#pragma once

#include "AresMesh.h"
#include <Core/Graphics/AresRenderMaterial.h>
#include "SuperMaterialCompiler.h"

namespace Ares
{
	// SuperMaterialUsage
	enum SuperMaterialUsage
	{
		SMU_OutputDiffuse   = 1<<0,
		SMU_OutputNormal    = 1<<1,
		SMU_OutputDepth		= 1<<2,
		SMU_StaticLighting	= 1<<3,
		SMU_SkeletalMesh	= 1<<4,
		SMU_Total			= 5,
	};

	//-------------------------------------
	// 超级材质 2012-12-19 帝林
	//-------------------------------------
	class SuperMaterial : public RenderMaterial
	{
	public:
		struct IDCard
		{
			// 参数
			struct Parameter
			{
				string	m_name;					// 参数名称
				string	m_type;					// 参数类型
				string	m_value;				// 参数值

				// 构造函数
				Parameter() {}
				Parameter( const char* name, const char* type, const char* value)
					: m_name( name), m_type( type), m_value( value)
				{}

				// 序列化
				template <typename Archive> void serialize( Archive& ar, const unsigned int version)
				{
					ar & m_name;
					ar & m_type;
					ar & m_value;
				}
			};

			BITFIELD				m_renderQueue;				// 渲染队列
			string					m_shader;					// shader
			vector<Parameter>		m_parameters;				// 参数
			DepthStencilStateDesc	m_depthStencilStateDesc;	// 深度模板状态

			// 获取值
			Parameter* Parameters( const string& name);

			// 重初始化材质
			void ReBuildSuperMaterial( SuperMaterial& superMaterial, QWORD usage);

			// 序列化
			template <typename Archive> void serialize( Archive& ar, const unsigned int version)
			{
				ar & m_renderQueue;
				ar & m_shader;
				ar & m_parameters;
				ar & m_depthStencilStateDesc;
			}
		};
		typedef s_ptr<IDCard> IDCardPtr;

	public:
		// Constructors
		SuperMaterial();

		/** Get vertex stream flag that the material used
		 */
		UINT GetVertexStreamFlag();

		// Set the shadertree path
		void SetShaderTreePath( const char* path);

		/**	Sets the given usage flag
		 *	@param  usage			 - The usage flag to set
		 *	@return BOOL			 - TURE if the material can be used for rendering with the given type
		 */
		void SetUsage( QWORD usage) { m_usage = usage; }

		/** Gets the value associated with the given usage flag. */
		QWORD GetUsage( SuperMaterialUsage usage) const { return m_usage; }

		/** Compiles shader for the given platform*/
		void Compile();

	public:
		// Get ztest mask texture
		ShaderParamPtr GetZTestMaskTexture();

	private:
		// Generate Shader By ShaderTree
		string GenerateShaderByShaderTree( const char* shaderTreePath);

	public:
		QWORD						m_usage;						// 宏标记
		//BITFIELD					m_bOutputDiffuse:1;					// 输出漫反射
		//BITFIELD					m_bOutputNormal:1;					// 输出法线
		BITFIELD					m_bOutputDepth:1;					// 输出深度

		BITFIELD					m_bUsedWithStaticLighting:1;		// 静态光照
		BITFIELD					m_bUsedWithSkeletalMesh:1;			// 骨骼动画
		BITFIELD					m_bUsedAsSpecialEngineMaterial:1;
		BITFIELD					m_bWireframe:1;						// 网格显示
		BITFIELD					m_bFallbackMaterial:1;				// 是否为退却材质

		string						m_shadertreePath;					// 材质树路径
		SMShaderTreePtr				m_shaderTree;						// 材质树
	};
	typedef s_ptr<SuperMaterial> SuperMaterialPtr;
}