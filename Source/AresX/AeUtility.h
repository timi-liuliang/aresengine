#pragma once

#include <max.h>
#include <utilapi.h>

namespace M1_IGame
{
	//---------------------------------------
	// 导出插件 2011-07-20     帝林
	//---------------------------------------
	class CAeUtility : public UtilityObj
	{
	public:
		// 构造函数
		CAeUtility();

		// 析构函数
		~CAeUtility();

		// 未知
		void BeginEditParams(Interface *ip, IUtil *iu);

		// 未知
		void EndEditParams(Interface *ip,IUtil *iu);

		// 删除
		void DeleteThis() {}
	
	private:
		IUtil		*m_iu;
		Interface	*m_ip;
		HWND		 m_panelAbout;		// About窗口
		HWND		 m_panelGeneral;	// 常规面板
		HWND		 m_panelModel;		// 模型面板
		HWND		 m_panelMaterial;	// 材质面板
		HWND		 m_panelAnimation;	// 动画面版
	};
}