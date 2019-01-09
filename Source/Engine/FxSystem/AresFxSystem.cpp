//#include <Engine/FxSystem/AresFxSystem.h>
//#include <Engine/FxSystem/Model.h>
//#include <Engine/FxSystem/AresFxFlow.h>
//#include "SPhericEmitter.h"
//#include "PointZone.h"
//#include "PlaneZone.h"
//#include <Engine/FxSystem/Obstacle.h>
//#include <Engine/FxSystem/AresFxPointRender.h>
//
//namespace Ares
//{
//	// 新建FxModel
//	FxModel* FxSystem::CreateModel( int enableFlag, int mutableFlag, int randomFlag, int interpolateFlag)
//	{
//		FxModel* model = new_ FxModel( enableFlag, mutableFlag, randomFlag, interpolateFlag);
//	
//		return model;
//	}
//
//	// 创建球形粒子发射器
//	FxSphericEmitter* FxSystem::CreateSphericEmitter(  const Vector3& dir, float angleA, float angleB)
//	{
//		FxSphericEmitter* emitter = new_ FxSphericEmitter( dir, angleA, angleB);
//
//		return emitter;
//	}
//
//	// 新建组
//	FxParticleSystem* FxSystem::CreateGroup(  FxModel* model, size_t capacity)
//	{
//		//FxFlow* group = new FxFlow( model, capacity);
//
//		//return group;
//		return NULL;
//	}
//
//	// 创建点区域
//	FxPointZone* FxSystem::CreatePointZone( const Vector3& position)
//	{
//		FxPointZone* zone = new_ FxPointZone( position);
//
//		return zone;
//	}
//
//	// 创建平面区域
//	FxPlaneZone* FxSystem::CreatePlaneZone(const Vector3& position, const Vector3& normal)
//	{
//		FxPlaneZone* zone = new_ FxPlaneZone( position, normal);
//		
//		return zone;
//	}
//
//	// 创建障碍物
//	IFxObstacle* FxSystem::CreateObstacle( FxZone* zone, FxModifier::EModifierTrigger trigger, float bouncingRatio, float friction)
//	{
//		CObstacle* obstacle = new_ CObstacle( zone, trigger, bouncingRatio, friction);
//
//		return obstacle;
//	}
//
//	// 创建PointRender
//	FxPointRender* FxSystem::CreatePointRender()
//	{
//		FxPointRender* pointRender = new_ FxPointRender;
//
//		A_ASSERT( pointRender);
//
//		return pointRender;
//	}
//
//	// 创建QuadRender
//	FxQuadRender* FxSystem::CreateQuadRender()
//	{
//		FxQuadRender* quadRender = new_ FxQuadRender;
//
//		A_ASSERT( quadRender);
//
//		return quadRender;
//	}
//}