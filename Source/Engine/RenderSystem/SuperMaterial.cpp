#include <Engine/AresRoot.h>
#include <Engine/RenderSystem/SuperMaterial.h>
#include <Engine/RenderSystem/SuperMaterialShaderTree.h>
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <Core/AresFileUtils.h>
#include <fstream>

using namespace boost;

namespace Ares
{
	// 获取值
	SuperMaterial::IDCard::Parameter* SuperMaterial::IDCard::Parameters( const string& name)
	{
		foreach_( Parameter& param, m_parameters)
		{
			if( param.m_name==name)
				return &param;
		}

		return NULL;
	}

	// 重初始化材质
	void SuperMaterial::IDCard::ReBuildSuperMaterial( SuperMaterial& superMaterial, QWORD usage)
	{
		superMaterial.SetRenderQueue( m_renderQueue);
		superMaterial.SetShaderTreePath( m_shader.c_str());
		superMaterial.SetUsage( usage);
		superMaterial.Compile();
		for( size_t i=0; i<m_parameters.size(); i++)
		{
			if( m_parameters[i].m_value.substr( 0, 5) == "SPAB_")
			{
				superMaterial.SetParamAutoBind( m_parameters[i].m_name, m_parameters[i].m_value);
			}
			else
			{
				const string& paramType = m_parameters[i].m_type;
				if( paramType == "int")
				{

				}
				else if( paramType == "float")
				{

				}
				else if( paramType == "int_array")
				{

				}
				else if( paramType == "SPT_FloatArray")
				{
					typedef tokenizer<char_separator<char> > tokenizer;

					vector<float> paramValue;
					char_separator<char> sep(" ,;");
					tokenizer tok( m_parameters[i].m_value, sep);
					for( tokenizer::iterator it=tok.begin(); it!=tok.end(); it++)
					{
						string seperate = *it;
						paramValue.push_back( lexical_cast<float>( seperate));
					}

					superMaterial.SetParamRef( m_parameters[i].m_name, MakeShaderParam( paramValue));
				}
				else if( paramType == "SPT_Texture")
				{
					// 纹理参数
					TexturePtr texture = AResSystem.SyncLoadDDS( m_parameters[i].m_value.c_str(), EAH_GPURead | EAH_Immutable);

					superMaterial.SetParamRef( m_parameters[i].m_name, MakeShaderParam( texture));
				}
			}
		}

		if( m_depthStencilStateDesc != DepthStencilStateDesc())
		{
			DepthStencilStatePtr depthStencilState = ARenderDevice->CreateDepthStencilState( m_depthStencilStateDesc);
			superMaterial.SetDepthStencilState( depthStencilState);
		}
	}

	// Constructors
	SuperMaterial::SuperMaterial()
		: RenderMaterial()
		, m_bUsedAsSpecialEngineMaterial( FALSE)
		, m_bUsedWithStaticLighting( FALSE)
		, m_bUsedWithSkeletalMesh( FALSE)
	{

	}

	/** Get vertex stream flag that the material used
	 */
	UINT SuperMaterial::GetVertexStreamFlag()
	{
		UINT vsf = Mesh::VSF_Position;
		
		if( m_usage | SMU_OutputDiffuse)
			vsf |= Mesh::VSF_DiffuseUV;
		if( m_usage | SMU_OutputNormal)
			vsf |= Mesh::VSF_Normal;
		if( m_bUsedWithStaticLighting)
			vsf |= Mesh::VSF_LightmapUV;
		if( m_bUsedWithSkeletalMesh)
			vsf |= Mesh::VSF_BoneInfulence;

		return vsf;
	}

	// Set the shadertree path
	void SuperMaterial::SetShaderTreePath( const char* path) 
	{ 
		FileUtils::ReplaceExt( m_shadertreePath, path, ".ast");
	}

	// Generate Shader By ShaderTree
	string SuperMaterial::GenerateShaderByShaderTree( const char* shaderTreePath)
	{
		// 1.shader保存路径
		string  shaderPath; 
		FileUtils::ReplaceExt( shaderPath, shaderTreePath, ".hlsl"); 
		shaderPath = AResSystem.FullPath( shaderPath);
		if( FileUtils::IsFileExist( shaderTreePath))
		{
			string			   resultCode;
			string			   templateShaderCode;

			// 1.读模板
            string fullPath = AResSystem.FullPath( "SuperMaterialTemplate.hlsl");
			ifstream templateIfs( fullPath.c_str());
			if( templateIfs.is_open())
			{
				ostringstream oss; oss<<templateIfs.rdbuf();
				templateShaderCode = oss.str();
			}

			// 2.读材质树
            string shaderTreeFullPath = AResSystem.FullPath( shaderTreePath);
			ifstream shaderTreeIfs( shaderTreeFullPath.c_str());
			if( shaderTreeIfs.is_open())
			{
				MaterialShaderTree* shaderTree=NULL;

				boost::archive::binary_iarchive biarchive( shaderTreeIfs);
				biarchive >> shaderTree;

				shaderTree->Compile( resultCode, templateShaderCode);

				SAFE_DELETE( shaderTree);
			}

			
			ofstream ofs( shaderPath.c_str());
			if( ofs.is_open())
				ofs << resultCode;
		}

		return shaderPath;
	}

	// Get ztest mask texture
	ShaderParamPtr SuperMaterial::GetZTestMaskTexture()
	{
		return GetShaderParam( "DiffuseTex");
	}

	/** Compiles shader for the given platform*/
	void SuperMaterial::Compile()
	{
		if( ARES_EDITOR_MODE)
		{
			// macros
			ShaderMacros macros;
			if( m_usage & SMU_OutputDiffuse)
			{
				macros.push_back( ShaderMacro( "OUTPUT_DIFFUSE", ""));
			}
			if( m_usage & SMU_OutputNormal)
			{
				macros.push_back( ShaderMacro( "OUTPUT_NORMAL", ""));
			}
			if( m_usage & SMU_OutputDepth)
			{
				macros.push_back( ShaderMacro( "OUTPUT_DEPTH", ""));
			}
			if( m_usage & SMU_StaticLighting)
			{
				macros.push_back( ShaderMacro( "USE_LIGHT_MAP", ""));
			}
			if( m_usage & SMU_SkeletalMesh)
			{
				macros.push_back( ShaderMacro( "USE_BONE_ANIMATION", ""));
			}

			// 根据材质树生成Shader
			string shaderPath = GenerateShaderByShaderTree( m_shadertreePath.c_str());

			// Update RenderMethod
            RenderMethodPtr rMethod = AResSystem.LoadMethod( shaderPath, macros, m_usage);
			SetMethod( rMethod);
		}
		else
		{
			// Warning if the flag is missing in game, since compiling shaders in game is not supported on some platforms
			ALogSystem.Warning( "SuperMaterial missing");	
		}
	}
}