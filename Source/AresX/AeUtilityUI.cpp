#include "resource.h"
#include "AeUtilityUI.h"
#include <Windows.h>
#include <string>
#include "AeConfig.h"
#include "IGame_M1ExportManager.h"
#include "IGame_AnimationManager.h"

using namespace std;
using namespace Ares;

extern Interface*   g_pI;

namespace M1_IGame
{
	extern SExportConfig	  g_config;				// 导出配置

	// Dummy function for progress bar
	DWORD WINAPI DProcessBar(LPVOID arg)
	{
		return(0);
	}

	// 更改文件名
	static string ChangeExt( const char* source, const char* ext)
	{
		// 去掉绝对路径
		char  fileDrive[20];
		char  fileDir[2048];
		char  fileName[256];
		char  fileNew[2048];

		_splitpath( source, fileDrive, fileDir, fileName, NULL);

		sprintf( fileNew, "%s%s%s%s", fileDrive, fileDir, fileName, ext);

		return fileNew;
	}

	// 构造函数
	CAeUtilityUI::CAeUtilityUI()
	{
	}

	// 初始化
	void CAeUtilityUI::InitGeneralPanel( HWND hwnd)
	{
		m_panelGeneral = hwnd;

		CheckDlgButton( hwnd, IDC_CHECK1, g_config.m_exportM1);
		CheckDlgButton( hwnd, IDC_CHECK2, g_config.m_exportMod);
		CheckDlgButton( hwnd, IDC_CHECK3, g_config.m_exportMat);
		CheckDlgButton( hwnd, IDC_CHECK4, g_config.m_exportAni);
		CheckDlgButton( hwnd, IDC_CHECK5, g_config.m_exportTri);

		// m1
		SetWindowText( GetDlgItem( hwnd, IDC_EDIT2), g_config.m_m1Location.c_str());

		// model
		SetWindowText( GetDlgItem( hwnd, IDC_EDIT5), g_config.m_modLocation.c_str());

		// material
		SetWindowText( GetDlgItem( hwnd, IDC_EDIT3), g_config.m_matLocation.c_str());

		// animation
		SetWindowText( GetDlgItem( hwnd, IDC_EDIT4), g_config.m_aniLocation.c_str());

		// collided triangle
		SetWindowText( GetDlgItem( hwnd, IDC_EDIT8), g_config.m_triLocation.c_str());
	}

	// 选择导出路径
	void CAeUtilityUI::SetExportDir(HWND hwnd)
	{
		OPENFILENAME ofn;
		memset(&ofn, 0, sizeof(OPENFILENAME));

		char filename[MAX_PATH] = "";
		ofn.lpstrFile		= filename;
		ofn.nMaxFile		= MAX_PATH;

		ofn.lpstrFilter		= "配置文件(*.m1)\0*.m1;";
		ofn.lpstrTitle		= ("保存m1");
		ofn.Flags			= OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
		ofn.lStructSize		= sizeof(OPENFILENAME);

		if( GetSaveFileName( &ofn))
		{	
			g_config.m_m1Location  = ChangeExt( filename, ".m1");
			g_config.m_modLocation = ChangeExt( filename, ".mod");
			g_config.m_matLocation = ChangeExt( filename, ".mat");
			g_config.m_aniLocation = ChangeExt( filename, ".ani");
			g_config.m_triLocation = ChangeExt( filename, ".tri");

			// m1
			SetWindowText( GetDlgItem( hwnd, IDC_EDIT2), g_config.m_m1Location.c_str());

			// model
			SetWindowText( GetDlgItem( hwnd, IDC_EDIT5), g_config.m_modLocation.c_str());

			// material
			SetWindowText( GetDlgItem( hwnd, IDC_EDIT3), g_config.m_matLocation.c_str());

			// animation
			SetWindowText( GetDlgItem( hwnd, IDC_EDIT4), g_config.m_aniLocation.c_str());

			// collided triangle
			SetWindowText( GetDlgItem( hwnd, IDC_EDIT8), g_config.m_triLocation.c_str());

			// 更新动画显示
			UpdateAnimationPanelShow();
		}
	}

	// 执行常规导出
	void CAeUtilityUI::DoExport()
	{
		// start the progress bar
		g_pI->ProgressStart( "AresEngine", TRUE, DProcessBar, NULL);

		// update config
		UpdateConfig();

		SExportM1	      m1Export;
		CM1ExportManager  m1ExportManager;	

		m1ExportManager.Build( m1Export);
		m1Export.Save();

		UpdateAnimationPanelShow();

		g_pI->ProgressEnd();
	}

	// 预览模型
	void CAeUtilityUI::PreViewModel()
	{
		if( !g_config.m_m1Location.empty())
		{
			// 打开注册表
			HKEY rooKey = HKEY_CURRENT_USER;
			HKEY tKey	= NULL;
			if( ERROR_SUCCESS == RegOpenKeyExA( rooKey, "Software\\Ares\\ModelEditor", 0, KEY_READ, &tKey))
			{
				// 成功打开,继续操作
				char  tValue[1024];
				DWORD tLen = 1023;
				DWORD tKeyType = REG_SZ;
				RegQueryValueEx( tKey, "exePath", NULL, &tKeyType, (LPBYTE)tValue, &tLen);
				RegCloseKey( tKey);

				string tCmd = tValue + string(" ") + g_config.m_m1Location;
				WinExec( tCmd.c_str(), SW_NORMAL);
			}
		}
	}

