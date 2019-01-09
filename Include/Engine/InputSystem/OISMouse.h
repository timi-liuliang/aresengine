/*
The zlib/libpng License

Copyright (c) 2005-2007 Phillip Castaneda (pjcast -- www.wreckedgames.com)

This software is provided 'as-is', without any express or implied warranty. In no event will
the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial
applications, and to alter it and redistribute it freely, subject to the following
restrictions:

    1. The origin of this software must not be misrepresented; you must not claim that
		you wrote the original software. If you use this software in a product,
		an acknowledgment in the product documentation would be appreciated but is
		not required.

    2. Altered source versions must be plainly marked as such, and must not be
		misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.
*/
#ifndef OIS_Mouse_H
#define OIS_Mouse_H
#include "OISObject.h"
#include "OISEvents.h"
#include <Core/AresTRect.hpp>
#include <vector>

namespace OIS
{
	//! Button ID for mouse devices
	enum MouseButtonID
	{
		MB_None		= -1,
		MB_Left		= 0, 
		MB_Right, 
		MB_Middle,
		MB_Button3, 
		MB_Button4,	
		MB_Button5, 
		MB_Button6,	
		MB_Button7,
		MB_Max,
	};

	//	Represents the state of the mouse
	//	All members are valid for both buffered and non buffered mode
	class _OISExport MouseState
	{
	public:
		MouseState() : width(50), height(50), buttons(0) {};

		/** Represents the height/width of your display area.. used if mouse clipping
		or mouse grabbed in case of X11 - defaults to 50.. Make sure to set this
		and change when your size changes.. */
		mutable int width, height;

		//! X Axis component
		Axis X;

		//! Y Axis Component
		Axis Y;

		//! Z Axis Component
		Axis Z;

		//! represents all buttons - bit position indicates button down
		int buttons;

		//! Button down test
		inline bool IsButtonDown( MouseButtonID button ) const
		{
			return ((buttons & ( 1L << button )) == 0) ? false : true;
		}

		//! Clear all the values
		void clear()
		{
			X.clear();
			Y.clear();
			Z.clear();
			buttons = 0;
		}
	};

	/** Specialised for mouse events */
	class _OISExport MouseEvent : public EventArg
	{
	public:
		MouseEvent( Object *obj, const MouseState &ms )	: EventArg(obj), state(ms) {}
		virtual ~MouseEvent() {}

		//! The state of the mouse - including buttons and axes
		const MouseState &state;
	};

	/**
		To recieve buffered mouse input, derive a class from this, and implement the
		methods here. Then set the call back to your Mouse instance with Mouse::setEventCallback
	*/
	class _OISExport MouseListener
	{
	public:
		virtual ~MouseListener() {}

		// 获取范围
		virtual bool IsValid() { return true; }

		// 鼠标移动
		virtual bool mouseMoved( const MouseEvent &arg ) { return true; }

		// 鼠标按下
		virtual bool mousePressed( const MouseEvent &arg, MouseButtonID id ) { return true; }

		// 鼠标释放
		virtual bool mouseReleased( const MouseEvent &arg, MouseButtonID id ) { return true; }
	};

	/**
		Mouse base class. To be implemented by specific system (ie. DirectX Mouse)
		This class is useful as you remain OS independent using this common interface.
	*/
	class _OISExport Mouse : public Object
	{
		typedef std::vector<MouseListener*> MouseListeners;
	public:
		virtual ~Mouse() {}

		// 注册
		virtual bool RegisterEventCallback( MouseListener *mouseListener );

		// 注销
		virtual bool UnRegisterEventCallback( MouseListener* mouseListener);

		// 返回注册回调列表
		const MouseListeners& GetEventCallbacks() const { return m_listeners;}

		// Returns the state of the mouse - is valid for both buffered and non buffered mode 
		const MouseState& GetMouseState() const { return mState; }

	protected:
		Mouse( const std::string &vendor, bool buffered, int devID, InputSystem* creator)
			: Object(vendor, OISMouse, buffered, devID, creator)
		{}

	protected:		
		MouseState	   mState;			// The state of the mouse	
		MouseListeners m_listeners;		// Used for buffered/actionmapping callback
	};
}
#endif
