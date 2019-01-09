//#include "AresEffect.h"
//#include "AresResource.h"
//#include "ParticleEmitter.h"
//
//namespace Ares
//{
//	// 全局变量
//	IResourceManager* g_pResManager = NULL;
//	LPDIRECT3DDEVICE9 g_pDevice=NULL;			// 待封装
//
//	// 初始化特效模块
//	EFFECT_API void AresEffectModuleInitialize(LPDIRECT3DDEVICE9 pDevice, IResourceManager* pResourceMgr)
//	{
//		g_pResManager = pResourceMgr;
//		g_pDevice     = pDevice;
//	}
//
//
//	// 根据类型返回粒子发射器
//	EFFECT_API CPtr<IParticleEmitter> AresCreateParticleEmitter()
//	{
//		return new CParticleEmitter();
//	}
//}