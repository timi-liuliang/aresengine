#include "SCLogic.h"
#include <Engine/InputSystem/OISInputSystem.h>

namespace Ares
{
	using namespace OIS;

	InputListener::InputListener( HWND hwnd)
	{
		m_inputSystem = OIS::InputSystem::createInputSystem( (size_t)hwnd);
		m_mouse		  = static_cast<OIS::Mouse*>( m_inputSystem->createInputObject( OIS::OISMouse, true));
		m_keyBoard	  = static_cast<OIS::Keyboard*>( m_inputSystem->createInputObject( OIS::OISKeyboard, false));
	
		m_mouse->RegisterEventCallback( this);
		m_keyBoard->setEventCallback( this);
	}

	InputListener::~InputListener()
	{

	}

	// 注册
	void InputListener::RegisterMouseEventCallback( OIS::MouseListener* mouseListener)
	{
		m_mouse->RegisterEventCallback( mouseListener);
	}

	// capture
	void InputListener::Capture()
	{
		m_mouse->capture();
		m_keyBoard->capture();
	}

	// 获取鼠标状态
	const OIS::MouseState& InputListener::GetMouseState()
	{ 
		return m_mouse->GetMouseState();
	}

	// 是否鼠标按下
	bool InputListener::IsMouseDown( OIS::MouseButtonID button )
	{
		return m_mouse->GetMouseState().IsButtonDown( button);
	}

	// 按键是否按下
	bool InputListener::IsKeyDown( OIS::KeyCode key)
	{
		return m_keyBoard->isKeyDown( key);
	}

	// 鼠标移动
	bool InputListener::mouseMoved( const MouseEvent &arg)
	{
		return false;
	}

	// 鼠标按下
	bool InputListener::mousePressed( const MouseEvent &arg, MouseButtonID id)
	{
		return false;
	}

	// 鼠标释放
	bool InputListener::mouseReleased( const MouseEvent &arg, MouseButtonID id)
	{
		return false;
	}

	// 键按下
	bool InputListener::keyPressed(const KeyEvent &arg)
	{
		return false;
	}

	// 键释放
	bool InputListener::keyReleased(const KeyEvent &arg)
	{
		return false;
	}

	// 鼠标左键是否按下
	bool IsMLBDown()
	{
		return SCLogic_Input->IsMouseDown(OIS::MB_Left);
	}

	// 鼠标右键是否按下
	bool IsMRBDown()
	{
		return SCLogic_Input->IsMouseDown(OIS::MB_Right);
	}

	// 是否按下
	bool IsKeyDown(  OIS::KeyCode key)
	{
		return SCLogic_Input->IsKeyDown( key);
	}
}