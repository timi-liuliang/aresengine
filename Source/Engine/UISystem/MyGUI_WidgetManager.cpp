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
#include "Engine/UISystem/MyGUI_WidgetManager.h"
#include "Engine/UISystem/AresUISystem.h"
#include "Engine/UISystem/MyGUI_Widget.h"
#include "Engine/UISystem/MyGUI_FactoryManager.h"

#include "Engine/UISystem/MyGUI_Button.h"
#include "Engine/UISystem/MyGUI_Canvas.h"
#include "Engine/UISystem/MyGUI_ComboBox.h"
#include "Engine/UISystem/MyGUI_DDContainer.h"
#include "Engine/UISystem/MyGUI_EditBox.h"
#include "Engine/UISystem/MyGUI_ItemBox.h"
#include "Engine/UISystem/MyGUI_ListBox.h"
#include "Engine/UISystem/MyGUI_MenuBar.h"
#include "Engine/UISystem/MyGUI_MenuControl.h"
#include "Engine/UISystem/MyGUI_MenuItem.h"
#include "Engine/UISystem/MyGUI_MultiListBox.h"
#include "Engine/UISystem/MyGUI_MultiListItem.h"
#include "Engine/UISystem/MyGUI_PopupMenu.h"
#include "Engine/UISystem/MyGUI_ProgressBar.h"
#include "Engine/UISystem/MyGUI_ScrollBar.h"
#include "Engine/UISystem/MyGUI_ScrollView.h"
#include "Engine/UISystem/MyGUI_ImageBox.h"
#include "Engine/UISystem/MyGUI_TextBox.h"
#include "Engine/UISystem/MyGUI_TabControl.h"
#include "Engine/UISystem/MyGUI_TabItem.h"
#include "Engine/UISystem/MyGUI_Widget.h"
#include "Engine/UISystem/MyGUI_Window.h"
#include "Engine/UISystem/MyGUI_BackwardCompatibility.h"

namespace Ares
{
	using namespace MyGUI;

	template <> WidgetManager* Ares::Singleton<WidgetManager>::m_singleton = nullptr;
	//template <> const char* Ares::Singleton<WidgetManager>::mClassTypeName("WidgetManager");
}

namespace MyGUI
{
	// ¹¹Ôìº¯Êý
	WidgetManager::WidgetManager() :
		mIsInitialise(false)
	{
	}

	void WidgetManager::initialise()
	{
		MYGUI_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << getClassTypeName());

		FactoryManager& factory = FactoryManager::GetSingleton();

		factory.registerFactory<Button>("Widget");
		factory.registerFactory<Canvas>("Widget");
		factory.registerFactory<ComboBox>("Widget");
		factory.registerFactory<DDContainer>("Widget");
		factory.registerFactory<EditBox>("Widget");
		factory.registerFactory<ItemBox>("Widget");
		factory.registerFactory<ListBox>("Widget");
		factory.registerFactory<MenuBar>("Widget");
		factory.registerFactory<MenuControl>("Widget");
		factory.registerFactory<MenuItem>("Widget");
		factory.registerFactory<MultiListBox>("Widget");
		factory.registerFactory<MultiListItem>("Widget");
		factory.registerFactory<PopupMenu>("Widget");
		factory.registerFactory<ProgressBar>("Widget");
		factory.registerFactory<ScrollBar>("Widget");
		factory.registerFactory<ScrollView>("Widget");
		factory.registerFactory<ImageBox>("Widget");
		factory.registerFactory<TextBox>("Widget");
		factory.registerFactory<TabControl>("Widget");
		factory.registerFactory<TabItem>("Widget");
		factory.registerFactory<Widget>("Widget");
		factory.registerFactory<Window>("Widget");

		BackwardCompatibility::registerWidgetTypes();

		Gui::GetSingleton().eventFrameStart += newDelegate(this, &WidgetManager::notifyEventFrameStart);

		MYGUI_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void WidgetManager::shutdown()
	{
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_LOG(Info, "* Shutdown: " << getClassTypeName());

		Gui::GetSingleton().eventFrameStart -= newDelegate(this, &WidgetManager::notifyEventFrameStart);
		_deleteDelayWidgets();

		mVectorIUnlinkWidget.clear();

		FactoryManager::GetSingleton().unregisterFactory("Widget");

		MYGUI_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	Widget* WidgetManager::createWidget(WidgetStyle _style, const std::string& _type, const std::string& _skin, const IntCoord& _coord, Widget* _parent, ICroppedRectangle* _cropeedParent, const std::string& _name)
	{
		IObject* object = FactoryManager::GetSingleton().createObject("Widget", _type);
		if (object != nullptr)
		{
			Widget* widget = object->castType<Widget>();
			widget->_initialise(_style, _coord, _skin, _parent, _cropeedParent, _name);

			return widget;
		}

		MYGUI_EXCEPT("factory '" << _type << "' not found");
	}

	void WidgetManager::destroyWidget(Widget* _widget)
	{
		Gui::GetSingleton().destroyWidget(_widget);
	}

	void WidgetManager::destroyWidgets(const VectorWidgetPtr& _widgets)
	{
		Gui::GetSingleton().destroyWidgets(_widgets);
	}

	void WidgetManager::destroyWidgets(EnumeratorWidgetPtr _widgets)
	{
		Gui::GetSingleton().destroyWidgets(_widgets);
	}

	void WidgetManager::registerUnlinker(IUnlinkWidget* _unlink)
	{
		unregisterUnlinker(_unlink);
		mVectorIUnlinkWidget.push_back(_unlink);
	}

	void WidgetManager::unregisterUnlinker(IUnlinkWidget* _unlink)
	{
		VectorIUnlinkWidget::iterator iter = std::remove(mVectorIUnlinkWidget.begin(), mVectorIUnlinkWidget.end(), _unlink);
		if (iter != mVectorIUnlinkWidget.end())
			mVectorIUnlinkWidget.erase(iter);
	}

	void WidgetManager::unlinkFromUnlinkers(Widget* _widget)
	{
		for (VectorIUnlinkWidget::iterator iter = mVectorIUnlinkWidget.begin(); iter != mVectorIUnlinkWidget.end(); ++iter)
		{
			(*iter)->_unlinkWidget(_widget);
		}
	}

	bool WidgetManager::isFactoryExist(const std::string& _type)
	{
		if (FactoryManager::GetSingleton().isFactoryExist("Widget", _type))
		{
			return true;
		}

		return false;
	}

	void WidgetManager::notifyEventFrameStart(float _time)
	{
		_deleteDelayWidgets();
	}

	void WidgetManager::_deleteWidget(Widget* _widget)
	{
		_widget->_shutdown();

		for (VectorWidgetPtr::iterator entry = mDestroyWidgets.begin(); entry != mDestroyWidgets.end(); ++entry)
		{
			/*if ((*entry) == _widget)
				return;*/
			MYGUI_ASSERT((*entry) != _widget, "double delete widget");
		}

		mDestroyWidgets.push_back(_widget);
	}

	void WidgetManager::_deleteDelayWidgets()
	{
		if (!mDestroyWidgets.empty())
		{
			for (VectorWidgetPtr::iterator entry = mDestroyWidgets.begin(); entry != mDestroyWidgets.end(); ++entry)
				delete (*entry);
			mDestroyWidgets.clear();
		}
	}

} // namespace MyGUI
