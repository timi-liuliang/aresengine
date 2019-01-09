#include "Physics/Rect3.h"

BOOST_CLASS_EXPORT( Ares::Rect3)

namespace Ares
{
	const Rect3  Rect3::Infinity = Rect3( Vector3( -FLT_BIG, -FLT_BIG, -FLT_BIG), Vector3( FLT_BIG, FLT_BIG, FLT_BIG));

	// 2D区域构造函数
	Rect2::Rect2()
	{
		Reset();
	}

	// 重置
	void Rect2::Reset()
	{
		x0 =  FLT_MAX;
		x1 = -FLT_MAX;

		y0 =  FLT_MAX;
		y1 = -FLT_MAX;
	}

	// 顶点是否在2D区域内
	bool Rect2::IsPointIn( float x, float y)
	{
		if( x>=x0 && x<=x1 && y>=y0 && y<=y1)
			return true;

		return false;
	}

	// 2D区域构造函数
	Rect2::Rect2(float _x0, float _x1, float _y0, float _y1)
	{
		x0 = _x0;
		x1 = _x1;
		y0 = _y0;
		y1 = _y1;
	}

	// 重载运算符"="
	void  Rect2::operator = (const Rect2& src)
	{
		x0 = src.x0;
		x1 = src.x1;
		y0 = src.y0;
		y1 = src.y1;

		// 注：memcpy(this, src, sizeof(CRect2D)); 较慢, 故不采用
	}

	// 重载符号  "="
	void Rect2::operator = (const Rect3& src)
	{
		x0 = src.m_min.x;
		x1 = src.m_max.x;
		y0 = src.m_min.y;
		y1 = src.m_max.y;
	}

	// 合并点
	void Rect2::UnionPoint( float x, float y)
	{
		x0 = std::min<FLOAT>( x0, x);
		y0 = std::min<FLOAT>( y0, y);

		x1 = std::max<FLOAT>( x1, x);
		y1 = std::max<FLOAT>( y1, y);
	}

	// 构造函数
	Rect3::Rect3()
		: Shape( ST_Rect3)
	{
		Reset();
	}

	// 重置
	void Rect3::Reset()
	{
        m_min = Vector3( FLT_BIG, FLT_BIG, FLT_BIG);
        m_max = Vector3(-FLT_BIG,-FLT_BIG,-FLT_BIG);
	}

	// 复制构造函数
	Rect3::Rect3(const Rect3& src)
		: Shape( ST_Rect3)
	{
        m_min = src.m_min;
        m_max = src.m_max;
	}

	// 构造函数
	Rect3::Rect3( const Vector3& v0, const Vector3& v1)
		: Shape( ST_Rect3)
	{
		Reset();
		UnionPoint( v0);
		UnionPoint( v1);
	}

	// 设置
	void Rect3::Set(float _x0, float _x1, float _y0, float _y1, float _z0, float _z1)
	{
        m_min = Vector3( _x0, _y0, _z0);
        m_max = Vector3( _x1, _y1, _z1);
	}

	// 顶点是否在2D区域内
	bool Rect3::IsPointIn( const Vector3& p) const
	{
		if( p.x<m_min.x || p.x>m_max.x || p.y<m_min.y || p.y>m_max.y || p.z<m_min.z || p.z>m_max.z)
			return false;

		return true;
	}

	// AABB是否在区域内
	bool Rect3::IsContain( const Rect3& aabb) const
	{
		return (m_min.x<=aabb.m_min.x) && (m_min.y<=aabb.m_min.y) && (m_min.z<=aabb.m_min.z) &&
			   (m_max.x>=aabb.m_max.x) && (m_max.y>=aabb.m_max.y) && (m_max.z>=aabb.m_max.z);
	}

	// 重载等号  "="
	void Rect3::operator = (const Rect2& src)
	{
        m_min.x = src.x0;
        m_min.y = src.y0;
        m_max.x = src.x1;
        m_max.y = src.y1;
	}

	// CRect3D 重载 "=" 
	void  Rect3::operator = (const Rect3& src)
	{
        m_min = src.m_min;
        m_max = src.m_max;

		// 注：memcpy(this, src, sizeof(CRect3D)); 较慢, 故不采用
	}

