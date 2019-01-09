#ifndef OIS_InputManager_H
#define OIS_InputManager_H

#include "OISPrereqs.h"

namespace OIS
{
	//Forward declare a couple of classes we might use later
	class LIRCFactoryCreator;
	class WiiMoteFactoryCreator;

	//------------------------------------------------
	//	Base Manager class. No longer a Singleton; 
	//  so feel free to create as many InputManager's 
	//  as you have windows.
	//-----------------------------------------------
	class _OISExport InputSystem
	{
	public:
		// @remarks Returns version number (useful in DLL/SO libs)
		// @returns Bits: 1-8 Patch number, 9-16 Minor version, 17-32 Major version
		static unsigned int getVersionNumber();

		// @remarks Returns version string (useful in DLL/SO libs)
		// @returns Version name
		const std::string &getVersionName();

		// 创建输入系统
		static InputSystem* createInputSystem( std::size_t winHandle );

		// 创建输入系统
		static InputSystem* createInputSystem( ParamList &paramList );

		// @remarks Destroys the InputManager
		// @param manager Manager to destroy
		static void destroyInputSystem(InputSystem* manager);

		// Gets the name of the current platform input system
		const std::string& inputSystemName();

		/**
		@remarks
			Returns the number of the specified OIS::Type devices discovered by OIS
		@param iType
			Type that you are interested in
		*/
		int getNumberOfDevices( Type iType );

		/**
		@remarks
			Lists all unused devices
		@returns
			DeviceList which contains Type and vendor of device
		*/
		DeviceList listFreeDevices();

		/**
		@remarks
			Tries to create an object with the specified vendor. If you have no
			preference of vendor, leave vender as default (""). Raises exception on failure
		*/
		Object* createInputObject( Type iType, bool bufferMode, const std::string &vendor = "");

		/**
		@remarks Destroys Input Object
		*/
		void destroyInputObject( Object* obj );

		/**
		@remarks
			Add a custom object factory to allow for user controls.
		@param factory
			Factory instance to add
		@notes
			Make sure you do not delete the factory before devices created from
			the factory are destroyed (either by calling RemoveFactoryCreator, or shutting down
			the input system). Order should be something like the following:
				* Create Input System
				* Create Factory Instance
				* AddFactoryCreator(factory)
				* Create a device from the InputManager (device created by factory)
				* One of the follwoing:
					* removeFactoryCreator(factory)
					* inputManager->destroyInputObject(obj)
				* destroyInputSystem(inputManager)
				* destroy Factory Instance
			You can safely delete the factory instance once you have removed it or shut down the
			input manager.
		*/
		void addFactoryCreator( FactoryCreator* factory );

		/**
		@remarks
			Remove a previously added object factory
		@param factory
			Factory object to remove.
		@notes
			Removing a factory will automatically destroy any Objects created from the factory
		*/
		void removeFactoryCreator( FactoryCreator* factory );

		//! All generic devices OIS supports internally (if they are compiled in)
		enum AddOnFactories
		{
			AddOn_All = 0,		//All Devices
			AddOn_LIRC = 1,		//PC Linux Infrared Remote Control
			AddOn_WiiMote = 2	//PC WiiMote Support
		};

		/**
		@remarks
			Enable an addon FactoryCreator extension. By default, none are activated.
			If the desired support was not compiled in, this has no effect. Calling
			multiple times has no effect. Once activated, there is no way to deactivate -
			simply destroy and recreate input manager.
		*/
		void enableAddOnFactory(AddOnFactories factory);

	protected:
		// Called from createInputSystem, gives derived input class a chance to setup after it is created
		virtual void _initialize(ParamList &paramList)=0;

		// Derived classes must provide input system name
		InputSystem(const std::string& name);

		// Virtual Destructor - this base class will clean up all devices still opened in mFactoryObjects list
		virtual ~InputSystem();

	protected:
		const std::string		m_versionName;		//! OIS Version name
		FactoryList				m_factories;		//! FactoryCreator list	
		FactoryCreatedObject	m_factoryObjects;	//! Factory created objects - useful so we can find creator to send destruction request to
		const std::string		m_inputSystemName;	//! Name of the input system
		LIRCFactoryCreator*		m_lircSupport;		//! Extra factory (not enabled by default)
		WiiMoteFactoryCreator*	m_wiiMoteSupport;	
	};
}
#endif
