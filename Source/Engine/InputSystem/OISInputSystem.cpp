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
#include "Engine/InputSystem/OISInputSystem.h"
#include "Engine/InputSystem/OISException.h"
#include "Engine/InputSystem/OISFactoryCreator.h"
#include "Engine/InputSystem/OISObject.h"
#include <Core/AresPlatform.h>
#include <sstream>
#include <algorithm>

//Bring in correct Header / InputManager for current build platform
#if defined OIS_SDL_PLATFORM
#  include "SDL/SDLInputManager.h"
#elif defined OIS_WIN32_PLATFORM
#  include "Engine/InputSystem/win32/Win32InputManager.h"
#elif defined OIS_LINUX_PLATFORM
#  include "linux/LinuxInputManager.h"
#elif defined OIS_APPLE_PLATFORM
#  include "mac/MacInputManager.h"
#elif defined OIS_IPHONE_PLATFORM
#  include "iphone/iPhoneInputManager.h"
#elif defined OIS_XBOX_PLATFORM
#  include "xbox/XBoxInputManager.h"
#endif

//Bring in extra controls
#if defined OIS_LIRC_SUPPORT
#  include "extras/LIRC/OISLIRCFactoryCreator.h"
#endif
#if defined OIS_WIN32_WIIMOTE_SUPPORT
#  include "win32/extras/WiiMote/OISWiiMoteFactoryCreator.h"
#endif


using namespace OIS;

//----------------------------------------------------------------------------//
InputSystem::InputSystem(const std::string& name) :
	m_versionName(OIS_VERSION_NAME),
	m_inputSystemName(name),
	m_lircSupport(0),
	m_wiiMoteSupport(0)
{
    m_factories.clear();
    m_factoryObjects.clear();
}

//----------------------------------------------------------------------------//
InputSystem::~InputSystem()
{
#if defined OIS_LIRC_SUPPORT
	delete m_lircSupport;
#endif

#if defined OIS_WIN32_WIIMOTE_SUPPORT
	delete m_wiiMoteSupport;
#endif
}

//----------------------------------------------------------------------------//
unsigned int InputSystem::getVersionNumber()
{
	return OIS_VERSION;
}

//----------------------------------------------------------------------------//
const std::string &InputSystem::getVersionName()
{
	return m_versionName;
}

/**
@remarks
	Creates appropriate input system dependent on platform. 
@param winHandle
	Contains OS specific window handle (such as HWND or X11 Window)
@returns
	A pointer to the created manager, or raises Exception
*/
InputSystem* InputSystem::createInputSystem( std::size_t windowhandle )
{
	ParamList pl;
	std::ostringstream wnd;
	wnd << windowhandle;
	pl.insert(std::make_pair( std::string("WINDOW"), wnd.str() ));

	return createInputSystem( pl );
}

/**
@remarks
	Creates appropriate input system dependent on platform. 
@param paramList
	ParamList contains OS specific info (such as HWND and HINSTANCE for window apps),
	and access mode.
@returns
	A pointer to the created manager, or raises Exception
*/
InputSystem* InputSystem::createInputSystem( ParamList &paramList )
{
	InputSystem* im = 0;

#if defined OIS_SDL_PLATFORM
	im = new_ SDLInputManager();
#elif defined OIS_WIN32_PLATFORM
	im = new_ Win32InputManager();
#elif defined OIS_XBOX_PLATFORM
	im = new_ XBoxInputManager();
#elif defined OIS_LINUX_PLATFORM
	im = new_ LinuxInputManager();
#elif defined OIS_APPLE_PLATFORM
	im = new_ MacInputManager();
#elif defined OIS_IPHONE_PLATFORM
	im = new_ iPhoneInputManager();
#else
	OIS_EXCEPT(E_General, "No platform library.. check build platform defines!");
#endif 

	try
	{
		im->_initialize(paramList);
	}
	catch(...)
	{
		delete im;
		throw; //rethrow
	}

	return im;
}

