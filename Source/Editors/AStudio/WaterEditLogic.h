#pragma once

#include <Engine/ResSystem/AresResSystem.h>

namespace Ares
{
	//-------------------------------------
	// 水摆放编辑逻辑	2011-05-27	帝林
	//-------------------------------------
	class CWaterEditLogic
	{
	public:
		enum WaterType
		{
			EM_WOW_WATER = 0,
			EM_GPU_GEMS_2_19,	// gpu gems 2 第19章节
			EM_WATER_MASK,		// 折射遮罩
			EM_MAX_WATER_TYPES,
		};

		// 构造函数
		CWaterEditLogic();

		// 新建水
		void New( const Vector3& pos, WaterType type=EM_GPU_GEMS_2_19);

	private:
		//SPtr<ISurfaceMaterial>  m_pMaterialWater[EM_MAX_WATER_TYPES];
	};
}