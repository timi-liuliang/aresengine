/*!
	@file
	@author		Albert Semenov
	@date		02/2008
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
#include "Engine/UISystem/MyGUI_LayerManager.h"
#include "Engine/UISystem/MyGUI_LayerItem.h"
#include "Engine/UISystem/MyGUI_WidgetManager.h"
#include "Engine/UISystem/MyGUI_RenderManager.h"
#include "Engine/UISystem/MyGUI_Widget.h"
#include "Engine/UISystem/MyGUI_FactoryManager.h"

#include "Engine/UISystem/MyGUI_SharedLayer.h"
#include "Engine/UISystem/MyGUI_OverlappedLayer.h"

namespace Ares
{
	using namespace MyGUI;

	template <> LayerManager* Singleton<LayerManager>::m_singleton = nullptr;
	//template <> const char* Singleton<LayerManager>::mClassTypeName("LayerManager");
}

namespace MyGUI
{
	const std::string XML_TYPE("Layer");

	LayerManager::LayerManager() :
		mIsInitialise(false)
	{
	}

	// 初始化
	void LayerManager::initialise()
	{
		MYGUI_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << getClassTypeName());

		WidgetManager::GetSingleton().registerUnlinker(this);
		ResourceManager::GetSingleton().registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &LayerManager::_load);

		FactoryManager::GetSingleton().registerFactory<SharedLayer>(XML_TYPE);
		FactoryManager::GetSingleton().registerFactory<OverlappedLayer>(XML_TYPE);

		MYGUI_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void LayerManager::shutdown()
	{
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_LOG(Info, "* Shutdown: " << getClassTypeName());

		FactoryManager::GetSingleton().unregisterFactory<SharedLayer>(XML_TYPE);
		FactoryManager::GetSingleton().unregisterFactory<OverlappedLayer>(XML_TYPE);

		// удаляем все хранители слоев
		clear();

		WidgetManager::GetSingleton().unregisterUnlinker(this);
		ResourceManager::GetSingleton().unregisterLoadXmlDelegate(XML_TYPE);

		MYGUI_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	void LayerManager::clear()
	{
		for (VectorLayer::iterator iter = mLayerNodes.begin(); iter != mLayerNodes.end(); ++iter)
		{
			destroy(*iter);
		}
		mLayerNodes.clear();
	}

	void LayerManager::_load(xml::ElementPtr _node, const std::string& _file, Version _version)
	{
		VectorLayer layers;
		// берем детей и крутимся, основной цикл
		xml::ElementEnumerator layer = _node->getElementEnumerator();
		while (layer.next(XML_TYPE))
		{

			std::string name;

			if ( !layer->findAttribute("name", name))
			{
				MYGUI_LOG(Warning, "Attribute 'name' not found (file : " << _file << ")");
				continue;
			}

			for (VectorLayer::iterator iter = layers.begin(); iter != layers.end(); ++iter)
			{
				MYGUI_ASSERT((*iter)->getName() != name, "Layer '" << name << "' already exist (file : " << _file << ")");
			}

			std::string type = layer->findAttribute("type");
			if (type.empty() && _version <= Version(1, 0))
			{
				bool overlapped = utility::parseBool(layer->findAttribute("overlapped"));
				type = overlapped ? "OverlappedLayer" : "SharedLayer";
			}

			IObject* object = FactoryManager::GetSingleton().createObject(XML_TYPE, type);
			MYGUI_ASSERT(object != nullptr, "factory '" << type << "' is not found");

			ILayer* item = object->castType<ILayer>();
			item->deserialization(layer.current(), _version);

			layers.push_back(item);
		}

		// теперь мержим новые и старые слои
		merge(layers);
	}

	void LayerManager::_unlinkWidget(Widget* _widget)
	{
		detachFromLayer(_widget);
	}

	// поправить на виджет и проверять на рутовость
	void LayerManager::attachToLayerNode(const std::string& _name, Widget* _item)
	{
		MYGUI_ASSERT(nullptr != _item, "pointer must be valid");
		MYGUI_ASSERT(_item->isRootWidget(), "attached widget must be root");

		// 分离层
		_item->detachFromLayer();

		// 附加到指定层
		for (VectorLayer::iterator iter = mLayerNodes.begin(); iter != mLayerNodes.end(); ++iter)
		{
			if (_name == (*iter)->getName())
			{
				ILayerNode* node = (*iter)->createChildItemNode();
				node->attachLayerItem(_item);

				return;
			}
		}

		// 添加新的层

		MYGUI_LOG(Error, "Layer '" << _name << "' is not found");
	}

	void LayerManager::detachFromLayer(Widget* _item)
	{
		MYGUI_ASSERT(nullptr != _item, "pointer must be valid");
		_item->detachFromLayer();
	}

	void LayerManager::upLayerItem(Widget* _item)
	{
		MYGUI_ASSERT(nullptr != _item, "pointer must be valid");
		_item->upLayerItem();
	}

	bool LayerManager::isExist(const std::string& _name) const
	{
		return getByName(_name, false) != nullptr;
	}

	void LayerManager::merge(VectorLayer& _layers)
	{
		for (VectorLayer::iterator iter = mLayerNodes.begin(); iter != mLayerNodes.end(); ++iter)
		{
			if ((*iter) == nullptr) continue;
			bool find = false;
			std::string name = (*iter)->getName();
			for (VectorLayer::iterator iter2 = _layers.begin(); iter2 != _layers.end(); ++iter2)
			{
				if (name == (*iter2)->getName())
				{
					// заменяем новый слой, на уже существующий
					delete (*iter2);
					(*iter2) = (*iter);
					(*iter) = nullptr;
					find = true;
					break;
				}
			}
			if (!find)
			{
				destroy(*iter);
				(*iter) = nullptr;
			}
		}

		// теперь в основной
		mLayerNodes = _layers;
	}

	void LayerManager::destroy(ILayer* _layer)
	{
		MYGUI_LOG(Info, "destroy layer '" << _layer->getName() << "'");
		delete _layer;
	}
	Widget* LayerManager::getWidgetFromPoint(int _left, int _top)
	{
		VectorLayer::reverse_iterator iter = mLayerNodes.rbegin();
		while (iter != mLayerNodes.rend())
		{
			ILayerItem* item = (*iter)->getLayerItemByPoint(_left, _top);
			if (item != nullptr) return static_cast<Widget*>(item);
			++iter;
		}
		return nullptr;
	}

	void LayerManager::renderToTarget(IRenderTarget* _target, bool _update)
	{
		for (VectorLayer::iterator iter = mLayerNodes.begin(); iter != mLayerNodes.end(); ++iter)
		{
			(*iter)->renderToTarget(_target, _update);
		}
	}

	ILayer* LayerManager::getByName(const std::string& _name, bool _throw) const
	{
		for (VectorLayer::const_iterator iter = mLayerNodes.begin(); iter != mLayerNodes.end(); ++iter)
		{
			if (_name == (*iter)->getName())
				return (*iter);
		}
		MYGUI_ASSERT(!_throw, "Layer '" << _name << "' not found");
		return nullptr;
	}

	LayerManager::EnumeratorLayer LayerManager::getEnumerator() const
	{
		return EnumeratorLayer(mLayerNodes);
	}

	void LayerManager::resizeView(const IntSize& _viewSize)
	{
		for (VectorLayer::const_iterator iter = mLayerNodes.begin(); iter != mLayerNodes.end(); ++iter)
			(*iter)->resizeView(_viewSize);
	}

} // namespace MyGUI
