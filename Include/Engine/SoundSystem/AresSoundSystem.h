#ifndef ARES_SOUNDSYSTEM_H
#define ARES_SOUNDSYSTEM_H

#include <Core/AresMath.h>

namespace Ares
{
	//--------------------------------------
	// 声音盒 2011-07-15   帝林
	//--------------------------------------
	class  ISound
	{
	public:
		// 设置3D属性
		virtual bool Set3DAttributes( const Vector3& pos, const Vector3& velocity)=0;
	};

	//--------------------------------------
	// 声音盒 2011-07-15   帝林
	//--------------------------------------
	class  ISoundBox
	{
	public:
		// 设置3D属性
		virtual bool Set3DAttributes( const Vector3& pos, const Vector3& velocity)=0;
	};


	//-----------------------------------
	// 声音系统 2013-7-8 帝林
	//-----------------------------------
	class SoundSystem
	{
	public:
		SoundSystem(){}
		virtual ~SoundSystem(){}

		// 创建系统
		static SoundSystem* Create();

		// 初始化
		virtual bool Init( int numsoftwarechannels)=0;

		// 播放声音
		virtual bool PlaySound( const char* fileName)=0;

		// 播放声音
		virtual bool PlaySound( ISound* sound){ return false;}

		// 加载声音
		virtual ISound* LoadSound( const char* fileName)=0; 

		// 设置听众
		virtual bool Set3DListenerAttributes(  int listener, const Vector3& pos, const Vector3& velocity, const Vector3& forward, const Vector3& up)=0;		
	};
}

#endif