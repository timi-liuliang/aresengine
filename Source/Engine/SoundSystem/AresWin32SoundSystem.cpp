#include <Engine/SoundSystem/AresWin32SoundSystem.h>

namespace Ares
{
	// 构造函数
	Win32SoundSystem::Win32SoundSystem()
	{
		m_system = NULL;
	}

	// 析构函数
	Win32SoundSystem::~Win32SoundSystem()
	{
		// 释放声音资源
		for( SoundMap::iterator it = m_soundList.begin(); it != m_soundList.end(); it++)
		{
			if( FMOD::Sound* sound = it->second->m_sound)
				sound->release();
		}

		if( m_system)
		{
			m_system->close();
			m_system->release();
		}
	}

	// 初始化
	bool Win32SoundSystem::Init( int numsoftwarechannels)
	{
		// Create a system object and initialize
		FMOD::System_Create( &m_system);

		if( !m_system)
			return false;

		// Sets the maximum number of software mixed channels possible
		m_system->setSoftwareChannels( numsoftwarechannels);
		m_system->init( numsoftwarechannels, FMOD_INIT_NORMAL, NULL);

		return true;
	}

	// 播放声音
	bool Win32SoundSystem::PlaySound( const char* fileName)
	{
		SSoundChannel* soundChannel = GetSoundChannel( fileName);

		if( !soundChannel)
			soundChannel = NewSoundChannel( fileName);

		if( soundChannel)
		{
			m_system->playSound( FMOD_CHANNEL_FREE, soundChannel->m_sound, false, &soundChannel->m_channel);

			return true;
		}

		return false;
	}

	// 根据文件名获取声音
	Win32SoundSystem::SSoundChannel* Win32SoundSystem::GetSoundChannel( const char* fileName)
	{
		if( !fileName)
			return NULL;

		SoundMap::iterator it = m_soundList.find( fileName);

		if( it == m_soundList.end())
			return NULL;

		return it->second;
	}

	// 新建声音及通道
	Win32SoundSystem::SSoundChannel* Win32SoundSystem::NewSoundChannel( const char* fileName)
	{
		SSoundChannel* newSC = new_ SSoundChannel;

		// 创建
		m_system->createStream( fileName, FMOD_3D | FMOD_HARDWARE | FMOD_LOOP_NORMAL, 0, &newSC->m_sound);

		// 加入列表
		m_soundList.insert( make_pair( fileName, newSC));

		return newSC;
	}

	// 加载声音
	ISound* Win32SoundSystem::LoadSound( const char* fileName)
	{
		SSoundChannel* newSC = new_ SSoundChannel;

		// 创建
		m_system->createStream( fileName, FMOD_3D | FMOD_HARDWARE | FMOD_LOOP_NORMAL, 0, &newSC->m_sound);

		// 加入列表
		m_soundList.insert( make_pair( fileName, newSC));

		return NULL;
	}

	// 设置听众
	bool Win32SoundSystem::Set3DListenerAttributes(  int listener, const Vector3& pos, const Vector3& velocity, const Vector3& forward, const Vector3& up)
	{
		return false;
	}
}