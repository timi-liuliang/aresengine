#include "Core/AresMath.h"

// Platform specific vector intrinsics include
#if ENABLE_VECTORINTRINSICS
#include <Core/AresMathSSE.h>
#else
#include <Core/AresMathFpu.h>
#endif

namespace Ares
{
	const Vector3  Vector3::Zero	= Vector3( 0.f, 0.f, 0.f);
	const Vector3  Vector3::XAxis	= Vector3( 1.f, 0.f, 0.f);		// 默认X轴
	const Vector3  Vector3::YAxis	= Vector3( 0.f, 1.f, 0.f);		// 默认Y轴
	const Vector3  Vector3::ZAxis	= Vector3( 0.f, 0.f, 1.f);		// 默认Z轴

	const Quat		  Quat::Identity= Quat( 0.f, 0.f, 0.f, 1.f);

	const Matrix44 Matrix44::Zero     = Matrix44( 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f);	// 单位矩阵
	const Matrix44 Matrix44::Identity = Matrix44( 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f);	// 单位矩阵

	const ColorRGB ColorRGB::Red   = ColorRGB( 1.f, 0.f, 0.f);		// 红
	const ColorRGB ColorRGB::Green = ColorRGB( 0.f, 1.f, 0.f);		// 绿
	const ColorRGB ColorRGB::Blue  = ColorRGB( 0.f, 0.f, 1.f);		// 蓝
	const ColorRGB ColorRGB::White = ColorRGB( 1.f, 1.f, 1.f);		// 白
	const ColorRGB ColorRGB::Black = ColorRGB( 0.f, 0.f, 0.f);		// 黑

	// Concatenation operator
	Matrix44 Matrix44::operator* ( const Matrix44& other) const
	{
		Matrix44 result;

		VectorMatrixMultiply( &result, this, &other);

		return result;
	}

	// Concatenation operator
	Matrix44 Matrix44::operator*=( const Matrix44& other)
	{
		VectorMatrixMultiply( this, this, &other);

		return *this;
	}

	// Inverse
	// Fast path, doesn't check for nil matrices in final release builds
	const Matrix44& Matrix44Inverse( Matrix44& result, const Matrix44& src)
	{
		VectorMatrixInverse( &result, &src);

		return result;
	}

	// Matrix44ViewDirLH
	void Matrix44ViewDirLH( Matrix44& result, const Vector3& eye, const Vector3& viewDir, const Vector3& up)
	{
		Vector3 zAxis = viewDir;						zAxis.Normalize();		
		Vector3 xAxis = Vector3Cross( up, zAxis);		xAxis.Normalize();
		Vector3 yAxis = Vector3Cross( zAxis, xAxis);	yAxis.Normalize();

		Matrix44ViewLH( result, xAxis, yAxis, zAxis, eye);
	}

	// Matrix44LookAtLH
	void Matrix44ViewAtLH( Matrix44& result, const Vector3& eye, const Vector3& viewAt, const Vector3& up)
	{
		Vector3 zAxis = viewAt - eye;					zAxis.Normalize();		
		Vector3 xAxis = Vector3Cross( up, zAxis);		xAxis.Normalize();
		Vector3 yAxis = Vector3Cross( zAxis, xAxis);	yAxis.Normalize();

		Matrix44ViewLH( result, xAxis, yAxis, zAxis, eye);
	}

	// Matrix44OrthoLH
	void Matrix44OrthoLH( Matrix44& result, float w, float h, float nearZ, float farZ)
	{
		Matrix44OrthoLH_D3DX( result, w, h, nearZ, farZ);
	}

	// Matrix44OrthoLH_D3DX
	void Matrix44OrthoLH_D3DX( Matrix44& result, float w, float h, float nearZ, float farZ)
	{
		result.SetIdentity();

		FLOAT xScale = 2.f / w;
		FLOAT yScale = 2.f / h;
		FLOAT zScale =    1.f / (farZ-nearZ);
		FLOAT zOffset= -nearZ / (farZ-nearZ);

		result._11 = xScale;
		result._22 = yScale;
		result._33 = zScale;
		result._43 = zOffset;
	}

	// Matrix44OrthoLH_OpenGL
	void Matrix44OrthoLH_OpenGL( Matrix44& result, float w, float h, float nearZ, float farZ)
	{

	}

	// Matrix44PerspectiveFovLH_D3DX
	void Matrix44PerspectiveFovLH_D3DX( Matrix44& result, FLOAT fovH, FLOAT aspect, FLOAT nearZ, FLOAT farZ)
	{
		FLOAT yScale = 1.f / appTan( fovH);
		FLOAT xScale = yScale / aspect;

		result.SetIdentity();
		result.m[0][0] = xScale;
		result.m[1][1] = yScale,		
		result.m[2][2] = farZ / (farZ-nearZ);	   
		result.m[2][3] = 1.f,
		result.m[3][2] = -(nearZ * farZ)/(farZ-nearZ);
		result.m[3][3] = 0.f;
	}

