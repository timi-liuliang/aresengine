#include "AeUtility.h"
#include "resource.h"
#include "AeUtilityUI.h"
#include "AeConfig.h"
#include <notify.h>
#include <IGame/IGame.h>

Interface*   g_pI = NULL;

extern HINSTANCE g_hInstance;

namespace M1_IGame
{
	SExportConfig	  g_config;				// 导出配置
	CAeUtilityUI*	  g_utilityUI = NULL;

	extern IGameScene *  g_pIGame;

	// About消息
	static INT_PTR CALLBACK AresXAboutProc(HWND hWnd, UINT msg,WPARAM wParam, LPARAM lParam)
	{
		switch (msg) 
		{
		case WM_INITDIALOG:
			break;
		case WM_DESTROY:
			break;
		case WM_COMMAND:
			{
				switch( LOWORD( wParam))
				{
				case IDC_LOGO_SHOW:
					{

					}
					break;
				}
			}
			break;
		default:
			return FALSE;
		}

		return TRUE;
	}

	static INT_PTR CALLBACK AresXGeneralProc( HWND hWnd, UINT msg,WPARAM wParam, LPARAM lParam)
	{
		switch (msg) 
		{
		case WM_INITDIALOG:
			g_utilityUI->InitGeneralPanel( hWnd);
			break;
		case WM_DESTROY:
			break;
		case WM_COMMAND:
			{
				switch( LOWORD( wParam))
				{
				case IDC_BUTTON3:
					{
						// 获取界面配置信息
						g_utilityUI->SetExportDir( hWnd);
					}
					break;
				case IDC_BUTTON1:
					{
						g_utilityUI->DoExport();
						//g_pExportLogic->DoExport();
					}
					break;
				case IDC_BUTTON_VIEW:
					{
						g_utilityUI->PreViewModel();
					}
					break;
				}
			}
			break;
		default:
			return FALSE;
		}

		return TRUE;
	}

	//  模型面板消息
	static INT_PTR CALLBACK AresXModelProc(HWND hWnd, UINT msg,WPARAM wParam, LPARAM lParam)
	{
		switch (msg) 
		{
		case WM_INITDIALOG:
			g_utilityUI->InitModelPanel( hWnd);
			break;
		case WM_DESTROY:
			break;
		case WM_COMMAND:
			{
				switch( LOWORD( wParam))
				{
				case IDC_EDIT1:
					{
						switch (HIWORD(wParam)) 
						{
						case EN_SETFOCUS:
							DisableAccelerators();
							break;
						case EN_KILLFOCUS:
							EnableAccelerators();
							break;
						}
					}
					break;
				}
			}
			break;
		default:
			return FALSE;
		}

		return TRUE;
	}

	// 材质面板消息
	static INT_PTR CALLBACK AresXMaterialProc(HWND hWnd, UINT msg,WPARAM wParam, LPARAM lParam)
	{
		switch (msg) 
		{
		case WM_INITDIALOG:
			g_utilityUI->InitMaterialPanel( hWnd);
			break;
		case WM_DESTROY:
			break;
		default:
			return FALSE;
		}

		return TRUE;
	}

	// 动画面板消息
	static INT_PTR CALLBACK AresXAnimationProc(HWND hWnd, UINT msg,WPARAM wParam, LPARAM lParam)
	{
		switch (msg) 
		{
		case WM_INITDIALOG:
			{
				g_utilityUI->InitAnimationPanel( hWnd);
			}
			break;
		case WM_DESTROY:
			break;
		case WM_COMMAND:
			{
				switch( LOWORD( wParam))
				{
				case IDC_BUTTON2:
					{
						// 获取界面配置信息
						g_utilityUI->AddAnimation( hWnd);
					}
					break;

				case IDC_BUTTON7:
					{
						// 删除动画
						g_utilityUI->DelAnimation( hWnd);
					}
					break;
				case IDC_EDIT1:
				case IDC_EDIT3:
				case IDC_EDIT6:
				case IDC_EDIT7:
					{
						switch (HIWORD(wParam)) 
						{
						case EN_SETFOCUS:
							DisableAccelerators();
							break;
						case EN_KILLFOCUS:
							EnableAccelerators();
							break;
						}
					}
					break;
				}
			}
			break;
		default:
			return FALSE;
		}

		return TRUE;
	}

