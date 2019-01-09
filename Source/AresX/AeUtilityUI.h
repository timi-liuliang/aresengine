#pragma once

#include <max.h>
#include <utilapi.h>
#include "M1Export.h"

namespace M1_IGame
{
	//---------------------------------------
	// 导出插件 2011-07-22     帝林
	//---------------------------------------
	class CAeUtilityUI
	{
	public:
		// 构造函数
		CAeUtilityUI();

		// 常规面板
	public:
		// 初始化
		void InitGeneralPanel( HWND hwnd);

		// 选择导出路径
		void SetExportDir( HWND hwnd);

		// 执行常规导出
		void DoExport();

		// 预览模型
		void PreViewModel();

		// 更新配置
		void UpdateConfig();

		// 模型面板
	public:
		// 初始化
		void InitModelPanel( HWND hwnd);

		// 材质面板
	public:
		// 初始化
		void InitMaterialPanel( HWND hwnd);
		
		// 动画面板
	public:
		// 初始化
		void InitAnimationPanel( HWND hwnd);

		// 更新界面显示
		void UpdateAnimationPanelShow();

		// 添加动画
		void AddAnimation( HWND hwnd);

		// 删除动画
		void DelAnimation( HWND hwnd);

	public:
		// 设置动画时间范围
		void SetAnimationRange( const char* begin, const char* end);

	private:
		HWND		 m_panelAbout;		// About窗口
		HWND		 m_panelGeneral;	// 常规面板
		HWND		 m_panelModel;		// 模型面板
		HWND		 m_panelMaterial;	// 材质面板
		HWND		 m_panelAnimation;	// 动画面版
	};
}