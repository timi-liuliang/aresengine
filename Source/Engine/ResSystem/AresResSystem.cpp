#include <string>
#include <Engine/ResSystem/AresResSystem.h>
#include <Engine/AresRoot.h>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/filesystem.hpp>
#include <boost/functional/hash.hpp>
//#include //<Core/3rdParty/RapidXml/rapidxml.hpp>
//#include //<Core/3rdParty/RapidXml/rapidxml_utils.hpp>
#include <Engine/RenderSystem/AresTerrain.h>
#include <Core/AresFileUtils.h>

using namespace std;
using namespace boost;
//using namespace rapidxml;

namespace Ares
{
	// 构造函数
	ResSystem::ResSystem( const ResPaths& paths)
	{
        m_fileUtils = FileUtils::Create();
        
	#ifdef ARES_PLATFORM_WINDOWS
		char buf[MAX_PATH];
		GetModuleFileNameA( NULL, buf, sizeof(buf));
		m_exePath = buf;
		m_exePath = m_exePath.substr( 0, m_exePath.rfind("\\"));
	#endif

		for( size_t i=0; i<paths.size(); i++)
			AddPath( paths[i]);
	}

	// 析构函数
	ResSystem::~ResSystem()
	{

	}

	// 添加路径
	void ResSystem::AddPath( const string& path)
	{
        m_fileUtils->AddSearchPath( path.c_str());
        
		/*filesystem::path newPath( path);
		if( !newPath.is_complete())
		{
			filesystem::path fullPath = m_exePath / newPath;
			if( !FileUtils::IsFileExist( fullPath.string()))
			{
				fullPath = filesystem::current_path() / newPath;
				if( !FileUtils::IsFileExist( fullPath.string()))
					return;
			}

			newPath = fullPath;
		}

		string pathStr = newPath.string();
		if( pathStr[pathStr.length()-1] != '/' && pathStr[pathStr.length()-1] != '\\')
			pathStr.push_back( '/');

		m_pathes.push_back( pathStr);*/
	}

	// 移除路径
	void ResSystem::RemovePath( const string& path)
	{

	}

	// 求全路径
	string ResSystem::FullPath( const string& name)
	{
        string fullPath = m_fileUtils->FullPath( name.c_str());
        
		/*if( FileUtils::IsFileExist( name))
			return name;
		else
		{
			for ( size_t i=0; i<m_pathes.size(); i++)
			{
				string resPath = m_pathes[i] + name;
				if( FileUtils::IsFileExist( resPath))
					return resPath;
			}
		}*/

        // 警告
        if( !fullPath.length())
            ALogSystem.Warning( "%s not found!", name.c_str());

		return fullPath;
	}

	// 加载模型实体文件 *.m1
	ModelPtr  ResSystem::LoadModel( const char* filePath)
	{
		string fullPath = FullPath( filePath);

		Model::IDCard* idcard = NULL;
		Serialization::UnSerializeFromBinaryFile( fullPath.c_str(), idcard);

		ModelPtr model = ModelPtr( new_ Model);
		model->SetFilePath( fullPath.c_str());
		idcard->RebuildModel( *model);

		return model;
	}

	// 加载mod文件 *.mod
	MeshPtr ResSystem::LoadMesh( const char* filePath, int index)
	{
		static Mesh::IDCard serializationPtr;

		if( filePath)
		{
			string meshName = filePath + boost::lexical_cast<string>(index);
			map<string, MeshPtr>::iterator it = m_meshPool.find( meshName);
			if( it==m_meshPool.end())
			{
				vector<Mesh::IDCard*> meshList;
				Serialization::UnSerializeFromBinaryFile( FullPath( filePath).c_str(), meshList);
				for( size_t i=0; i<meshList.size(); i++)
				{
					string subMeshName = filePath + boost::lexical_cast<string>(i);

					MeshPtr mesh = MeshPtr( new_ Mesh);
					meshList[i]->ReBuildMesh( *mesh);

					m_meshPool[subMeshName] = mesh;
				}

				return LoadMesh( filePath, index);
			}
			else
				return it->second;
		}

		return MeshPtr();
	}

	// 加载材质
	SuperMaterialPtr ResSystem::LoadMaterialParameter( SuperMaterialPtr& material, const char* filePath, int index, QWORD usage)
	{
		if( filePath)
		{
			string materialName = filePath + boost::lexical_cast<string>(index);
			map<string, SuperMaterial::IDCardPtr>::iterator it = m_materialPool.find( materialName);
			if( it==m_materialPool.end())
			{
				vector<SuperMaterial::IDCard*> materialList;
				Serialization::UnSerializeFromBinaryFile( FullPath(filePath).c_str(), materialList);
				for( size_t i=0; i<materialList.size(); i++)
				{
					string subMeshName = filePath + boost::lexical_cast<string>(i);

					m_materialPool[subMeshName] = SuperMaterial::IDCardPtr( materialList[i]);
				}

				return LoadMaterialParameter( material, filePath, index, usage);
			}
			else
			{
				it->second->ReBuildSuperMaterial( *material, usage);

				return material;
			}
		}

		return SuperMaterialPtr();
	}

