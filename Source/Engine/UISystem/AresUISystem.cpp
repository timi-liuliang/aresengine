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
#include "Engine/UISystem/AresUISystem.h"
#include "Engine/UISystem/MyGUI_Widget.h"

#include "Engine/UISystem/MyGUI_InputManager.h"
#include "Engine/UISystem/MyGUI_SubWidgetManager.h"
#include "Engine/UISystem/MyGUI_SkinManager.h"
#include "Engine/UISystem/MyGUI_WidgetManager.h"
#include "Engine/UISystem/MyGUI_LayerManager.h"
#include "Engine/UISystem/MyGUI_FontManager.h"
#include "Engine/UISystem/MyGUI_ControllerManager.h"
#include "Engine/UISystem/MyGUI_PointerManager.h"
#include "Engine/UISystem/MyGUI_ClipboardManager.h"
#include "Engine/UISystem/MyGUI_LayoutManager.h"
#include "Engine/UISystem/MyGUI_LanguageManager.h"
#include "Engine/UISystem/MyGUI_ResourceManager.h"
#include "Engine/UISystem/MyGUI_RenderManager.h"
#include "Engine/UISystem/MyGUI_FactoryManager.h"
#include "Engine/UISystem/MyGUI_ToolTipManager.h"
#include "Engine/UISystem/MyGUI_DataManager.h"

namespace Ares
{
	using namespace MyGUI;

	template <> Gui*				Singleton<Gui>::m_singleton					= nullptr;
	template <> FontManager*		Singleton<FontManager>::m_singleton			= nullptr;
	template <> FactoryManager*		Singleton<FactoryManager>::m_singleton		= nullptr;
	template <> DataManager*		Singleton<DataManager>::m_singleton			= nullptr;
	template <> ControllerManager*	Singleton<ControllerManager>::m_singleton	= nullptr;
	template <> ClipboardManager*	Singleton<ClipboardManager>::m_singleton	= nullptr;
	template <> RenderManager*		Singleton<RenderManager>::m_singleton		= nullptr;
}

namespace MyGUI
{
	Gui::Gui() :
		m_inputMgr(nullptr),
		mSubWidgetManager(nullptr),
		m_layerMgr(nullptr),
		m_skinMgr(nullptr),
		mWidgetManager(nullptr),
		mFontManager(nullptr),
		mControllerManager(nullptr),
		mPointerManager(nullptr),
		mClipboardManager(nullptr),
		mLayoutManager(nullptr),
		mLanguageManager(nullptr),
		mResourceManager(nullptr),
		mFactoryManager(nullptr),
		mToolTipManager(nullptr),
		mIsInitialise(false)
	{
	}

	// 初始化
	void Gui::Initialise(const std::string& _core)
	{
		MYGUI_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << getClassTypeName());

#ifdef MYGUI_SVN_REVISION
		MYGUI_LOG(Info, "* MyGUI version "
			<< MYGUI_VERSION_MAJOR << "."
			<< MYGUI_VERSION_MINOR << "."
			<< MYGUI_VERSION_PATCH << "."
			<< MYGUI_SVN_REVISION);
#else
		MYGUI_LOG(Info, "* MyGUI version "
			<< MYGUI_VERSION_MAJOR << "."
			<< MYGUI_VERSION_MINOR << "."
			<< MYGUI_VERSION_PATCH);
#endif

		// создаем и инициализируем синглтоны
		mResourceManager	= new ResourceManager();
		m_layerMgr			= new LayerManager();
		mWidgetManager		= new WidgetManager();
		m_inputMgr			= new InputManager();
		mSubWidgetManager	= new SubWidgetManager();
		m_skinMgr		= new SkinManager();
		mFontManager		= new FontManager();
		mControllerManager	= new ControllerManager();
		mPointerManager		= new PointerManager();
		mClipboardManager	= new ClipboardManager();
		mLayoutManager		= new LayoutManager();
		mLanguageManager	= new LanguageManager();
		mFactoryManager		= new FactoryManager();
		mToolTipManager		= new ToolTipManager();
		m_renderMgr			= new RenderManager;
							  new DataManager;				