	// Matrix44PerspectiveFovLH_OpenGL
	void Matrix44PerspectiveFovLH_OpenGL( Matrix44& result, FLOAT fov, FLOAT aspect, FLOAT nearZ, FLOAT farZ)
	{
		FLOAT yScale = 1.f / appTan( fov);
		FLOAT xScale = yScale / aspect;

		result.SetIdentity();
		result.m[0][0] = xScale;
		result.m[1][1] = yScale,		
		result.m[2][2] = (farZ + nearZ) / (farZ-nearZ);	   
		result.m[2][3] = 1.f,
		result.m[3][2] = -(2.f * nearZ * farZ)/(farZ-nearZ);
		result.m[3][3] = 0.f;
	}

	// Matrix44RotationX
	void Matrix44RotationX( Matrix44& result, FLOAT radian)
	{
		FLOAT fSin, fCos;
		fSin = sin( radian);
		fCos = cos( radian);

		result.Set( 1.f,   0.f,  0.f, 0.f,
					0.f,  fCos, fSin, 0.f,
					0.f, -fSin, fCos, 0.f,
					0.f,   0.f,  0.f, 1.f);
	}

	// Matrix44RotationY
	void Matrix44RotationY( Matrix44& result, FLOAT radian)
	{
		FLOAT fSin, fCos;
		fSin = sin( radian);
		fCos = cos( radian);

		result.Set(  fCos, 0.f, -fSin, 0.f,
					  0.f, 1.f,   0.f, 0.f,
					 fSin, 0.f,  fCos, 0.f,
					  0.f, 0.f,   0.f, 1.f);
	}

	// Matrix44RotationZ
	void Matrix44RotationZ( Matrix44& result, FLOAT radian)
	{
		FLOAT fSin, fCos;
		fSin = sin( radian);
		fCos = cos( radian);

		result.Set(  fCos, fSin, 0.f, 0.f,
					-fSin, fCos, 0.f, 0.f,
					  0.f,  0.f, 1.f, 0.f,
					  0.f,	0.f, 0.f, 1.f);
	}

	// Matrix44RotationYawPitchRoll
	void Matrix44RotationYawPitchRoll( Matrix44& result, FLOAT yaw, FLOAT pitch, FLOAT roll)
	{
		Matrix44 rotX, rotY, rotZ;

		Matrix44RotationX( rotX, pitch);
		Matrix44RotationY( rotY, yaw);
		Matrix44RotationZ( rotZ, roll);

		result = rotZ * rotX * rotY;
	}

	// Matrix44RotationAxisAngle
	void Matrix44RotationAxisAngle( Matrix44& result, const Vector3& axis, FLOAT angle)
	{
		float x = axis.x;
		float y = axis.y;
		float z = axis.z;

		float fSin, fCos;
		fSin = sin( angle);
		fCos = cos( angle);

		result.m[0][0] = ( x * x ) * ( 1.f - fCos ) + fCos;
		result.m[0][1] = ( x * y ) * ( 1.f - fCos ) + (z * fSin);
		result.m[0][2] = ( x * z ) * ( 1.f - fCos ) - (y * fSin);
		result.m[0][3] = 0.0f;

		result.m[1][0] = ( y * x ) * ( 1.f - fCos ) - (z * fSin);
		result.m[1][1] = ( y * y ) * ( 1.f - fCos ) + fCos ;
		result.m[1][2] = ( y * z ) * ( 1.f - fCos ) + (x * fSin);
		result.m[1][3] = 0.0f;

		result.m[2][0] = ( z * x ) * ( 1.f - fCos ) + (y * fSin);
		result.m[2][1] = ( z * y ) * ( 1.f - fCos ) - (x * fSin);
		result.m[2][2] = ( z * z ) * ( 1.f - fCos ) + fCos;
		result.m[2][3] = 0.0f;

		result.m[3][0] = 0.0f;
		result.m[3][1] = 0.0f;
		result.m[3][2] = 0.0f;
		result.m[3][3] = 1.0f;
	}

	// Matrix44RotationAxis
	void Matrix44RotationAxis( Matrix44& result, const Vector3& dirFrom, const Vector3& dirTo)
	{
		Vector3 vFrom = dirFrom;
		Vector3 vTo   = dirTo;
		vFrom.Normalize();
		vTo.Normalize();

		// 求角度
		float aCos;
		float angle;
		aCos = Vector3Dot( vFrom, vTo);
		aCos = std::max<float>(-1.0f, aCos);
		aCos = std::min<float>( 1.0f, aCos);
		angle= std::acos(aCos);

		// 求面法线
		Vector3 axis = Vector3Cross( vFrom, vTo);
		Matrix44RotationAxisAngle( result, axis, angle);
	}