	// 初始化
	void CAeUtilityUI::InitModelPanel( HWND hwnd)
	{
		m_panelModel = hwnd;

		// 过滤
		SetWindowText( GetDlgItem( hwnd, IDC_EDIT1), g_config.m_colTriFilter.c_str());
	}

	// 初始化
	void CAeUtilityUI::InitMaterialPanel( HWND hwnd)
	{
		CheckDlgButton( hwnd, IDC_CHECK1, g_config.m_bCopyTexture);
	}

	// 初始化
	void CAeUtilityUI::InitAnimationPanel( HWND hwnd)
	{
		m_panelAnimation = hwnd;

		UpdateAnimationPanelShow();
	}

	// 更新配置
	void CAeUtilityUI::UpdateConfig()
	{
		// 导出配置
		g_config.m_exportM1  = GetCheckBox( m_panelGeneral, IDC_CHECK1);
		g_config.m_exportMod = GetCheckBox( m_panelGeneral, IDC_CHECK2);
		g_config.m_exportMat = GetCheckBox( m_panelGeneral, IDC_CHECK3);
		g_config.m_exportAni = GetCheckBox( m_panelGeneral, IDC_CHECK4);
		g_config.m_exportTri = GetCheckBox( m_panelGeneral, IDC_CHECK5);

		g_config.m_keyFrame  = GetCheckBox( m_panelAnimation, IDC_CHECK3);

		char tTriFilter[512];
		GetWindowText( GetDlgItem( m_panelModel, IDC_EDIT1), tTriFilter, 512);
		g_config.m_colTriFilter = tTriFilter;
	}

	// 更新界面显示
	void CAeUtilityUI::UpdateAnimationPanelShow()
	{
		// 显示采样or导出关键帧
		CheckDlgButton( m_panelAnimation, IDC_CHECK3, g_config.m_keyFrame);

		// 采样频率
		char tSampleDensity[256];
		itoa( g_config.m_sampledensity, tSampleDensity, 255);
		SetWindowText( GetDlgItem( m_panelAnimation, IDC_EDIT3), tSampleDensity);

		if( g_config.m_aniLocation.empty())
			return;

		// Empty animations list.
		HWND aniList = GetDlgItem( m_panelAnimation, IDC_LISTANI);
		SendMessage( aniList, LB_RESETCONTENT, 0, 0 );

		CM1Animation m1Animation;

		// Fill the animations list.
		if( m1Animation.Load( g_config.m_aniLocation.c_str()))
		{
			// 显示所有动画集到列表
			for ( size_t i=0; i<m1Animation.m_header.m_iNumAnimationSet; i++)
			{
				const SM1AnimationSet& tAniSet = m1Animation.m_pAnimationSets[i];

				SendMessage( aniList, LB_INSERTSTRING, i, (LPARAM)tAniSet.m_header.m_cName );
			}
		}
	}

	// 添加动画
	void CAeUtilityUI::AddAnimation( HWND hwnd)
	{
		if( g_config.m_aniLocation.empty())
			return;

		UpdateConfig();

		CM1Animation m1Animation;

		if( m1Animation.Load( g_config.m_aniLocation.c_str()))
		{
			// name, begin end
			char name[1024];
			char begin[256];
			char end[256];

			GetWindowText( GetDlgItem( hwnd, IDC_EDIT1), name, 1024);
			GetWindowText( GetDlgItem( hwnd, IDC_EDIT6), begin,256);
			GetWindowText( GetDlgItem( hwnd, IDC_EDIT7), end,  256);

			// 是否按关键帧导出
			g_config.m_keyFrame = GetCheckBox( m_panelAnimation, IDC_CHECK3);

			// 采样密度
			char tSampleDensity[256];
			GetWindowText( GetDlgItem( m_panelAnimation, IDC_EDIT2), tSampleDensity, 255);
		    int tSD = atoi(tSampleDensity);
			if( tSD >=1 && tSD<=6000)
				g_config.m_sampledensity = tSD;

			// 合法性检测 
			if( strlen( name) <= 0)
				return;

			int iStart = atoi( begin);
			int iEnd   = atoi( end);

			CAnimationManager tAniMgr;
			if( tAniMgr.AddAnimation( name, iStart, iEnd, m1Animation))
			{
				m1Animation.Save( g_config.m_aniLocation.c_str());

				UpdateAnimationPanelShow();
			}
		}
	}

	// 删除动画
	void CAeUtilityUI::DelAnimation( HWND hwnd)
	{
		if( g_config.m_aniLocation.empty())
			return;

		CM1Animation m1Animation;

		if( m1Animation.Load( g_config.m_aniLocation.c_str()))
		{
			// 获取当前选择
			int curSel;
			HWND aniList = GetDlgItem( m_panelAnimation, IDC_LISTANI);
			curSel = SendMessage( aniList, LB_GETCURSEL, 0, 0);

			if( curSel == -1)
				return;

			CAnimationManager tAniMgr;
			if( tAniMgr.DelAnimation( curSel, m1Animation))
			{
				m1Animation.Save( g_config.m_aniLocation.c_str());

				UpdateAnimationPanelShow();
			}
		}
	}

	// 设置动画时间范围
	void CAeUtilityUI::SetAnimationRange( const char* begin, const char* end)
	{
		SetWindowText( GetDlgItem( m_panelAnimation, IDC_EDIT6), begin);
		SetWindowText( GetDlgItem( m_panelAnimation, IDC_EDIT7), end);
	}
}