#pragma once

#include <Core/AresPlatform.h>
#include <Engine/InputSystem/OIS.h>

namespace Ares
{
	//---------------------------------------
	// 输入监听器 2012-8-17 帝林
	//---------------------------------------
	class MOInputListener : public OIS::KeyListener
	{
	public:
		MOInputListener( HWND hwnd);
		~MOInputListener();

	public:
		// capture
		void Capture();

		// 注册
		void RegisterMouseEventCallback( OIS::MouseListener* mouseListener);

		// 注销
		void UnRegisterEventCallback( OIS::MouseListener* mouseListener);

		// 是否鼠标按下
		bool IsMouseDown( OIS::MouseButtonID button );

		// 获取鼠标状态
		const OIS::MouseState& GetMouseState();

		// 按键是否按下
		bool IsKeyDown( OIS::KeyCode key);

	public:
		// 键按下
		bool keyPressed(const OIS::KeyEvent &arg);

		// 键释放
		bool keyReleased(const OIS::KeyEvent &arg);

	private:
		OIS::InputSystem*		m_inputSystem;		// 输入系统
		OIS::Mouse*				m_mouse;			// 鼠标对象
		OIS::Keyboard*			m_keyBoard;			// 键盘对象
	};
}