	// 计算法线(仅适用于Int型索引, trianglelist拓扑类型)
	void TBNComputeNormal( Vector3* normals, const Vector3* positions, int numVertex, const int* tris, int numTris)
	{
		memset( normals, 0, sizeof(Vector3)*numVertex);
		for ( int i=0; i<numTris; i++)
		{
			int idxB = i*3;
			int idx0 = tris[idxB+0];
			int idx1 = tris[idxB+1];
			int idx2 = tris[idxB+2];

			A_ASSERT( idx0<numVertex);
			A_ASSERT( idx1<numVertex);
			A_ASSERT( idx2<numVertex);

			Vector3 edge0 = positions[idx1] - positions[idx0];
			Vector3 edge1 = positions[idx2] - positions[idx0];

			// calc triangle normal
			Vector3 faceNormal = Vector3Cross( edge0, edge1);
			faceNormal.Normalize();

			normals[idx0] += faceNormal;
			normals[idx1] += faceNormal;
			normals[idx2] += faceNormal;
		}

		// 单位化
		for ( int i=0; i<numVertex; i++)
			normals[i].Normalize();
	}

	// 计算法线(仅适用于Int型索引, trianglelist拓扑类型)
	void TBNComputeNormal( Vector3* normals, const Vector3* positions, int numVertex, const WORD* tris, int numTris)
	{
		memset( normals, 0, sizeof(Vector3)*numVertex);
		for ( int i=0; i<numTris; i++)
		{
			int  idxB = i*3;
			WORD idx0 = tris[idxB+0];
			WORD idx1 = tris[idxB+1];
			WORD idx2 = tris[idxB+2];

			A_ASSERT( idx0<numVertex);
			A_ASSERT( idx1<numVertex);
			A_ASSERT( idx2<numVertex);

			Vector3 edge0 = positions[idx1] - positions[idx0];
			Vector3 edge1 = positions[idx2] - positions[idx0];

			// calc triangle normal
			Vector3 faceNormal = Vector3Cross( edge0, edge1);
			faceNormal.Normalize();

			normals[idx0] += faceNormal;
			normals[idx1] += faceNormal;
			normals[idx2] += faceNormal;
		}

		// 单位化
		for ( int i=0; i<numVertex; i++)
			normals[i].Normalize();
	}

	//  Calculate tangent binormal
	void TBNComputeTangentBinormal( Vector3* tangents, Vector3* binormals, const Vector3* positions, const Vector2* uvs, const Vector3* normals, int numVertex, const int* tris, int numTris)
	{
		memset( tangents, 0, sizeof(Vector3)*numVertex);
		memset( binormals,0, sizeof(Vector3)*numVertex);

		for ( int i=0; i<numTris; i++)
		{
			int idxB = i*3;
			int idx0 = tris[idxB+0];
			int idx1 = tris[idxB+1];
			int idx2 = tris[idxB+2];

			const Vector3& v0 = positions[idx0];
			const Vector3& v1 = positions[idx1];
			const Vector3& v2 = positions[idx2];

			Vector3 v1v0	  = v1 - v0;
			Vector3 v2v0	  = v2 - v0;

			const Vector2& uv0= uvs[idx0];
			const Vector2& uv1= uvs[idx1];
			const Vector2& uv2= uvs[idx2];

			float s1 = uv1.x - uv0.x;
			float t1 = uv1.y - uv0.y;

			float s2 = uv2.x - uv0.x;
			float t2 = uv2.y - uv0.y;

			float denominator = s1 * t2 - s2 * t1;

			Vector3 tangent, binormal;
			if( std::abs(denominator) < std::numeric_limits<float>::epsilon())
			{
				tangent = Vector3( 1, 0, 0);
				binormal= Vector3( 0, 1, 0);
			}
			else
			{
				tangent  = ( t2 * v1v0 - t1 * v2v0) / denominator;
				binormal = ( s1 * v2v0 - s2 * v1v0) / denominator;
			}

			// 副线
			binormals[idx0] += binormal;
			binormals[idx1] += binormal;
			binormals[idx2] += binormal;

			// 切线
			tangents[idx0]  += tangent;
			tangents[idx1]  += tangent;
			tangents[idx2]  += tangent;
		}

		for( int i=0; i<numVertex; i++)
		{
			Vector3 tT = tangents[i];
			Vector3 tB = binormals[i];
			Vector3 tN = normals[i];

			// Gram-Schmidt orthogonalize
			tT = ( tT - tN * Vector3Dot( tT, tN));
			tT.Normalize();

			// calculate handedness
			float k=1;
			if( Vector3Dot( Vector3Cross( tN, tT), tB) <0)
				k = -1;			

			tangents[i]  = k * tT;
			binormals[i] = Vector3Cross( tN, tT);
		}
	}

