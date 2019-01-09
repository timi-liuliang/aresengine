#include <Physics/Frustum3.h>

namespace Ares
{
	// 构造函数
	Frustum3::Frustum3()
		: Shape( ST_Frustum)
	{
		// do nothing
	}

	// 设置Near
	void  Frustum3::SetNear( const float fNear) 
	{
		m_fNearZ = fNear; 

		m_flags.set(EM_AABB_UPDATE);
		m_flags.set(EM_VERTEX_UPDATE);
	}

	// 设置Far
	void  Frustum3::SetFar( const float fFar) 
	{ 
		m_fFarZ = fFar; 

		m_flags.set(EM_AABB_UPDATE);
		m_flags.set(EM_VERTEX_UPDATE);
	}

	// 设置Perspective  （这些内容不轻易改变）
	void  Frustum3::SetPerspective( const float fovH, const float fAspect, const float fNear, const float fFar)
	{
		m_fUfactor = tanf( fovH);
		m_fRfactor = m_fUfactor * fAspect;
		m_fNearZ   = fNear;
		m_fFarZ    = fFar;

		m_flags.set(EM_AABB_UPDATE);
		m_flags.set(EM_VERTEX_UPDATE);
	}

	// 构建
	void Frustum3::Build(const Vector3& vEye, const Vector3& vForward, const Vector3& vUp, bool haveNormalize/* =false */)
	{
		m_vEyePosition = vEye;
		m_vForward     = vForward;
		m_vUp		   = vUp;

		m_vForward.Normalize();
		m_vUp.Normalize();

		// 计算up
		m_vRight = Vector3Cross( m_vUp, m_vForward);		m_vRight.Normalize();
		m_vUp	 = Vector3Cross( m_vForward, m_vRight);		m_vUp.Normalize();

		m_flags.set(EM_AABB_UPDATE);
		m_flags.set(EM_VERTEX_UPDATE);
	}

	// 获取八个顶点
	const Vector3*  Frustum3::GetVertexs()
	{
		if ( !m_flags.test( EM_VERTEX_UPDATE))
			return m_vVertexs;

		// 更新顶点
		Vector3  n   = m_vForward * m_fNearZ,	
			     f   = m_vForward * m_fFarZ,			// 前向(须保证 m_vForward为单位向量)

			     nr  = m_vRight * m_fNearZ * m_fRfactor,	
				 nu  = m_vUp * m_fNearZ * m_fUfactor,	// 

				 fr  = m_vRight * m_fFarZ * m_fRfactor,
				 fu  = m_vUp * m_fFarZ * m_fUfactor;

		m_vVertexs[0] = n - nr - nu;		m_vVertexs[4] = f - fr - fu;
		m_vVertexs[1] = n + nr - nu;		m_vVertexs[5] = f + fr - fu;
		m_vVertexs[2] = n + nr + nu;		m_vVertexs[6] = f + fr + fu;
		m_vVertexs[3] = n - nr + nu;		m_vVertexs[7] = f - fr + fu;

		m_vVertexs[0] += m_vEyePosition;	m_vVertexs[4] += m_vEyePosition;
		m_vVertexs[1] += m_vEyePosition;	m_vVertexs[5] += m_vEyePosition;
		m_vVertexs[2] += m_vEyePosition;	m_vVertexs[6] += m_vEyePosition;
		m_vVertexs[3] += m_vEyePosition;	m_vVertexs[7] += m_vEyePosition;

		//     7+------+6
		//     /|     /|         z
		//    / |    / |         |
		//   / 4+---/--+5		 |
		// 3+------+2 /          *-----x
		//  | /    | /          /
		//  |/     |/          /
		// 0+------+1		  y   	

		m_flags.reset( EM_VERTEX_UPDATE);

		return m_vVertexs;
	}

	// 构建AABB包围盒
	void Frustum3::BuildAABB( Rect3& box, const Transform& trans) const
	{
		for ( size_t i=0; i<8; i++)
		{
			box.UnionPoint( m_vVertexs[i]);
		}
	}

	// 获取平面
	bool Frustum3::BuildPlane( vector<Vector3>& plane, float length)
	{
		// 更新顶点
		Vector3  n   = m_vForward * length;
		Vector3	 nr  = m_vRight * length * m_fRfactor;
		Vector3	 nu  = m_vUp * length * m_fUfactor;


		plane[0] = n - nr - nu;
		plane[1] = n + nr - nu;
		plane[2] = n + nr + nu;
		plane[3] = n - nr + nu;

		plane[0] += m_vEyePosition;
		plane[1] += m_vEyePosition;
		plane[2] += m_vEyePosition;
		plane[3] += m_vEyePosition;

		return true;
	}