	// 加载动画文件
	s_ptr<CAresAnimation> ResSystem::LoadAnimation( const char* filePath)
	{
		if( filePath)
		{
			map<string, s_ptr<CAresAnimation> >::iterator it = m_m1Animations.find( filePath);
			if( it==m_m1Animations.end())
			{
				s_ptr<CAresAnimation> m1Animation = s_ptr<CAresAnimation>( new_ CAresAnimation);

				A_ASSERT( m1Animation.get());

				m1Animation->Load( filePath);
				m_m1Animations[filePath] = m1Animation;

				return m1Animation;
			}

			return it->second;
		}

		return s_ptr<CAresAnimation>();
	}

	// 加载渲染方法
	RenderMethodPtr ResSystem::LoadMethod( string fileName, const ShaderMacros& macros, QWORD guid)
	{
		// compute hash seed
		size_t seed = 0;
		boost::hash_combine( seed, fileName);
		boost::hash_combine( seed, guid);
		foreach_( const ShaderMacro& macro, macros)
		{
			boost::hash_combine( seed, macro.m_name);
			boost::hash_combine( seed, macro.m_definition);
		}

		filesystem::path tPath( fileName);
		//string filePath = tPath.filename().string() + "_" + boost::lexical_cast<string>(guid) + tPath.extension().string();
		map<size_t, RenderMethodPtr>::iterator it = m_methodPool.find( seed);
		if( it==m_methodPool.end())
		{
			RenderMethodPtr method = ARenderDevice->CreateRenderMethod();
			if( ARenderDevice->GetName()==string( "D3D11"))
				tPath.replace_extension( ".hlsl");
			else /*if(ARenderDevice->GetName()==string("OGL"))*/
				tPath.replace_extension( ".glsl");
						
			if( method->Load( FullPath( tPath.string()).c_str(), macros))
				m_methodPool[seed] = method;

			return method;
		}

		return it->second;
	}

	// 加载脚本
	LuaScriptPtr ResSystem::LoadLuaScript( const char* filePath)
	{
		//map<string, LuaScriptPtr>::iterator it = m_luaScriptPool.find( filePath);
		//if( it==m_luaScriptPool.end())
		//{
		//	LuaScriptPtr script = LuaScriptPtr( new_ LuaScript( FullPath(filePath).c_str()));
		//	if( script)
		//		m_luaScriptPool[filePath] = script;

		//	return script;
		//}

		return LuaScriptPtr( new_ LuaScript( FullPath(filePath).c_str()));
	}

	// 获取自动绑定shader参数
	void ResSystem::AutoBindParam( RenderMaterialPtr& material, const RenderEntity* renderEntity)
	{
		for ( size_t i=0; i<material->GetParamNum(); i++)
		{
			const string& spabDesc = material->GetParamAutoBind( i);
			if( spabDesc.substr( 0, 5) == "SPAB_")
			{
				if( spabDesc == "SPAB_WorldViewProj")
					material->SetParamRef( i, renderEntity->GetSPAB( RenderEntity::SP_WorldViewProj));
			}
		}
	}

	// 同步加载纹理
	TexturePtr ResSystem::SyncLoadDDS( const char* texPath, UINT accessHint)
	{
		map<string, TexturePtr>::iterator it = m_texturePool.find( texPath);
		if( it == m_texturePool.end())
		{
			TexturePtr result;
			Texture::TextureType	type;
			UINT				    width;
			UINT				    height;
			UINT				    depth;
			UINT				    numMipMaps;
			UINT				    arraySize;
			ElementFormat		    format;
			vector<ElementInitData> initData;
			vector<BYTE>			dataBlock;
			if( Texture::LoadTexture( FullPath(texPath).c_str(), type, width, height, depth, numMipMaps, arraySize, format, initData, dataBlock))
			{
				const RenderDeviceCaps& caps = ARenderDevice->GetDeviceCaps();
				if( caps.IsTextureFormatSupport( format))
				{
					switch( type)
					{
					case Texture::TT_1D:
						break;

					case Texture::TT_2D:
						result = ARenderDevice->CreateTexture2D( width, height, numMipMaps, arraySize, format, 1, 0, accessHint, initData.data());
						break;

					case Texture::TT_3D:
						break;

					case Texture::TT_Cube:
						break;
					}

					if( result)
					{	
						result->SetResPath( texPath);
						m_texturePool[texPath] = result;
					}
				}
			}

			return result;
		}

		return it->second;
	}

