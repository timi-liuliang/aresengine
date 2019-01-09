#include <Engine/SoundSystem/AresSoundSystem.h>

#ifdef ARES_PLATFORM_WINDOWS
	#include <Engine/SoundSystem/AresWin32SoundSystem.h>
#endif

namespace Ares
{
	// 创建系统
	SoundSystem* SoundSystem::Create()
	{
#ifdef ARES_PLATFORM_WINDOWS
		return new_ Win32SoundSystem;
#else
		return (SoundSystem*)(0xFF);
#endif
	}
}