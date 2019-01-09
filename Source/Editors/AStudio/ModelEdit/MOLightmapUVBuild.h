#pragma once

#include <string>
#include <d3dx9.h>
#include <Core/AresMath.h>
#include <Physics/Rect3.h>
#include <Engine/RenderSystem/AresModelIDCard.h>
#include <boost/filesystem.hpp>

using namespace boost;
using namespace boost::filesystem;

namespace Ares
{
	//---------------------------------------
	// BuildSettings 2012-7-10 帝林
	//---------------------------------------
	struct BuildSettings
	{
		int				m_maxCharts;
		int				m_width;
		int				m_height;
		float			m_maxStretch;
		float			m_gutter;
		BYTE			m_outputTextureIndex;
		bool			m_topologicalAdjacency;
		bool			m_geometricAdjacency;
		bool			m_falseEdges;
		bool			m_fileAdjacency;
		string			m_szAdjacencyFilename;
		string			m_szFalseEdgesFilename;

		bool			m_userAbort;
		bool			m_subDirs;
		bool			m_colorMesh;
		bool			m_verBose;
		bool			m_IMT;
		bool			m_textureSignal;
		bool			m_PRTSignal;
		bool			m_vertexSignal;
		DWORD			m_qualityFlag;
		D3DDECLUSAGE	m_vertexSignalUsage;
		BYTE			m_vertexSignalIndex;
		BYTE			m_IMTInputTextureIndex;
		string			m_szIMTInputFilename;

		// 构造函数
		BuildSettings()
		{
			m_userAbort				= false;
			m_subDirs				= false;
			m_topologicalAdjacency	= false;
			m_geometricAdjacency	= false;
			m_fileAdjacency			= false;
			m_qualityFlag			= D3DXUVATLAS_DEFAULT;

			m_falseEdges			= false;
			m_maxCharts				= 0;
			m_maxStretch			= 1 / 2.f;
			m_gutter				= 4.f;
			m_width					= 512;
			m_height				= 512;
			m_colorMesh				= false;
			m_outputTextureIndex	= 0;

			m_IMT					= false;
			m_textureSignal			= false;
			m_PRTSignal				= false;
			m_vertexSignal			= false;
			m_vertexSignalUsage		= D3DDECLUSAGE_COLOR;
			m_vertexSignalIndex		= 0;
		}
	};

	//--------------------------------------
	// 光照图UV生成管理器 2012-7-10 帝林
	//--------------------------------------
	class LightmapUVBuild
	{
	public:
		// 构造函数
		LightmapUVBuild();

		// 指定文件
		void Open( const char* filePath);

		// 保存文件
		void Save( const char* filePath = NULL);

		// 执行生成( *mod, *.卞安)
		void Build( const BuildSettings& config);

		// 还原
		void Restore();

		// 获取结果
		void OrganizationResult( vector<Int3>& rIndices, vector<Vector2>& rUvs, Rect2& rUvRect, vector<DWORD>& rFacePartitioning, vector<DWORD>& rVertexRemapArray, ID3DXMesh* inMesh, LPD3DXBUFFER pFacePartitioning, LPD3DXBUFFER pVertexRemapArray);

		// 修改Mesh
		void ModifyMesh( vector<Int3>& rIndices, vector<Vector2>& rUvs, Rect2& rUvRect, vector<DWORD>& rFacePartitioning, vector<DWORD>& rVertexRemapArray, Mesh::IDCard& mesh);

		// 显示结果
		void ShowResult( const vector<Int3>& rIndices, const vector<Vector2>& rUvs);

	private:
		// 构建Mod
		void BuildMod();

		// 创建D3D9Device
		void CreateNullRefDevice();

		// 获取UV对应
		int VertexMapping(  int idx, Mesh::IDCard& mesh);

		// 备份文件
		void BackupMod();
		
		// 判断是否有源数据
		bool IsOrigMod();

	private:
		path					m_filePath;			// 要生成光照图uv的文件
		BuildSettings			m_settings;			// 生成配置选项
		IDirect3DDevice9*		m_device;			// 无奈呀(fuck ms)
		vector<Mesh::IDCard*>	m_meshs;			// 模型
	};

	extern LightmapUVBuild*	g_lightmapUVBuild;
	#define AUVBuild		g_lightmapUVBuild
}