//----------------------------------------------------------------------------//
void InputSystem::destroyInputSystem(InputSystem* manager)
{
	if( manager == 0 )
		return;

	//Cleanup before deleting...
	for( FactoryCreatedObject::iterator i = manager->m_factoryObjects.begin(); i != manager->m_factoryObjects.end(); ++i )
	{
		i->second->destroyObject( i->first );
	}

	manager->m_factoryObjects.clear();
	delete manager;
}

// 获取输入系统名称
const std::string& InputSystem::inputSystemName()
{
	return m_inputSystemName;
}

//--------------------------------------------------------------------------------//
int InputSystem::getNumberOfDevices( Type iType )
{
	//Count up all the factories devices
	int factoyObjects = 0;
	FactoryList::iterator i = m_factories.begin(), e = m_factories.end();
	for( ; i != e; ++i )
		factoyObjects += (*i)->totalDevices(iType);

	return factoyObjects;
}

//----------------------------------------------------------------------------//
DeviceList InputSystem::listFreeDevices()
{
	DeviceList list;
	FactoryList::iterator i = m_factories.begin(), e = m_factories.end();
	for( ; i != e; ++i )
	{
		DeviceList temp = (*i)->freeDeviceList();
		list.insert(temp.begin(), temp.end());
	}

	return list;
}

// 创建输入对象
Object* InputSystem::createInputObject( Type iType, bool bufferMode, const std::string &vendor )
{
	Object* obj = 0;
	FactoryList::iterator i = m_factories.begin(), e = m_factories.end();
	for( ; i != e; ++i)
	{
		if( (*i)->freeDevices(iType) > 0 )
		{
			if( vendor == "" || (*i)->vendorExist(iType, vendor) )
			{
				obj = (*i)->createObject(this, iType, bufferMode, vendor);
				m_factoryObjects[obj] = (*i);
				break;
			}
		}
	}

	if(!obj)
		OIS_EXCEPT(E_InputDeviceNonExistant, "No devices match requested type.");

	try
	{	//Intialize device
		obj->_initialize();
	}
	catch(...)
	{	//Somekind of error, cleanup and rethrow
		destroyInputObject(obj);
		throw;
	}

	return obj;
}

//----------------------------------------------------------------------------//
void InputSystem::destroyInputObject( Object* obj )
{
	if( obj == 0 )
		return;

	FactoryCreatedObject::iterator i = m_factoryObjects.find(obj);
	if( i != m_factoryObjects.end() )
	{
		i->second->destroyObject(obj);
		m_factoryObjects.erase(i);
	}
	else
	{
		OIS_EXCEPT(E_General, "Object creator not known.");
	}
}

//----------------------------------------------------------------------------//
void InputSystem::addFactoryCreator( FactoryCreator* factory )
{
	if(factory != 0)
		m_factories.push_back(factory);
}

//----------------------------------------------------------------------------//
void InputSystem::removeFactoryCreator( FactoryCreator* factory )
{
	if(factory != 0)
	{
		//First, destroy all devices created with the factory
		for( FactoryCreatedObject::iterator i = m_factoryObjects.begin(); i != m_factoryObjects.end(); ++i )
		{
			if( i->second == factory )
			{
				i->second->destroyObject(i->first);
				m_factoryObjects.erase(i++);
			}
		}

		//Now, remove the factory itself
		FactoryList::iterator fact = std::find(m_factories.begin(), m_factories.end(), factory);
		if( fact != m_factories.end() )
			m_factories.erase(fact);
	}
}

//----------------------------------------------------------------------------//
void InputSystem::enableAddOnFactory(AddOnFactories factory)
{
#if defined OIS_LIRC_SUPPORT
	if( factory == AddOn_LIRC || factory == AddOn_All )
	{
		if( m_lircSupport == 0 )
		{
			m_lircSupport = new_ LIRCFactoryCreator();
			addFactoryCreator(m_lircSupport);
		}
	}
#endif

#if defined OIS_WIN32_WIIMOTE_SUPPORT
	if( factory == AddOn_WiiMote || factory == AddOn_All )
	{
		if( m_wiiMoteSupport == 0 )
		{
			m_wiiMoteSupport = new_ WiiMoteFactoryCreator();
			addFactoryCreator(m_wiiMoteSupport);
		}
	}
#endif
}
