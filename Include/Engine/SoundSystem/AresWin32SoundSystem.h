#pragma once

#include <map>
#include <string>
#include <fmod.hpp>
#include <Core/AresMath.h>
#include "AresSoundSystem.h"

using namespace std;

namespace Ares
{
	//---------------------------------------
	// SoundSystem 2010-08-03  帝林
	//---------------------------------------
	class Win32SoundSystem : public SoundSystem
	{
		struct SSoundChannel
		{
			FMOD::Sound*	m_sound;
			FMOD::Channel*  m_channel;

			// 构造函数
			SSoundChannel()
			{
				m_sound   = NULL;
				m_channel = NULL;
			}
		};

		typedef map<string, SSoundChannel*>   SoundMap;

	public:
		Win32SoundSystem();
		~Win32SoundSystem();

	public:
		// 初始化
		virtual bool Init( int numsoftwarechannels);

		// 播放声音
		virtual bool PlaySound( const char* fileName);

		// 播放声音
		virtual bool PlaySound( ISound* sound){ return false;}

		// 加载声音
		virtual ISound* LoadSound( const char* fileName); 

		// 设置听众
		virtual bool Set3DListenerAttributes(  int listener, const Vector3& pos, const Vector3& velocity, const Vector3& forward, const Vector3& up);

	private:
		// 根据文件名获取声音
		SSoundChannel* GetSoundChannel( const char* fileName);

		// 新建声音及通道
		SSoundChannel* NewSoundChannel( const char* fileName);

	private:
		FMOD::System	*m_system;			// 声音管理器

		SoundMap         m_soundList;		// 声音列表
	};
}