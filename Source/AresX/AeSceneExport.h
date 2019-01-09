//#pragma once
//
//#include "max.h"
//
//namespace M1_IGame
//{
//	//---------------------------------------
//	// 导出插件 2010-08-26     帝林
//	//---------------------------------------
//	class CAeSceneExport : public SceneExport
//	{
//	public:
//		// 构造函数
//		CAeSceneExport();
//
//		// 析构函数
//		~CAeSceneExport();
//
//		// 扩展名数量
//		virtual int  ExtCount();
//
//		// 导出文件后缀名(例 "3DS")
//		virtual const char* Ext(int n);		
//
//		// 详细描述( 例： "Autodesk 3D Studio File")
//		virtual const char*	LongDesc();			
//
//		// 简短描述
//		virtual const char*	ShortDesc();
//
//		// 作者名
//		virtual const char*	AuthorName();
//
//		// 版权信息
//		virtual const char*	CopyrightMessage();		
//
//		// 额外信息1
//		virtual const char*	OtherMessage1();
//
//		// 额外信息2
//		virtual const char*	OtherMessage2();	
//
//		// 版本号( 例. v3.01 = 301)
//		virtual UINT Version();
//
//		// 显示窗口Show DLL's "About..." box
//		virtual void ShowAbout(HWND hWnd);
//
//		// 执行导出
//		virtual int	DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);
//	};
//}