	// 新建方向光
	LightPtr ResSystem::MakeLight( LightType type)
	{
		switch( type)
		{
		case LT_Direction:
			return MakeSharePtr( new_ DirectionalLight);

		case LT_Point:
			return MakeSharePtr( new_ PointLight);

		case LT_Ambient:
			return MakeSharePtr( new_ AmbientLight);
		}
	
		return LightPtr();
	}

	// 同步保存纹理
	void ResSystem::SyncSaveTexture( const TexturePtr& texture, const char* filePath, ElementFormat destFmt)
	{
		ElementFormat format	 = destFmt!=EF_UnKnown ? destFmt : texture->GetFormat();
		UINT		  numMipmaps = texture->GetNumMipMaps();
		UINT		  arraySize  = texture->GetArraySize();

		TexturePtr textureSysMem;
		switch( texture->GetType())
		{
		case Texture::TT_2D:
			{
				textureSysMem = ARenderDevice->CreateTexture2D( texture->GetWidth(0), texture->GetHeight(0), numMipmaps, arraySize, format, 1, 0, EAH_CPURead, NULL);
			}
			break;
		
		default:
			{
				A_ASSERT( false);
			}
			break;
		}

		texture->CopyToTexure( *textureSysMem);

		UINT formatSize    = NumFormatBytes( format);
		UINT mainImageSize = textureSysMem->GetWidth(0) * textureSysMem->GetHeight(0)*formatSize;
		if( IsCompressedFormat( format))
		{
			if( EF_BC1==format || EF_SIGNED_BC1==format || EF_BC1_SRGB==format || EF_BC4==format || EF_SIGNED_BC4==format || EF_BC4_SRGB==format)
				mainImageSize = textureSysMem->GetWidth(0) * textureSysMem->GetHeight(0) / 2;
			else
				mainImageSize = textureSysMem->GetWidth(0) * textureSysMem->GetHeight(0);
		}

		std::vector<ElementInitData> initData;
		std::vector<size_t>			 base;
		std::vector<BYTE>			 dataBlock;
		switch( textureSysMem->GetType())
		{
		case Texture::TT_2D:
			{
				initData.resize( arraySize * numMipmaps);
				base.resize( arraySize * numMipmaps);
				for( UINT arrayIndex = 0; arrayIndex<arraySize; arrayIndex++)
				{
					for( UINT level=0; level<numMipmaps; level++)
					{
						UINT index = arrayIndex * numMipmaps + level;
						UINT width = textureSysMem->GetWidth( level);
						UINT height= textureSysMem->GetHeight( level);

						if( IsCompressedFormat( format))
						{
							int blockSize;
							if( EF_BC1==format || EF_SIGNED_BC1==format || EF_BC1_SRGB==format || EF_BC4==format || EF_SIGNED_BC4==format || EF_BC4_SRGB==format)
								blockSize = 8;
							else
								blockSize = 16;

							UINT imageSize = ( (width+3)/4*(height+3)/4) * blockSize;
							{
								Texture::Modifier modifier( *textureSysMem, TMA_ReadOnly, arrayIndex, level, 0, 0, width, height);			
								BYTE* data = modifier.GetPointer<BYTE>(); 
								base[index] = dataBlock.size();
								dataBlock.resize( dataBlock.size() + imageSize);
								for ( UINT y=0; y<(height+3)/4; y++)
								{
									std::memcpy( &dataBlock[base[index]+y*((width+3)/4)*blockSize], data, ((width+3)/4)*blockSize);
									data += modifier.GetRowPitch();
								}
							}	
						}
						else
						{
							Texture::Modifier modifie( *textureSysMem, TMA_ReadOnly, arrayIndex, level, 0, 0, width, height);
							BYTE* data = modifie.GetPointer<BYTE>();
							base[index]= dataBlock.size();
							dataBlock.resize( dataBlock.size() + width * height * formatSize);
							for ( UINT y=0; y<height; y++)
							{
								std::memcpy( &dataBlock[base[index]+y*width*formatSize], data, width*formatSize);
								data += modifie.GetRowPitch();
							}
						}
					}						
				}
			}
			break;
		}

		for ( size_t i=0; i<base.size(); i++)
		{
			initData[i].m_data = &dataBlock[base[i]];
		}

		Texture::SaveTexture( filePath, textureSysMem->GetType(), textureSysMem->GetWidth(0), textureSysMem->GetHeight(0), textureSysMem->GetDepth(0), numMipmaps, arraySize, format, initData);
	}

	// 新建纹理集
	TextureSet* ResSystem::CreateTextureSet( TexturePtr& texture)
	{
		return new_ TextureSet( texture);
	}

	// 清理(Garbage clear)
	UINT ResSystem::Clear()
	{
		return 0;
	}
}