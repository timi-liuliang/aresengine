#include <iostream>
#include <Physics/HeightField.h>

BOOST_CLASS_EXPORT( Ares::HeightField)

namespace Ares
{
	// 构造函数
	HeightField::HeightField()
		: Shape( ST_HeightField)
	{
		m_version	 = 1;

		m_hollowWidth = 0;
		m_tableHeight = 0;

		m_hollowWidth = 0;
		m_hollowHeight= 0;
	}

	// 析构函数
	HeightField::~HeightField()
	{
		Reset();
	}

	// 重置
	void HeightField::Reset()
	{
	}

	// 初始化
	void HeightField::Init( UINT width, UINT height, float* heightData/*=NULL*/, UINT gridSize/*=1*/)
	{
		// 重置,使HeightField可以被多次初始化
		Reset();

		m_tableWidth  = width;
		m_tableHeight = height;
		m_gridSize	  = gridSize;

		// 顶点数
		m_heightData.resize( (width+1) * (height+1), 0.f);
		if( heightData)
		{
			for( size_t i=0; i<m_heightData.size(); i++)
				m_heightData[i] = heightData[i];
		}
	}

	// 设置镂空
	void HeightField::SetHollow( UINT width, UINT height, BYTE* hollowData/*=NULL*/)
	{
		m_hollowWidth = width;
		m_hollowHeight= height;

		// 顶点数
		m_hollowData.resize( (width+1) * (height+1), 0);
		if( hollowData)
		{
			for( size_t i=0; i<m_hollowData.size(); i++)
				m_hollowData[i] = hollowData[i];
		}
	}

	// 是否为空洞
	BYTE HeightField::GetFlag( UINT x, UINT y) const
	{
		if( m_hollowData.empty())
			return 0x0;

		x = (std::min)( x, m_hollowWidth);
		y = (std::min)( y, m_hollowHeight);

		UINT hollowIdx = y * m_hollowWidth + x;

		return m_hollowData[hollowIdx];
	}

	// 根据位置获取索引
	UINT HeightField::TableIdx( UINT x, UINT y) const
	{
		x = std::min<UINT>( x, m_tableWidth);
		y = std::min<UINT>( y, m_tableHeight);

		return y * ( m_tableWidth+1) + x;
	}

	// 获取某点高度
	float HeightField::GetHeightAt( UINT x, UINT y) const
	{
		UINT idx = TableIdx( x, y);

		return m_heightData[idx];
	}

	// 获取某点高度(根据索引)
	float HeightField::GetHeightAt( UINT idx)
	{
		return m_heightData[idx];
	}

	// 获取法线
	Vector3 HeightField::GetNormalAt( UINT x, UINT y)
	{
		// 计算法线(详情请参考:游戏编程精粹3-4.2 快速高度场法线的计算 P302)
		float h0 = GetHeightAt( x, y);
		float h1 = GetHeightAt( x+1,   y) - h0;
		float h2 = GetHeightAt(   x, y-1) - h0;
		float h3 = GetHeightAt( x-1,   y) - h0;
		float h4 = GetHeightAt(   x, y+1) - h0;

		Vector3 normal( h3 - h1, h2 - h4, 2);
		normal.Normalize();
		
		return normal;
	}

	// 设置某点高度
	void HeightField::SetHeightAt( UINT x, UINT y, float height)
	{
		UINT idx = TableIdx( x, y);

		m_heightData[idx] = height;
	}

	// 根据区域大小获取索引范围
	void HeightField::GetIdxRegion( int& minX, int& maxX, int& minY, int& maxY, const Rect3& region) const
	{
		maxX = ((int)region.m_max.x + m_gridSize) / m_gridSize;
		maxY = ((int)region.m_max.y + m_gridSize) / m_gridSize;
		minX = (int)region.m_min.x /  m_gridSize;
		minY = (int)region.m_min.y /  m_gridSize;

		maxX = std::min<int>( m_tableWidth, maxX);
		maxY = std::min<int>( m_tableHeight,maxY);
		minX = std::max<int>( 0, minX);
		minY = std::max<int>( 0, minY);
	}

	// 添加对应格子的两个三角形到数组中
	void HeightField::AddTri( vector<Triangle3>& triangles, UINT x, UINT y) const
	{
		// 构建三角形
		Triangle3 triangle;

		float hx1y1 = GetHeightAt( x+1, y+1);

		triangle.m_v[0].x = (float)x;
		triangle.m_v[0].y = (float)y;
		triangle.m_v[0].z =  GetHeightAt( x, y);
		triangle.m_v[1].x = (float)x+1;
		triangle.m_v[1].y = (float)y;
		triangle.m_v[1].z =  GetHeightAt( x + 1, y);
		triangle.m_v[2].x = (float)x+1;
		triangle.m_v[2].y = (float)y+1;
		triangle.m_v[2].z =  hx1y1;

		triangles.push_back( triangle);

		//triangle.m_v[1].x = (float)x+1;
		triangle.m_v[1].y = (float)y+1;
		triangle.m_v[1].z =  hx1y1;
		triangle.m_v[2].x = (float)x;
		//triangle.m_v[2].y = (float)y+1;
		triangle.m_v[2].z =  GetHeightAt( x, y+1);

		triangles.push_back( triangle);
	}

	// 获取三角形
	size_t HeightField::GetTriangle(vector<Triangle3>& triangles, const Rect3& region) const
	{
		// 清空
		triangles.clear();

		int minX, maxX, minY, maxY;
		GetIdxRegion( minX, maxX, minY, maxY, region);

		// 构建三角形
		Triangle3 triangle;

		// 检测
		for ( int y=maxY; y>=minY; y--)
		{
			for ( int x=maxX; x>=minX; x--)
				AddTri( triangles, x, y);
		}

		return triangles.size();
	}

	// 获取三角形(去除空洞)
	size_t HeightField::GetTriangleByFlag( vector<Triangle3>& triangles, const Rect3& region, BYTE flag) const
	{
		if( m_hollowData.empty())
			return GetTriangle( triangles, region);

		// 清空
		triangles.clear();

		int minX, maxX, minY, maxY;
		GetIdxRegion( minX, maxX, minY, maxY, region);

		// 构建三角形
		Triangle3 triangle;

		// 检测
		for ( int y=maxY; y>=minY; y--)
		{
			for ( int x=maxX; x>=minX; x--)
			{
				if( GetFlag( x, y) & flag)
					AddTri( triangles, x, y);
			}
		}

		return triangles.size();
	}

	// 构建AABB包围盒组
	void HeightField::BuildAABBArrayByXY( vector<Rect3>& aabbArrays, const Transform& trans, const Parabola3& parabola, float t0, float t1) const
	{
		aabbArrays.clear();

		Rect3   region;
		Vector3 beginPos, endPos, dirxy;
		parabola.BuildPos( beginPos, t0);
		parabola.BuildPos( endPos,	 t1);
		region.UnionPoint( beginPos);
		region.UnionPoint( endPos);
		dirxy = endPos - beginPos;

		int minX, maxX, minY, maxY;
		GetIdxRegion( minX, maxX, minY, maxY, region);
		int		splitNum = max( abs( maxX-minX) < abs( maxY-minY) ? abs( maxX-minX) : abs( maxY-minY), 1);
		Vector3 stepLen  = dirxy / (float)splitNum;
		for ( int i=0; i<splitNum; i++)
		{
			Rect3 aabb;
			aabb.UnionPoint( beginPos+(i+0.001f)*stepLen);
			aabb.UnionPoint( beginPos+(i+0.999f)*stepLen);

			aabbArrays.push_back( aabb);
		}
	}
}