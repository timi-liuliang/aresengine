#include <Physics/Parabola3.h>

namespace Ares
{
	// 构造函数
	Parabola3::Parabola3( const Vector3& orig,  const Vector3& v0, const Vector3& g)
		: m_orig( orig)
		, m_v0( v0)
		, m_g( g)
	{}

	// 构建AABB包围盒组
	void Parabola3::BuildAABBArray( vector<Rect3>& aabbArrays, const Transform& trans, float t0, float t1, size_t num) const
	{
		vector<Vector3> intePos;
		Interpolate( intePos, t0, t1, num);

		A_ASSERT( intePos.size() > 1);

		aabbArrays.resize( intePos.size()-1);
		for( size_t i=0; i<aabbArrays.size(); i++)
		{
			aabbArrays[i].Reset();
			aabbArrays[i].UnionPoint(intePos[i]);
			aabbArrays[i].UnionPoint(intePos[i+1]);
		}
	}

	// get turn point
	float Parabola3::GetTurnPos( Vector3& oPos)
	{
		float t = Vector3Dot( -m_v0, m_g) / m_g.Length();

		BuildPos( oPos, t);

		return t;
	}

	// build curent pos by time
	void Parabola3::BuildPos( Vector3& oPos, float t) const
	{
		oPos = m_orig + ( m_v0 + 0.5f * m_g * t) * t;
	}

	// interpolate
	void Parabola3::Interpolate( vector<Vector3>& oPos, float t0, float t1, size_t intepolateNum) const
	{
		intepolateNum = std::max<int>( 1, intepolateNum);
		oPos.resize( intepolateNum+1);

		float stepTime = ( t1 - t0) / std::max<int>( 1, intepolateNum);
		for( size_t i=0; i<=intepolateNum; i++)
		{
			BuildPos( oPos[i], stepTime * i);
		}
	}

	// 执行转换
	void Parabola3::DoTransform( const Transform& transform)
	{
		m_orig = transform.TransformVector3( m_orig);
		m_v0   = transform.m_rotation.Rotate( m_v0);
		m_g	   = transform.m_rotation.Rotate( m_g);
	}

	// 计算与点的距离
	float Parabola3::DistanceTo( const Vector3& p) const
	{
		return 1e30f;
	}
}