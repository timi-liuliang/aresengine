#pragma once

#include "AresElementFormat.h"

#ifdef ARES_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace Ares
{
	// 渲染设备设备类型
	enum RenderDeviceType
	{
		D3D11,
		OPENGL,
		OPENGLES,
	};

	// FXAA级别
	enum FXAALevel
	{
		FXAA_Off	 = 0,		// off
		FXAA_Preset1,			// low
		FXAA_Preset2,			
		FXAA_Preset3,			// default
		FXAA_Preset4,
		FXAA_Preset5,
		FXAA_Preset6			// high
	};

	// ToneMappingLevel
	enum ToneMappingLevel
	{
		TML_Off = 0,
		TML_On,
	};

	//------------------------------------
	// 渲染引擎设置 2011-12-28 帝林
	//------------------------------------
	struct RenderSettings
	{
		string				m_title;					// window title
		bool				m_fullScreen;				// is fullscreen
		int					m_left;
		int					m_top;
		int					m_width;					// window width
		int					m_height;					// window height
		bool				m_hdr;						// is high dynamic range
		UINT				m_sampleCount;				// sample count
		UINT				m_sampleQuality;			// sample quality
		UINT				m_syncInterval;				// sync interval
		bool				m_gamma;					// 
		FXAALevel			m_fxaa;						// fxaa level
		ToneMappingLevel	m_toneMapping;				// tone mapping enabled

		bool				m_auxiliaryDisplay;			// auxiliary display
		bool				m_spaceDistorted;			// space distorted

		bool				m_useMyGUI;					// use mygui or not

		ElementFormat		m_colorFormat;				// color format
		ElementFormat		m_depthStencilFormat;		// depth stencil format

		// 构造函数
		RenderSettings()
		{}

	#ifdef ARES_PLATFORM_WINDOWS
		HWND	m_hwnd;						// window handle
        
        // 构造函数
		RenderSettings( HWND hwnd)
        : m_title( "Ares")
		, m_hdr(false)
		, m_left(0)
		, m_top(0)
		, m_width( 800)
		, m_height( 600)
		, m_colorFormat( EF_ARGB8)
		, m_depthStencilFormat(EF_D32F)
		, m_sampleCount( 1)
		, m_sampleQuality( 0)
		, m_syncInterval( 0)
        , m_spaceDistorted( true)
        , m_auxiliaryDisplay( true)
        , m_useMyGUI( false)
		, m_fxaa( FXAA_Preset1)
		, m_toneMapping( TML_Off)
        {
            m_hwnd = hwnd;
            m_fullScreen = static_cast<bool>(!hwnd);
            if( !m_fullScreen)
            {
                RECT tRect;
                GetClientRect( hwnd, &tRect);
                m_width = tRect.right  - tRect.left;
                m_height= tRect.bottom - tRect.top;
            }

			if( m_toneMapping)
			{
				m_colorFormat = EF_ABGR16F;
			}
        }
    #elif defined ARES_PLATFORM_IOS
        void*   m_eaglLayer;                // ios layer
        
        // Constructor
        RenderSettings(void* eaglLayer)
        : m_title( "Ares")
        , m_hdr(false)
        , m_left(0)
        , m_top(0)
        , m_width( 800)
        , m_height( 600)
        , m_eaglLayer( eaglLayer)
        , m_colorFormat( EF_ARGB8)
        , m_depthStencilFormat(EF_D32F)
        , m_sampleCount( 1)
        , m_sampleQuality( 0)
        , m_syncInterval( 0)
        , m_spaceDistorted( true)
        , m_auxiliaryDisplay( true)
        , m_useMyGUI( false)
        {

        }
    #endif
	};
}