#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

namespace Ares
{
	class CInputManager
	{
	public:
		// 构造函数
		CInputManager();

		// 析构函数
		~CInputManager();

		BOOL Initialize(HWND hWnd);
		void FrameMove( float fTime);

		DIMOUSESTATE* GetMouseState() { return &m_mouseState;};

		// 决断某键是否按下
		BOOL IsKeyDown( int key) { return m_keys[key] & 0x80; }

		// 判断鼠标某键是否按下
		BOOL IsMouseDown(int key);

		// 获取lz
		long GetLZ() { return m_LZ; }	

	public:
		LPDIRECTINPUT8       m_pDirectInput;		// DirectInput对象
		LPDIRECTINPUTDEVICE8 m_pKeyboard;           // 键盘设备对象
		LPDIRECTINPUTDEVICE8 m_pMouse;              // 鼠标设备对象
		DIMOUSESTATE         m_mouseState;			// 鼠标状态
		DIMOUSESTATE         m_oldMouseState;		// 鼠标状态

		long				 m_LZ;					// 鼠标中键操作
		char m_keys[256];							// 键盘状态
	};
}