#pragma once

#include <QtGui/QDialog>
#include "ui_VideoSetting.h"

namespace QT_UI
{
	//----------------------------------------
	// 模型编辑器 主窗口 2010-12-30
	//----------------------------------------
	class CVideoSetting : public QDialog, public Ui_VideoSetting
	{
		Q_OBJECT

	public:
		CVideoSetting( QWidget* parent = 0);

	public slots:
		// 设置fps
		void  SetFps(float fps);

		// 更新摄像机设置
		void ApplyCameraSettings();

		// 更新最近打开文件列表显示
		void UpdateCameraSettingsDisplay();

	private slots:
		// 摄像机参数界面控制改变
		void SetCameraSettings();

		// 更新VideoFillMode
		void  UpdateFillMode();

		// 设置alpha测试值
		void SetAlphaTestValue( int value);

		// 设定Axis缩放
		void  SetAxisScale(  double scale);

		// 设置阴影SoftNess
		void  SetShadowSampleSize( int size);

		// 设置阴影SoftNess
		void  SetShadowLBR( double lbr);

		// 设置阴影是否进行模糊
		void SetBlur( bool blur);

		// 是否开启阴影
		void SetShadow( bool bShadow);

		// 是否显示胶囊
		void SetCapsuleVisible( bool visible);

		// 设置FXAA
		void UpdateFXAA();

		// 设置色调映射
		void SetToneMapping();
		
	public slots:
		// 显示阴影 Foucs region B( 找Bug临时用)
		void ShowFocusRegionB();
	};
}