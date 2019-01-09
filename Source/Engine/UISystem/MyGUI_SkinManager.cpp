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
#include "Engine/UISystem/MyGUI_SkinManager.h"
#include "Engine/UISystem/MyGUI_LanguageManager.h"
#include "Engine/UISystem/MyGUI_ResourceSkin.h"
#include "Engine/UISystem/MyGUI_XmlDocument.h"
#include "Engine/UISystem/MyGUI_SubWidgetManager.h"
#include "Engine/UISystem/AresUISystem.h"
#include "Engine/UISystem/MyGUI_DataManager.h"
#include "Engine/UISystem/MyGUI_FactoryManager.h"
#include "Engine/UISystem/MyGUI_IStateInfo.h"
#include "Engine/UISystem/MyGUI_LayoutManager.h"
#include "Engine/UISystem/MyGUI_BackwardCompatibility.h"

namespace Ares
{
	using namespace MyGUI;

	template <> SkinManager* Ares::Singleton<SkinManager>::m_singleton = nullptr;
	//template <> const char* Singleton<SkinManager>::mClassTypeName("SkinManager");
}

namespace MyGUI
{
	const std::string XML_TYPE("Skin");
	const std::string XML_TYPE_RESOURCE("Resource");
	const std::string RESOURCE_DEFAULT_NAME("Default");

	SkinManager::SkinManager() :
		mIsInitialise(false)
	{
	}

	void SkinManager::initialise()
	{
		MYGUI_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << getClassTypeName());

		ResourceManager::GetSingleton().registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &SkinManager::_load);
		FactoryManager::GetSingleton().registerFactory<ResourceSkin>(XML_TYPE_RESOURCE);

		mDefaultName = "skin_Default";
		createDefault(mDefaultName);

		MYGUI_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void SkinManager::shutdown()
	{
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_LOG(Info, "* Shutdown: " << getClassTypeName());

		ResourceManager::GetSingleton().unregisterLoadXmlDelegate(XML_TYPE);
		FactoryManager::GetSingleton().unregisterFactory<ResourceSkin>(XML_TYPE_RESOURCE);

		MYGUI_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	// 皮肤资源加载
	void SkinManager::_load(xml::ElementPtr _node, const std::string& _file, Version _version)
	{
		xml::ElementEnumerator skin = _node->getElementEnumerator();
		while(	skin.next(XML_TYPE))
		{
			/*std::string name = */skin->findAttribute("name");
			std::string type = skin->findAttribute("type");
			if (type.empty())
				type = "ResourceSkin";

			IObject* object = FactoryManager::GetSingleton().createObject(XML_TYPE_RESOURCE, type);
			if (object != nullptr)
			{
				ResourceSkin* data = object->castType<ResourceSkin>();
				data->deserialization(skin.current(), _version);

				ResourceManager::GetSingleton().addResource(data);
			}
		}
	}

	void SkinManager::createDefault(const std::string& _value)
	{
		xml::Document doc;
		xml::ElementPtr root = doc.createRoot("MyGUI");
		xml::ElementPtr newnode = root->createChild("Resource");
		newnode->addAttribute("type", ResourceSkin::getClassTypeName());
		newnode->addAttribute("name", _value);

		ResourceManager::GetSingleton().loadFromXmlNode(root, "", Version());
	}

	ResourceSkin* SkinManager::getByName(const std::string& _name) const
	{
		std::string skinName = BackwardCompatibility::getSkinRename(_name);
		IResource* result = nullptr;
		if (!skinName.empty() && skinName != RESOURCE_DEFAULT_NAME)
			result = ResourceManager::GetSingleton().getByName(skinName, false);

		if (result == nullptr)
		{
			result = ResourceManager::GetSingleton().getByName(mDefaultName, false);
			if (!skinName.empty() && skinName != RESOURCE_DEFAULT_NAME)
			{
				MYGUI_LOG(Error, "Skin '" << skinName << "' not found. Replaced with default skin." << " [" << LayoutManager::GetSingleton().getCurrentLayout() << "]");
			}
		}

		return result ? result->castType<ResourceSkin>(false) : nullptr;
	}

	bool SkinManager::isExist(const std::string& _name) const
	{
		std::string skinName = BackwardCompatibility::getSkinRename(_name);
		IResource* result = ResourceManager::GetSingleton().getByName(skinName, false);
		return (result != nullptr) && (result->isType<ResourceSkin>());
	}

	void SkinManager::setDefaultSkin(const std::string& _value)
	{
		mDefaultName = _value;
	}

	const std::string SkinManager::getDefaultSkin() const
	{
		return mDefaultName;
	}

} // namespace MyGUI