	// 重载运算符 "+="
	const Rect3& Rect3::operator += (const Vector3& vec)
	{
        m_min += vec;
        m_max += vec;

		return *this;
	}

	// 返回盒子区域尺寸
	Vector3 Rect3::GetSize()
	{
		return m_max - m_min;
	}

	// 获取八个点
	void Rect3::GetEightPoints( vector<Vector3>& points) const
	{
		points[0].x = m_min.x;	points[0].y = m_min.y;	points[0].z = m_min.z;
		points[1].x = m_min.x;	points[1].y = m_max.y;	points[1].z = m_min.z;
		points[2].x = m_max.x;	points[2].y = m_max.y;	points[2].z = m_min.z;
		points[3].x = m_max.x;	points[3].y = m_min.y;	points[3].z = m_min.z;

		points[4].x = m_min.x;	points[4].y = m_min.y;	points[4].z = m_max.z;
		points[5].x = m_min.x;	points[5].y = m_max.y;	points[5].z = m_max.z;
		points[6].x = m_max.x;	points[6].y = m_max.y;	points[6].z = m_max.z;
		points[7].x = m_max.x;	points[7].y = m_min.y;	points[7].z = m_max.z;
	}

	// 返回表面积
	float Rect3::GetSurfaceArea() const
	{
		return ( GetXSize() * GetYSize() + GetXSize() * GetZSize() + GetYSize() * GetZSize()) * 2.f;
	}

	// 合并X
	void Rect3::UnionX( float x)
	{
		m_min.x = std::min<FLOAT>( m_min.x, x);
		m_max.x = std::max<FLOAT>( m_max.x, x);
	}

	// 合并Y
	void Rect3::UnionY( float y)
	{
		m_min.y = std::min<FLOAT>( m_min.y, y);
		m_max.y = std::max<FLOAT>( m_max.y, y);
	}

	// 合并Z
	void Rect3::UnionZ( float z)
	{
		m_min.z = std::min<FLOAT>( m_min.z, z);
		m_max.z = std::max<FLOAT>( m_max.z, z);
	}

	// 合并点
	void Rect3::UnionPoint(const Vector3& point)
	{
		UnionPoint( point.x, point.y, point.z);
	}

	// 合并点
	void Rect3::UnionPoint( float x, float y, float z)
	{
		UnionX( x);
        UnionY( y);
        UnionZ( z);
	}

	// 被矩阵作用
	void Rect3::TransformByMatrix( const Matrix44& matrix)
	{
		Vector3 basePoint( m_min);
		Vector3 xLeg( m_max.x, m_min.y, m_min.z);
		Vector3 yLeg( m_min.x, m_max.y, m_min.z);
		Vector3 zLeg( m_min.x, m_min.y, m_max.z);

		basePoint = matrix.TransformVector3( basePoint);
		xLeg	  = matrix.TransformVector3( xLeg);
		yLeg	  = matrix.TransformVector3( yLeg);
		zLeg	  = matrix.TransformVector3( zLeg);

		xLeg -= basePoint;
		yLeg -= basePoint;
		zLeg -= basePoint;

		Vector3 farPoint( basePoint + xLeg + yLeg + zLeg);

		Set( basePoint.x,farPoint.x, basePoint.y,farPoint.y, basePoint.z,farPoint.z);

		UnionPoint(basePoint + xLeg);
		UnionPoint(basePoint + yLeg);
		UnionPoint(basePoint + zLeg);
		UnionPoint(basePoint + xLeg + yLeg);
		UnionPoint(basePoint + yLeg + zLeg);
		UnionPoint(basePoint + zLeg + xLeg);
	}

	// 构建AABB包围盒
	void Rect3::BuildAABB( Rect3& box, const Transform& trans) const
	{
		box = *this;
		box.TransformByMatrix( trans.GetMatrix());
	}

	// 合并,静态方法
	void Rect3::Merge( Rect3& result, const Rect3& inA, const Rect3& inB)
	{
		result = inA;
		result.UnionPoint( inB.GetMinPoint());
		result.UnionPoint( inB.GetMaxPoint());
	}
}