	// 打开新的模型
	static void MaxFileOpenProc(void* param, NotifyInfo* pInfo)
	{
		// 更改导出动画处数据		
		Interval interval = g_pI->GetAnimRange();

		char cBegin[256];
		char cEnd[256];

		itoa( interval.Start() / GetTicksPerFrame(), cBegin, 10);
		itoa( interval.End() / GetTicksPerFrame(),   cEnd,	10);

		g_utilityUI->SetAnimationRange( cBegin, cEnd);
	}
	
	// 动画面板消息
	// 打开新的模型
	static void MaxPreFileOpenProc(void* param, NotifyInfo* pInfo)
	{
		int a = 10;
	}

	// 构造函数
	CAeUtility::CAeUtility()
		: UtilityObj()
	{
		m_ip = NULL;
		m_iu = NULL;

		m_panelAbout    = 0;
		m_panelGeneral  = 0;
		m_panelModel    = 0;
		m_panelMaterial = 0;
		m_panelAnimation= 0;

		g_utilityUI = new CAeUtilityUI;
	}

	// 析构函数
	CAeUtility::~CAeUtility()
	{

	}

	// 未知
	void CAeUtility::BeginEditParams(Interface *ip, IUtil *iu)
	{
		RegisterNotification(MaxFileOpenProc,    this, NOTIFY_FILE_POST_OPEN);
		RegisterNotification(MaxPreFileOpenProc, this, NOTIFY_FILE_PRE_OPEN);

		m_iu = iu;
		m_ip = ip;
		g_pI = ip;

		// AresX about
 		m_panelAbout     = ip->AddRollupPage( g_hInstance, MAKEINTRESOURCE( IDD_ARESX_ABOUT),    AresXAboutProc, _T("About"), 0);

		// General
		m_panelGeneral	 = ip->AddRollupPage( g_hInstance, MAKEINTRESOURCE( IDD_ARESX_GENERAL),  AresXGeneralProc, _T("General"), 0, APPENDROLL_CLOSED);

		// Model
		m_panelModel     = ip->AddRollupPage( g_hInstance, MAKEINTRESOURCE( IDD_ARESX_MODEL),    AresXModelProc, _T("Model"), 0, APPENDROLL_CLOSED);

		// Material
		m_panelMaterial  = ip->AddRollupPage( g_hInstance, MAKEINTRESOURCE( IDD_ARESX_Material), AresXMaterialProc, _T("Material"), 0, APPENDROLL_CLOSED);

		// Animation
		m_panelAnimation = ip->AddRollupPage( g_hInstance, MAKEINTRESOURCE(IDD_ARESX_ANIMATION), AresXAnimationProc, _T("Animation"));

		int a = 10;
	}

	// 未知
	void CAeUtility::EndEditParams(Interface *ip,IUtil *iu)
	{	
		UnRegisterNotification(MaxFileOpenProc,    this, NOTIFY_FILE_POST_OPEN);
		UnRegisterNotification(MaxPreFileOpenProc, this, NOTIFY_FILE_PRE_OPEN);
		//iu->CloseUtility();

		ip->DeleteRollupPage( m_panelAnimation);
		ip->DeleteRollupPage( m_panelGeneral);
		ip->DeleteRollupPage( m_panelMaterial);
		ip->DeleteRollupPage( m_panelModel);
		ip->DeleteRollupPage( m_panelAbout);

		m_iu = NULL;
		m_ip = NULL;

		m_panelAbout	 = NULL;
		m_panelModel	 = NULL;
		m_panelMaterial	 = NULL;
		m_panelAnimation = NULL;
	}

}