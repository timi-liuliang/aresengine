#ifndef ARES_CLOTH_FABRIC_BUILDER_H
#define ARES_CLOTH_FABRIC_BUILDER_H

#include <vector>
#include <Core/AresTransform.h>
#include <Physics/MassSpringSystem.h>
#include "AresClothFabric.h"

using namespace std;

namespace Ares
{
	// GridClothBuilder 2013-7-11
	class GridClothBuilder
	{
	public:
		// 构造函数
		GridClothBuilder( ClothFabric& cloth, int width, int height, const Transform& transform);

	private:
		// 辅助函数,获取索引
		bool GetMassIndex( int& index, int w, int h);

		// 添加弹簧
		void AddSpring( MassSpringSystem::Spring& spring);

	public:
		int									m_width;			// 宽
		int									m_height;			// 高
		vector<WORD>						m_triIndices;		// 三角形索引
		vector<MassSpringSystem::MassPoint>	m_massPoints;		// 质点
		vector<MassSpringSystem::Spring>	m_springs;			// 弹簧
	};
}

#endif