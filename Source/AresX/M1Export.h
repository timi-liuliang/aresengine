#pragma once

#include "AeConfig.h"
#include "Function.h"
#include <string>
#include <Engine/RenderSystem/M1/M1Loader.h>

using namespace std;
using namespace Ares;

namespace M1_IGame
{
	struct SExportM1
	{
	public:
		// Save an M1 model
		bool Save();

	public:
		IDCardModel				m_m1;			// 配置
		CM1Model		m_model;		// 模型
		CM1Material		m_material;		// 材质	
		CM1Animation	m_animation;	// 动画
		CM1CollMesh		m_collidedTri;	// 碰撞简易网格
	};
}