	// 获取AABB包围盒
	const Rect3& Frustum3::GetAABB() 
	{
		if( !m_flags.test( EM_AABB_UPDATE))
			return m_AABB;

		GetVertexs();

		m_AABB.Reset();

		m_AABB.UnionPoint( m_vVertexs[0]);		m_AABB.UnionPoint( m_vVertexs[4]);
		m_AABB.UnionPoint( m_vVertexs[1]);		m_AABB.UnionPoint( m_vVertexs[5]);
		m_AABB.UnionPoint( m_vVertexs[2]);		m_AABB.UnionPoint( m_vVertexs[6]);
		m_AABB.UnionPoint( m_vVertexs[3]);		m_AABB.UnionPoint( m_vVertexs[7]);

		m_flags.reset( EM_AABB_UPDATE);

		return m_AABB;
	}

	// 判断点是否在平截头体内
	bool  Frustum3::IsPointIn( const Vector3& point)
	{
		Vector3 op = point - m_vEyePosition;

		// 前投影
		float f = Vector3Dot( op, m_vForward);
		if ( f<m_fNearZ || f > m_fFarZ) return false;

		// 右投影
		float r = Vector3Dot( op, m_vRight);
		float rLimit = m_fRfactor * f;
		if ( r < -rLimit || r > rLimit) return false;

		// 上投影
		float u = Vector3Dot( op, m_vUp);
		float uLimit = m_fUfactor * f;
		if ( u < -uLimit || u > uLimit) return false;

		return true;
	}

	// 判断球是否在平截头体内
	bool Frustum3::IsSphereIn( const Vector3& center, const float fRadius)
	{
		Vector3 op = center - m_vEyePosition;

		// 前投影
		float f = Vector3Dot( op, m_vForward);
		if ( f < m_fNearZ - fRadius || f > m_fFarZ + fRadius) return false;
	
		// 右投影
		float r = Vector3Dot( op, m_vRight);
		float rLimit = m_fRfactor * f;
		float rTop = rLimit + fRadius;
		if ( r < -rTop || r > rTop) return false;

		// 上投影
		float u = Vector3Dot( op, m_vRight);
		float uLimit = m_fUfactor * f;
		float uTop = uLimit + fRadius;
		if ( u < -uTop || u > uTop) return false;

		// 相交或在完全在平截头体内
		return true;
	}

	// 判断AABB包围盒
	bool Frustum3::IsAABBIn( const Vector3& minPoint, const Vector3& maxPoint) const
	{
		Vector3 p;
		int nOutofLeft = 0, nOutofRight=0, nOutofNear=0, nOutofFar = 0, nOutofTop = 0, nOutofBottom = 0;
		bool bIsInRightTest, bIsInUpTest, bIsInFrontTest;

		Vector3 corners[2];
		corners[0] = minPoint - m_vEyePosition;
		corners[1] = maxPoint - m_vEyePosition;

		for ( int i=0; i<8; i++)
		{
			bIsInRightTest = bIsInUpTest = bIsInFrontTest = false;

			p.x = corners[i&1].x;
			p.y = corners[(i>>2)&1].y;
			p.z = corners[(i>>1)&1].z;

			// 点积运算
			float r = m_vRight.x * p.x + m_vRight.y * p.y + m_vRight.z * p.z;
			float u = m_vUp.x * p.x + m_vUp.y * p.y + m_vUp.z * p.z;
			float f = m_vForward.x * p.x + m_vForward.y * p.y + m_vForward.z * p.z;

			if ( r < -m_fRfactor * f) ++nOutofLeft;
			else if( r > m_fRfactor * f) ++nOutofRight;
			else bIsInRightTest = true;

			if (u < -m_fUfactor*f) ++nOutofBottom;
			else if (u > m_fUfactor*f) ++nOutofTop;
			else bIsInUpTest=true;

			if (f < m_fNearZ) ++nOutofNear;		
			else if (f > m_fFarZ) ++nOutofFar;
			else bIsInFrontTest=true;

			if( bIsInRightTest && bIsInFrontTest && bIsInUpTest) return true;
		}

		if( nOutofLeft==8 || nOutofRight==8 || nOutofFar==8 || nOutofNear==8 || nOutofTop==8 || nOutofBottom==8) return false;

		return true;
	}
}