	// 构造函数 
	Transform::Transform()
		: m_rotation( 0.f, 0.f, 0.f, 1.f)
		, m_origin( Vector3::Zero)
		, m_scale( 1.f, 1.f, 1.f)
		, m_dirtyFlag( true)
	{
	}

	// 赋值构造函数
	Transform::Transform(const Transform& src)
		: m_rotation( src.m_rotation)
		, m_origin( src.m_origin)
		, m_scale( src.m_scale)
	{
	}

	// 赋值构造函数
	Transform::Transform( const Quat& rot, const Vector3& orig)
		: m_rotation( rot)
		, m_origin( orig)
		, m_scale( Vector3( 1.f, 1.f, 1.f))
	{}

	// 设置
	void Transform::SetIdentity()
	{
		m_rotation = Quat( 0.f, 0.f, 0.f, 1.f);
		m_origin   = Vector3::Zero;

		m_dirtyFlag = true;
	}

	// 是否为单位化
	bool Transform::IsIdentity()
	{
		return m_rotation==Quat( 0.f, 0.f, 0.f, 1.f) && m_origin==Vector3::Zero;
	}

	// 重载运算符"-"
	Transform Transform::operator + ( const Transform& tf) const
	{
		return Transform( m_rotation+tf.m_rotation, m_origin+tf.m_origin);
	}

	// 重载运算符 "+="
	Transform Transform::operator += ( const Transform& tf)
	{
		m_rotation = m_rotation + tf.m_rotation;
		m_origin   = m_origin + tf.m_origin;

		return *this;
	}

	// 重载运算符"-"
	Transform Transform::operator - ( const Transform& tf) const
	{
		return Transform( m_rotation-tf.m_rotation, m_origin-tf.m_origin);
	}

	// 重载运算符 "="
	Transform& Transform::operator = (const Transform& src)
	{
		m_rotation	= src.m_rotation;
		m_origin	= src.m_origin;
		m_scale		= src.m_scale;

		return (*this);
	}

	// 运算符重载 "*"
	Transform Transform::operator* (float scale) const
	{
		return Transform( m_rotation*scale, m_origin*scale);
	}

	// 运算符重载 "="
	Transform Transform::operator * ( const Transform& other) const
	{
		Transform result;
		result.m_rotation = m_rotation * other.m_rotation;
		result.m_origin   = this->TransformVector3( other.m_origin);

		return result;
	}

	// 设置缩放
	void Transform::SetScale( float scale)
	{
		m_scale = Vector3( scale, scale, scale);

		m_dirtyFlag = true;
	}

	// Transform a location
	Vector3 Transform::TransformVector3( const Vector3& v) const
	{
		return m_origin + m_rotation.Rotate( v);
	}

	// 更改位置
	void  Transform::SetTrans( float x, float y, float z)
	{
		SetTrans( Vector3( x, y, z));

		m_dirtyFlag = true;
	}

	// 更改位置
	void  Transform::SetTrans( const Vector3& trans)
	{
		m_origin = trans;

		m_dirtyFlag = true;
	}

	// 旋转(在原来的基础上添加)
	void Transform::AddRotation(  const Vector3& axis, FLOAT angle)
	{
		Quat quat;
		quat.FromAxisAngle( axis, angle);

		m_rotation = m_rotation * quat;

		m_dirtyFlag = true;
	}

	// 设置旋转(原朝向,当前朝向, 请确保两参数均为单位向量)
	void  Transform::SetRotation( const Vector3& origDir, const Vector3& curDir)
	{
		m_rotation.FromArc( origDir, curDir);

		m_dirtyFlag = true;
	}

	// 设置矩阵
	void Transform::SetMatrix( const Matrix44& mat)
	{
		m_rotation.FromMatrix( mat);
		m_scale  = Vector3( 1.f, 1.f, 1.f);
		m_origin = Vector3( mat._41, mat._42, mat._43);
	}

	//// X轴反转
	//void Transform::InvXAxis()
	//{
	//	m_scale.x = -m_scale.x;
	//}

	//// Y轴反转
	//void Transform::InvYAxis()
	//{
	//	m_scale.y = -m_scale.y;
	//}

	//// Z轴反转
	//void Transform::InvZAxis()
	//{
	//	m_scale.z = -m_scale.z;
	//}

	// 世界矩阵
	const Matrix44 Transform::GetMatrix() const
	{
		// 缩放
		Matrix44 result;
		Matrix44Scaling( result, m_scale.x, m_scale.y, m_scale.z);

		// 旋转
		Matrix44 rotation;
		m_rotation.ToMatrix( rotation);

		result *= rotation;

		// 平移
		result._41 = m_origin.x; 
		result._42 = m_origin.y; 
		result._43 = m_origin.z;

		return result;
	}
}