#pragma once

#include "Triangle3.h"
#include "Parabola3.h"
#include "Physics/Rect3.h"
#include <vector>

using namespace std;

namespace Ares
{
	//------------------------------------
	// HeightField   2011-08-05 帝林
	//------------------------------------
	class HeightField : public Shape
	{
	public:
		HeightField();
		~HeightField();

		// 初始化
		void Init( UINT width, UINT height, float* heightData=NULL, UINT gridSize=1);

		// 设置镂空
		void SetHollow( UINT width, UINT height, BYTE* hollowData=NULL);

		// 获取某点高度
		float GetHeightAt( UINT x, UINT y) const;

		// 获取某点高度(根据索引)
		float GetHeightAt( UINT idx);

		// 获取法线
		Vector3 GetNormalAt( UINT x, UINT y);

		// 获取三角形
		size_t GetTriangle(vector<Triangle3>& triangles, const Rect3& region) const;

		// 获取三角形(去除空洞)
		size_t GetTriangleByFlag( vector<Triangle3>& triangles, const Rect3& region, BYTE flag) const;

		// 添加对应格子的两个三角形到数组中
		void AddTri( vector<Triangle3>& triangles, UINT x, UINT y) const;

		// 设置格子大小
		void SetGridSize( UINT size=1) { m_gridSize = size; }

		// 设置某点高度
		void  SetHeightAt( UINT x, UINT y, float height);

		// 获取宽(列数)
		UINT GetWidth() const { return m_tableWidth; }

		// 获取高(行数)
		UINT GetHeight() const { return m_tableHeight; }

		// 构建AABB包围盒
		void BuildAABB( Rect3& box, const Transform& trans) const{}

		// 构建AABB包围盒组
		void BuildAABBArrayByXY( vector<Rect3>& aabbArrays, const Transform& trans, const Parabola3& parabola, float t0, float t1) const;

		// 计算运动物体惯性
		virtual Vector3 CalculateLocalInertia( float mass) { A_ASSERT(false); return Vector3::Zero; }

		// 序列化
		template <typename Archive> void serialize( Archive& ar, const unsigned int version)
		{
			ar & boost::serialization::base_object<Shape>(*this);
			ar & m_version;
			ar & m_gridSize;
			ar & m_tableWidth;
			ar & m_tableHeight;
			ar & m_heightData;
			ar & m_hollowWidth;
			ar & m_hollowHeight;
			ar & m_hollowData;
		}

	private:
		// 索引
		UINT TableIdx( UINT x, UINT y) const;

		// 是否为空洞
		BYTE GetFlag( UINT x, UINT y) const;

		// 根据区域大小获取索引范围
		void GetIdxRegion( int& minX, int& maxX, int& minY, int& maxY, const Rect3& region) const;

		// 重置
		void Reset();
		
	private:
		int				m_version;			// 版本
		UINT			m_gridSize;			// 格子大小
		UINT			m_tableWidth;		// 行数
		UINT			m_tableHeight;		// 列数
		vector<float>	m_heightData;		// 顶点高度数据

		UINT			m_hollowWidth;		// 镂空宽度
		UINT			m_hollowHeight;		// 镂空高度
		vector<BYTE>	m_hollowData;		// 镂空数据
	};
}