#include "MOLogic.h"
#include <Engine/InputSystem/OISInputSystem.h>

namespace Ares
{
	using namespace OIS;

	MOInputListener::MOInputListener( HWND hwnd)
	{
		m_inputSystem = OIS::InputSystem::createInputSystem( (size_t)hwnd);
		m_mouse		  = static_cast<OIS::Mouse*>( m_inputSystem->createInputObject( OIS::OISMouse, true));
		m_keyBoard	  = static_cast<OIS::Keyboard*>( m_inputSystem->createInputObject( OIS::OISKeyboard, false));

		m_keyBoard->setEventCallback( this);
	}

	// 注册
	void MOInputListener::RegisterMouseEventCallback( OIS::MouseListener* mouseListener)
	{
		m_mouse->RegisterEventCallback( mouseListener);
	}

	// 注销
	void MOInputListener::UnRegisterEventCallback( OIS::MouseListener* mouseListener)
	{
		m_mouse->UnRegisterEventCallback( mouseListener);
	}

	MOInputListener::~MOInputListener()
	{

	}

	// capture
	void MOInputListener::Capture()
	{
		m_mouse->capture();
		m_keyBoard->capture();
	}

	// 获取鼠标状态
	const OIS::MouseState& MOInputListener::GetMouseState()
	{ 
		return m_mouse->GetMouseState();
	}

	// 是否鼠标按下
	bool MOInputListener::IsMouseDown( OIS::MouseButtonID button )
	{
		return m_mouse->GetMouseState().IsButtonDown( button);
	}

	// 按键是否按下
	bool MOInputListener::IsKeyDown( OIS::KeyCode key)
	{
		return m_keyBoard->isKeyDown( key);
	}

	// 键按下
	bool MOInputListener::keyPressed(const KeyEvent &arg)
	{
		return false;
	}

	// 键释放
	bool MOInputListener::keyReleased(const KeyEvent &arg)
	{
		return false;
	}
}