		mResourceManager->initialise();
		m_layerMgr->initialise();
		mWidgetManager->initialise();
		m_inputMgr->initialise();
		mSubWidgetManager->initialise();
		m_skinMgr->initialise();
		mFontManager->initialise();
		mControllerManager->initialise();
		mPointerManager->initialise();
		mClipboardManager->initialise();
		mLayoutManager->initialise();
		mLanguageManager->initialise();
		mFactoryManager->initialise();
		mToolTipManager->initialise();

		WidgetManager::GetSingleton().registerUnlinker(this);

		// 加载资源(无必要)
		if (!_core.empty())
			mResourceManager->load(_core);

		BackwardCompatibility::initialise();

		MYGUI_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

#ifndef MYGUI_DONT_USE_OBSOLETE
	void Gui::Initialise(const std::string& _core, const std::string& _logFileName)
	{
		Initialise( _core);
	}
#endif // MYGUI_DONT_USE_OBSOLETE

	void Gui::shutdown()
	{
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_LOG(Info, "* Shutdown: " << getClassTypeName());

		BackwardCompatibility::shutdown();

		_destroyAllChildWidget();

		// деинициализируем и удаляем синглтоны
		mPointerManager->shutdown();
		m_inputMgr->shutdown();
		m_skinMgr->shutdown();
		mSubWidgetManager->shutdown();
		m_layerMgr->shutdown();
		mFontManager->shutdown();
		mControllerManager->shutdown();
		mClipboardManager->shutdown();
		mLayoutManager->shutdown();
		mLanguageManager->shutdown();
		mResourceManager->shutdown();
		mFactoryManager->shutdown();
		mToolTipManager->shutdown();

		WidgetManager::GetSingleton().unregisterUnlinker(this);
		mWidgetManager->shutdown();

		delete mPointerManager;
		delete mWidgetManager;
		delete m_inputMgr;
		delete m_skinMgr;
		delete mSubWidgetManager;
		delete m_layerMgr;
		delete mFontManager;
		delete mControllerManager;
		delete mClipboardManager;
		delete mLayoutManager;
		delete mLanguageManager;
		delete mResourceManager;
		delete mFactoryManager;
		delete mToolTipManager;

		MYGUI_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	Widget* Gui::baseCreateWidget(WidgetStyle _style, const std::string& _type, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _layer, const std::string& _name)
	{
		Widget* widget = WidgetManager::GetSingleton().createWidget(_style, _type, _skin, _coord, /*_align, */nullptr, nullptr, _name);
		mWidgetChild.push_back(widget);

		widget->setAlign(_align);

		// 附加到层
		if (!_layer.empty())
			LayerManager::GetSingleton().attachToLayerNode(_layer, widget);

		return widget;
	}

	Widget* Gui::findWidgetT(const std::string& _name, bool _throw)
	{
		for (VectorWidgetPtr::iterator iter = mWidgetChild.begin(); iter != mWidgetChild.end(); ++iter)
		{
			Widget* widget = (*iter)->findWidget(_name);
			if (widget != nullptr) return widget;
		}
		MYGUI_ASSERT(!_throw, "Widget '" << _name << "' not found");
		return nullptr;
	}

	// удяляет неудачника
	void Gui::_destroyChildWidget(Widget* _widget)
	{
		MYGUI_ASSERT(nullptr != _widget, "invalid widget pointer");

		VectorWidgetPtr::iterator iter = std::find(mWidgetChild.begin(), mWidgetChild.end(), _widget);
		if (iter != mWidgetChild.end())
		{
			// сохраняем указатель
			MyGUI::Widget* widget = *iter;

			// удаляем из списка
			mWidgetChild.erase(iter);

			// отписываем от всех
			mWidgetManager->unlinkFromUnlinkers(_widget);

			// непосредственное удаление
			WidgetManager::GetSingleton()._deleteWidget(widget);
		}
		else
		{
			MYGUI_EXCEPT("Widget '" << _widget->getName() << "' not found");
		}
	}

	// удаляет всех детей
	void Gui::_destroyAllChildWidget()
	{
		while (!mWidgetChild.empty())
		{
			// сразу себя отписывем, иначе вложенной удаление убивает все
			Widget* widget = mWidgetChild.back();
			mWidgetChild.pop_back();

			// отписываем от всех
			mWidgetManager->unlinkFromUnlinkers(widget);

			// и сами удалим, так как его больше в списке нет
			WidgetManager::GetSingleton()._deleteWidget(widget);
		}
	}

	void Gui::destroyWidget(Widget* _widget)
	{
		Widget* parent = _widget->getParent();
		if (parent != nullptr)
			parent->_destroyChildWidget(_widget);
		else
			_destroyChildWidget(_widget);
	}

	void Gui::destroyWidgets(const VectorWidgetPtr& _widgets)
	{
		for (VectorWidgetPtr::const_iterator iter = _widgets.begin(); iter != _widgets.end(); ++iter)
			destroyWidget(*iter);
	}

	void Gui::destroyWidgets(EnumeratorWidgetPtr& _widgets)
	{
		VectorWidgetPtr widgets;
		while (_widgets.next())
			widgets.push_back(_widgets.current());
		destroyWidgets(widgets);
	}

	void Gui::_unlinkWidget(Widget* _widget)
	{
		eventFrameStart.clear(_widget);
	}

	void Gui::_linkChildWidget(Widget* _widget)
	{
		VectorWidgetPtr::iterator iter = std::find(mWidgetChild.begin(), mWidgetChild.end(), _widget);
		MYGUI_ASSERT(iter == mWidgetChild.end(), "widget already exist");
		mWidgetChild.push_back(_widget);
	}

	void Gui::_unlinkChildWidget(Widget* _widget)
	{
		VectorWidgetPtr::iterator iter = std::remove(mWidgetChild.begin(), mWidgetChild.end(), _widget);
		MYGUI_ASSERT(iter != mWidgetChild.end(), "widget not found");
		mWidgetChild.erase(iter);
	}

	Widget* Gui::createWidgetT(const std::string& _type, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _layer, const std::string& _name)
	{
		return baseCreateWidget(WidgetStyle::Overlapped, _type, _skin, _coord, _align, _layer, _name);
	}
	/** See Gui::createWidgetT */
	Widget* Gui::createWidgetT(const std::string& _type, const std::string& _skin, int _left, int _top, int _width, int _height, Align _align, const std::string& _layer, const std::string& _name)
	{
		return createWidgetT(_type, _skin, IntCoord(_left, _top, _width, _height), _align, _layer, _name);
	}
	/** Create widget using coordinates relative to parent widget. see Gui::createWidgetT */
	Widget* Gui::createWidgetRealT(const std::string& _type, const std::string& _skin, const FloatCoord& _coord, Align _align, const std::string& _layer, const std::string& _name)
	{
		IntSize size = RenderManager::GetSingleton().getViewSize();
		return createWidgetT(_type, _skin, IntCoord((int)(_coord.left * size.width), (int)(_coord.top * size.height), (int)(_coord.width * size.width), (int)(_coord.height * size.height)), _align, _layer, _name);
	}
	/** Create widget using coordinates relative to parent. see Gui::createWidgetT */
	Widget* Gui::createWidgetRealT(const std::string& _type, const std::string& _skin, float _left, float _top, float _width, float _height, Align _align, const std::string& _layer, const std::string& _name)
	{
		IntSize size = RenderManager::GetSingleton().getViewSize();
		return createWidgetT(_type, _skin, IntCoord((int)(_left * size.width), (int)(_top * size.height), (int)(_width * size.width), (int)(_height * size.height)), _align, _layer, _name);
	}

	Widget* Gui::findWidgetT(const std::string& _name, const std::string& _prefix, bool _throw)
	{
		return findWidgetT(_prefix + _name, _throw);
	}

	void Gui::destroyChildWidget(Widget* _widget)
	{
		_destroyChildWidget(_widget);
	}

	void Gui::destroyAllChildWidget()
	{
		_destroyAllChildWidget();
	}

	EnumeratorWidgetPtr Gui::getEnumerator() const
	{
		return EnumeratorWidgetPtr(mWidgetChild);
	}

	void Gui::frameEvent(float _time)
	{
		eventFrameStart(_time);
	}

	// 渲染
	void Gui::Render( IRenderTarget* renderTarget)
	{
		if( m_layerMgr)
			m_layerMgr->renderToTarget( renderTarget, true);
	}
}
