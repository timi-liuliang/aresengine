//#pragma once
//
//#define EFFECT_API __declspec(dllexport)
//
//#include <AresEntity.h>
//#include <AresResource.h>
//
//namespace Ares
//{
//	//---------------------------------------------
//	// 粒子发射器 2011-04-02 帝林
//	//---------------------------------------------
//	class EFFECT_API IParticleEmitter : public IRenderEntityGeneral
//	{
//	public:
//		// 初始化
//		virtual bool Initialize(char* cTextureFileName)=0;
//	};
//
//	// 特效模块初始化
//	EFFECT_API void AresEffectModuleInitialize(LPDIRECT3DDEVICE9 pDevice, IResourceManager* pResourceMgr);
//	EFFECT_API CPtr<IParticleEmitter> AresCreateParticleEmitter(); 
//}
//
//
//
