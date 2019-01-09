#pragma once

#include <bitset>
#include "AresMath.h"

using namespace std;

namespace Ares
{
	////-----------------------------------------------
	//// RTTransform 2012-11-13 帝林
	//// Transforms with only translation and roation
	////-----------------------------------------------
	//struct RTTransform
	//{
	//public:
	//	// 构造函数
	//	RTTransform();

	//	// 设置原点
	//	void SetOrigin( const Vector3& origin) { m_origin = origin; }

	//	// 设置旋转
	//	void SetRotation( const Quat& quat) { m_rotation = quat; }

	//	// 世界矩阵
	//	Matrix44 GetMatrix() const;

	//	// 序列化
	//	template <typename Archive>
	//	void serialize( Archive& ar, const unsigned int version)
	//	{
	//		ar & m_rotation;
	//		ar & m_origin;
	//	}

	//protected:
	//	Quat		m_rotation;			// 旋转
	//	Vector3		m_origin;			// storage for translation
	//};

}