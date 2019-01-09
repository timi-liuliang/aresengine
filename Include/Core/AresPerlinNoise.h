#pragma once

#include "AresCore.h"

namespace Ares
{
	//----------------------------------
	//	2010-03-09	17:48   帝林    
	//  柏林噪音实现(二维) 
	//-----------------------------------
	class CPerlinNoise
	{
	public:
		enum
		{
			EM_TABLE_SIZE =  256,
			EM_TABLE_MASK = EM_TABLE_SIZE - 1,
		};

		// 构造函数
		CPerlinNoise();

		// 起躁
		float Noise(int x, int y, float scale);

		// 起躁
		float Noise(float x, float y, float scale);

	private:
		// 私有函数
		void SetUp();

		// 获取随机单位向量 只要x y 值不变,返回值也不变!这样我们就不需要为每个顶点存储一个法线向量
		const Vector2* GetVec(int x, int y) const;		

	private:
		Vector2		  m_vTable[EM_TABLE_SIZE];
		unsigned char m_lut[EM_TABLE_SIZE];
	};
}