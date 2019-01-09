#pragma once

#include "AresFxModel.h"
#include "AresFxEmitter.h"
#include "AresFxParticleSystem.h"
#include "AresFxZone.h"
#include "AresFxModifier.h"
#include "SphericEmitter.h"
#include "PointZone.h"
#include "PlaneZone.h"
#include "Box3Zone.h"
#include "Obstacle.h"

namespace Ares
{
	//---------------------------
	// 粒子系统 2011-9-13 帝林
	//---------------------------
	class FxSystem
	{
//	public:
//		// 新建FxModel
//		FxModel* CreateModel( int enableFlag = FxModel::FLAG_RED | FxModel::FLAG_GREEN | FxModel::FLAG_BLUE, int mutableFlag = FxModel::FLAG_NONE, int randomFlag = FxModel::FLAG_NONE, int interpolateFlag = FxModel::FLAG_NONE);
//
//		// 创建球形粒子发射器
//		FxSphericEmitter* CreateSphericEmitter( const Vector3& dir = Vector3(0.f, 0.f, -1.f), float angleA=0.f,float angleB=0.f);
//
//		// 新建组
//		FxParticleSystem* CreateGroup( FxModel* model, size_t capacity);
//
//		// 创建点区域
//		FxPointZone* CreatePointZone( const Vector3& position = Vector3( 0.f, 0.f, 0.f));
//
//		// 创建平面区域
//		FxPlaneZone* CreatePlaneZone(const Vector3& position=Vector3(0.f,0.f,0.f), const Vector3& normal=Vector3(0.f,0.f,1.f));
//
//		// 创建障碍物
//		IFxObstacle* CreateObstacle( FxZone* zone=NULL, FxModifier::EModifierTrigger trigger=FxModifier::INTERSECT_ZONE, float bouncingRatio = 1.f, float friction=1.f);
//
//	public:
//		// 创建PointRender
//		FxPointRender* CreatePointRender();
//
//		// 创建QuadRender
//		FxQuadRender* CreateQuadRender();

	private:
	};
}