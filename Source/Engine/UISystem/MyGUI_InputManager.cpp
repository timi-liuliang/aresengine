/*!
	@file
	@author		Albert Semenov
	@date		11/2007
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Engine/UISystem/MyGUI_Precompiled.h"
#include "Engine/UISystem/MyGUI_InputManager.h"
#include "Engine/UISystem/MyGUI_Widget.h"
#include "Engine/UISystem/MyGUI_WidgetManager.h"
#include "Engine/UISystem/AresUISystem.h"
#include "Engine/UISystem/MyGUI_WidgetManager.h"
#include "Engine/UISystem/MyGUI_Constants.h"

namespace Ares
{
	using namespace MyGUI;

	template <> InputManager* Ares::Singleton<InputManager>::m_singleton = nullptr;
	//template <> const char* Singleton<InputManager>::mClassTypeName("InputManager");
}

namespace MyGUI
{
	const unsigned long INPUT_TIME_DOUBLE_CLICK = 250; //measured in milliseconds
	const float INPUT_DELAY_FIRST_KEY = 0.4f;
	const float INPUT_INTERVAL_KEY = 0.05f;

	InputManager::InputManager() :
		mWidgetMouseFocus(nullptr),
		mWidgetKeyFocus(nullptr),
		mLayerMouseFocus(nullptr),
		mIsShiftPressed(false),
		mIsControlPressed(false),
		mHoldKey( OIS::KC_UNASSIGNED),
		mHoldChar(0),
		mFirstPressKey(false),
		mTimerKey(0.0f),
		mOldAbsZ(0),
		mIsInitialise(false)
	{
		resetMouseCaptureWidget();
	}

	void InputManager::initialise()
	{
		MYGUI_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << getClassTypeName());

		mWidgetMouseFocus = 0;
		mWidgetKeyFocus = 0;
		mLayerMouseFocus = 0;
		for (int i = OIS::MB_Left; i<OIS::MB_Max; ++i)
			mMouseCapture[i] = false;

		mIsShiftPressed = false;
		mIsControlPressed = false;
		mHoldKey = OIS::KC_UNASSIGNED;
		mHoldChar = 0;
		mFirstPressKey = true;
		mTimerKey = 0.0f;
		mOldAbsZ = 0;

		WidgetManager::GetSingleton().registerUnlinker(this);
		Gui::GetSingleton().eventFrameStart += newDelegate(this, &InputManager::frameEntered);

		MYGUI_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void InputManager::shutdown()
	{
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_LOG(Info, "* Shutdown: " << getClassTypeName());

		Gui::GetSingleton().eventFrameStart -= newDelegate(this, &InputManager::frameEntered);
		WidgetManager::GetSingleton().unregisterUnlinker(this);

		MYGUI_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	bool InputManager::injectMouseMove(int _absx, int _absy, int _absz)
	{
		// 记录鼠标位置
		m_mousePosition.set( _absx, _absy);

		// вычисляем прирост по колеса
		int relz = _absz - mOldAbsZ;
		mOldAbsZ = _absz;

		// проверка на скролл
		if (relz != 0)
		{
			bool isFocus = isFocusMouse();
			if (isFocusMouse())
				mWidgetMouseFocus->_riseMouseWheel(relz);
			return isFocus;
		}

		if (isCaptureMouse())
		{
			if (isFocusMouse())
			{
				if (mLayerMouseFocus != nullptr)
				{
					IntPoint point = mLayerMouseFocus->getPosition(_absx, _absy);
					for (int i = OIS::MB_Left; i < OIS::MB_Max; ++i)
					{
						if (mMouseCapture[i])
							mWidgetMouseFocus->_riseMouseDrag(point.left, point.top, OIS::MouseButtonID( i));
					}
				}
			}
			else
			{
				resetMouseCaptureWidget();
			}

			return true;
		}

		Widget* old_mouse_focus = mWidgetMouseFocus;

		// ищем активное окно
		Widget* item = LayerManager::GetSingleton().getWidgetFromPoint(_absx, _absy);

		// ничего не изменилось
		if (mWidgetMouseFocus == item)
		{
			bool isFocus = isFocusMouse();
			if (isFocusMouse())
			{
				if (mLayerMouseFocus != nullptr)
				{
					IntPoint point = mLayerMouseFocus->getPosition(_absx, _absy);
					mWidgetMouseFocus->_riseMouseMove(point.left, point.top);
				}
			}
			return isFocus;
		}

		if (item)
		{
			// поднимаемся до рута
			Widget* root = item;
			while (root->getParent()) root = root->getParent();

			// проверяем на модальность
			if (!mVectorModalRootWidget.empty())
			{
				if (root != mVectorModalRootWidget.back())
				{
					item = nullptr;
				}
			}

			if (item != nullptr)
			{
				mLayerMouseFocus = root->getLayer();
			}
		}

		//-------------------------------------------------------------------------------------//
		// новый вид рутового фокуса мыши
		Widget* save_widget = nullptr;

		// спускаемся по новому виджету и устанавливаем рутовый фокус
		Widget* root_focus = item;
		while (root_focus != nullptr)
		{
			if (root_focus->getRootMouseFocus())
			{
				save_widget = root_focus;
				break;
			}

			root_focus->_setRootMouseFocus(true);
			root_focus->_riseMouseChangeRootFocus(true);
			root_focus = root_focus->getParent();
		}

		// спускаемся по старому виджету и сбрасываем фокус
		root_focus = mWidgetMouseFocus;
		while (root_focus != nullptr)
		{
			if (root_focus == save_widget)
				break;

			root_focus->_setRootMouseFocus(false);
			root_focus->_riseMouseChangeRootFocus(false);
			root_focus = root_focus->getParent();
		}
		//-------------------------------------------------------------------------------------//

		// смена фокуса, проверяем на доступность виджета
		if (isFocusMouse() && mWidgetMouseFocus->getEnabled())
		{
			mWidgetMouseFocus->_riseMouseLostFocus(item);
		}

		if ((item != nullptr) && (item->getEnabled()))
		{
			item->_riseMouseMove(_absx, _absy);
			item->_riseMouseSetFocus(mWidgetMouseFocus);
		}

		// запоминаем текущее окно
		mWidgetMouseFocus = item;

		if (old_mouse_focus != mWidgetMouseFocus)
			eventChangeMouseFocus(mWidgetMouseFocus);

		return isFocusMouse();
	}

	bool InputManager::injectMousePress(int _absx, int _absy, OIS::MouseButtonID _id)
	{
		Widget* old_key_focus = mWidgetKeyFocus;

		// если мы щелкнули не на гуй
		if (!isFocusMouse())
		{
			resetKeyFocusWidget();

			if (old_key_focus != mWidgetKeyFocus)
				eventChangeKeyFocus(mWidgetKeyFocus);

			return false;
		}

		// если активный элемент заблокирован
		//FIXME
		if (!mWidgetMouseFocus->getEnabled())
			return true;

		if ( OIS::MB_None != _id && OIS::MB_Max != _id)
		{
			// start capture
			mMouseCapture[_id] = true;
			// remember last pressed position
			if (mLayerMouseFocus != nullptr)
			{
				IntPoint point = mLayerMouseFocus->getPosition(_absx, _absy);
				mLastPressed[_id] = point;
			}
		}

		// ищем вверх тот виджет который может принимать фокус
		Widget* item = mWidgetMouseFocus;
		while ((item != nullptr) && (!item->getNeedKeyFocus()))
			item = item->getParent();

		// устанавливаем перед вызовом т.к. возможно внутри ктонить поменяет фокус под себя
		setKeyFocusWidget(item);

		if (isFocusMouse())
		{
			mWidgetMouseFocus->_riseMouseButtonPressed(_absx, _absy, _id);

			// после пресса может сброситься
			if ( mWidgetMouseFocus)
			{
				// поднимаем виджет, надо подумать что делать если поменялся фокус клавы
				LayerManager::GetSingleton().upLayerItem(mWidgetMouseFocus);

				// поднимаем пикинг Overlapped окон
				Widget* pick = mWidgetMouseFocus;
				do
				{
					// если оверлаппед, то поднимаем пикинг
					if (pick->getWidgetStyle() == WidgetStyle::Overlapped)
					{
						if (pick->getParent()) pick->getParent()->_forcePick(pick);
					}

					pick = pick->getParent();
				}
				while (pick);
			}
		}

		if (old_key_focus != mWidgetKeyFocus)
			eventChangeKeyFocus(mWidgetKeyFocus);

		return true;
	}

	bool InputManager::injectMouseRelease(int _absx, int _absy, OIS::MouseButtonID _id)
	{
		if (isFocusMouse())
		{
			// если активный элемент заблокирован
			if (!mWidgetMouseFocus->getEnabled())
				return true;

			if (_id != OIS::MB_None && _id != OIS::MB_Max)
			{
				if (mMouseCapture[_id])
				{
					// drop capture
					mMouseCapture[_id] = false;
				}
			}

			mWidgetMouseFocus->_riseMouseButtonReleased(_absx, _absy, _id);

			// после вызова, виджет может быть сброшен
			if (nullptr != mWidgetMouseFocus)
			{
				if ( OIS::MB_Left == _id)
				{
					if (mTimer.getMilliseconds() < INPUT_TIME_DOUBLE_CLICK)
					{
						mWidgetMouseFocus->_riseMouseButtonClick();
						// после вызова, виджет может быть сброшен
						if (nullptr != mWidgetMouseFocus)
							mWidgetMouseFocus->_riseMouseButtonDoubleClick();
					}
					else
					{
						// проверяем над тем ли мы окном сейчас что и были при нажатии
						Widget* item = LayerManager::GetSingleton().getWidgetFromPoint(_absx, _absy);
						if ( item == mWidgetMouseFocus)
						{
							mWidgetMouseFocus->_riseMouseButtonClick();
						}
						mTimer.reset();
					}
				}
			}

			// для корректного отображения
			injectMouseMove(_absx, _absy, mOldAbsZ);

			return true;
		}

		return false;
	}

	bool InputManager::injectKeyPress( OIS::KeyCode _key, Char _text)
	{
		// проверка на переключение языков
		firstEncoding(_key, true);

		// запоминаем клавишу
		storeKey(_key, _text);

		bool wasFocusKey = isFocusKey();

		//Pass keystrokes to the current active text widget
		if (isFocusKey())
		{
			mWidgetKeyFocus->_riseKeyButtonPressed(_key, _text);
		}

		return wasFocusKey;
	}

	bool InputManager::injectKeyRelease( OIS::KeyCode _key)
	{
		// проверка на переключение языков
		firstEncoding(_key, false);

		// сбрасываем клавишу
		resetKey();

		bool wasFocusKey = isFocusKey();

		if (isFocusKey())
			mWidgetKeyFocus->_riseKeyButtonReleased(_key);

		return wasFocusKey;
	}

	void InputManager::firstEncoding( OIS::KeyCode _key, bool bIsKeyPressed)
	{
		if ((_key == OIS::KC_LSHIFT) || (_key == OIS::KC_RSHIFT))
			mIsShiftPressed = bIsKeyPressed;
		if ((_key == OIS::KC_LCONTROL) || (_key == OIS::KC_RCONTROL))
			mIsControlPressed = bIsKeyPressed;
	}

	void InputManager::setKeyFocusWidget(Widget* _widget)
	{
		if (_widget == mWidgetKeyFocus)
			return;

		//-------------------------------------------------------------------------------------//
		// новый вид рутового фокуса
		Widget* save_widget = nullptr;

		// спускаемся по новому виджету и устанавливаем рутовый фокус
		Widget* root_focus = _widget;
		while (root_focus != nullptr)
		{
			if (root_focus->getRootKeyFocus())
			{
				save_widget = root_focus;
				break;
			}

			root_focus->_setRootKeyFocus(true);
			root_focus->_riseKeyChangeRootFocus(true);
			root_focus = root_focus->getParent();
		}

		// спускаемся по старому виджету и сбрасываем фокус
		root_focus = mWidgetKeyFocus;
		while (root_focus != nullptr)
		{
			if (root_focus == save_widget)
				break;

			root_focus->_setRootKeyFocus(false);
			root_focus->_riseKeyChangeRootFocus(false);
			root_focus = root_focus->getParent();
		}
		//-------------------------------------------------------------------------------------//

		// сбрасываем старый
		if (mWidgetKeyFocus)
		{
			mWidgetKeyFocus->_riseKeyLostFocus(_widget);
		}

		// устанавливаем новый
		if (_widget && _widget->getNeedKeyFocus())
		{
			_widget->_riseKeySetFocus(mWidgetKeyFocus);
		}

		mWidgetKeyFocus = _widget;
	}

	void InputManager::_resetMouseFocusWidget()
	{
		Widget* mouseFocus = mWidgetMouseFocus;
		mWidgetMouseFocus = nullptr;

		// спускаемся по старому виджету и сбрасываем фокус
		Widget* root_focus = mouseFocus;
		while (root_focus != nullptr)
		{
			root_focus->_setRootMouseFocus(false);
			root_focus->_riseMouseChangeRootFocus(false);
			root_focus = root_focus->getParent();
		}

		for (int i = OIS::MB_Left; i < OIS::MB_Max; ++i)
		{
			if (mMouseCapture[i])
			{
				mMouseCapture[i] = false;
				mouseFocus->_riseMouseButtonReleased(mLastPressed[i].left, mLastPressed[i].top, OIS::MouseButtonID(i));
			}
		}

		if (nullptr != mouseFocus)
		{
			mouseFocus->_riseMouseLostFocus(nullptr);
		}
	}

	// удаляем данный виджет из всех возможных мест
	void InputManager::_unlinkWidget(Widget* _widget)
	{
		if (nullptr == _widget)
			return;

		if (mWidgetMouseFocus == _widget)
			_resetMouseFocusWidget();

		if (_widget == mWidgetKeyFocus)
		{
			mWidgetKeyFocus = nullptr;
		}

		// ручками сбрасываем, чтобы не менять фокусы
		for (VectorWidgetPtr::iterator iter = mVectorModalRootWidget.begin(); iter != mVectorModalRootWidget.end(); ++iter)
		{
			if ((*iter == _widget))
			{
				mVectorModalRootWidget.erase(iter);
				break;
			}
		}
	}

	void InputManager::addWidgetModal(Widget* _widget)
	{
		if (nullptr == _widget)
			return;
		MYGUI_ASSERT(nullptr == _widget->getParent(), "Modal widget must be root");

		_resetMouseFocusWidget();
		removeWidgetModal(_widget);
		mVectorModalRootWidget.push_back(_widget);

		setKeyFocusWidget(_widget);
		LayerManager::GetSingleton().upLayerItem(_widget);
	}

	void InputManager::removeWidgetModal(Widget* _widget)
	{
		resetKeyFocusWidget(_widget);
		_resetMouseFocusWidget();

		for (VectorWidgetPtr::iterator iter = mVectorModalRootWidget.begin(); iter != mVectorModalRootWidget.end(); ++iter)
		{
			if ((*iter == _widget))
			{
				mVectorModalRootWidget.erase(iter);
				break;
			}
		}
		// если еще есть модальные то их фокусируем и поднимаем
		if (!mVectorModalRootWidget.empty())
		{
			setKeyFocusWidget(mVectorModalRootWidget.back());
			LayerManager::GetSingleton().upLayerItem(mVectorModalRootWidget.back());
		}
	}

	void InputManager::storeKey( OIS::KeyCode _key, Char _text)
	{
		mHoldKey = OIS::KC_UNASSIGNED;
		mHoldChar = 0;

		if ( !isFocusKey() ) return;
		if ( (  _key == OIS::KC_LSHIFT) || (_key == OIS::KC_RSHIFT)
			|| (_key == OIS::KC_LCONTROL) || (_key == OIS::KC_RCONTROL)
			|| (_key == OIS::KC_CALCULATOR) || (_key == OIS::KC_CALCULATOR)
			) return;

		mFirstPressKey = true;
		mHoldKey = _key;
		mHoldChar = _text;
		mTimerKey = 0.0f;
	}

	void InputManager::resetKey()
	{
		mHoldKey = OIS::KC_UNASSIGNED;
		mHoldChar = 0;
	}

	void InputManager::frameEntered(float _frame)
	{
		if ( mHoldKey == OIS::KC_UNASSIGNED)
			return;

		if ( !isFocusKey() )
		{
			mHoldKey = OIS::KC_UNASSIGNED;
			mHoldChar = 0;
			return;
		}

		mTimerKey += _frame;

		if (mFirstPressKey)
		{
			if (mTimerKey > INPUT_DELAY_FIRST_KEY)
			{
				mFirstPressKey = false;
				mTimerKey = 0.0f;
			}
		}
		else
		{
			if (mTimerKey > INPUT_INTERVAL_KEY)
			{
				while (mTimerKey > INPUT_INTERVAL_KEY)
					mTimerKey -= INPUT_INTERVAL_KEY;
				mWidgetKeyFocus->_riseKeyButtonPressed(mHoldKey, mHoldChar);
				// focus can be dropped in onKeyButtonPressed
				if (isFocusKey())
					mWidgetKeyFocus->_riseKeyButtonReleased(mHoldKey);
			}
		}

	}

	void InputManager::resetKeyFocusWidget(Widget* _widget)
	{
		if (mWidgetKeyFocus == _widget)
			setKeyFocusWidget(nullptr);
	}

	// 根据界面层获取鼠标位置
	IntPoint InputManager::getMousePositionByLayer()
	{
		if (mLayerMouseFocus != nullptr)
			return mLayerMouseFocus->getPosition( m_mousePosition.left, m_mousePosition.top);
		
		return m_mousePosition;
	}

	bool InputManager::isFocusMouse() const
	{
		return mWidgetMouseFocus != nullptr;
	}

	bool InputManager::isFocusKey() const
	{
		return mWidgetKeyFocus != nullptr;
	}

	bool InputManager::isCaptureMouse() const
	{
		for (int i = OIS::MB_Left; i < OIS::MB_Max; ++i)
		{
			if (mMouseCapture[i])
				return true;
		}
		return false;
	}

	void InputManager::resetKeyFocusWidget()
	{
		setKeyFocusWidget(nullptr);
	}

	Widget* InputManager::getMouseFocusWidget() const
	{
		return mWidgetMouseFocus;
	}

	Widget* InputManager::getKeyFocusWidget() const
	{
		return mWidgetKeyFocus;
	}

	// 获取鼠标按下位置
	const IntPoint& InputManager::getLastPressedPosition(OIS::MouseButtonID _id) const
	{
		if (_id != OIS::MB_None && _id != OIS::MB_Max)
		{
			return mLastPressed[_id];
		}

		return Constants::getZeroIntPoint();
	}

	bool InputManager::isModalAny() const
	{
		return !mVectorModalRootWidget.empty();
	}

	bool InputManager::isControlPressed() const
	{
		return mIsControlPressed;
	}

	bool InputManager::isShiftPressed() const
	{
		return mIsShiftPressed;
	}

	void InputManager::resetMouseCaptureWidget()
	{
		for (int i = OIS::MB_Left; i < OIS::MB_Max; ++i)
		{
			mMouseCapture[i] = false;
		}
	}

	void InputManager::unlinkWidget(Widget* _widget)
	{
		_unlinkWidget(_widget);
	}

} // namespace MyGUI
