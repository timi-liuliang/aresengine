#include "IGame_MtlKeeper.h"
#include <string>
#include "AeConfig.h"

using namespace std;

extern Interface*   g_pI;

namespace M1_IGame
{
	extern SExportConfig  g_config;	// 导出配置

	// 从全局路径中拆分出纹理名,可更改后缀名
	static string SplitNameExt( const char* fullPath, const char* aimExt)
	{
		// 去掉绝对路径
		char  fileName[128];
		char  fileExt[20];
		char  fileNameAndExt[148];

		_splitpath( fullPath, NULL, NULL, fileName, fileExt);

		if ( aimExt) sprintf( fileNameAndExt, "%s%s", fileName, aimExt);
		else		 sprintf( fileNameAndExt, "%s%s", fileName, fileExt);

		return fileNameAndExt;
	}

	// 更改文件名
	static string ChangeName( const char* source, const char* Name)
	{
		// 去掉绝对路径
		char  fileDiv[128];
		char  fileDir[2048];
		char  fileNew[2048];

		_splitpath( source, fileDiv, fileDir, NULL, NULL);

		sprintf( fileNew, "%s%s%s", fileDiv, fileDir, Name);

		return fileNew;
	}

	// 获取材质ID
	int CMtlKeeper::GetMtlID( IGameMaterial* pMtl)
	{
		if( pMtl)
		{
			size_t numMtls = m_mtlList.size();
			for ( size_t i=0; i<numMtls; i++)
			{
				if( m_mtlList[i] == pMtl) 
					return i;
			}

			m_mtlList.push_back( pMtl);
			return m_mtlList.size()-1;
		}

		return 0;
	}

	// 构建材质
	void CMtlKeeper::BuildMtls( CM1Material& M1)
	{
		M1.m_header.m_iId        = 20100925;
		M1.m_header.m_iVersion   = 15;
		M1.m_header.m_iNumMtls = m_mtlList.size();

		M1.m_pMtls = new SM1Material[M1.m_header.m_iNumMtls];
		for ( size_t i=0; i<M1.m_header.m_iNumMtls; i++)
		{
			IGameTextureMap* pTexMap = m_mtlList[i]->GetIGameTextureMap(0);
			if( pTexMap)
			{
				char* texName = pTexMap->GetBitmapFileName();
				char* texPath = g_pI->GetCurFilePath();
				string fileNameAndExt;
				string tFileNameExt = SplitNameExt( texName, NULL);

				fileNameAndExt = SplitNameExt( texName, NULL);

				if ( fileNameAndExt.length() < 68)
				{
					M1.m_pMtls[i].m_iTexNum = 1;

					M1.m_pMtls[i].m_pTextures = new SM1TexName[1];
					strcpy( M1.m_pMtls[i].m_pTextures[0].m_cName, fileNameAndExt.c_str());

					// Copy 到新目录下( 并转换格式)
					if( g_config.m_bCopyTexture)
					{
						string tTexPath = ChangeName( texPath, tFileNameExt.c_str());

						string texConv;
						CopyTextureToTargetRoot( texConv, tTexPath.c_str(), fileNameAndExt.c_str());

						// 打开注册表
						//string texConv;
						//HKEY rooKey = HKEY_CURRENT_USER;
						//HKEY tKey	= NULL;
						//if( ERROR_SUCCESS == RegOpenKeyExA( rooKey, "Software\\Ares\\ModelEditor", 0, KEY_READ, &tKey))
						//{
						//	// 成功打开,继续操作
						//	char  tValue[1024];
						//	DWORD tLen = 1023;
						//	DWORD tKeyType = REG_SZ;
						//	RegQueryValueEx( tKey, "exePath", NULL, &tKeyType, (LPBYTE)tValue, &tLen);
						//	RegCloseKey( tKey);

						//	texConv = ChangeName( tValue, "texconv.exe");
						//}
						//else
						//{
							//::MessageBoxA( NULL, "读注册表失败,找不到 texconv.exe, 不能转换纹理格式为dds", "AresX Warn", MB_OK);
						//}
					}
				}		
			}
		}

		// 配置图片转换目标格式
		//ConfigTransFormat();
	}

	// 拷贝纹理到目的路径
	void  CMtlKeeper::CopyTextureToTargetRoot( const string& texConv, const char* fileName, const char* aimName, const char* format/*=" DXT1 "*/)
	{
		// 计算一个临时存放位置
		//char tdrive[256];
		//_splitpath( g_config.m_m1Location.c_str(), tdrive, NULL, NULL, NULL);
		//string drive = string( tdrive)+"\\";

		// 拷贝到临时位置, 因命令行程序无法处理空格路径
		//string tempLoc = ChangeName( drive.c_str(), aimName);
		//CopyFile( fileName, tempLoc.c_str(), FALSE);

		// 格式转换
		string targetPath = ChangeName( g_config.m_m1Location.c_str(), aimName);
		CopyFile( fileName, targetPath.c_str(), FALSE);
		//string tCmd = texConv + string(" -ft DDS -f ") + format + tempLoc + string(" -o ") + targetPath;
		//WinExec( tCmd.c_str(), SW_HIDE);

		// 删除临时文件
		//Sleep( 1000);
		//remove( tempLoc.c_str());
	}

	// 配置文件转换格式
	bool CMtlKeeper::ConfigTransFormat()
	{
		return false;
	}
}