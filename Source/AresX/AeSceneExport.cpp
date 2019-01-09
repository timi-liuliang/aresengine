//#include "AeSceneExport.h"
//#include "resource.h"
//#include "AeConfig.h"
//
//extern Interface*   g_pI;
//extern HINSTANCE    g_hInstance;
//
//namespace M1_IGame
//{
//	extern SExportConfig	  g_config;				// 导出配置
//
//	// 静态窗口消息处理函数
//	static INT_PTR CALLBACK ExportDlgProc(HWND hWnd, UINT msg,WPARAM wParam, LPARAM lParam)
//	{
//		switch ( msg)
//		{
//			// 初始化窗口
//		case WM_INITDIALOG:
//			{
//				// 放到Max窗口中心
//				CenterWindow( hWnd, GetParent(hWnd));
//			}
//			break;
//
//		case WM_COMMAND:
//			{
//				switch( LOWORD( wParam))
//				{
//				case IDOK:
//					{
//						// 获取界面配置信息
//						//g_config.binary = IsDlgButtonChecked( hWnd, IDC_CHECK_BINARY);
//
//						//g_pExportLogic->DoExport();
//						
//						EndDialog( hWnd, TRUE);
//					}
//					break;
//				case IDCANCEL:
//					EndDialog( hWnd, NULL);
//					break;
//				}
//			}
//			break;
//
//		case WM_KEYDOWN:
//			{
//				int a =4;
//			}
//			break;;
//
//		case BM_CLICK:
//			{
//				int a = 4;
//			}
//			break;
//
//		default:
//			return FALSE;
//		}
//
//		return TRUE;
//	}
//
//	// 构造函数
//	CAeSceneExport::CAeSceneExport()
//	{
//	}
//
//	// 析构函数
//	CAeSceneExport::~CAeSceneExport()
//	{
//
//	}
//
//	// 扩展名数量
//	int CAeSceneExport::ExtCount()
//	{
//		return 1;
//	}
//
//	// 导出文件后缀名(例 "3DS")
//	const char* CAeSceneExport::Ext(int n)
//	{
//		return "m1";
//	}
//
//	// 详细描述( 例： "Autodesk 3D Studio File")
//	const char*	CAeSceneExport::LongDesc()
//	{
//		return "AresEngine model File";
//	}
//
//	// 简短描述
//	const char*	CAeSceneExport::ShortDesc()
//	{
//		return "Ae Export(AresEngine)";
//	}
//
//	// 作者名
//	const char*	CAeSceneExport::AuthorName()
//	{
//		return "帝林";
//	}
//
//	// 版权信息
//	const char*	CAeSceneExport::CopyrightMessage()
//	{
//		return "2010 趣游天际";
//	}
//
//	// 额外信息1
//	const char*	CAeSceneExport::OtherMessage1()
//	{
//		return "Ares";
//	}
//
//	// 额外信息2
//	const char*	CAeSceneExport::OtherMessage2()
//	{
//		return " other message 2";
//	}
//
//	// 版本号( 例. v3.01 = 301)
//	UINT CAeSceneExport::Version()
//	{
//		return 001;
//	}
//
//	// 显示窗口Show DLL's "About..." box
//	void CAeSceneExport::ShowAbout(HWND hWnd)
//	{
//		// do nothing
//	}
//
//	// 执行导出
//	int	CAeSceneExport::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts/*=FALSE*/, DWORD options/*=0*/)
//	{
//		// 设置变量
//		g_pI = i;
//		g_config.m_m1Location = name;
//
//		// 显示导出界面
//		if ( !DialogBoxParam( g_hInstance, MAKEINTRESOURCE( IDD_MAIN_DIALOG), i->GetMAXHWnd(), ExportDlgProc, (LPARAM)this))
//		{
//			return 1;
//		}
//
//		return 1;
//	}
//}