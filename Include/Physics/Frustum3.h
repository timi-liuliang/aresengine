#pragma once

#include <Core/AresCore.h>
#include "Physics/Rect3.h"

using namespace std;

namespace Ares
{
	//-----------------------------------------------------
	// 改进后的视锥剔除算法 <<game programing gems 5>>
	//							    Page 52  
	//		2010-08-24   帝林
	//-----------------------------------------------------
	class Frustum3 : public Shape
	{
	public:
		enum Flags
		{
			EM_AABB_UPDATE =0,  // AABB包围盒需要更新
			EM_VERTEX_UPDATE,	// 八个顶点需要更新 
		};

		// 构造函数
		Frustum3();

		// 设置Near
		void  SetNear( const float fNear);

		// 获取Near
		float GetNear() const { return m_fNearZ; }

		// 设置Far
		void  SetFar( const float fFar);

		// 获取Far
		float GetFar() const { return m_fFarZ; }

		// 设置Perspective  （这些内容不轻易改变）
		void  SetPerspective( const float fovH, const float fAspect, const float fNear, const float fFar);

		// 构建
		void  Build( const Vector3& vEye, const Vector3& vForward, const Vector3& vUp, bool haveNormalize=false);

		// 计算运动物体惯性
		virtual Vector3 CalculateLocalInertia( float mass) { A_ASSERT(false); return Vector3::Zero; }

		// 获取AABB包围盒
		const Rect3& GetAABB();

		// 获取八个顶点
		const Vector3*  GetVertexs();

		// 构建AABB包围盒
		virtual void BuildAABB( Rect3& box, const Transform& trans) const;

		// 获取平面
		bool BuildPlane( vector<Vector3>& plane, float length);

		// 判断点是否在平截头体内
		bool  IsPointIn( const Vector3& point);

		// 判断球是否在平截头体内
		bool  IsSphereIn( const Vector3& center, const float fRadius);

		// 判断AABB包围盒
		bool  IsAABBIn( const Vector3& minPoint, const Vector3& maxPoint) const;

		// 获取右向量
		const Vector3& GetRight() const { return m_vRight; }

		// 获取上方向
		const Vector3& GetUp() const { return m_vUp; }

	private:
		Vector3	  m_vEyePosition,		// 视点
				  m_vForward,			// 前视向量
				  m_vRight,				// 右视向量
				  m_vUp;				// 上向量

		float     m_fRfactor,			// 右判断因子
				  m_fUfactor;			// 上判断因子

		float     m_fNearZ,				// 近截面
			      m_fFarZ;				// 远截面	

		Vector3   m_vVertexs[8]; 		
		                                    //     7+------+6
		                                    //     /|     /|         z
		                                    //    / |    / |         |
		                                    //   / 4+---/--+5		 |
		                                    // 3+------+2 /          *-----x
		                                    //  | /    | /          /
		                                    //  |/     |/          /
		                                    // 0+------+1		  y   		 
		Rect3		 m_AABB;				// 包围盒
		bitset<16>   m_flags;				// 标记
	};
}