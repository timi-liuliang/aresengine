#include <QtGui/QtGui>
#include "VideoSetting.h"
#include <boost/lexical_cast.hpp>
#include "SCLogic.h"

using namespace Ares;

namespace QT_UI
{
	// 构造函数
	CVideoSetting::CVideoSetting( QWidget* parent/* = 0*/)
		: QDialog( parent)
	{
		setupUi( this);

		connect( m_fillSolid,     SIGNAL( toggled(bool)), this, SLOT( UpdateFillMode()));
		connect( m_fillWireFrame, SIGNAL( toggled(bool)), this, SLOT( UpdateFillMode()));
		connect( m_fillPoint,     SIGNAL( toggled(bool)), this, SLOT( UpdateFillMode()));

		connect( m_pAlphaTest, SIGNAL(valueChanged(int)), this, SLOT( SetAlphaTestValue( int)));
		connect( m_cameraSpeed, SIGNAL(valueChanged(double)), this, SLOT( SetCameraSettings()));
		connect( m_cameraNear, SIGNAL(valueChanged(double)), this,  SLOT( SetCameraSettings()));
		connect( m_cameraFar, SIGNAL(valueChanged(double)), this,   SLOT( SetCameraSettings()));
		connect( m_cameraFov, SIGNAL(valueChanged(double)), this,   SLOT( SetCameraSettings()));
		connect( m_pAxisScale, SIGNAL(valueChanged(double)), this, SLOT( SetAxisScale( double)));
		connect( m_pShadowSampleSize, SIGNAL(valueChanged(int)), this, SLOT(SetShadowSampleSize(int)));
		connect( m_pShadowLBR, SIGNAL(valueChanged(double)), this, SLOT(SetShadowLBR(double)));
		connect( m_bBoxBlur, SIGNAL( clicked(bool)), this, SLOT(SetBlur(bool)));
		connect( m_pShadow, SIGNAL(toggled(bool)), this, SLOT( SetShadow( bool)));
		connect( m_showCapsule, SIGNAL(toggled(bool)), this, SLOT( SetCapsuleVisible(bool)));
		//connect( m_focusRegionB, SIGNAL(clicked()), this, SLOT(ShowFocusRegionB()));
		connect( m_confirmSettings,SIGNAL(clicked()), this, SLOT( ApplyCameraSettings()));
		connect( m_applySettings,  SIGNAL(clicked()), this, SLOT( ApplyCameraSettings()));
		connect( m_cancelSettings, SIGNAL(clicked()), this, SLOT( UpdateCameraSettingsDisplay()));
		connect( m_comboBoxFXAA,	   SIGNAL(currentIndexChanged(int)),	this, SLOT(UpdateFXAA()));
		connect( m_comboBoxToneMapping,SIGNAL(currentIndexChanged(int)),	this, SLOT(SetToneMapping()));
	}

	// 更新摄像机设置
	void CVideoSetting::ApplyCameraSettings()
	{
		CameraSettings cameraSettings = Logic_Cfg->GetCameraSettings();
		cameraSettings.m_farPlane = static_cast<float>(m_cameraFar->value());
		cameraSettings.m_speed	  = static_cast<float>(m_cameraSpeed->value());

		Logic_Cfg->SetCameraSettings( cameraSettings);

		// 禁用
		m_applySettings->setEnabled( false);
	}

	// 恢复摄像机设置
	void CVideoSetting::UpdateCameraSettingsDisplay()
	{
		CameraSettings cameraSettings = Logic_Cfg->GetCameraSettings();

		m_cameraFar->setValue( cameraSettings.m_farPlane);
		m_cameraSpeed->setValue( cameraSettings.m_speed);
	}

	// 更新应用按钮状态 (为了减少函数数量,降低了效率)
	void CVideoSetting::SetCameraSettings()
	{
		SCLogic_Camera->SetNearFar( (float)m_cameraNear->value(), (float)m_cameraFar->value());
		SCLogic_Camera->SetFov( DEGtoRAD((float)m_cameraFov->value()));
		SCLogic_Move->GetDefaultProxy()->SetSpeed( (float)m_cameraSpeed->value());

		m_applySettings->setEnabled( true);
	}

	// 设置阴影是否进行模糊
	void CVideoSetting::SetBlur( bool blur)
	{
		SCLogic_Main->SetShadowBlur( blur);
	}

	// 设置阴影SoftNess
	void  CVideoSetting::SetShadowSampleSize( int size)
	{
		SCLogic_Main->SetShadowSampleSize( size);
	}

	// 设置阴影SoftNess
	void  CVideoSetting::SetShadowLBR( double lbr)
	{
		SCLogic_Main->SetShadowLBR((float)lbr);
	}

	// 更新VideoFillMode
	void CVideoSetting::UpdateFillMode()
	{
		// 设置填充模式
		//if( m_fillSolid->isChecked())
		//	g_pLogic->SetFillMode( 2);
		//else if( m_fillWireFrame->isChecked())
		//	g_pLogic->SetFillMode( 1);
		//else 
		//	g_pLogic->SetFillMode( 0);
	}

	// 是否开启阴影
	void CVideoSetting::SetShadow( bool bShadow)
	{
		SCLogic_Main->SetShadow( bShadow);
	}

	// 设置Fps
	void  CVideoSetting::SetFps(float fps)
	{
		char tfps[512];
		sprintf( tfps, "fps:%2f", SCLogic_Main->GetFPS());
		QString text = tfps;
		m_pFps->setText( text);
	}

	// 设置alpha测试值
	void CVideoSetting::SetAlphaTestValue( int value)
	{
		SCLogic_Main->SetAlphaTestValue( value);
	}

	// 设定Axis缩放
	void CVideoSetting::SetAxisScale( double scale)
	{
		SCLogic_EntityEdit->SetAxisScale( (float)scale);
	}

	// 是否显示胶囊
	void CVideoSetting::SetCapsuleVisible( bool visible)
	{
		if( SCLogic_Move)
			SCLogic_Move->SetCapsuleVisible( visible);
	}

	// 显示阴影 Foucs Region B
	void CVideoSetting::ShowFocusRegionB()
	{/*
		ILiSPSM* lipsm = Root::GetRenderSystem()->GetLiSPSMManager();
		if( lipsm)
		{
			// 获取Foucs Region B
			const Rect3& tRect = lipsm->GetLightSearchRect();

			static IBox* tBox = NULL;
			if ( !tBox)
				tBox = Root::GetRenderSystem()->GetGeometryManager()->CreateBox();

			// 显示包围盒
			tBox->SetParam( tRect.x0, tRect.y0, tRect.z0, tRect.x1, tRect.y1,tRect.z1);
		}
		*/
	}

	// 设置FXAA
	void CVideoSetting::UpdateFXAA()
	{
		FXAALevel level = FXAALevel( m_comboBoxFXAA->currentIndex());
		
		RenderSettings settings = SCLogic_Scene->GetRenderSystem().GetRenderSettings();
		settings.m_fxaa  = level;

		SCLogic_Scene->GetRenderSystem().ApplyRenderSettings( settings);
	}

	// 设置色调映射
	void CVideoSetting::SetToneMapping()
	{
		ToneMappingLevel level = ToneMappingLevel( m_comboBoxToneMapping->currentIndex());

		RenderSettings settings = SCLogic_Scene->GetRenderSystem().GetRenderSettings();
		settings.m_toneMapping  = level;

		SCLogic_Scene->GetRenderSystem().ApplyRenderSettings( settings);
	}
}